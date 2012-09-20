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
 * TODO: handle an underground shot for computer award when the underground arrow
 * is also lit
 */

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

//prototypes
void combo_reset (void);
void player_combo_reset (void);
void flash_combos(void);
void choose_random_flag_set(void);

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
	if (flag_test (FLAG_IS_COMPUTER_ACTIVATED) ) callset_invoke(computer_light_on);
		else callset_invoke(computer_light_off);
}//end of function

void player_combo_reset (void) {
	combo_goal = COMBO_EASY_GOAL;
	computer_award_activated = FALSE;
	combo_counter = 0;
	flag_off (FLAG_IS_COMPUTER_ACTIVATED);
	combo_reset();
}//end of function

CALLSET_ENTRY (combo, start_player) { player_combo_reset(); }
CALLSET_ENTRY (combo, start_ball) { combo_reset(); }
CALLSET_ENTRY (combo, valid_playfield) { callset_invoke(combo_init); }



/****************************************************************************
 * playfield lights and flags
 ***************************************************************************/
CALLSET_ENTRY (combo, computer_light_on) {
	flag_on (FLAG_IS_COMPUTER_ACTIVATED);
	lamp_tristate_flash (LM_COMPUTER);
}//end of function



CALLSET_ENTRY (combo, computer_light_off) {
	flag_off (FLAG_IS_COMPUTER_ACTIVATED);
	lamp_tristate_off (LM_COMPUTER);
}//end of function

/****************************************************************************
 * body
 *
 ****************************************************************************/
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
	callset_invoke (all_arrow_update);
	task_exit();
}//end of function



CALLSET_ENTRY (combo, combo_init) {
	flag_off (FLAG_IS_COMBOS_KILLED);
	choose_random_flag_set();
	callset_invoke (all_arrow_update);//at arrow_handler.c
	task_create_gid1 (GID_COMBO, combo_task);
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



CALLSET_ENTRY (combo, combo_hit) {
	if (++combo_counter >= combo_goal  && !computer_award_activated) {
		combo_goal += COMBO_EASY_GOAL_STEP;
		callset_invoke(computer_light_on);
		deff_start (DEFF_COMPUTER_EFFECT);
	} else 	if (combo_counter < combo_goal) deff_start (DEFF_COMBO_EFFECT);
	// reset the task timer
	task_recreate_gid (GID_COMBO, combo_task);
	choose_random_flag_set();
	callset_invoke (all_arrow_update);
}//end of function



//called from computer award in underground.c
CALLSET_ENTRY (combo, comp_award_light_arrows) {
	flag_off (FLAG_IS_COMBOS_KILLED);
	choose_random_flag_set();
	callset_invoke (all_arrow_update);
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
		flag_on (FLAG_IS_COMBO_SIDERAMP_ACTIVATED);
		flag_on (FLAG_IS_COMBO_LEFTRAMP_ACTIVATED);
		flag_off (FLAG_IS_COMBO_RIGHTRAMP_ACTIVATED);
		flag_off (FLAG_IS_COMBO_UNDER_ACTIVATED);
		flag_on (FLAG_IS_COMBO_CENTERRAMP_ACTIVATED);
		flag_off (FLAG_IS_COMBO_LEFTORB_ACTIVATED);
		flag_on (FLAG_IS_COMBO_RIGHTORB_ACTIVATED);
		break;
	case 1:
		flag_on (FLAG_IS_COMBO_SIDERAMP_ACTIVATED);
		flag_on (FLAG_IS_COMBO_LEFTRAMP_ACTIVATED);
		flag_off (FLAG_IS_COMBO_RIGHTRAMP_ACTIVATED);
		flag_off (FLAG_IS_COMBO_UNDER_ACTIVATED);
		flag_off (FLAG_IS_COMBO_CENTERRAMP_ACTIVATED);
		flag_on (FLAG_IS_COMBO_LEFTORB_ACTIVATED);
		flag_off (FLAG_IS_COMBO_RIGHTORB_ACTIVATED);
		break;
	case 2:
		flag_off (FLAG_IS_COMBO_SIDERAMP_ACTIVATED);
		flag_off (FLAG_IS_COMBO_LEFTRAMP_ACTIVATED);
		flag_on (FLAG_IS_COMBO_RIGHTRAMP_ACTIVATED);
		flag_on (FLAG_IS_COMBO_UNDER_ACTIVATED);
		flag_on (FLAG_IS_COMBO_CENTERRAMP_ACTIVATED);
		flag_off (FLAG_IS_COMBO_LEFTORB_ACTIVATED);
		flag_on (FLAG_IS_COMBO_RIGHTORB_ACTIVATED);
		break;
	case 3:
		flag_off (FLAG_IS_COMBO_SIDERAMP_ACTIVATED);
		flag_off (FLAG_IS_COMBO_LEFTRAMP_ACTIVATED);
		flag_on (FLAG_IS_COMBO_RIGHTRAMP_ACTIVATED);
		flag_on (FLAG_IS_COMBO_UNDER_ACTIVATED);
		flag_off (FLAG_IS_COMBO_CENTERRAMP_ACTIVATED);
		flag_on (FLAG_IS_COMBO_LEFTORB_ACTIVATED);
		flag_off (FLAG_IS_COMBO_RIGHTORB_ACTIVATED);
		break;
	case 4:
		flag_on (FLAG_IS_COMBO_SIDERAMP_ACTIVATED);
		flag_off (FLAG_IS_COMBO_LEFTRAMP_ACTIVATED);
		flag_on (FLAG_IS_COMBO_RIGHTRAMP_ACTIVATED);
		flag_off (FLAG_IS_COMBO_UNDER_ACTIVATED);
		flag_off (FLAG_IS_COMBO_CENTERRAMP_ACTIVATED);
		flag_off (FLAG_IS_COMBO_LEFTORB_ACTIVATED);
		flag_on (FLAG_IS_COMBO_RIGHTORB_ACTIVATED);
		break;
	}//end of switch
}//end of function


/****************************************************************************
 *
 * display effects
 *
 ****************************************************************************/

void combo_effect_deff(void) {
	dmd_alloc_low_clean ();
	sprintf ("%d COMBOS", combo_counter);
	font_render_string_center (&font_term6, DMD_MIDDLE_X, DMD_BIG_CY_Top, sprintf_buffer);
	sprintf ("%d TO COMPUTER", combo_goal - combo_counter);
	font_render_string_center (&font_term6, DMD_MIDDLE_X, DMD_BIG_CY_Bot, sprintf_buffer);
	dmd_show_low ();
	task_sleep_sec (3);
	deff_exit ();
}//end of deff



void computer_effect_deff(void) {
	dmd_alloc_low_clean ();
	sprintf ("COMPUTER LIT");
	font_render_string_center (&font_term6, DMD_MIDDLE_X, DMD_BIG_CY_Top, sprintf_buffer);
	sprintf ("SHOOT SUBWAY", combo_goal - combo_counter);
	font_render_string_center (&font_term6, DMD_MIDDLE_X, DMD_BIG_CY_Bot, sprintf_buffer);
	dmd_show_low ();
	task_sleep_sec (3);
	deff_exit ();
}//end of deff

