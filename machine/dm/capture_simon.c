/*
 * demolition man
 * capture_simon.c
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
 *
 * TODO: write scoring code
 * TODO: potentially we can make a 2nd and 3rd capture_simon mode that score differently
 *
 */
#include <freewpc.h>
//constants

//local variables
U8			capture_simon_SoundCounter;
U8 			capture_simon_mode_counter;
U8 			capture_simon_modes_achieved;
score_t 	capture_simon_mode_score;
__boolean 	capture_simon_mode_activated;

//prototypes
void capture_simon_reset (void);
void capture_simon_player_reset (void);

/****************************************************************************
 * initialize  and exit
 ***************************************************************************/
void capture_simon_reset (void) {
	capture_simon_mode_counter = 0;
	capture_simon_mode_activated = FALSE;
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
	capture_simon_mode_activated = TRUE;
	++capture_simon_modes_achieved;
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

	switch (capture_simon_modes_achieved ){
		case 0:
			//as it is right now we score 6 mill + 1 million for each extra shot
			score (SC_5M);
			break;
		case 1:
			//2nd time we are in capture_simon - score differently
			score (SC_6M);
			break;
		case 2:
			//3rd time we are in capture_simon - score differently
			score (SC_7M);
			break;
		default:
			//all cases past 3rd time we are in capture_simon
			score (SC_7M);
			break;
	}//end of switch

	//TODO: display effects
}//end of function



/****************************************************************************
 * DMD display and sound effects
 ****************************************************************************/
