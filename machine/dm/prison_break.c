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
 * as it is right now we are using prison_break style scoring on only side ramp and underground shots
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
U8 			prison_break_modes_completed;
U8			prison_break_mode_timer;
score_t 	prison_break_mode_score; //score for this mode only
score_t 	prison_break_mode_last_score;
score_t 	prison_break_mode_next_score;
score_t 	prison_break_mode_score_total_score;



//external variables

//prototypes
void prison_break_reset (void);
void prison_break_player_reset (void);
void prison_break_effect_deff(void);
void prison_break_mode_init (void);
void prison_break_mode_expire (void);
void prison_break_mode_exit (void);



/****************************************************************************
 * mode definition structure
 ***************************************************************************/
struct timed_mode_ops prison_break_mode = {
	DEFAULT_MODE,
	.init = prison_break_mode_init,
	.exit = prison_break_mode_exit,
	.gid = GID_PRISON_BREAK_MODE_RUNNING,
	.music = MUS_MD_CRYO_PRISON_BREAKOUT,
	.deff_starting = DEFF_PRISON_BREAK_START_EFFECT,
	.deff_running = DEFF_PRISON_BREAK_EFFECT,
//	.deff_ending = DEFF_prison_break_END_EFFECT,
	.prio = PRI_GAME_MODE1,
	.init_timer = 43,
	.timer = &prison_break_mode_timer,
	.grace_timer = 2,
//	.pause = system_timer_pause,
};



/****************************************************************************
 * initialize  and exit
 ***************************************************************************/
void prison_break_reset (void) {
	flag_off (FLAG_IS_PRISON_BREAK_ACTIVATED);
}//end of function



void prison_break_player_reset (void) {
	prison_break_reset();
	prison_break_modes_achieved = 0;
	prison_break_modes_completed = 0;
	score_zero(prison_break_mode_score_total_score);
}//end of function



void prison_break_mode_init (void) {
	prison_break_mode_shots_made = 0;
	flag_on (FLAG_IS_PRISON_BREAK_ACTIVATED);
	++prison_break_modes_achieved;
	sound_start (ST_SPEECH, SPCH_CRYO_PRISON_BREAKOUT, SL_4S, PRI_GAME_QUICK5);
	//flash lamp for a time
	lamp_tristate_flash(LM_CLAW_PRISON_BREAK);
	task_sleep(TIME_500MS);
	lamp_tristate_on(LM_CLAW_PRISON_BREAK);
	score_zero(prison_break_mode_score);
	score_zero(prison_break_mode_last_score);
	score_zero(prison_break_mode_next_score);
	switch (prison_break_modes_achieved) {
		case 1: score_add(prison_break_mode_next_score, score_table[SC_6M]); break;
		case 2: score_add(prison_break_mode_next_score, score_table[SC_7M]); break;
		case 3: score_add(prison_break_mode_next_score, score_table[SC_8M]); break;
		default: score_add(prison_break_mode_next_score, score_table[SC_8M]);
	}//end of switch
	//TODO: store standard arrow status?
	lamp_tristate_flash(LM_UNDERGROUND_ARROW);
	lamp_tristate_flash(LM_SIDE_RAMP_ARROW);
	score (prison_break_mode_last_score);
}//end of function



void prison_break_mode_expire (void) {
	flag_off (FLAG_IS_PRISON_BREAK_ACTIVATED);
}//end of function


void prison_break_mode_exit (void) { prison_break_mode_expire();}


/****************************************************************************
 * external event listeners
 ****************************************************************************/
CALLSET_ENTRY (prison_break, music_refresh)  	{ timed_mode_music_refresh (&prison_break_mode); }
CALLSET_ENTRY (prison_break, end_ball) 		{ if (timed_mode_running_p(&prison_break_mode) ) timed_mode_end (&prison_break_mode); }
CALLSET_ENTRY (prison_break, display_update) 	{ timed_mode_display_update (&prison_break_mode); }

CALLSET_ENTRY (prison_break, start_player) 	{ prison_break_player_reset(); }
CALLSET_ENTRY (prison_break, start_ball) 		{ prison_break_reset(); }




/****************************************************************************
 * body
 *
 ***************************************************************************/
CALLSET_ENTRY (prison_break, sw_claw_prison_break) {
	timed_mode_begin (&prison_break_mode);//start mode
}//end of function





CALLSET_ENTRY (prison_break, prison_break_made) {
	++prison_break_mode_shots_made;
	sound_start (ST_SAMPLE, EXPLOSION, SL_2S, PRI_GAME_QUICK5);
	score_zero(prison_break_mode_last_score);
	switch (prison_break_modes_achieved ){
		case 1:
			score (SC_6M);
			score_add(prison_break_mode_last_score, score_table[SC_6M]);
			break;
		case 2:
			//2nd time we are in prison_break - score differently
			score (SC_7M);
			score_add(prison_break_mode_last_score, score_table[SC_7M]);
			break;
		case 3:
			//3rd time we are in prison_break - score differently
			score (SC_8M);
			score_add(prison_break_mode_last_score, score_table[SC_8M]);
			break;
		default:
			//all cases past 3rd time we are in prison_break
			score (SC_8M);
			score_add(prison_break_mode_last_score, score_table[SC_8M]);
			break;
	}//end of switch
	score_add(prison_break_mode_score, prison_break_mode_last_score);
	score_add (prison_break_mode_score_total_score, prison_break_mode_last_score);
	deff_start (DEFF_PRISON_BREAK_HIT_EFFECT);//under /kernel/deff.c
}//end of function




/****************************************************************************
 * DMD display and sound effects
 ****************************************************************************/
void prison_break_start_effect_deff(void) {
	dmd_alloc_low_clean ();
	font_render_string_center (&font_v5prc, DMD_BIG_CX_Top, DMD_BIG_CY_Top, "BREAKOUT");
	font_render_string_center (&font_v5prc, DMD_BIG_CX_Bot, DMD_BIG_CY_Bot, "SHOOT ARROWS");
	dmd_show_low ();
	task_sleep_sec (2);
	deff_exit ();
}//end of mode_effect_deff



void prison_break_hit_effect_deff(void) {
	dmd_alloc_low_clean ();
	font_render_string_center (&font_v5prc, DMD_BIG_CX_Top, DMD_BIG_CY_Top, "BREAKOUT");
	sprintf_score (prison_break_mode_last_score);
	font_render_string_center (&font_term6, DMD_BIG_CX_Bot, DMD_BIG_CY_Bot, sprintf_buffer);
	dmd_show_low ();
	task_sleep_sec (2);
	deff_exit ();
}//end of mode_effect_deff



void prison_break_effect_deff(void) {
	for (;;) {
		dmd_alloc_low_clean ();
		font_render_string_center (&font_v5prc, DMD_BIG_CX_Top, DMD_BIG_CY_Top, "BREAKOUT");
		sprintf ("%d SEC LEFT,  %d HIT", prison_break_mode_timer, prison_break_mode_shots_made);
		font_render_string_center (&font_mono5, DMD_SMALL_CX_3, DMD_SMALL_CY_3, sprintf_buffer);
		sprintf_score (prison_break_mode_next_score);
		font_render_string_center (&font_mono5, DMD_SMALL_CX_4, DMD_SMALL_CY_4, sprintf_buffer);
		dmd_show_low ();
		task_sleep (TIME_200MS);
	}//END OF ENDLESS LOOP
}//end of mode_effect_deff





void prison_break_end_effect_deff(void) {
	dmd_alloc_low_clean ();
	font_render_string_center (&font_v5prc, DMD_BIG_CX_Top, DMD_BIG_CY_Top, "BREAKOUT");
	sprintf("COMPLETED");
	font_render_string_center (&font_v5prc, DMD_BIG_CX_Bot, DMD_BIG_CY_Bot, sprintf_buffer);
	dmd_show_low ();
	task_sleep_sec (2);
	deff_exit ();
	}//end of mode_effect_deff
