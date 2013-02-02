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
 */
/* CALLSET_SECTION (prison_break, __machine3__) */

#include <freewpc.h>
#include "dm/global_constants.h"
#include "clawmagnet.h"

//constants
const U8 			PRISON_BREAK_EASY_GOAL 	= 3;
const U8 			PRISON_BREAK_MED_GOAL 	= 4;
const U8 			PRISON_BREAK_HARD_GOAL 	= 5;
const U8 			PRISON_BREAK_GOAL_INCREMENT = 1;
const U8 			PRISON_BREAK_GOAL_MAX 	= 10;

//local variables
U8 			prison_break_mode_shots_made; //number of shots made this mode
__local__ U8 			prison_break_modes_achieved;
U8			prison_break_mode_timer;
score_t 	prison_break_mode_score; //score for this mode only
score_t 	prison_break_mode_last_score;
score_t 	prison_break_mode_next_score;
score_t 	prison_break_mode_score_total_score;

//external variables

//internally called function prototypes  --external found at protos.h
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
	.prio = PRI_GAME_MODE6,
	.init_timer = 48,
	.timer = &prison_break_mode_timer,
	.grace_timer = 2,
//	.pause = system_timer_pause,
};


/****************************************************************************
 * initialize  and exit
 ***************************************************************************/
void prison_break_reset (void) {
	flag_off (FLAG_IS_PBREAK_RUNNING);
}//end of function



void prison_break_player_reset (void) {
	prison_break_reset();
	prison_break_modes_achieved = 0;
	prison_break_mode_shots_made = 0;
	score_zero(prison_break_mode_score_total_score);
}//end of function



void prison_break_mode_init (void) {
			//the claw mode can expire on its own and since it is a lower priority it will not display
			//callset_invoke (end_claw_mode); // this seemed to cause occasional crashes
			clawmagnet_off ();
			flag_off(FLAG_IS_BALL_ON_CLAW);
			flipper_enable ();
			ballsave_add_time (10);
	prison_break_mode_shots_made = 0;
	++prison_break_modes_achieved;
	sound_start (ST_SPEECH, SPCH_CRYO_PRISON_BREAKOUT, SL_4S, PRI_GAME_QUICK5);
	//flash lamp for a time
	lamp_tristate_flash(LM_CLAW_PRISON_BREAK);
	task_sleep(TIME_500MS);
	lamp_tristate_on(LM_CLAW_PRISON_BREAK);
	score_zero(prison_break_mode_score);
	score_zero(prison_break_mode_last_score);
	score_add(prison_break_mode_last_score, score_table[PRIS_BREAK_HIT_SCORE]);

	score_zero(prison_break_mode_next_score);
	switch (prison_break_modes_achieved) {
		case 1:
			score(PRIS_BREAK_START_SCORE1);
			score_add(prison_break_mode_score, score_table[PRIS_BREAK_START_SCORE1]);
			break;
		case 2:
			score(PRIS_BREAK_START_SCORE2);
			score_add(prison_break_mode_score, score_table[PRIS_BREAK_START_SCORE2]);
			break;
		default:
			score(PRIS_BREAK_START_SCORE3);
			score_add(prison_break_mode_score, score_table[PRIS_BREAK_START_SCORE3]);
	}//end of switch
	flag_on (FLAG_IS_PBREAK_RUNNING);
}//end of function



void prison_break_mode_expire (void) {
	flag_off (FLAG_IS_PBREAK_RUNNING);
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
 *
 * body
 *
 ***************************************************************************/
CALLSET_ENTRY (prison_break, sw_claw_prison_break) {
	demotime_increment();
	timed_mode_begin (&prison_break_mode);//start mode
}//end of function



void prison_break_made (void) {
	++prison_break_mode_shots_made;
	score_add(prison_break_mode_score, prison_break_mode_last_score);
	score_add (prison_break_mode_score_total_score, prison_break_mode_last_score);
	score_long (prison_break_mode_last_score);
	deff_start (DEFF_PRISON_BREAK_HIT_EFFECT);//under /kernel/deff.c
}//end of function



/****************************************************************************
 *
 * display effects
 *
 ****************************************************************************/
void prison_break_animation_display_effect (U16 start_frame, U16 end_frame){
	U16 fno;
	for (fno = start_frame; fno <= end_frame; fno += 2) {
		dmd_alloc_pair ();
		frame_draw(fno);
		dmd_show2 ();
		task_sleep (TIME_100MS);
	}//end of inner loop
}



void prison_break_frame_bitfade_fast (U16 frame){
	dmd_sched_transition (&trans_bitfade_fast);
	dmd_alloc_pair ();
	frame_draw(frame);
	dmd_show2 ();
	task_sleep (TIME_100MS);
}



void frame_with_words_display_v5prc_effect (U16 frame, U8 x, U8 y, char *words){
	dmd_alloc_pair_clean ();// Clean both pages
	dmd_map_overlay ();
	dmd_clean_page_low ();
	font_render_string_center (&font_v5prc, x, y, words);
	dmd_text_outline ();
	dmd_alloc_pair ();
	frame_draw(frame);
	dmd_overlay_outline ();
	dmd_show2 ();
	task_sleep (TIME_100MS);
}



void prison_break_start_effect_deff(void) {
	U8 			prison_break_MessageCounter;
	prison_break_MessageCounter = random_scaled(2);
	dmd_clean_page_high ();//
	dmd_clean_page_low ();//
	switch (prison_break_MessageCounter) {
		default:
		case 0:
			prison_break_animation_display_effect (IMG_CHIEF_EARLE2_START, IMG_CHIEF_EARLE2_END);
			prison_break_frame_bitfade_fast(IMG_JAIL_BREAK_START);
			prison_break_animation_display_effect (IMG_JAIL_BREAK_START, IMG_JAIL_BREAK_END);
			prison_break_frame_bitfade_fast(IMG_ASSOC_BOB_START);
			prison_break_animation_display_effect (IMG_ASSOC_BOB_START, IMG_ASSOC_BOB_END);
			frame_with_words_display_v5prc_effect (IMG_ASSOC_BOB_END, DMD_MIDDLE_X - 10, DMD_BIG_CY_Bot, "BREAKOUT");
			break;
		case 1:
			prison_break_animation_display_effect (IMG_ASSOC_BOB_START, IMG_ASSOC_BOB_END);
			prison_break_frame_bitfade_fast(IMG_PRISON_BREAKOUT_B_START);
			prison_break_animation_display_effect (IMG_PRISON_BREAKOUT_B_START, IMG_PRISON_BREAKOUT_B_MID);
			prison_break_animation_display_effect (IMG_PRISON_BREAKOUT_B_MID, IMG_PRISON_BREAKOUT_B_END);
			prison_break_frame_bitfade_fast(IMG_CHIEF_EARLE2_START);
			prison_break_animation_display_effect (IMG_CHIEF_EARLE2_START, IMG_CHIEF_EARLE2_END);
			frame_with_words_display_v5prc_effect (IMG_CHIEF_EARLE2_END, DMD_MIDDLE_X - 10, DMD_BIG_CY_Bot, "BREAKOUT");
			break;
	}//end of switch
	task_sleep_sec (2);
	deff_exit ();
}//end of mode_effect_deff



U8 			prison_break_MessageCounter;
void prison_break_hit_effect_deff(void) {
//	if (++prison_break_MessageCounter > 6) 	prison_break_MessageCounter = 0; //for testing
	prison_break_MessageCounter = random_scaled(7);

	dmd_alloc_pair_clean ();
	sound_start (ST_SAMPLE, EXPLOSION, SL_2S, PRI_GAME_QUICK5);
	switch (prison_break_MessageCounter) {
		default:
		case 0:
			prison_break_animation_display_effect (IMG_CHIEF_EARLE2_START, IMG_CHIEF_EARLE2_END);
			sprintf_score (prison_break_mode_score);
			frame_with_words_display_v5prc_effect (IMG_CHIEF_EARLE2_END, DMD_MIDDLE_X + 20, DMD_BIG_CY_Top, sprintf_buffer);
			break;
		case 1:
			prison_break_animation_display_effect (IMG_JAIL_BREAK_START, IMG_JAIL_BREAK_END);
			sprintf_score (prison_break_mode_score);
			frame_with_words_display_v5prc_effect (IMG_JAIL_BREAK_END, DMD_MIDDLE_X + 20, DMD_BIG_CY_Top, sprintf_buffer);
			break;
		case 2:
			prison_break_animation_display_effect (IMG_ASSOC_BOB_START, IMG_ASSOC_BOB_END);
			sprintf_score (prison_break_mode_score);
			frame_with_words_display_v5prc_effect (IMG_ASSOC_BOB_END, DMD_MIDDLE_X + 20, DMD_BIG_CY_Top, sprintf_buffer);
			break;
		case 3:
			prison_break_animation_display_effect (IMG_PRISON_BREAKOUT_B_START, IMG_PRISON_BREAKOUT_B_MID);
			sprintf_score (prison_break_mode_score);
			frame_with_words_display_v5prc_effect (IMG_PRISON_BREAKOUT_B_MID, DMD_MIDDLE_X + 20, DMD_BIG_CY_Top, sprintf_buffer);
			break;
		case 4:
			prison_break_animation_display_effect (IMG_PRISON_BREAKOUT_B_MID, IMG_PRISON_BREAKOUT_B_END);
			sprintf_score (prison_break_mode_score);
			frame_with_words_display_v5prc_effect (IMG_PRISON_BREAKOUT_B_END, DMD_MIDDLE_X + 20, DMD_BIG_CY_Top, sprintf_buffer);
			break;
		case 5:
			prison_break_animation_display_effect (IMG_PRISON_BREAK_D1_START, IMG_PRISON_BREAK_D1_END);
			sprintf_score (prison_break_mode_score);
			frame_with_words_display_v5prc_effect (IMG_PRISON_BREAK_D1_END, DMD_MIDDLE_X + 20, DMD_BIG_CY_Top, sprintf_buffer);
			break;
		case 6:
			prison_break_animation_display_effect (IMG_PRISON_BREAK_D2_START, IMG_PRISON_BREAK_D2_END);
			sprintf_score (prison_break_mode_score);
			frame_with_words_display_v5prc_effect (IMG_PRISON_BREAK_D2_END, DMD_MIDDLE_X + 20, DMD_BIG_CY_Top, sprintf_buffer);
			break;
		}//end of switch
	task_sleep (TIME_500MS);
	deff_exit ();
}//end of mode_effect_deff



//THE MORE HITS THAT ARE MADE, THE LESS BARS THAT ARE DRAWN
void dmd_draw_jail_bars (U8 *dbuf) {
	U8 i;
	dbuf += 16; //skip first 2 rows
	dbuf += 16;
	for (i = 1; i <= 28; i++) {
		if (prison_break_mode_shots_made < 1) 	dbuf[8] = 0x07;
		if (prison_break_mode_shots_made < 2) 	dbuf[10] = 0x07;
		if (prison_break_mode_shots_made < 3)	dbuf[6] = 0x07;
		if (prison_break_mode_shots_made < 4) 	dbuf[12] = 0x07;
		if (prison_break_mode_shots_made < 5) 	dbuf[4] = 0x07;
		if (prison_break_mode_shots_made < 6) 	dbuf[14] = 0x07;
		if (prison_break_mode_shots_made < 7)	dbuf[2] = 0x07;
		dbuf += 16;
	}//end of loop
}//end of mode_effect_deff





void prison_break_effect_deff(void) {
	U8 draw_jail_bars_timer;
	draw_jail_bars_timer = 0;
	U8 i = 0;
	U8 toggle = 0;

	for (;;) {
		i++;

		if (i % 20 == 0) if (++toggle > 3) toggle = 0;//change TOGGLE once per n seconds

		dmd_alloc_pair_clean();
		dmd_draw_border (dmd_low_buffer);

		if (IN_TEST) {
			++draw_jail_bars_timer;
			if ( (draw_jail_bars_timer % 5) == 0) prison_break_mode_shots_made++;
		}

		dmd_draw_jail_bars (dmd_low_buffer);

		sprintf("BREAKOUT");
		sprintf_buffer[1 + prison_break_mode_shots_made] = '\0';
		font_render_string_center (&font_v5prc, DMD_MIDDLE_X, DMD_SMALL_CY_2, sprintf_buffer);

		sprintf ("%d", prison_break_mode_timer);
		font_render_string_center (&font_term6, DMD_MIDDLE_X - 55, DMD_SMALL_CY_4, sprintf_buffer);
		font_render_string_center (&font_term6, DMD_MIDDLE_X + 55, DMD_SMALL_CY_4, sprintf_buffer);

		if (toggle == 1) 		sprintf ("%d SEC LEFT", prison_break_mode_timer);
		else if (toggle == 2)	sprintf ("%d HIT", prison_break_mode_shots_made);
		else if (toggle == 0)	sprintf ("SHOOT EVERYTHING");

		font_render_string_center (&font_var5, DMD_MIDDLE_X, DMD_SMALL_CY_3 + 1, sprintf_buffer);

		sprintf_score (prison_break_mode_score);
		font_render_string_center (&font_var5, DMD_MIDDLE_X, DMD_SMALL_CY_5 - 3, sprintf_buffer);
		dmd_show_low ();
		task_sleep (TIME_500MS);
	}//END OF ENDLESS LOOP
}//end of mode_effect_deff
