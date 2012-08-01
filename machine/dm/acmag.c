/*
 * demolition man
 * acmag.c
 *
 * Location Description:
 * mode started by claw switch acmag -points scored by center ramp.
 *
 * Scoring Description: (original game)
 * counts up as time goes down from 6 million to 13.5 million, typically
 * about a 20 second mode.  Only shot that scores is the very wide
 * center ramp
 *
 * Scoring Description: (my rules)
 * same as above except:
 *
 * TODO: potentially we can make a 2nd and 3rd acmag mode that score differently
 * TODO: 20 second timer - currently mode never ends until end of ball
 */
#include <freewpc.h>
//constants

//local variables
U8 			acmag_mode_counter;
U8 			acmag_modes_achieved;
score_t 	acmag_mode_score;
__boolean 	acmag_mode_activated;

//prototypes
void acmag_reset (void);
void acmag_player_reset (void);

/****************************************************************************
 * initialize  and exit
 ***************************************************************************/
void acmag_reset (void) {
	acmag_mode_counter = 0;
	acmag_mode_activated = FALSE;
	score_zero(acmag_mode_score);
	}

void acmag_player_reset (void) {
	acmag_reset();
	acmag_modes_achieved = 0;
}

CALLSET_ENTRY (acmag, start_player) { acmag_player_reset(); }
CALLSET_ENTRY (acmag, start_ball) { acmag_reset(); }


/****************************************************************************
 * body
 *
 ***************************************************************************/

/****************************************************************************
 * ACMAG Claw switch activation starts mode
 *
 * TODO: verify ball was just shot into claw
 * perhaps make a claw mode enabled flag for all claw shots
 ***************************************************************************/
CALLSET_ENTRY (acmag, sw_claw_acmag) {
	score (SC_250K);
	acmag_mode_activated = TRUE;
	++acmag_modes_achieved;
	sound_start (ST_MUSIC, MUS_ACMAG, 0, SP_NORMAL);
	sound_start (ST_SPEECH, SPCH_ACMAG_ACTIVATED, SL_1S, PRI_GAME_QUICK5);
	//flash lamp for a time
	lamp_tristate_flash(LM_CLAW_ACMAG);
	task_sleep(TIME_500MS);
	lamp_tristate_off(LM_CLAW_ACMAG);
	}//end of function


/****************************************************************************
 * center ramp shot made during acmag mode
 * call sent from ramps.c
 ***************************************************************************/
CALLSET_ENTRY (acmag, acmag_made) {
	++acmag_mode_counter;
	sound_start (ST_SAMPLE, EXPLOSION, SL_1S, PRI_GAME_QUICK5);
	//flash lamp for a time
	lamp_tristate_flash(LM_CENTER_RAMP_MIDDLE);
	lamp_tristate_flash(LM_CENTER_RAMP_OUTER);
	lamp_tristate_flash(LM_CENTER_RAMP_INNER);
	task_sleep(TIME_200MS);
	lamp_tristate_off(LM_CENTER_RAMP_MIDDLE);
	lamp_tristate_off(LM_CENTER_RAMP_OUTER);
	lamp_tristate_off(LM_CENTER_RAMP_INNER);
	//TODO: score rolls up as time goes?
	//score_add(acmag_mode_score, score_table[SC_6M]);

	switch (acmag_modes_achieved ){
		case 0:
			//as it is right now we score 6 mill + 1 million for each extra shot
			score (SC_6M);
			score (acmag_mode_counter * SC_1M);
			break;
		case 1:
			//2nd time we are in acmag - score differently
			score (SC_7M);
			score (acmag_mode_counter * SC_2M);
			break;
		case 2:
			//3rd time we are in acmag - score differently
			score (SC_8M);
			score (acmag_mode_counter * SC_3M);
			break;
		default:
			//all cases past 3rd time we are in acmag
			score (SC_8M);
			score (acmag_mode_counter * SC_3M);
			break;
	}//end of switch

	//TODO: display effects
}//end of function



/****************************************************************************
 * DMD display and sound effects
 ****************************************************************************/
