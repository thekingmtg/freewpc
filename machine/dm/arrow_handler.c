/*
 * demolition man
 * arrow_handler.c
 *
 * written by James Cardona
 *
 * Location Description:
 *
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
CALLSET_ENTRY (arrow_handler, center_ramp_arrow_update) {
	if (	!flag_test (FLAG_IS_C_RAMP_ARROW_ACTIVATED)
	&& ( 	flag_test (FLAG_IS_CAPSIM_CENTERRAMP_ACTIVATED)
		||	flag_test (FLAG_IS_ACMAG_ACTIVATED)
		||	flag_test (FLAG_IS_PBREAK_CENTERRAMP_ACTIVATED)
		||	flag_test (FLAG_IS_COMBO_CENTERRAMP_ACTIVATED) ) )
		callset_invoke (cramp_arrow_light_on);
	else
		callset_invoke (cramp_arrow_light_off);
}//end of function



CALLSET_ENTRY (arrow_handler, uground_arrow_update) {
	if (	!flag_test (FLAG_IS_UGROUND_ARROW_ACTIVATED)
	&& ( 	flag_test (FLAG_IS_CAPSIM_UNDER_ACTIVATED)
		||	flag_test (FLAG_IS_PBREAK_UNDER_ACTIVATED)
		||	flag_test (FLAG_IS_COMBO_UNDER_ACTIVATED) ) )
		callset_invoke (underground_arrow_light_on);
	else
		callset_invoke (underground_arrow_light_off);
}//end of function



CALLSET_ENTRY (arrow_handler, l_ramp_arrow_update) {
	if (	!flag_test (FLAG_IS_L_RAMP_ARROW_ACTIVATED)
	&& ( 	flag_test (FLAG_IS_CAPSIM_LEFTRAMP_ACTIVATED)
		||	flag_test (FLAG_IS_PBREAK_LEFTRAMP_ACTIVATED)
		||	flag_test (FLAG_IS_COMBO_LEFTRAMP_ACTIVATED) ) )
		callset_invoke (lramp_arrow_light_on);
	else
		callset_invoke (lramp_arrow_light_off);
}//end of function



CALLSET_ENTRY (arrow_handler, s_ramp_arrow_update) {
	if (	!flag_test (FLAG_IS_S_RAMP_ARROW_ACTIVATED)
	&& ( 	flag_test (FLAG_IS_CAPSIM_SIDERAMP_ACTIVATED)
		||	flag_test (FLAG_IS_PBREAK_SIDERAMP_ACTIVATED)
		||	flag_test (FLAG_IS_COMBO_SIDERAMP_ACTIVATED) ) )
		callset_invoke (sramp_arrow_light_on);
	else
		callset_invoke (sramp_arrow_light_off);
}//end of function




CALLSET_ENTRY (arrow_handler, r_ramp_arrow_update) {
	if (	!flag_test (FLAG_IS_R_RAMP_ARROW_ACTIVATED)
	&& ( 	flag_test (FLAG_IS_CAPSIM_RIGHTRAMP_ACTIVATED)
		||	flag_test (FLAG_IS_PBREAK_RIGHTRAMP_ACTIVATED)
		||	flag_test (FLAG_IS_COMBO_RIGHTRAMP_ACTIVATED) ) )
		callset_invoke (rramp_arrow_light_on);
	else
		callset_invoke (rramp_arrow_light_on);
}//end of function




CALLSET_ENTRY (arrow_handler, l_orb_arrow_update) {
	if (	!flag_test (FLAG_IS_L_LOOP_ARROW_ACTIVATED)
	&& ( 	flag_test (FLAG_IS_CAPSIM_LEFTORB_ACTIVATED)
		||	flag_test (FLAG_IS_PBREAK_LEFTORB_ACTIVATED)
		||	flag_test (FLAG_IS_COMBO_LEFTORB_ACTIVATED) ) )
		callset_invoke (ll_arrow_light_on);
	else
		callset_invoke (ll_arrow_light_off);
}//end of function



CALLSET_ENTRY (arrow_handler, r_orb_arrow_update) {
	if (	!flag_test (FLAG_IS_R_LOOP_ARROW_ACTIVATED)
	&& ( 	flag_test (FLAG_IS_CAPSIM_RIGHTORB_ACTIVATED)
		||	flag_test (FLAG_IS_PBREAK_RIGHTORB_ACTIVATED)
		||	flag_test (FLAG_IS_COMBO_RIGHTORB_ACTIVATED) ) )
		callset_invoke (rl_arrow_light_on);
	else
		callset_invoke (rl_arrow_light_off);
}//end of function


CALLSET_ENTRY (arrow_handler, all_arrow_update) {
	callset_invoke(center_ramp_arrow_update);
	callset_invoke(l_ramp_arrow_update);
	callset_invoke(r_ramp_arrow_update);
	callset_invoke(s_ramp_arrow_update);
	callset_invoke(l_orb_arrow_update);
	callset_invoke(r_orb_arrow_update);
	callset_invoke(uground_arrow_update);
}//end of function
