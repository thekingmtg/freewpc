/*
 * demolition man
 * capture_simon.c
 *
 * written by James Cardona
 *
 * Location Description:
 * mode started by claw switch capture_simon
 *
 * Scoring Description: (original game)
 * The ball is dropped down a tunnel and served to the
 * right ramp just like the Underground. The seven
 * major shots are lit for 5 mil each for three shots.
 * A 25 million award for completing all three shots for a total of 40 million.

 * Scoring Description: (my rules)
 * same as above except:
 * major shots are lit for 15 mil each for three shots.
 * A 50 million award for completing all three shots for a total of 95 million.
 *
 * estimate of average capture simon mode score: 30 million to 95 million
 *
 */
#include <freewpc.h>
#include "dm/global_constants.h"

//constants

//local variables
U8			capture_simon_SoundCounter;
U8			capture_simon_shots_made;
U8 			capture_simon_mode_counter;
U8 			capture_simon_modes_achieved;
score_t 	capture_simon_mode_score;
__boolean 	is_capture_simon_mode_activated;
//track status of possible shots
__boolean 	isCapSimSideRampActive;
__boolean 	isCapSimLeftRampActive;
__boolean 	isCapSimRightRampActive;
__boolean 	isCapSimUndergroundRampActive;




//external variables

//prototypes
void capture_simon_reset (void);
void capture_simon_player_reset (void);
void capturesimon_effect_deff(void);

/****************************************************************************
 * initialize  and exit
 ***************************************************************************/
void capture_simon_reset (void) {
	capture_simon_mode_counter = 0;
	capture_simon_shots_made = 0;
	is_capture_simon_mode_activated = FALSE;
	isCapSimSideRampActive = FALSE;
	isCapSimLeftRampActive = FALSE;
	isCapSimRightRampActive = FALSE;
	isCapSimUndergroundRampActive = FALSE;
	score_zero(capture_simon_mode_score);
	}

void capture_simon_player_reset (void) {
	capture_simon_reset();
	capture_simon_modes_achieved = 0;
	capture_simon_SoundCounter = 0;
}

CALLSET_ENTRY (capture_simon, start_player) { capture_simon_player_reset(); }
CALLSET_ENTRY (capture_simon, start_ball) { capture_simon_reset(); }


/****************************************************************************
 * body
 *
 ***************************************************************************/

/****************************************************************************
 * capture_simon Claw switch activation starts mode
 *
 * TODO: verify ball was just shot into claw
 * perhaps make a claw mode enabled flag for all claw shots
 ***************************************************************************/
CALLSET_ENTRY (capture_simon, sw_claw_capture_simon) {
	score (SC_250K);
	is_capture_simon_mode_activated = TRUE;
	++capture_simon_modes_achieved;
	capture_simon_shots_made = 0;
	music_disable();
	deff_start (DEFF_CAPTURESIMON_EFFECT);//under /kernel/deff.c
	sound_start (ST_SPEECH, SPCH_UNDER_ARREST, SL_5S, PRI_GAME_QUICK5);
	lamp_tristate_flash(LM_CLAW_CAPTURE_SIMON);	//flash lamp for a time
	task_sleep(TIME_500MS);
	task_sleep(TIME_500MS);
	task_sleep(TIME_500MS);
	sound_start (ST_SPEECH, SPCH_SO_SCARED, SL_5S, PRI_GAME_QUICK5);
	lamp_tristate_on(LM_CLAW_CAPTURE_SIMON);
	task_sleep(TIME_500MS);
	task_sleep(TIME_500MS);
	task_sleep(TIME_500MS);
	music_set (MUS_MD_CAPTURE_SIMON);

	isCapSimSideRampActive = TRUE;
	lamp_tristate_flash(LM_SIDE_RAMP_ARROW);
	isCapSimLeftRampActive = TRUE;
	lamp_tristate_flash(LM_LEFT_RAMP_ARROW);
	isCapSimRightRampActive = TRUE;
	lamp_tristate_flash(LM_RIGHT_RAMP_ARROW);
	isCapSimUndergroundRampActive = FALSE;
	//music_enable();
}//end of function


/****************************************************************************
 * shot made during capture_simon mode
 * call sent from ramps.c, underground.c, orbits.c
 *
 * TODO: flash appropriate arrows and keep track if shot made
 * was for appropriate arrow
 ***************************************************************************/
CALLSET_ENTRY (capture_simon, capture_simon_made) {
	if ( (capture_simon_SoundCounter++ % 3) == 0 )
		sound_start (ST_SPEECH, SPCH_WES_LAUGH1, SL_5S, PRI_GAME_QUICK5);
	else if ( (capture_simon_SoundCounter++ % 3) == 1 )
		sound_start (ST_SPEECH, SPCH_WES_LAUGH2, SL_5S, PRI_GAME_QUICK5);
	else if ( (capture_simon_SoundCounter++ % 3) == 2 )
		sound_start (ST_SPEECH, SPCH_WES_LAUGH3, SL_5S, PRI_GAME_QUICK5);

	++capture_simon_shots_made;
	if (capture_simon_shots_made == 1) {
		isCapSimLeftRampActive = TRUE;
		isCapSimRightRampActive = TRUE;
		lamp_tristate_flash(LM_LEFT_RAMP_ARROW);
		lamp_tristate_flash(LM_RIGHT_RAMP_ARROW);
		isCapSimSideRampActive = FALSE;
		lamp_tristate_off(LM_SIDE_RAMP_ARROW);
		deff_start (DEFF_CAPTURESIMON_EFFECT);
	}
	else if (capture_simon_shots_made == 2) {
		lamp_tristate_flash(LM_SIDE_RAMP_ARROW);
		isCapSimSideRampActive = TRUE;
		isCapSimLeftRampActive = FALSE;
		isCapSimRightRampActive = FALSE;
		lamp_tristate_off(LM_LEFT_RAMP_ARROW);
		lamp_tristate_off(LM_RIGHT_RAMP_ARROW);
		deff_start (DEFF_CAPTURESIMON_EFFECT);
	}
		//IF FINAL CAPTURE SIMON SHOT MADE
	else if (capture_simon_shots_made >= 3) {
		lamp_tristate_off(LM_SIDE_RAMP_ARROW);
		lamp_tristate_off(LM_LEFT_RAMP_ARROW);
		lamp_tristate_off(LM_RIGHT_RAMP_ARROW);
		score (SC_50M);
	//	is_capture_simon_mode_activated = FALSE;
		deff_start (DEFF_CAPTURESIMON_EFFECT);
//		deff_start (DEFF_CAPTURESIMON_COMPLETED_EFFECT);
		music_set (MUS_BG); //return to normal music
		}

	switch (capture_simon_modes_achieved ){
		case 0:
			score (SC_15M);
			break;
		case 1:
			//2nd time we are in capture_simon - score differently
			score (SC_20M);
			break;
		case 2:
			//3rd time we are in capture_simon - score differently
			score (SC_25M);
			break;
		default:
			//all cases past 3rd time we are in capture_simon
			score (SC_25M);
	}//end of switch
}//end of function



/****************************************************************************
 * DMD display and sound effects
 ****************************************************************************/
void capturesimon_start_effect_deff(void) {
	dmd_alloc_low_clean ();
	font_render_string_center (&font_fixed6, DMD_BIG_CX_Top, DMD_BIG_CY_Top, "CAPTURE SIMON");
	font_render_string_center (&font_fixed6, DMD_BIG_CX_Bot, DMD_BIG_CY_Bot, "SHOOT ARROWS");
	dmd_show_low ();
	task_sleep_sec (2);
	deff_exit ();
	}//end of mode_effect_deff



void capturesimon_effect_deff(void) {
	dmd_alloc_low_clean ();
	font_render_string_center (&font_fixed6, DMD_BIG_CX_Top, DMD_BIG_CY_Top, "CAPTURE SIMON");
	sprintf ("%d", capture_simon_shots_made);
	font_render_string_center (&font_fixed6, DMD_BIG_CX_Bot, DMD_BIG_CY_Bot, sprintf_buffer);
	dmd_show_low ();
	task_sleep_sec (2);
	deff_exit ();
	}//end of mode_effect_deff



void capturesimon_completed_effect_deff(void) {
	dmd_alloc_low_clean ();
	font_render_string_center (&font_fixed6, DMD_BIG_CX_Top, DMD_BIG_CY_Top, "CAPTURE SIMON");
	font_render_string_center (&font_fixed6, DMD_BIG_CX_Bot, DMD_BIG_CY_Bot, "COMPLETED");
	dmd_show_low ();
	task_sleep_sec (2);
	deff_exit ();
	}//end of mode_effect_deff

/****************************************************************************
 * status display
 *
 * called from common/status.c automatically whenever either flipper button
 * is held for 4 seconds or longer.  since called by callset, order of
 * various status reports will be random depending upon call stack
****************************************************************************/
CALLSET_ENTRY (capture_simon, status_report){
		if (is_capture_simon_mode_activated) sprintf ("CAPTURE SIMON");
		font_render_string_left (&font_fixed6, DMD_BIG_CX_Top, DMD_BIG_CY_Top, sprintf_buffer);

//	sprintf ("%d CAPTURE SIMON modes completed", capture_simon_modes_achieved);
//	font_render_string_center (&font_mono5, 64, 7, sprintf_buffer);

//	sprintf ("CAPTURE SIMON score: %d", capture_simon_mode_score);
//	font_render_string_center (&font_mono5, 64, 13, sprintf_buffer);

//		sprintf ("%d CAPTURE SIMON shots made", capture_simon_shots_made);
//		font_render_string_center (&font_mono5, 64, 19, sprintf_buffer);

		//deff_exit (); is called at end of calling function - not needed here?
}//end of function

