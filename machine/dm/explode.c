/*
 * demolition man
 * explode.c
 *
 * written by James Cardona
 *
 * Location Description:
 * Retina Scan: This is the leftmost shot of the game. The ball passes through a gate
 * and into an eject that feeds the left inlane.
 * If hit hard enough through the gate, it will hit the captive eyeball.
 *
 * Scoring Description: (original game)
 * Shots that knock the captive eyeball into the upper target award the Retina Scan value.
 * The Jet Bumpers increase the Retina Value.(eyeball)
 * It starts at 5M and goes up 100K per jet.
 *
 * At certain numbers of Retina Scans hits, Explode Hurry Up is activated.
 * It takes 1 hit for the first Hurry-Up, four for the next on the easiest level.
 *
 * All four Explode ramps are lit at a value of 15M that begins counting down.
 * Hit any explode lamp to collect and add 10M to the value for the next Explode shot.
 * The round times out when the countdown reaches 5M (or higher if you've collected a few of the shots).
 * An extra ball is lit at three Retina Scans.
 * TODO: Retina Scan shots feed to the left saucer (also called eject) which drops the ball into the left inlane.
 *
 *
 * Scoring Description: (my rules)
 * same as above except explode mode times out after 30 seconds or ball drain
 * and eyeball shot can be hit in addition to explode arrows
 *
 * estimate of average explode mode score: 80 million to 120 million
 * estimate of average eyeball score: 10 million to 20 million
 *
 */
/* CALLSET_SECTION (explode, __machine2__) */


#include <freewpc.h>
#include "dm/global_constants.h"

//constants
const U8 EXPLODE_TIMER_DEFAULT = 33;

//local variables
U8				explode_MessageCounter;
U8 				explode_temp_counter; //temporary counter
U8 				explode_SoundCounter;
U8 				explode_mode_timer;
U8 				explode_mode_shots_made;//number of times an explode is hit
__local__ U8 	explode_modes_achieved_counter;//number of times mode achieved
score_t 		explode_mode_score;
score_t 		explode_mode_last_score;
score_t 		explode_mode_temp_score;
score_t 		explode_mode_next_score;
score_t 		explode_mode_display_score;
//external variables

/****************************************************************************
 * mode definition structure
 ***************************************************************************/
struct timed_mode_ops explode_mode = {
	DEFAULT_MODE,
	.init = explode_mode_init,
	.exit = explode_mode_exit,
	.gid = GID_EXPLODE_MODE_RUNNING,
	.music = MUS_MD_EXPLODE,
	.deff_starting = DEFF_EXPLODE_START_EFFECT,
	.deff_running = DEFF_EXPLODE_EFFECT,
//	.deff_ending = DEFF_EXPLODE_END,
	.prio = PRI_GAME_MODE5,//shorter the mode time, make priority higher
	.init_timer = 33, //make sure to set EXPLODE_TIMER_DEFAULT to the same value
	.timer = &explode_mode_timer,
	.grace_timer = 2, //DEFAULT IS 2
//	.pause = system_timer_pause,
};


/****************************************************************************
 * initialize  and exit
 ***************************************************************************/
void player_explode_reset (void) {
	flag_off (FLAG_IS_EXPLODE_MODE_ACTIVATED);
	explode_modes_achieved_counter = 0;
	explode_mode_shots_made = 0;
	explode_SoundCounter = 0;
}//end of function

void explode_reset (void) {
	flag_off (FLAG_IS_EXPLODE_MODE_ACTIVATED);
}//end of function



void explode_mode_init (void) {
	flag_on (FLAG_IS_EXPLODE_MODE_ACTIVATED);
	explode_mode_shots_made = 0;
	if ( (explode_SoundCounter++ % 2) == 0 )//check if even
		sound_start (ST_SPEECH, SPCH_EXPLODE_ACTIVATED, SL_4S, PRI_GAME_QUICK5);
	else
		sound_start (ST_SPEECH, SPCH_EXPLODE_HURRYUP, SL_4S, PRI_GAME_QUICK5);
	score_zero(explode_mode_score);
	score_zero(explode_mode_next_score);
	score_add (explode_mode_next_score, score_table[SC_15M]);
	callset_invoke(activate_explode_inserts);
	++explode_modes_achieved_counter;
	serve_ball_auto(); //add one ball to the playfield
}//end of function



void explode_mode_expire (void) {
	flag_off (FLAG_IS_EXPLODE_MODE_ACTIVATED);
	callset_invoke(deactivate_explode_inserts);//sent to ramps.c and orbits.c
}//end of function


void explode_mode_exit (void) { explode_mode_expire();}


/****************************************************************************
 * external event listeners
 ****************************************************************************/
CALLSET_ENTRY (explode, music_refresh)  { timed_mode_music_refresh (&explode_mode); }
CALLSET_ENTRY (explode, end_ball)	 	{ if (timed_mode_running_p(&explode_mode) ) timed_mode_end (&explode_mode); }
CALLSET_ENTRY (explode, display_update) { timed_mode_display_update (&explode_mode); }
CALLSET_ENTRY (explode, start_player) 	{ player_explode_reset(); }
CALLSET_ENTRY (explode, start_ball) 	{ explode_reset(); }

/****************************************************************************
 * body
 *
 ***************************************************************************/
CALLSET_ENTRY (explode, start_explode) {
		timed_mode_begin (&explode_mode);//start explode mode
}//end of function



CALLSET_ENTRY (explode, explode_made) {
	++explode_mode_shots_made;
	sound_start (ST_SAMPLE, EXPLOSION, SL_2S, PRI_GAME_QUICK1);
	//score xx million counting down to 0 at
	//end of mode  + 1 million for # of explodes hit
	score_zero(explode_mode_temp_score);
	score_add (explode_mode_temp_score, score_table[SC_1M]);
	score_mul (explode_mode_temp_score, explode_mode_timer);
	score_zero(explode_mode_last_score);
	score_add (explode_mode_last_score, explode_mode_temp_score);
	score_long (explode_mode_temp_score);
	//bonus here

/*	switch (explode_mode_shots_made) {
	case 0: 	break;
	case 1:  { score (SC_1M); score_add (explode_mode_last_score, score_table[SC_1M]); break;}
	case 2:  { score (SC_2M); score_add (explode_mode_last_score, score_table[SC_2M]); break;}
	case 3:  { score (SC_3M); score_add (explode_mode_last_score, score_table[SC_3M]); break;}
	case 4:	 { score (SC_4M); score_add (explode_mode_last_score, score_table[SC_4M]); break;}
	default: { score (SC_4M); score_add (explode_mode_last_score, score_table[SC_4M]); break;}
	}//end of switch
	score_add (explode_mode_score, explode_mode_last_score);
*/
	explode_effect();
}//end of function



/****************************************************************************
 * DMD display and sound effects
 ****************************************************************************/
void explode_start_effect_deff(void) {
	U8 count = 8;
	dmd_alloc_pair_clean ();
	font_render_string_center (&font_lithograph, DMD_MIDDLE_X, DMD_BIG_CY_Cent, "EXPLODE");
	/* low = text, high = blank */
	while (--count > 0){
		dmd_show2 ();
		task_sleep (TIME_100MS);

		dmd_flip_low_high ();
		dmd_show2 ();
		task_sleep (TIME_100MS);

		dmd_show_high ();
		task_sleep (TIME_200MS);

		dmd_show2 ();
		task_sleep (TIME_100MS);
		dmd_flip_low_high ();
	}
	deff_exit ();
}//end of mode_effect_deff



void explode_effect(void) {
	switch (++explode_MessageCounter % 4) {
		case 0:  deff_start (DEFF_EXPLODE_HIT1_EFFECT); break;
		case 1:  deff_start (DEFF_EXPLODE_HIT2_EFFECT); break;
		case 2:  deff_start (DEFF_EXPLODE_HIT3_EFFECT); break;
		default: deff_start (DEFF_EXPLODE_HIT4_EFFECT); break;
		}//end of switch
}//end of FUNCTION



void explode_hit1_effect_deff(void) {
	dmd_alloc_pair_clean ();
	font_render_string_center (&font_lithograph, DMD_MIDDLE_X, DMD_BIG_CY_Top, "BOOM BABY");
	sprintf_score (explode_mode_last_score);
	font_render_string_center (&font_term6, DMD_MIDDLE_X, DMD_BIG_CY_Bot, sprintf_buffer);
	dmd_show_low ();
	task_sleep_sec (2);
	deff_exit ();
}//end of mode_effect_deff



void explode_hit2_effect_deff(void) {
	dmd_alloc_pair_clean ();
	font_render_string_center (&font_lithograph, DMD_MIDDLE_X, DMD_BIG_CY_Top, "BOOOOOYAH");
	sprintf_score (explode_mode_last_score);
	font_render_string_center (&font_term6, DMD_MIDDLE_X, DMD_BIG_CY_Bot, sprintf_buffer);
	dmd_show_low ();
	task_sleep_sec (2);
	deff_exit ();
}//end of mode_effect_deff



void explode_hit3_effect_deff(void) {
	dmd_alloc_pair_clean ();
	font_render_string_center (&font_lithograph, DMD_MIDDLE_X, DMD_BIG_CY_Top, "BOOM BOOM");
	sprintf_score (explode_mode_last_score);
	font_render_string_center (&font_term6, DMD_MIDDLE_X, DMD_BIG_CY_Bot, sprintf_buffer);
	dmd_show_low ();
	task_sleep_sec (2);
	deff_exit ();
}//end of mode_effect_deff



void explode_hit4_effect_deff(void) {
	dmd_alloc_low_clean ();
	font_render_string_center (&font_lithograph, DMD_MIDDLE_X, DMD_BIG_CY_Top, "POW");
	sprintf_score (explode_mode_last_score);
	font_render_string_center (&font_term6, DMD_MIDDLE_X, DMD_BIG_CY_Bot, sprintf_buffer);
	dmd_show_low ();
	task_sleep_sec (2);
	deff_exit ();
}//end of mode_effect_deff



void explode_effect_deff(void) {
	U8 i = 0;
	for (;;) {
			dmd_alloc_low_clean ();
			font_render_string_center (&font_lithograph, DMD_MIDDLE_X, DMD_BIG_CY_Top, "EXPLODE");
			score_zero(explode_mode_display_score);
			score_add (explode_mode_display_score, score_table[SC_1M]);
			score_mul (explode_mode_display_score, explode_mode_timer);
			sprintf_score (explode_mode_display_score);
			font_render_string_center (&font_var5, DMD_MIDDLE_X, DMD_SMALL_CY_3, sprintf_buffer);

			//DMD size is 128x32
			for (i = 5; i < (118 - (explode_mode_timer * ( 118/EXPLODE_TIMER_DEFAULT) ) ); i++) {
				sprintf ("I");
				font_render_string_left (&font_var5, i, DMD_SMALL_CY_4, sprintf_buffer);
			}//end of for loop
			dmd_show_low ();
			task_sleep (TIME_500MS);//score goes down by second so update every 500ms is plenty soon enough
	}//END OF ENDLESS LOOP
}//end of mode_effect_deff




void explode_end_deff(void) {
	dmd_alloc_low_clean ();
	font_render_string_center (&font_lithograph, DMD_MIDDLE_X, DMD_BIG_CY_Top, "EXPLODE");
	font_render_string_center (&font_lithograph, DMD_MIDDLE_X, DMD_BIG_CY_Bot, "COMPLETED");
	dmd_show_low ();
	task_sleep_sec (2);
	deff_exit ();
}//end of mode_effect_deff
