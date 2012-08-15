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
//constants

//local variables
U8 			acmag_mode_shots_made;
U8 			acmag_modes_achieved;
U8 			acmag_modes_completed;
score_t 	acmag_mode_score;
score_t 	acmag_mode_score_total_score;
__boolean 	is_acmag_mode_activated;

//external variables
extern __boolean 	inTest; //located in global_constants.c

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
	score (SC_250K);
	is_acmag_mode_activated = TRUE;
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
	++acmag_mode_shots_made;
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
	//TODO: complete mode if xx number of shots made?
	//increment acmag_modes_completed

	switch (acmag_modes_achieved ){
		case 0:
			//as it is right now we score xx mill + xx million for each extra shot
			score (SC_15M);
			score_add (acmag_mode_score, score_table[SC_15M]);
			score_add (acmag_mode_score_total_score, score_table[SC_15M]);
			//score (acmag_mode_shots_made * SC_5M);
			//cannot do this here - is there an easier way than a do loop
			//score_add (acmag_mode_score, acmag_mode_shots_made * score_table[SC_5M]);
			//score_add (acmag_mode_score_total_score, acmag_mode_shots_made * score_table[SC_5M]);
			break;
		case 1:
			//2nd time we are in acmag - score differently
			score (SC_20M);
			score_add (acmag_mode_score, score_table[SC_20M]);
			score_add (acmag_mode_score_total_score, score_table[SC_20M]);
			//score (acmag_mode_shots_made * SC_7M);
			break;
		case 2:
			//3rd time we are in acmag - score differently
			score (SC_25M);
			score_add (acmag_mode_score, score_table[SC_25M]);
			score_add (acmag_mode_score_total_score, score_table[SC_25M]);
			//score (acmag_mode_shots_made * SC_8M);
			break;
		default:
			//all cases past 3rd time we are in acmag
			score (SC_25M);
			score_add (acmag_mode_score, score_table[SC_25M]);
			score_add (acmag_mode_score_total_score, score_table[SC_25M]);
			//score (acmag_mode_shots_made * SC_8M);
			break;
	}//end of switch

	deff_start (DEFF_ACMAG_EFFECT);//under /kernel/deff.c
}//end of function



/****************************************************************************
 * DMD display and sound effects
 ****************************************************************************/
void acmag_effect_deff(void) {
	dmd_alloc_low_clean ();
	font_render_string_center (&font_mono5, 96, 5, "acmag");
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
CALLSET_ENTRY (acmag, status_report){
	if (inTest) {
		if (is_acmag_mode_activated) sprintf ("acmag mode is activated");
		else sprintf ("acmag mode is not activated");
		font_render_string_center (&font_mono5, 64, 1, sprintf_buffer);
	}//end of 	if (inTest)

	sprintf ("%d acmag modes completed", acmag_modes_achieved);
	font_render_string_center (&font_mono5, 64, 7, sprintf_buffer);

	sprintf ("acmag score: %d", acmag_mode_score);
	font_render_string_center (&font_mono5, 64, 13, sprintf_buffer);

	if (inTest) {
		sprintf ("%d acmag shots made", acmag_mode_shots_made);
		font_render_string_center (&font_mono5, 64, 19, sprintf_buffer);
	}//end of 	if (inTest)
	//deff_exit (); is called at end of calling function - not needed here?
}//end of function

