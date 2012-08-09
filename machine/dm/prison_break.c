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
//constants

//local variables
U8 			prison_break_mode_counter; //number of shots made this mode
U8 			prison_break_modes_achieved;
score_t 	prison_break_mode_score; //score for this mode only
__boolean 	prison_break_mode_activated;

//prototypes
void prison_break_reset (void);
void prison_break_player_reset (void);
void prisonbreak_effect_deff (void);

/****************************************************************************
 * initialize  and exit
 ***************************************************************************/
void prison_break_reset (void) {
	prison_break_mode_counter = 0;
	prison_break_mode_activated = FALSE;
	score_zero(prison_break_mode_score);
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
	score (SC_15M);
	prison_break_mode_activated = TRUE;
	++prison_break_modes_achieved;
	sound_start (ST_MUSIC, MUS_MD_CRYO_PRISON_BREAKOUT, 0, SP_NORMAL);
	sound_start (ST_SPEECH, SPCH_CRYO_PRISON_BREAKOUT, SL_1S, PRI_GAME_QUICK5);
	//flash lamp for a time
	lamp_tristate_flash(LM_CLAW_PRISON_BREAK);
	task_sleep(TIME_500MS);
	lamp_tristate_on(LM_CLAW_PRISON_BREAK);
	}//end of function


/****************************************************************************
 * shot made during prison_break mode
 * call sent from ramps.c or underground.c
 ***************************************************************************/
CALLSET_ENTRY (prison_break, prison_break_made) {
	++prison_break_mode_counter;
	sound_start (ST_SAMPLE, EXPLOSION, SL_1S, PRI_GAME_QUICK5);
	deff_start (DEFF_PRISONBREAK_EFFECT);
	//flash lamp for a time
	//lamp_tristate_flash(LM_CENTER_RAMP_MIDDLE);
	//lamp_tristate_flash(LM_CENTER_RAMP_OUTER);
	//task_sleep(TIME_200MS);
	//lamp_tristate_off(LM_CENTER_RAMP_OUTER);
	//lamp_tristate_off(LM_CENTER_RAMP_INNER);
	//TODO: score rolls up as time goes?
	//score_add(prison_break_mode_score, score_table[SC_6M]);

	switch (prison_break_modes_achieved ){
		case 0:
			//as it is right now we score 6 mill + 1 million for each extra shot
			score (SC_6M);
			score (prison_break_mode_counter * SC_1M);
			break;
		case 1:
			//2nd time we are in prison_break - score differently
			score (SC_7M);
			score (prison_break_mode_counter * SC_2M);
			break;
		case 2:
			//3rd time we are in prison_break - score differently
			score (SC_8M);
			score (prison_break_mode_counter * SC_3M);
			break;
		default:
			//all cases past 3rd time we are in prison_break
			score (SC_8M);
			score (prison_break_mode_counter * SC_3M);
			break;
	}//end of switch

	//TODO: display effects
}//end of function



/****************************************************************************
 * DMD display and sound effects
 ****************************************************************************/
void prisonbreak_effect_deff(void) {
	dmd_alloc_low_clean ();
	font_render_string_center (&font_mono5, 96, 5, "Prison Break");
	dmd_show_low ();
	task_sleep_sec (2);
	deff_exit ();
}
