/*
 * demolition man
 * combos.c
 *
 * written by James Cardona
 *
 *
 * Scoring Description: (original game)
 * a combo is obviously making more than one shot in a row
 * we detect this using a task timer and change the arrows that are lit after
 * each shot.  if the player makes the next lit arrow then they are
 * awarded a combo and so on.  if N number of combos are reached then computer award is lit.
 * computer award can be gotten at underground shot.
 *
 */
/* CALLSET_SECTION (combos, __machine5__) */


#include <freewpc.h>
#include "dm/global_constants.h"

//constants
const U8 COMBO_WAIT_TIME = 12; //TOTAL = WAIT + HURRYUP
const U8 COMBO_HURRY_TIME = 5;
const U8 COMBO_EASY_GOAL = 10;
const U8 COMBO_HARD_GOAL = 20;
const U8 COMBO_EASY_GOAL_STEP = 10;
const U8 COMBO_HARD_GOAL_STEP = 20;
const U8 COMBO_VM_EASY_GOAL = 5;
const U8 COMBO_VM_HARD_GOAL = 15;

//local variables
U8 				combo_counter;
__local__ U8 	combo_goal;
__local__ U8 	combo_vm_goal;

//external variables

//internally called function prototypes  --external found at protos.h
void combo_reset (void);
void player_combo_reset (void);
void flash_combos(void);
void choose_random_flag_set(void);
void combo_task (void);

/****************************************************************************
 * initialize  and exit
 ***************************************************************************/
//we always start with these 3 lights on because those are the skillshots
void combo_reset (void) {
	flag_on (FLAG_IS_COMBO_SIDERAMP_ACTIVATED);
	flag_on (FLAG_IS_COMBO_LEFTRAMP_ACTIVATED);
	flag_off (FLAG_IS_COMBO_RIGHTRAMP_ACTIVATED);
	flag_on (FLAG_IS_COMBO_UNDER_ACTIVATED);
	flag_off (FLAG_IS_COMBO_CENTERRAMP_ACTIVATED);
	flag_off (FLAG_IS_COMBO_LEFTORB_ACTIVATED);
	flag_off (FLAG_IS_COMBO_RIGHTORB_ACTIVATED);
	flag_on (FLAG_IS_COMBOS_RESTARTABLE);
	if (flag_test (FLAG_IS_COMPUTER_ACTIVATED) ) computer_light_on();
	all_arrow_update(); // at arrow_handler.c
}//end of function



void player_combo_reset (void) {
	if (IN_TEST) 	{ combo_goal = 10; combo_vm_goal = 2; }
	else		 	{ combo_goal = COMBO_EASY_GOAL; combo_vm_goal = COMBO_VM_EASY_GOAL; }

	combo_counter = 0;
	flag_off (FLAG_IS_COMPUTER_ACTIVATED);

	#ifdef CONFIG_DIFFICULTY_LEVEL
if (system_config.difficulty == EASY) {
	combo_goal = COMBO_EASY_GOAL;
	combo_vm_goal = COMBO_VM_EASY_GOAL;
}//end of if
else	{
	combo_goal = COMBO_HARD_GOAL;
	combo_vm_goal = COMBO_VM_HARD_GOAL;
} //END OF ELSE
#endif
}//end of function

CALLSET_ENTRY (combo, start_player) 	{ player_combo_reset(); }
CALLSET_ENTRY (combo, start_ball) 		{ combo_reset(); }
CALLSET_ENTRY (combo, valid_playfield) 	{ combo_init(); }

/****************************************************************************
 *
 * body
 *
 ****************************************************************************/
void combo_init(void) {
	flag_off (FLAG_IS_COMBOS_KILLED);
	flag_on (FLAG_IS_COMBOS_RESTARTABLE);
	choose_random_flag_set();
	all_arrow_update(); // at arrow_handler.c
	task_create_gid1 (GID_COMBO, combo_task);
}//end of function



//this is a hard kill that prevents combos from restarting
//we only use this during certain modes
void kill_combos(void) {
	flag_off (FLAG_IS_COMBO_SIDERAMP_ACTIVATED);
	flag_off (FLAG_IS_COMBO_LEFTRAMP_ACTIVATED);
	flag_off (FLAG_IS_COMBO_RIGHTRAMP_ACTIVATED);
	flag_off (FLAG_IS_COMBO_UNDER_ACTIVATED);
	flag_off (FLAG_IS_COMBO_CENTERRAMP_ACTIVATED);
	flag_off (FLAG_IS_COMBO_LEFTORB_ACTIVATED);
	flag_off (FLAG_IS_COMBO_RIGHTORB_ACTIVATED);

	flag_on (FLAG_IS_COMBOS_KILLED);
	flag_off (FLAG_IS_COMBOS_RESTARTABLE);
	all_arrow_update();
}//end of function



void combo_task (void) {
	task_sleep_sec(COMBO_WAIT_TIME);
	//no combo made yet so flash active lamps
	flash_combos();
	task_sleep_sec(COMBO_HURRY_TIME);
	//still no hits so turn off flags and lamps until something is hit
	flag_off (FLAG_IS_COMBO_SIDERAMP_ACTIVATED);
	flag_off (FLAG_IS_COMBO_LEFTRAMP_ACTIVATED);
	flag_off (FLAG_IS_COMBO_RIGHTRAMP_ACTIVATED);
	flag_off (FLAG_IS_COMBO_UNDER_ACTIVATED);
	flag_off (FLAG_IS_COMBO_CENTERRAMP_ACTIVATED);
	flag_off (FLAG_IS_COMBO_LEFTORB_ACTIVATED);
	flag_off (FLAG_IS_COMBO_RIGHTORB_ACTIVATED);
	flag_on (FLAG_IS_COMBOS_KILLED);
	flag_on (FLAG_IS_COMBOS_RESTARTABLE);
	all_arrow_update();
	task_exit();
}//end of function



//flashing overides being on solid, so if multiple modes
//are running, lamp will flash, but when timer ends, will go back to solid
void flash_combos(void){
	if (flag_test (FLAG_IS_COMBO_SIDERAMP_ACTIVATED) ) 		lamp_tristate_flash (LM_SIDE_RAMP_ARROW);
	if (flag_test (FLAG_IS_COMBO_LEFTRAMP_ACTIVATED) ) 		lamp_tristate_flash (LM_LEFT_RAMP_ARROW);
	if (flag_test (FLAG_IS_COMBO_RIGHTRAMP_ACTIVATED) ) 	lamp_tristate_flash (LM_RIGHT_RAMP_ARROW);
	if (flag_test (FLAG_IS_COMBO_UNDER_ACTIVATED) ) 		lamp_tristate_flash (LM_UNDERGROUND_ARROW);
	if (flag_test (FLAG_IS_COMBO_CENTERRAMP_ACTIVATED) ) 	lamp_tristate_flash (LM_CENTER_RAMP_ARROW);
	if (flag_test (FLAG_IS_COMBO_LEFTORB_ACTIVATED) ) 		lamp_tristate_flash (LM_LEFT_LOOP_ARROW);
	if (flag_test (FLAG_IS_COMBO_RIGHTORB_ACTIVATED) ) 		lamp_tristate_flash (LM_RIGHT_LOOP_ARROW);
}//end of function



void combo_hit(void ) {
	//video mode
	if (++combo_counter >= combo_vm_goal  && !flag_test (FLAG_VIDEO_MODE_ENABLED) ) {
		if (IN_TEST) 	combo_vm_goal += 2;
		else {
			#ifdef CONFIG_DIFFICULTY_LEVEL
			if (system_config.difficulty == EASY) 	combo_vm_goal += COMBO_EASY_GOAL_STEP;
			else									combo_vm_goal += COMBO_HARD_GOAL_STEP;
			#elif
													combo_vm_goal += COMBO_EASY_GOAL_STEP;
			#endif
		}//end of else

		flag_on (FLAG_VIDEO_MODE_ENABLED);
		deff_start (DEFF_VM_EFFECT);
	}
	//computer award
	else if (combo_counter >= combo_goal  && !flag_test (FLAG_IS_COMPUTER_ACTIVATED) ) {
		if (IN_TEST) 	combo_goal += 2;
		else {
			#ifdef CONFIG_DIFFICULTY_LEVEL
			if (system_config.difficulty == EASY) 	combo_goal += COMBO_EASY_GOAL_STEP;
			else									combo_goal += COMBO_HARD_GOAL_STEP;
			#elif
													combo_goal += COMBO_EASY_GOAL_STEP;
			#endif
		}//end of else

		computer_light_on(); 	//at underground.c
		deff_start (DEFF_COMBO_EFFECT);
	}
	//not at a goal yet
	else deff_start (DEFF_COMBO_EFFECT);
	// reset the task timer
	choose_random_flag_set();
	all_arrow_update();
	task_recreate_gid (GID_COMBO, combo_task);
}//end of function


//this is for stacking
void combo_rehit_check(void ) {
	//video mode
	if (combo_counter >= combo_vm_goal  && !flag_test (FLAG_VIDEO_MODE_ENABLED) ) {
		combo_vm_goal += COMBO_EASY_GOAL_STEP;
		flag_on (FLAG_VIDEO_MODE_ENABLED);
	}
	//computer award
	else if (combo_counter >= combo_goal  && !flag_test (FLAG_IS_COMPUTER_ACTIVATED) ) {
		combo_goal += COMBO_EASY_GOAL_STEP;
		computer_light_on(); 	//at underground.c
	}
}//end of function




//called from computer award in underground.c
void comp_award_light_arrows(void) {
	flag_off (FLAG_IS_COMBOS_KILLED);
	flag_on 	(FLAG_IS_COMBO_SIDERAMP_ACTIVATED);
	flag_on 	(FLAG_IS_COMBO_LEFTRAMP_ACTIVATED);
	flag_on 	(FLAG_IS_COMBO_RIGHTRAMP_ACTIVATED);
	flag_on 	(FLAG_IS_COMBO_UNDER_ACTIVATED);
	flag_on 	(FLAG_IS_COMBO_CENTERRAMP_ACTIVATED);
	flag_on 	(FLAG_IS_COMBO_LEFTORB_ACTIVATED);
	flag_on 	(FLAG_IS_COMBO_RIGHTORB_ACTIVATED);
	all_arrow_update();
	task_recreate_gid (GID_COMBO, combo_task);
}//end of function





/****************************************************************************
 *
 * randomize flag chosen
 *
 * we are using a very stupid and in-elegant
 * way of randomness - basically, we have a number
 * of predefined sets of flags and we randomly
 * pick one of those sets
 ****************************************************************************/
void choose_random_flag_set(void) {
	U8 random_chooser;
	random_chooser = random_scaled(5);
	switch (random_chooser) {
	case 0:
		flag_on 	(FLAG_IS_COMBO_SIDERAMP_ACTIVATED);
		flag_on 	(FLAG_IS_COMBO_LEFTRAMP_ACTIVATED);
		flag_off 	(FLAG_IS_COMBO_RIGHTRAMP_ACTIVATED);
		flag_off 	(FLAG_IS_COMBO_UNDER_ACTIVATED);
		flag_on 	(FLAG_IS_COMBO_CENTERRAMP_ACTIVATED);
		flag_off 	(FLAG_IS_COMBO_LEFTORB_ACTIVATED);
		flag_on 	(FLAG_IS_COMBO_RIGHTORB_ACTIVATED);
		break;
	case 1:
		flag_on 	(FLAG_IS_COMBO_SIDERAMP_ACTIVATED);
		flag_on 	(FLAG_IS_COMBO_LEFTRAMP_ACTIVATED);
		flag_off 	(FLAG_IS_COMBO_RIGHTRAMP_ACTIVATED);
		flag_off 	(FLAG_IS_COMBO_UNDER_ACTIVATED);
		flag_off 	(FLAG_IS_COMBO_CENTERRAMP_ACTIVATED);
		flag_on 	(FLAG_IS_COMBO_LEFTORB_ACTIVATED);
		flag_off 	(FLAG_IS_COMBO_RIGHTORB_ACTIVATED);
		break;
	case 2:
		flag_off 	(FLAG_IS_COMBO_SIDERAMP_ACTIVATED);
		flag_off 	(FLAG_IS_COMBO_LEFTRAMP_ACTIVATED);
		flag_on 	(FLAG_IS_COMBO_RIGHTRAMP_ACTIVATED);
		flag_on 	(FLAG_IS_COMBO_UNDER_ACTIVATED);
		flag_on 	(FLAG_IS_COMBO_CENTERRAMP_ACTIVATED);
		flag_off 	(FLAG_IS_COMBO_LEFTORB_ACTIVATED);
		flag_on 	(FLAG_IS_COMBO_RIGHTORB_ACTIVATED);
		break;
	case 3:
		flag_off 	(FLAG_IS_COMBO_SIDERAMP_ACTIVATED);
		flag_off 	(FLAG_IS_COMBO_LEFTRAMP_ACTIVATED);
		flag_on 	(FLAG_IS_COMBO_RIGHTRAMP_ACTIVATED);
		flag_on 	(FLAG_IS_COMBO_UNDER_ACTIVATED);
		flag_off 	(FLAG_IS_COMBO_CENTERRAMP_ACTIVATED);
		flag_on 	(FLAG_IS_COMBO_LEFTORB_ACTIVATED);
		flag_off 	(FLAG_IS_COMBO_RIGHTORB_ACTIVATED);
		break;
	case 4:
		flag_on 	(FLAG_IS_COMBO_SIDERAMP_ACTIVATED);
		flag_off 	(FLAG_IS_COMBO_LEFTRAMP_ACTIVATED);
		flag_on 	(FLAG_IS_COMBO_RIGHTRAMP_ACTIVATED);
		flag_off 	(FLAG_IS_COMBO_UNDER_ACTIVATED);
		flag_off 	(FLAG_IS_COMBO_CENTERRAMP_ACTIVATED);
		flag_off 	(FLAG_IS_COMBO_LEFTORB_ACTIVATED);
		flag_on 	(FLAG_IS_COMBO_RIGHTORB_ACTIVATED);
		break;
	}//end of switch
	//if 1 or less shot away from computer award then do not light underground shot
	if (	flag_test(FLAG_IS_COMBO_UNDER_ACTIVATED)
		&&	(flag_test(FLAG_IS_COMPUTER_ACTIVATED)
		||	combo_goal == combo_counter + 1) )			flag_off 	(FLAG_IS_COMBO_UNDER_ACTIVATED);
}//end of function


/****************************************************************************
 *
 * display effects
 *
 ****************************************************************************/
void combo_effect_deff(void) {
	U16 fno;
	if (IN_TEST) combo_counter++;

	sound_start (ST_EFFECT, SUBWAY2, SL_2S, SP_NORMAL);
	if (combo_counter % 2 == 0) {
		for (fno = IMG_COMBO1_START; fno <= IMG_COMBO1_END; fno += 2) {
					dmd_alloc_pair ();
					frame_draw(fno);
					dmd_show2 ();
					task_sleep (TIME_100MS);
		}//end of for loop
		sound_start (ST_EFFECT, SUBWAY2, SL_2S, SP_NORMAL);

		dmd_map_overlay ();
				dmd_clean_page_low ();
				if (flag_test (FLAG_IS_COMPUTER_ACTIVATED) ) sprintf ("COMPUTER READY");
				else 										 sprintf ("%d TO COMPUTER", combo_goal - combo_counter);
				font_render_string_center (&font_fixed6, DMD_MIDDLE_X, DMD_MED_CY_3, sprintf_buffer);

				sprintf ("%d", combo_counter);
				font_render_string_center (&font_fixed10, DMD_MIDDLE_X - 15, DMD_MED_CY_1 + 4, sprintf_buffer);

				dmd_text_outline ();
					dmd_alloc_pair ();
					frame_draw(IMG_COMBO1_END);
					dmd_overlay_outline ();
					dmd_show2 ();
	}//end f % 2
	else {
		for (fno = IMG_COMBO2_START; fno <= IMG_COMBO2_END; fno += 2) {
					dmd_alloc_pair ();
					frame_draw(fno);
					dmd_show2 ();
					task_sleep (TIME_100MS);
		}//end of for loop
		sound_start (ST_EFFECT, SUBWAY2, SL_2S, SP_NORMAL);

		dmd_map_overlay ();
				dmd_clean_page_low ();
				if (flag_test (FLAG_VIDEO_MODE_ENABLED) )	{
					font_render_string_center (&font_term6, DMD_MIDDLE_X - 20, DMD_MED_CY_2 + 6, "VIDEO MODE");
					font_render_string_center (&font_term6, DMD_MIDDLE_X + 45, DMD_MED_CY_3, "READY");
				}
				else {
					sprintf ("%d TO", combo_vm_goal - combo_counter);
					font_render_string_left (&font_term6, 5, DMD_MED_CY_2, sprintf_buffer);
					font_render_string_left (&font_term6, 50, DMD_MED_CY_3 - 5, "VIDEO MODE");
				}

				sprintf ("%d", combo_counter);
				font_render_string_center (&font_fixed10, DMD_MIDDLE_X - 20, DMD_MED_CY_1 + 2, sprintf_buffer);

				dmd_text_outline ();
					dmd_alloc_pair ();
					frame_draw(IMG_COMBO2_END);
					dmd_overlay_outline ();
					dmd_show2 ();
	}//end of else

	task_sleep_sec (2);
	deff_exit ();
}//end of deff





void vm_effect_deff(void) {
	U16 fno;
	sound_start (ST_EFFECT, SUBWAY2, SL_2S, SP_NORMAL);
	for (fno = IMG_COMBO1_START; fno <= IMG_COMBO1_END; fno += 2) {
				dmd_alloc_pair ();
				frame_draw(fno);
				dmd_show2 ();
				task_sleep (TIME_100MS);
	}//end of for loop
	sound_start (ST_EFFECT, SUBWAY2, SL_2S, SP_NORMAL);

	dmd_map_overlay ();
			dmd_clean_page_low ();
			font_render_string_center (&font_term6, DMD_MIDDLE_X - 20, DMD_MED_CY_2 + 6, "VIDEO MODE");
			font_render_string_center (&font_term6, DMD_MIDDLE_X + 45, DMD_MED_CY_3, "READY");

			sprintf ("%d", combo_counter);
			font_render_string_center (&font_fixed10, DMD_MIDDLE_X - 15, DMD_MED_CY_1 + 4, sprintf_buffer);

			dmd_text_outline ();
				dmd_alloc_pair ();
				frame_draw(IMG_COMBO1_END);
				dmd_overlay_outline ();
				dmd_show2 ();
				task_sleep_sec (2);
	deff_exit ();
}//end of deff

