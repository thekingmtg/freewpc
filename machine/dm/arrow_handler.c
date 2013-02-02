/*
 * demolition man
 * arrow_handler.c
 *
 * written by James Cardona
 *
 */

#include <freewpc.h>
#include "dm/global_constants.h"

//local variables

//prototypes

/****************************************************************************
 * initialize  and exit
 ***************************************************************************/
CALLSET_ENTRY (arrow_handler, start_player, start_ball) 	{
	flag_off (FLAG_IS_UGROUND_ARROW_ACTIVATED);

	flag_off (FLAG_IS_R_LOOP_ARROW_ACTIVATED);
	flag_off (FLAG_IS_L_LOOP_ARROW_ACTIVATED);

	flag_off (FLAG_IS_L_RAMP_ARROW_ACTIVATED);
	flag_off (FLAG_IS_C_RAMP_ARROW_ACTIVATED);
	flag_off (FLAG_IS_S_RAMP_ARROW_ACTIVATED);
	flag_off (FLAG_IS_R_RAMP_ARROW_ACTIVATED);
}//end of function


/****************************************************************************
 *
 * Arrow handlers
 *
 ****************************************************************************/
void center_ramp_arrow_update(void) {
	if (	flag_test (FLAG_IS_CAPSIM_CENTERRAMP_ACTIVATED)
		||	flag_test (FLAG_IS_ACMAG_RUNNING)
		||	flag_test (FLAG_IS_COMBO_CENTERRAMP_ACTIVATED)
		||	flag_test (FLAG_IS_C_RAMP_JACKPOT_ACTIVATED) )
		cramp_arrow_light_on();
	else
		cramp_arrow_light_off();
}//end of function



void uground_arrow_update(void) {
	if (	flag_test (FLAG_IS_CAPSIM_UNDER_ACTIVATED)
		||	flag_test (FLAG_IS_COMBO_UNDER_ACTIVATED)
		||	flag_test (FLAG_IS_UGROUND_JACKPOT_ACTIVATED) )
			underground_arrow_light_on();
	else
			underground_arrow_light_off();
}//end of function



void l_ramp_arrow_update(void) {
	if (	flag_test (FLAG_IS_CAPSIM_LEFTRAMP_ACTIVATED)
		||	flag_test (FLAG_IS_COMBO_LEFTRAMP_ACTIVATED)
		||	flag_test (FLAG_IS_L_RAMP_JACKPOT_ACTIVATED) )
		lramp_arrow_light_on();
	else
		lramp_arrow_light_off();
}//end of function



void s_ramp_arrow_update(void) {
	if (	flag_test (FLAG_IS_CAPSIM_SIDERAMP_ACTIVATED)
		||	flag_test (FLAG_IS_COMBO_SIDERAMP_ACTIVATED)
		||	flag_test (FLAG_IS_S_RAMP_JACKPOT_ACTIVATED) )
		sramp_arrow_light_on();
	else
		sramp_arrow_light_off();
}//end of function


void r_ramp_arrow_update(void) {
	if (	flag_test (FLAG_IS_CAPSIM_RIGHTRAMP_ACTIVATED)
		||	flag_test (FLAG_IS_COMBO_RIGHTRAMP_ACTIVATED)
		||	flag_test (FLAG_IS_R_RAMP_JACKPOT_ACTIVATED) )
		rramp_arrow_light_on();
	else
		rramp_arrow_light_off();
}//end of function




void l_orb_arrow_update(void) {
	if (	flag_test (FLAG_IS_CAPSIM_LEFTORB_ACTIVATED)
		||	flag_test (FLAG_IS_COMBO_LEFTORB_ACTIVATED)
		||	flag_test (FLAG_IS_L_LOOP_JACKPOT_ACTIVATED) )
		ll_arrow_light_on();
	else
		ll_arrow_light_off();
}//end of function



void r_orb_arrow_update(void) {
	if (	flag_test (FLAG_IS_CAPSIM_RIGHTORB_ACTIVATED)
		||	flag_test (FLAG_IS_COMBO_RIGHTORB_ACTIVATED)
		||	flag_test (FLAG_IS_R_LOOP_JACKPOT_ACTIVATED) )
		rl_arrow_light_on();
	else
		rl_arrow_light_off();
}//end of function


void all_arrow_update(void) {
	center_ramp_arrow_update();
	l_ramp_arrow_update();
	r_ramp_arrow_update();
	s_ramp_arrow_update();
	l_orb_arrow_update();
	r_orb_arrow_update();
	uground_arrow_update();
}//end of function
