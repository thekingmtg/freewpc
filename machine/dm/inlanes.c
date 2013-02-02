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
/* CALLSET_SECTION (inlanes, __machine__) */

#include <freewpc.h>
#include "dm/global_constants.h"

//local variables
__local__ __boolean 	left_inlane_Access_Claw_activated;
__local__ __boolean 	right_inlane_Light_Quick_Freeze_activated;
__local__ U8 increment_access_claw_light;
__local__ U8 increment_quick_freeze_light;
//external variables

//prototypes
void left_inlane_task (void);



/****************************************************************************
 * initialize  and exit
 ***************************************************************************/
void new_player_inlanes_reset (void) {
	left_inlane_Access_Claw_activated = TRUE;
	lamp_tristate_on(LM_ACCESS_CLAW);
	right_inlane_Light_Quick_Freeze_activated = TRUE;
	lamp_tristate_on(LM_LIGHT_QUICK_FREEZE);
	increment_access_claw_light = 0;
	increment_quick_freeze_light = 1;
}//end of reset



CALLSET_ENTRY (inlanes, start_player) {  new_player_inlanes_reset(); }

CALLSET_ENTRY (inlanes, start_ball) {
	if (left_inlane_Access_Claw_activated) lamp_tristate_on (LM_ACCESS_CLAW);
	if (right_inlane_Light_Quick_Freeze_activated) lamp_tristate_on (LM_LIGHT_QUICK_FREEZE);
}//end of function




/****************************************************************************
 * left inlane
 *
 * access claw can be lit by completing MTL rollovers, random award from top hole,
 * random award from computer (subway shot) or completing certain number of
 * arrow shots
 ***************************************************************************/
void increment_access_claw_light_on(void) {
	increment_access_claw_light++;
	if (!left_inlane_Access_Claw_activated) access_claw_light_on();
}//end of function



void check_access_claw_relight (void) {
	if (increment_access_claw_light) access_claw_light_on();
}//end of function



void access_claw_light_on(void) {
	increment_access_claw_light--;
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


//this task is here to detect a HUXLEY shot
void left_inlane_task (void) { task_sleep_sec(1); task_sleep (TIME_500MS); task_exit(); }



U8 light_tester;
CALLSET_ENTRY (inlanes, sw_left_inlane) {
	if (++light_tester >=8) light_tester = 0;
	switch (light_tester) {
			case 0: leff_start (LEFF_INLANE_QUICK_FREEZE); break;
			case 1: leff_start (LEFF_INLANE_CRYOCLAW); break;
//			case 2: leff_start (LEFF_GI_CYCLE); break;
//			case 3: leff_start (LEFF_NO_GI_TWOSEC); break;
//			case 4: leff_start (LEFF_NO_GI_ONESEC); break;
//			case 5: leff_start (LEFF_TURN_ON_GI); break;
//			case 6: leff_start (LEFF_FLASH_ALL); break;
//			case 7: leff_start (LEFF_FLASH_GI); break;
	}//end of switch

	task_create_gid1 (GID_LEFT_INLANE_MADE, left_inlane_task);
	score(INLANE_SCORE);
	sound_start (ST_SAMPLE, INLANE_SOUND, SL_2S, PRI_GAME_QUICK5);
	if (	!flag_test(FLAG_IS_R_RAMP_CLAWREADY)
		&&	!flag_test(FLAG_BACK_IN_THE_FRIDGE_RUNNING)
		&& 	left_inlane_Access_Claw_activated
		&&	!flag_test(FLAG_IS_CARCHASE_MODE_RUNNING)
		&&	!flag_test(FLAG_IS_EXPLODE_MODE_RUNNING)
		&&	!flag_test(FLAG_IS_FORTRESS_MB_RUNNING)
		&&	!flag_test(FLAG_IS_MUSEUM_MB_RUNNING)
		&&	!flag_test(FLAG_IS_CRYOPRISON_MB_RUNNING)
		&&	!flag_test(FLAG_IS_WASTELAND_MB_RUNNING)
		&&	!flag_test (FLAG_IS_CAPSIM_RIGHTRAMP_ACTIVATED)
		&&	!flag_test (FLAG_IS_HUXLEY_RUNNING)
		&&	!flag_test(FLAG_IS_DEMOTIME_RUNNING) ){
		rramp_clawready_on();//at ramps.c - diverter moved there
		access_claw_light_off();
		sound_start (ST_SPEECH, SPCH_CRYOCLAW_ACTIVATED, SL_4S, PRI_GAME_QUICK5);
		deff_start (DEFF_CLW_INLANES_EFFECT);
	}//end of if
}//end of function



/****************************************************************************
 * right inlane
 ***************************************************************************/
void increment_light_quick_freeze_light (void) {
	increment_quick_freeze_light++;
	if (!right_inlane_Light_Quick_Freeze_activated) access_claw_light_on();
}//end of function



void light_quick_freeze_light_on (void) {
	right_inlane_Light_Quick_Freeze_activated = TRUE;
	sound_start (ST_SPEECH, SPCH_QUICK_FREEZE_ACTIVATED, SL_4S, PRI_GAME_QUICK5);
	lamp_tristate_flash(LM_LIGHT_QUICK_FREEZE);
	task_sleep (TIME_500MS);
	lamp_tristate_on (LM_LIGHT_QUICK_FREEZE);
}//end of function


//called by lock_freeze...c when left ramp is hit and freeze is inc'ed
void light_quick_freeze_light_off(void) {
	if (--increment_quick_freeze_light <= 0){
		increment_quick_freeze_light = 0;
		right_inlane_Light_Quick_Freeze_activated = FALSE;
		lamp_tristate_flash(LM_LIGHT_QUICK_FREEZE);
		task_sleep (TIME_500MS);
		lamp_tristate_off (LM_LIGHT_QUICK_FREEZE);
	}
}//end of function



CALLSET_ENTRY (inlanes, sw_right_inlane) {
	score(INLANE_SCORE);
	sound_start (ST_SAMPLE, INLANE_SOUND, SL_2S, PRI_GAME_QUICK5);
	if (!flag_test(FLAG_IS_LRAMP_QUICKFREEZE_ACTIVATED) && right_inlane_Light_Quick_Freeze_activated) {
		activate_left_ramp_quickfreeze();//at ramps.c
		deff_start (DEFF_QF_INLANES_EFFECT);
	}
}//end of function


/****************************************************************************
 *
 * sound effects
 *
 ****************************************************************************/




/****************************************************************************
 *
 * lighting effects
 *
 ****************************************************************************/
void inlane_quick_freeze_leff (void) {
	U8 i;
	for (i = 0; i < 5; i++) {
		lamplist_apply (LAMPLIST_LEFT_RAMP_AWARDS, leff_toggle);
		task_sleep (TIME_66MS);
	}//end of loop
	leff_exit ();
}//end of function



void inlane_cryoclaw_leff (void) {
	U8 i;
	for (i = 0; i < 5; i++) {
		lamplist_apply (LAMPLIST_RIGHT_RAMP_AWARDS, leff_toggle);
		task_sleep (TIME_33MS);
	}//end of loop
	leff_exit ();
}//end of function




/****************************************************************************
 *
 * display effects
 *
 ****************************************************************************/
//the image is 4 color in 2 panes, so we must increment by 2's not 1's
U8 inlanes_cryoclaw_effect_deff_table[] = {	0, 2, 4, 4, 2, 0, 				//left stays up, right goes down and up --6 frames
									0, 6, 8, 10, 10, 8, 6, 0 }; 	 		//left goes down and up, right stays up --8 frames

void clw_inlanes_effect_deff(void) {
	U8 i;
	for (i = 0; i < 14; i++) {
			dmd_map_overlay ();
			dmd_clean_page_low ();
			font_render_string_center (&font_fixed6, DMD_MIDDLE_X, DMD_BIG_CY_Top, "CRYO CLAW");
			font_render_string_center (&font_fixed6, DMD_MIDDLE_X, DMD_BIG_CY_Bot, "RIGHT RAMP");
			dmd_text_outline ();
			dmd_alloc_pair ();
			frame_draw(IMG_CRYOPRISON_RUN_0 + inlanes_cryoclaw_effect_deff_table[i]);
			dmd_overlay_outline ();
			dmd_show2 ();
		task_sleep (TIME_100MS);
	}//END OF LOOP
	task_sleep_sec (1);
	deff_exit ();
}//end of mode_effect_deff





void qf_inlanes_effect_deff(void) {
	U16 fno;
	for (fno = IMG_FREEZE_A_START; fno <= IMG_FREEZE_A_END; fno += 2) {
		dmd_alloc_pair_clean ();
		frame_draw(fno);
		dmd_show2 ();
		task_sleep (TIME_100MS);
	}//end of inner loop
	dmd_alloc_pair_clean ();// Clean both pages
		dmd_map_overlay ();
		dmd_clean_page_low ();
		font_render_string_center (&font_fixed6, DMD_MIDDLE_X, DMD_BIG_CY_Top, "QUICK FREEZE");
		font_render_string_center (&font_fixed6, DMD_MIDDLE_X, DMD_BIG_CY_Bot, "LEFT RAMP");
		dmd_text_outline ();
		dmd_alloc_pair ();
		frame_draw(IMG_FREEZE_A_END);
		dmd_overlay_outline ();
		dmd_show2 ();
	task_sleep_sec (1);
	deff_exit ();
}//end of mode_effect_deff

