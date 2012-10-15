/*
 * demolition man
 * inlanes.c
 * 
 * written by James Cardona
 *
 * Location Description:
 * inlanes are lanes that feed to flippers as opposed to outlanes which feed
 * to the drain
 *
 * Scoring Description: (original game)
 * an unlit or lit lane score 5770
 * if lit, you recieve whatever the light says - access claw on left
 * will move the claw diverter to open it - when the diverter moves, the
 * light goes out
 * on the right inlane is quick freeze, getting this inlane will then
 * light quick freeze light on left ramp and to freeze a ball you must then shoot
 * the left ramp.  The light quick freeze inlane light does not go out until
 * the left ramp is shot and the ball is frozen
 *
 * access claw can be lit by completing MTL rollovers,
 *
 * Depending on machine settings, One, Two, or Three sets of
 * standup targets must be completed to light Quick Freeze, depending on
 * whether Quick Freeze is set to easy, medium, or hard.
 *
 * Scoring Description: (my rules)
 * same as above
 *
 *
 * */
/* CALLSET_SECTION (inlanes, __machine2__) */

#include <freewpc.h>
#include "dm/global_constants.h"

//local variables
__boolean 	left_inlane_Access_Claw_activated;
__boolean 	right_inlane_Light_Quick_Freeze_activated;

//external variables


/****************************************************************************
 * initialize  and exit
 ***************************************************************************/
void new_player_inlanes_reset (void) {
	left_inlane_Access_Claw_activated = TRUE;
	lamp_tristate_on(LM_ACCESS_CLAW);
	right_inlane_Light_Quick_Freeze_activated = TRUE;
	lamp_tristate_on(LM_LIGHT_QUICK_FREEZE);
}//end of reset

CALLSET_ENTRY (inlanes, start_player) {  new_player_inlanes_reset(); }


/****************************************************************************
 * left inlane
 *
 * access claw can be lit by completing MTL rollovers, random award from top hole,
 * random award from computer (subway shot) or completing certain number of
 * arrow shots
 ***************************************************************************/
void access_claw_light_on(void) {
	left_inlane_Access_Claw_activated = TRUE;
	lamp_tristate_flash(LM_ACCESS_CLAW);
	task_sleep (TIME_500MS);
	lamp_tristate_on (LM_ACCESS_CLAW);
}//end of function



void access_claw_light_off(void) {
	left_inlane_Access_Claw_activated = FALSE;
	lamp_tristate_flash(LM_ACCESS_CLAW);
	task_sleep (TIME_500MS);
	lamp_tristate_off (LM_ACCESS_CLAW);
}//end of function



CALLSET_ENTRY (inlanes, sw_left_inlane) {
	score(SC_5770);
	sound_start (ST_SAMPLE, INLANE_SOUND, SL_2S, PRI_GAME_QUICK5);
	if (	!flag_test(FLAG_IS_R_RAMP_CLAWREADY)
		&& 	left_inlane_Access_Claw_activated)	{
		rramp_clawready_on();//at ramps.c - diverter moved there
		access_claw_light_off();
	//	leff_start (LEFF_CRYOCLAW);//LIGHTING EFFECTS
		sound_start (ST_SPEECH, SPCH_CRYOCLAW_ACTIVATED, SL_4S, PRI_GAME_QUICK5);
		deff_start (DEFF_CLW_INLANES_EFFECT);
	}//end of if
}//end of function



/****************************************************************************
 * right inlane
 ***************************************************************************/
void light_quick_freeze_light_on (void) {
	right_inlane_Light_Quick_Freeze_activated = TRUE;
	sound_start (ST_SPEECH, SPCH_QUICK_FREEZE_ACTIVATED, SL_4S, PRI_GAME_QUICK5);
	lamp_tristate_flash(LM_LIGHT_QUICK_FREEZE);
	task_sleep (TIME_500MS);
	lamp_tristate_on (LM_LIGHT_QUICK_FREEZE);
}//end of function



void light_quick_freeze_light_off(void) {
	right_inlane_Light_Quick_Freeze_activated = FALSE;
	lamp_tristate_flash(LM_LIGHT_QUICK_FREEZE);
	task_sleep (TIME_500MS);
	lamp_tristate_off (LM_LIGHT_QUICK_FREEZE);
}//end of function



CALLSET_ENTRY (inlanes, sw_right_inlane) {
	score(SC_5770);
	sound_start (ST_SAMPLE, INLANE_SOUND, SL_2S, PRI_GAME_QUICK5);
	if (!flag_test(FLAG_IS_LRAMP_QUICKFREEZE_ACTIVATED) && right_inlane_Light_Quick_Freeze_activated) {
		//leff_start (LEFF_QUICK_FREEZE);//LIGHTING EFFECTS
		activate_left_ramp_quickfreeze();//at ramps.c
		deff_start (DEFF_QF_INLANES_EFFECT);
	}
}//end of function




/****************************************************************************
 * lighting effects
 ****************************************************************************/

static void quick_freeze_subtask (void) {
	U8 i;
	for (i = 0; i < 5; i++) {
		leff_toggle (LAMPLIST_LEFT_RAMP_AWARDS);
		task_sleep (TIME_100MS);
	}//end of loop
	task_exit ();
}//end of function

void quick_freeze_leff (void) {
	gi_leff_enable (PINIO_GI_STRINGS);
	leff_create_peer (quick_freeze_subtask);
	task_sleep (TIME_500MS);
	gi_leff_enable (PINIO_GI_STRINGS);
	leff_exit ();
}//end of function


static void cryoclaw_subtask (void) {
	U8 i;
	for (i = 0; i < 5; i++) {
		leff_toggle (LAMPLIST_RIGHT_RAMP_AWARDS);
		task_sleep (TIME_100MS);
	}//end of loop
	task_exit ();
}//end of function

void cryoclaw_leff (void) {
	gi_leff_enable (PINIO_GI_STRINGS);
	leff_create_peer (cryoclaw_subtask);
	task_sleep (TIME_500MS);
	gi_leff_enable (PINIO_GI_STRINGS);
	leff_exit ();
}//end of function



/****************************************************************************
 *
 * display effects
 *
 ****************************************************************************/
void clw_inlanes_effect_deff(void) {
	dmd_alloc_low_clean ();
	dmd_sched_transition (&trans_scroll_down);
	font_render_string_center (&font_fixed6, DMD_MIDDLE_X, DMD_BIG_CY_Top, "CRYO CLAW");
	font_render_string_center (&font_fixed6, DMD_MIDDLE_X, DMD_BIG_CY_Bot, "RIGHT RAMP");
	dmd_show_low ();
	task_sleep_sec (2);
	deff_exit ();
}//end of mode_effect_deff



void qf_inlanes_effect_deff(void) {
	dmd_alloc_low_clean ();
	dmd_sched_transition (&trans_scroll_down);
	font_render_string_center (&font_fixed6, DMD_MIDDLE_X, DMD_BIG_CY_Top, "QUICK FREEZE");
	font_render_string_center (&font_fixed6, DMD_MIDDLE_X, DMD_BIG_CY_Bot, "LEFT RAMP");
	dmd_show_low ();
	task_sleep_sec (2);
	deff_exit ();
}//end of mode_effect_deff





