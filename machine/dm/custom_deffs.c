/*
 * demolition man
 * custom_deffs.c
 *
 * written by James Cardona
 *
 * */


#include <freewpc.h>
#include "dm/global_constants.h"

//constants
const U8 	ball_save_TotalNumOfSounds = 6; //num between 0 and N-1 == N total
const sound_code_t ball_save_SoundsArray[] = {	SPCH_AINT_OVER_YET,		SPCH_DONT_MOVE_PHOENIX,		SPCH_DONT_MOVE,
												SPCH_SECOND_CHANCE,		SPCH_HEADS_UP,				SPCH_NEXT_SHOT };



const U8 	shoot_reminder_TotalNumOfSounds = 11; //num between 0 and N-1 == N total
const sound_code_t shoot_reminder_SoundsArray[] = {	SPCH_COME_GET_ME, 		SPCH_LETS_GO2, 			SPCH_MOVE_IT_SLY,
													SPCH_DRIVE, 			SPCH_GO_SLY, 			SPCH_DONT_YOU_WANT_SOMETHING_HAPPEN,
													SPCH_DO_SOMETHING, 		SPCH_MOVE, 				SPCH_MOVE_IT,
													SPCH_GO_WES, 			SPCH_NOW };

//local variables

//external variables
extern U8 	rollover_bonus_multiplier;
extern U8 	underground_shots_made;
extern U8 	NumBallsNeededForNextMB;
extern U8 	NumBallsFrozen;

//prototypes

/****************************************************************************
 *
 * BALL_SAVE - see ballsave.c
 *
 ****************************************************************************/
//DEFF_BALL_SAVE - OVERRIDES stock ball save
void ball_save_effect_deff(void) {
	U8 ball_save_SoundCounter;
	ball_save_SoundCounter = random_scaled(ball_save_TotalNumOfSounds);//from kernal/random.c
	sound_start (ST_SPEECH, ball_save_SoundsArray[ball_save_SoundCounter], SL_4S, PRI_GAME_QUICK5);
	dmd_alloc_low_clean ();
	sprintf ("BALL SAVE");
	font_render_string_center (&font_lithograph, DMD_BIG_CX_Top, DMD_BIG_CY_Top, sprintf_buffer);
	dmd_show_low ();
	task_sleep_sec (2);
	deff_exit ();
}//end of deff




/****************************************************************************
 *
 * shoot reminder - see shootalert.c
 *
 ****************************************************************************/
//DEFF_shoot reminder - OVERRIDES stock call
void plunge_ball_effect_deff(void) {
	U8 shoot_reminder_SoundCounter;
	shoot_reminder_SoundCounter = random_scaled(shoot_reminder_TotalNumOfSounds);//from kernal/random.c
	sound_start (ST_SPEECH, shoot_reminder_SoundsArray[shoot_reminder_SoundCounter], SL_4S, PRI_GAME_QUICK5);
	dmd_alloc_low_clean ();
	sprintf ("PLUNGE BALL");
	font_render_string_center (&font_lithograph, DMD_BIG_CX_Top, DMD_BIG_CY_Top, sprintf_buffer);
	dmd_show_low ();
	task_sleep_sec (2);
	deff_exit ();
}//end of deff







/****************************************************************************
 *
 * trouble shooting
 *
 ****************************************************************************/
CALLSET_ENTRY (custom_deffs, sw_left_handle_button) {deff_start (DEFF_TROUBLESHOOTING);}

void troubleshooting_deff(void) {
	dmd_alloc_low_clean ();
	if (in_game) sprintf ("IG"); 			font_render_string_center (&font_var5, 8, 5, sprintf_buffer);
	if (in_live_game) sprintf ("ILG"); 		font_render_string_center (&font_var5, 25, 5, sprintf_buffer);
	#if defined(MACHINE_LAUNCH_SWITCH)
		sprintf ("MLSW");
	#endif
												font_render_string_center (&font_var5, 50, 5, sprintf_buffer);
	#if defined(MACHINE_LAUNCH_SOLENOID)
		sprintf ("SOL");
	#endif
												font_render_string_center (&font_var5, 80, 5, sprintf_buffer);
	#if 	defined(MACHINE_SHOOTER_SWITCH)
		sprintf ("SHT");
	#endif
												font_render_string_center (&font_var5, 110, 5, sprintf_buffer);
	#if defined(INCLUDE_AUTOPLUNGER)
		sprintf ("PL");
	#endif
												font_render_string_center (&font_var5, 8, 11, sprintf_buffer);
	#if defined(HAVE_AUTO_SERVE)
		sprintf ("SRV");
	#endif
											font_render_string_center (&font_var5, 25 , 11, sprintf_buffer);
if (global_flag_test (GLOBAL_FLAG_BALL_AT_PLUNGER)) sprintf ("BALL");
											font_render_string_center (&font_var5, 50 , 11, sprintf_buffer);
	dmd_show_low ();
	task_sleep_sec (4);
	deff_exit ();
}//end of deff

