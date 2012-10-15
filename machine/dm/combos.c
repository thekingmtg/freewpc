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
/* CALLSET_SECTION (combos, __machine2__) */


#include <freewpc.h>
#include "dm/global_constants.h"

//constants
const U8 COMBO_WAIT_TIME = 12; //TOTAL = WAIT + HURRYUP
const U8 COMBO_HURRY_TIME = 5;
const U8 COMBO_EASY_GOAL = 10;
const U8 COMBO_MED_GOAL = 20;
const U8 COMBO_HARD_GOAL = 25;
const U8 COMBO_EASY_GOAL_STEP = 10;
const U8 COMBO_MED_GOAL_STEP = 15;
const U8 COMBO_HARD_GOAL_STEP = 20;

//local variables
U8 				combo_counter;
__local__ U8 	combo_goal;
__boolean 		computer_award_activated;

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
void combo_reset (void) {
	flag_off (FLAG_IS_COMBO_SIDERAMP_ACTIVATED);
	flag_off (FLAG_IS_COMBO_LEFTRAMP_ACTIVATED);
	flag_off (FLAG_IS_COMBO_RIGHTRAMP_ACTIVATED);
	flag_off (FLAG_IS_COMBO_UNDER_ACTIVATED);
	flag_off (FLAG_IS_COMBO_CENTERRAMP_ACTIVATED);
	flag_off (FLAG_IS_COMBO_LEFTORB_ACTIVATED);
	flag_off (FLAG_IS_COMBO_RIGHTORB_ACTIVATED);
	if (flag_test (FLAG_IS_COMPUTER_ACTIVATED) ) computer_light_on();
}//end of function

void player_combo_reset (void) {
	combo_goal = COMBO_EASY_GOAL;
	computer_award_activated = FALSE;
	combo_counter = 0;
	flag_off (FLAG_IS_COMPUTER_ACTIVATED);
	combo_reset();
}//end of function

CALLSET_ENTRY (combo, start_player) 	{ player_combo_reset(); }
CALLSET_ENTRY (combo, start_ball) 		{ combo_reset(); }
CALLSET_ENTRY (combo, valid_playfield) 	{ combo_init(); }

/****************************************************************************
 * body
 *
 ****************************************************************************/
void combo_init(void) {
	flag_off (FLAG_IS_COMBOS_KILLED);
	choose_random_flag_set();
	all_arrow_update(); // at arrow_handler.c
	task_create_gid1 (GID_COMBO, combo_task);
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
	if (++combo_counter >= combo_goal  && !computer_award_activated) {
		combo_goal += COMBO_EASY_GOAL_STEP;
		computer_light_on(); 	//at underground.c
		deff_start (DEFF_COMPUTER_EFFECT);
	} else 	if (combo_counter < combo_goal) deff_start (DEFF_COMBO_EFFECT);
	// reset the task timer
	choose_random_flag_set();
	all_arrow_update();
	task_recreate_gid (GID_COMBO, combo_task);
}//end of function



//called from computer award in underground.c
void comp_award_light_arrows(void) {
	flag_off (FLAG_IS_COMBOS_KILLED);
	choose_random_flag_set();
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
	U8 i;
	for (i=0; i < 4; i++) {
		dmd_sched_transition (&trans_scroll_up);
		dmd_alloc_low_clean ();
		sprintf ("%d COMBOS", combo_counter);
		font_render_string_center (&font_fixed6, DMD_MIDDLE_X, DMD_BIG_CY_Top, sprintf_buffer);

		sprintf ("%d COMBOS", combo_counter);
		font_render_string_center (&font_fixed6, DMD_MIDDLE_X, DMD_BIG_CY_Bot, sprintf_buffer);
		dmd_show_low ();
	}
	dmd_alloc_low_clean ();
	dmd_sched_transition (&trans_scroll_up);
	if (combo_goal - combo_counter >= 1) 	sprintf ("%d TO COMPUTER", combo_goal - combo_counter);
	else 									sprintf ("COMPUTER READY");
	font_render_string_center (&font_fixed6, DMD_MIDDLE_X, DMD_BIG_CY_Cent, sprintf_buffer);
	dmd_show_low ();
	task_sleep_sec (1);
	deff_exit ();
}//end of deff



void computer_effect_deff(void) {
	U8 i;
	for (i=0; i < 4; i++) {
		dmd_sched_transition (&trans_scroll_up);
		dmd_alloc_low_clean ();
		font_render_string_center (&font_fixed6, DMD_MIDDLE_X, DMD_BIG_CY_Top, "COMPUTER");
		font_render_string_center (&font_fixed6, DMD_MIDDLE_X, DMD_BIG_CY_Bot, "LIT");
		dmd_show_low ();
	}
	dmd_alloc_low_clean ();
	dmd_sched_transition (&trans_scroll_up);
	font_render_string_center (&font_fixed6, DMD_MIDDLE_X, DMD_BIG_CY_Top, "SHOOT");
	font_render_string_center (&font_fixed6, DMD_MIDDLE_X, DMD_BIG_CY_Bot, "SUBWAY");
	dmd_show_low ();
	task_sleep_sec (1);
	deff_exit ();
}//end of deff

