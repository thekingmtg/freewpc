/*
 * demolition man
 * acmag.c
 *
 * written by James Cardona
 *
 * Location Description:
 * mode started by claw switch acmag -points scored by center ramp.
 *
 * Scoring Description: (original game)
 * counts up as time goes down from 5 million to 12.5 million, typically
 * about a 20 second mode.  Only shot that scores is the very wide
 * center ramp
 *
 * Scoring Description: (my rules)
 * same as above except:
 * scoring is higher to balance with other modes
 * 2nd and 3rd acmag mode score higher to encourage doing mode more than once
 * TODO: 20 second timer - currently mode never ends until end of ball
 * TODO: currently score does not change over time
 *
 * estimate of average acmag mode score: 60 million to 125 million
 *
 *
 *
 */
#include <freewpc.h>
#include "dm/global_constants.h"

//constants

//local variables
U8 			acmag_mode_shots_made;
U8 			acmag_modes_achieved;
U8 			acmag_modes_completed;
score_t 	acmag_mode_score;
score_t 	acmag_mode_score_total_score;
__boolean 	is_acmag_mode_activated;

//external variables

//prototypes
void acmag_reset (void);
void acmag_player_reset (void);
void acmag_effect_deff(void);

/****************************************************************************
 * initialize  and exit
 ***************************************************************************/
void acmag_reset (void) {
	acmag_mode_shots_made = 0;
	is_acmag_mode_activated = FALSE;
	score_zero(acmag_mode_score);
	}

void acmag_player_reset (void) {
	acmag_reset();
	acmag_modes_achieved = 0;
	acmag_modes_completed = 0;
	score_zero(acmag_mode_score_total_score);
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
	deff_start (DEFF_ACMAG_EFFECT);//under /kernel/deff.c
	score (SC_250K);
	is_acmag_mode_activated = TRUE;
	++acmag_modes_achieved;
	sound_start (ST_SPEECH, SPCH_ACMAG_ACTIVATED, SL_5S, PRI_GAME_QUICK5);
	//flash lamp for a time
	lamp_tristate_flash(LM_CLAW_ACMAG);
	task_sleep(TIME_500MS);
	lamp_tristate_off(LM_CLAW_ACMAG);
	music_set(MUS_ACMAG); //from sound_effect.c
}//end of function


/****************************************************************************
 * center ramp shot made during acmag mode
 * call sent from ramps.c
 ***************************************************************************/
CALLSET_ENTRY (acmag, acmag_made) {
	++acmag_mode_shots_made;
	sound_start (ST_SAMPLE, EXPLOSION, SL_2S, PRI_GAME_QUICK5);
	//flash lamp for a time
	lamp_tristate_flash(LM_CENTER_RAMP_MIDDLE);
	lamp_tristate_flash(LM_CENTER_RAMP_OUTER);
	lamp_tristate_flash(LM_CENTER_RAMP_INNER);
	task_sleep(TIME_1S);
	lamp_tristate_off(LM_CENTER_RAMP_MIDDLE);
	lamp_tristate_off(LM_CENTER_RAMP_OUTER);
	lamp_tristate_off(LM_CENTER_RAMP_INNER);
	//TODO: complete mode if xx number of shots made?

	//TODO: score rolls up as time goes?
	switch (acmag_modes_achieved ){
		case 0:
			//as it is right now we score xx mill + xx million for each extra shot
			score (SC_15M);
			score_add (acmag_mode_score, score_table[SC_15M]);
			score_add (acmag_mode_score_total_score, score_table[SC_15M]);
			break;
		case 1:
			//2nd time we are in acmag - score differently
			score (SC_20M);
			score_add (acmag_mode_score, score_table[SC_20M]);
			score_add (acmag_mode_score_total_score, score_table[SC_20M]);
			break;
		case 2:
			//3rd time we are in acmag - score differently
			score (SC_25M);
			score_add (acmag_mode_score, score_table[SC_25M]);
			score_add (acmag_mode_score_total_score, score_table[SC_25M]);
			break;
		default:
			//all cases past 3rd time we are in acmag
			score (SC_25M);
			score_add (acmag_mode_score, score_table[SC_25M]);
			score_add (acmag_mode_score_total_score, score_table[SC_25M]);
			break;
	}//end of switch

	deff_start (DEFF_ACMAG_EFFECT);//under /kernel/deff.c
}//end of function



/****************************************************************************
 * DMD display and sound effects
 ****************************************************************************/
void acmag_effect_deff(void) {
	dmd_alloc_low_clean ();
	font_render_string_center (&font_fixed10, DMD_BIG_CX_Top, DMD_BIG_CY_Top, "ACMAG");
	sprintf ("%d MADE", acmag_mode_shots_made);
	font_render_string_center (&font_fixed6, DMD_BIG_CX_Bot, DMD_BIG_CY_Bot, sprintf_buffer);
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
CALLSET_ENTRY (acmag, status_report){
		if (is_acmag_mode_activated) sprintf ("ACMAG");
		else sprintf (" ");
		font_render_string_left (&font_fixed10, DMD_BIG_CX_Top, DMD_BIG_CY_Top,  sprintf_buffer);

//	sprintf ("%d ACMAG COMPLETED", acmag_modes_achieved);
//	font_render_string_center (&font_fixed10, 1, 16, sprintf_buffer);

//	sprintf ("acmag score: %d", acmag_mode_score);
//	font_render_string_center (&font_mono5, 64, 13, sprintf_buffer);

//		sprintf ("%d acmag shots made", acmag_mode_shots_made);
//		font_render_string_center (&font_mono5, 64, 19, sprintf_buffer);

		//deff_exit (); is called at end of calling function - not needed here?
}//end of function

