/*
 * demolition man
 * prison_break.c
 *
 * written by James Cardona
 *
 * Location Description:
 * mode started by claw switch Prison Break
 *
 * Scoring Description: (original game)
 * This is an award/shoot mode that's very quick. The ball is dropped above the upper left
 * flipper and you are awarded 15 million. A reflex shot into the
 * Underground doubles this, a reflex shot into the side ramp triples this.
 * Prison Break also lights Car Chase which is an alternating ramp award
 * very similar to PayBack Time on T2. Awards are 5 million per successful ramp.
 *
 * Scoring Description: (my rules)
 * I do not like this mode and plan on completely changing it to something else,
 * as it is right now we are using acmag style scoring on only side ramp and underground shots
 *
 * TODO: come up with proper mode
 * TODO: potentially we can make a 2nd and 3rd mode that score differently
 */
#include <freewpc.h>
#include "dm/global_constants.h"

//constants

//local variables
U8 			prison_break_mode_shots_made; //number of shots made this mode
U8 			prison_break_modes_achieved;
U8			prison_break_mode_shots_made_from_underground;
score_t 	prison_break_mode_score; //score for this mode only
__boolean 	is_prison_break_mode_activated;

//external variables

//prototypes
void prison_break_reset (void);
void prison_break_player_reset (void);
void prisonbreak_effect_deff (void);

/****************************************************************************
 * initialize  and exit
 ***************************************************************************/
void prison_break_reset (void) {
	prison_break_mode_shots_made = 0;
	is_prison_break_mode_activated = FALSE;
	prison_break_mode_shots_made_from_underground = 0;
	}

void prison_break_player_reset (void) {
	prison_break_reset();
	prison_break_modes_achieved = 0;
}

CALLSET_ENTRY (prison_break, start_player) { prison_break_player_reset(); }
CALLSET_ENTRY (prison_break, start_ball) { prison_break_reset(); }


/****************************************************************************
 * body
 *
 ***************************************************************************/

/****************************************************************************
 * prison_break Claw switch activation starts mode
 *
 * TODO: verify ball was just shot into claw
 * perhaps make a claw mode enabled flag for all claw shots
 ***************************************************************************/
CALLSET_ENTRY (prison_break, sw_claw_prison_break) {
	score_zero(prison_break_mode_score);
	score (SC_15M);
	is_prison_break_mode_activated = TRUE;
	++prison_break_modes_achieved;
	sound_start (ST_SPEECH, SPCH_CRYO_PRISON_BREAKOUT, SL_5S, PRI_GAME_QUICK5);
	music_set(MUS_MD_CRYO_PRISON_BREAKOUT); //from sound_effect.c
	//flash lamp for a time
	lamp_tristate_flash(LM_CLAW_PRISON_BREAK);
	task_sleep(TIME_500MS);
	lamp_tristate_on(LM_CLAW_PRISON_BREAK);
	//TODO: store standard arrow status?
	//TODO: flash prison break arrows
	lamp_tristate_flash(LM_UNDERGROUND_ARROW);
	lamp_tristate_flash(LM_SIDE_RAMP_ARROW);
	deff_start (DEFF_PRISONBREAK_START_EFFECT);
	}//end of function


/****************************************************************************
 * shot made during prison_break mode
 * call sent from ramps.c - side ramp or underground.c
 ***************************************************************************/
CALLSET_ENTRY (prison_break, prison_break_made) {
	++prison_break_mode_shots_made;
	sound_start (ST_SAMPLE, EXPLOSION, SL_2S, PRI_GAME_QUICK5);
	//flash lamp for a time
	//TODO: score rolls up as time goes?
	switch (prison_break_modes_achieved ){
		case 0:
			//as it is right now we score 6 mill + 1 million for each extra shot
			score (SC_6M);
			score_add(prison_break_mode_score, score_table[SC_6M]);
			break;
		case 1:
			//2nd time we are in prison_break - score differently
			score (SC_7M);
			score_add(prison_break_mode_score, score_table[SC_7M]);
			break;
		case 2:
			//3rd time we are in prison_break - score differently
			score (SC_8M);
			score_add(prison_break_mode_score, score_table[SC_8M]);
			break;
		default:
			//all cases past 3rd time we are in prison_break
			score (SC_8M);
			score_add(prison_break_mode_score, score_table[SC_8M]);
			break;
	}//end of switch
	//TODO: display effects
	deff_start (DEFF_PRISONBREAK_EFFECT);
}//end of function


/*
 * The following is a temporary workaround because
 * the underground bottom popper triggers its switch twice when it
 * fires the ball
 */
CALLSET_ENTRY (prison_break, prison_break_made_from_underground) {
	//check that trigger is odd, see note above
	if (prison_break_mode_shots_made_from_underground++ % 2 != 0) {
	++prison_break_mode_shots_made;
	sound_start (ST_SAMPLE, EXPLOSION, SL_2S, PRI_GAME_QUICK5);
	//flash lamp for a time
	//TODO: score rolls up as time goes?
	switch (prison_break_modes_achieved ){
		case 0:
			//as it is right now we score 6 mill + 1 million for each extra shot
			score (SC_6M);
			score_add(prison_break_mode_score, score_table[SC_6M]);
			break;
		case 1:
			//2nd time we are in prison_break - score differently
			score (SC_7M);
			score_add(prison_break_mode_score, score_table[SC_7M]);
			break;
		case 2:
			//3rd time we are in prison_break - score differently
			score (SC_8M);
			score_add(prison_break_mode_score, score_table[SC_8M]);
			break;
		default:
			//all cases past 3rd time we are in prison_break
			score (SC_8M);
			score_add(prison_break_mode_score, score_table[SC_8M]);
			break;
	}//end of switch
	//TODO: display effects
	deff_start (DEFF_PRISONBREAK_EFFECT);
	}//end of if
}//end of function


/****************************************************************************
 * DMD display and sound effects
 ****************************************************************************/
void prisonbreak_effect_deff(void) {
	dmd_alloc_low_clean ();
	sprintf ("%d PRISON BREAK", prison_break_mode_shots_made);
	font_render_string_center (&font_steel, DMD_BIG_CX_Top, DMD_BIG_CY_Top, sprintf_buffer);
	sprintf_score (prison_break_mode_score);
	font_render_string_center (&font_steel, DMD_BIG_CX_Bot, DMD_BIG_CY_Bot, sprintf_buffer);
	dmd_show_low ();
	task_sleep_sec (2);
	deff_exit ();
}


void prisonbreak_start_effect_deff(void) {
	dmd_alloc_low_clean ();
	font_render_string_center (&font_steel, DMD_BIG_CX_Top, DMD_BIG_CY_Top, "PRISON BREAK");
	font_render_string_center (&font_var5, DMD_BIG_CX_Bot, DMD_BIG_CY_Bot, "SHOOT ARROWS");
	dmd_show_low ();
	task_sleep_sec (2);
	deff_exit ();
}


/****************************************************************************
 * status display
 *
 * called from common/status.c automatically whenever either flipper button
 * is held for 4 seconds or longer.  since called by callset, order of
 * various status reports will be random depending upon call stack
****************************************************************************/
CALLSET_ENTRY (prison_break, status_report){
		if (is_prison_break_mode_activated) sprintf ("PB");
		else sprintf ("NPB");
		font_render_string_center(&font_fixed6, DMD_BIG_CX_Top, DMD_BIG_CY_Top, sprintf_buffer);

//	sprintf ("%d PRISON BREAKOUT MODES", prison_break_modes_achieved);
//	font_render_string_center(&font_mono5, 1, 16, sprintf_buffer);

//	sprintf ("prison break score: %d", prison_break_mode_score);
//	font_render_string_center(&font_mono5, 64, 13, sprintf_buffer);
//
//		sprintf ("%d prison break shots made", prison_break_mode_shots_made);
//		font_render_string_center (&font_mono5, 64, 19, sprintf_buffer);

		//deff_exit (); is called at end of calling function - not needed here?
}//end of function
