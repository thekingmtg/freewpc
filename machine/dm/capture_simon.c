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
//constants

//local variables
U8			capture_simon_SoundCounter;
U8			capture_simon_shots_made;
U8 			capture_simon_mode_counter;
U8 			capture_simon_modes_achieved;
score_t 	capture_simon_mode_score;
__boolean 	is_capture_simon_mode_activated;

//external variables
extern 	__boolean 		inTest; //located in global_constants.c

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
	deff_start (DEFF_CAPTURESIMON_EFFECT);//under /kernel/deff.c
	sound_start (ST_MUSIC, MUS_MD_CAPTURE_SIMON, 0, SP_NORMAL);
	sound_start (ST_SAMPLE, SPCH_UNDER_ARREST, SL_1S, PRI_GAME_QUICK5);
	task_sleep(TIME_500MS);
	task_sleep(TIME_500MS);
	sound_start (ST_SAMPLE, SPCH_SO_SCARED, SL_1S, PRI_GAME_QUICK5);
	//flash lamp for a time
	lamp_tristate_flash(LM_CLAW_CAPTURE_SIMON);
	task_sleep(TIME_500MS);
	lamp_tristate_on(LM_CLAW_CAPTURE_SIMON);
	}//end of function


/****************************************************************************
 * shot made during capture_simon mode
 * call sent from ramps.c, underground.c, orbits.c
 *
 * TODO: flash appropriate arrows and keep track if shot made
 * was for appropriate arrow
 ***************************************************************************/
CALLSET_ENTRY (capture_simon, capture_simon_made) {
	++capture_simon_mode_counter;
	if ( (capture_simon_SoundCounter++ % 3) == 0 )
		sound_start (ST_SPEECH, SPCH_WES_LAUGH1, SL_2S, PRI_GAME_QUICK5);
	else if ( (capture_simon_SoundCounter++ % 3) == 1 )
		sound_start (ST_SPEECH, SPCH_WES_LAUGH2, SL_2S, PRI_GAME_QUICK5);
	else if ( (capture_simon_SoundCounter++ % 3) == 2 )
		sound_start (ST_SPEECH, SPCH_WES_LAUGH3, SL_2S, PRI_GAME_QUICK5);

	//flash lamp for a time
	//lamp_tristate_flash(LM_CENTER_RAMP_MIDDLE);
	//task_sleep(TIME_200MS);
	//lamp_tristate_off(LM_CENTER_RAMP_INNER);
	++capture_simon_shots_made;
	//IF FINAL CAPTURE SIMON SHOT MADE
	if (capture_simon_shots_made > 2) {
		score (SC_50M);
		is_capture_simon_mode_activated = FALSE;
		//return to normal music
		sound_start (ST_MUSIC, MUS_BG, 0, SP_NORMAL);
		deff_start (DEFF_CAPTURESIMON_EFFECT);//under /kernel/deff.c
		}
	else deff_start (DEFF_CAPTURESIMON_EFFECT);//under /kernel/deff.c

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
			break;
	}//end of switch
}//end of function



/****************************************************************************
 * DMD display and sound effects
 ****************************************************************************/
void capturesimon_effect_deff(void) {
	dmd_alloc_low_clean ();
	font_render_string_center (&font_mono5, 96, 5, "capture simon");
	//sprintf ("%d", jet_count);
	//font_render_string_center (&font_fixed10, 96, 16, sprintf_buffer);
	//if (jet_count == jet_goal)
	//	sprintf ("JET BONUS");
	//else
	//	sprintf ("BONUS AT %d", jet_goal);
	//font_render_string_center (&font_var5, 64, 26, sprintf_buffer);
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
	if (inTest) {
		if (is_capture_simon_mode_activated) sprintf ("capture simon is activated");
		else sprintf ("capture simon is not activated");
		font_render_string_center (&font_mono5, 64, 1, sprintf_buffer);
	}//end of 	if (inTest)

	sprintf ("%d capture simon modes completed", capture_simon_modes_achieved);
	font_render_string_center (&font_mono5, 64, 7, sprintf_buffer);

	sprintf ("capture simon score: %d", capture_simon_mode_score);
	font_render_string_center (&font_mono5, 64, 13, sprintf_buffer);

	if (inTest) {
		sprintf ("%d capture simon shots made", capture_simon_shots_made);
		font_render_string_center (&font_mono5, 64, 19, sprintf_buffer);
	}//end of 	if (inTest)
	//deff_exit (); is called at end of calling function - not needed here?
}//end of function

