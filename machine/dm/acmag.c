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
U8			acmag_mode_timer;
score_t 	acmag_mode_score;
score_t 	acmag_mode_last_score;
score_t 	acmag_mode_next_score;
score_t 	acmag_mode_score_total_score;


//external variables

//prototypes
void acmag_reset (void);
void acmag_player_reset (void);
void acmag_effect_deff(void);
void acmag_mode_init (void);
void acmag_mode_expire (void);
void acmag_mode_exit (void);



/****************************************************************************
 * mode definition structure
 ***************************************************************************/
struct timed_mode_ops acmag_mode = {
	DEFAULT_MODE,
	.init = acmag_mode_init,
	.exit = acmag_mode_exit,
	.gid = GID_ACMAG_MODE_RUNNING,
	.music = MUS_ACMAG,
	.deff_starting = DEFF_ACMAG_START_EFFECT,
	.deff_running = DEFF_ACMAG_EFFECT,
//	.deff_ending = DEFF_ACMAG_END_EFFECT,
	.prio = PRI_GAME_MODE1,
	.init_timer = 23,
	.timer = &acmag_mode_timer,
	.grace_timer = 2,
//	.pause = system_timer_pause,
};



/****************************************************************************
 * initialize  and exit
 ***************************************************************************/
void acmag_reset (void) {
	flag_off (FLAG_IS_ACMAG_ACTIVATED);
}//end of function



void acmag_player_reset (void) {
	acmag_reset();
	acmag_modes_achieved = 0;
	acmag_modes_completed = 0;
	score_zero(acmag_mode_score_total_score);
}//end of function



void acmag_mode_init (void) {
	score (SC_250K);
	acmag_mode_shots_made = 0;
	flag_on (FLAG_IS_ACMAG_ACTIVATED);
	callset_invoke (center_ramp_arrow_update);
	++acmag_modes_achieved;
	sound_start (ST_SPEECH, SPCH_ACMAG_ACTIVATED, SL_4S, PRI_GAME_QUICK5);
	//flash lamp for a time
	lamp_tristate_flash(LM_CLAW_ACMAG);
	task_sleep(TIME_500MS);
	lamp_tristate_on(LM_CLAW_ACMAG);
	score_zero(acmag_mode_score);
	score_zero(acmag_mode_last_score);
	score_zero(acmag_mode_next_score);
	switch (acmag_modes_achieved ){
		case 1: score_add(acmag_mode_next_score, score_table[SC_15M]); break;
		case 2: score_add(acmag_mode_next_score, score_table[SC_20M]); break;
		case 3: score_add(acmag_mode_next_score, score_table[SC_25M]); break;
		default: score_add(acmag_mode_next_score, score_table[SC_25M]);
	}//end of switch
}//end of function



void acmag_mode_expire (void) {
	flag_off (FLAG_IS_ACMAG_ACTIVATED);
	callset_invoke (center_ramp_arrow_update);
}//end of function


void acmag_mode_exit (void) { acmag_mode_expire();}


/****************************************************************************
 * external event listeners
 ****************************************************************************/
CALLSET_ENTRY (acmag, music_refresh)  	{ timed_mode_music_refresh (&acmag_mode); }
CALLSET_ENTRY (acmag, end_ball) 		{ if (timed_mode_running_p(&acmag_mode) ) timed_mode_end (&acmag_mode); }
CALLSET_ENTRY (acmag, display_update) 	{ timed_mode_display_update (&acmag_mode); }

CALLSET_ENTRY (acmag, start_player) 	{ acmag_player_reset(); }
CALLSET_ENTRY (acmag, start_ball) 		{ acmag_reset(); }




/****************************************************************************
 * body
 *
 ***************************************************************************/
CALLSET_ENTRY (acmag, sw_claw_acmag) {
	timed_mode_begin (&acmag_mode);//start mode
}//end of function


 //center ramp shot made during acmag mode
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
		case 1:
			score (SC_15M);
			score_add (acmag_mode_score, score_table[SC_15M]);
			score_add (acmag_mode_score_total_score, score_table[SC_15M]);
			score_zero(acmag_mode_last_score);
			score_add(acmag_mode_last_score, score_table[SC_15M]);
			break;
		case 2:
			//2nd time we are in acmag - score differently
			score (SC_20M);
			score_add (acmag_mode_score, score_table[SC_20M]);
			score_add (acmag_mode_score_total_score, score_table[SC_20M]);
			score_zero(acmag_mode_last_score);
			score_add(acmag_mode_last_score, score_table[SC_20M]);
			break;
		case 3:
			//3rd time we are in acmag - score differently
			score (SC_25M);
			score_add (acmag_mode_score, score_table[SC_25M]);
			score_add (acmag_mode_score_total_score, score_table[SC_25M]);
			score_zero(acmag_mode_last_score);
			score_add(acmag_mode_last_score, score_table[SC_25M]);
			break;
		default:
			//all cases past 3rd time we are in acmag
			score (SC_25M);
			score_add (acmag_mode_score, score_table[SC_25M]);
			score_add (acmag_mode_score_total_score, score_table[SC_25M]);
			score_zero(acmag_mode_last_score);
			score_add(acmag_mode_last_score, score_table[SC_25M]);
			break;
	}//end of switch
	deff_start (DEFF_ACMAG_HIT_EFFECT);//under /kernel/deff.c
}//end of function



/****************************************************************************
 * DMD display and sound effects
 ****************************************************************************/
void acmag_start_effect_deff(void) {
	dmd_alloc_low_clean ();
	font_render_string_center (&font_steel, DMD_BIG_CX_Top, DMD_BIG_CY_Top, "ACMAG");
	font_render_string_center (&font_mono5, DMD_BIG_CX_Bot, DMD_BIG_CY_Bot, "CENTER RAMP");
	dmd_show_low ();
	task_sleep_sec (2);
	deff_exit ();
}//end of mode_effect_deff



void acmag_hit_effect_deff(void) {
	dmd_alloc_low_clean ();
	font_render_string_center (&font_steel, DMD_BIG_CX_Top, DMD_BIG_CY_Top, "ACMAG");
	sprintf_score (acmag_mode_last_score);
	font_render_string_center (&font_term6, DMD_BIG_CX_Bot, DMD_BIG_CY_Bot, sprintf_buffer);
	dmd_show_low ();
	task_sleep_sec (2);
	deff_exit ();
}//end of mode_effect_deff



void acmag_effect_deff(void) {
	for (;;) {
		dmd_alloc_low_clean ();
		font_render_string_center (&font_steel, DMD_BIG_CX_Top, DMD_BIG_CY_Top, "ACMAG");
		sprintf ("%d SEC LEFT,  %d HIT", acmag_mode_timer, acmag_mode_shots_made);
		font_render_string_center (&font_mono5, DMD_SMALL_CX_3, DMD_SMALL_CY_3, sprintf_buffer);
		sprintf_score (acmag_mode_next_score);
		font_render_string_center (&font_mono5, DMD_SMALL_CX_4, DMD_SMALL_CY_4, sprintf_buffer);
		dmd_show_low ();
		task_sleep (TIME_200MS);
	}//END OF ENDLESS LOOP
}//end of mode_effect_deff





void acmag_end_effect_deff(void) {
	dmd_alloc_low_clean ();
	font_render_string_center (&font_steel, DMD_BIG_CX_Top, DMD_BIG_CY_Top, "ACMAG");
	sprintf("COMPLETED");
	font_render_string_center (&font_steel, DMD_BIG_CX_Bot, DMD_BIG_CY_Bot, sprintf_buffer);
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
