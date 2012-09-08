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
 *
 */

#include <freewpc.h>
#include "dm/global_constants.h"

//constants
const U8 explode_mode_timer_value = 23;

//local variables
U8 explode_temp_counter; //temporary counter
U8 explode_SoundCounter;
U8 explode_mode_timer;
U8 explode_mode_shots_made;//number of times an explode arrow or eyeball is hit
U8 explode_modes_achieved_counter;//number of times mode achieved
score_t explode_mode_score;
score_t explode_mode_last_score;
score_t explode_mode_temp_score;
score_t explode_mode_next_score;

//external variables

//prototypes
void explode_mode_init (void);
void explode_mode_expire (void);
void explode_mode_exit (void);
void explode_reset (void);
void player_explode_reset (void);
void explode_effect_deff(void);



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
	.init_timer = 23, //explode_mode_timer_value should equal this
	.timer = &explode_mode_timer,
	.grace_timer = 2,
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
	callset_invoke(Activate_Explode_Inserts);
	++explode_modes_achieved_counter;
}//end of function



void explode_mode_expire (void) {
	flag_off (FLAG_IS_EXPLODE_MODE_ACTIVATED);
	callset_invoke(DeActivate_Explode_Inserts);//sent to ramps.c and orbits.c
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
	score (explode_mode_temp_score);
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
	deff_start (DEFF_EXPLODE_HIT_EFFECT);
}//end of function



/****************************************************************************
 * DMD display and sound effects
 ****************************************************************************/
void explode_start_effect_deff(void) {
	U8 count = 8;
	dmd_alloc_pair_clean ();
	font_render_string_center (&font_lithograph, 64, 9, "EXPLODE");
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


void explode_hit_effect_deff(void) {
	dmd_alloc_low_clean ();
	font_render_string_center (&font_lithograph, DMD_BIG_CX_Top, DMD_BIG_CY_Top, "BOOM BABY");
	sprintf_score (explode_mode_last_score);
	font_render_string_center (&font_term6, DMD_BIG_CX_Bot, DMD_BIG_CY_Bot, sprintf_buffer);
	dmd_show_low ();
	task_sleep_sec (2);
	deff_exit ();
}//end of mode_effect_deff



void explode_effect_deff(void) {
	U8 i;
	for (;;) {
		dmd_alloc_low_clean ();
		font_render_string_center (&font_lithograph, DMD_BIG_CX_Top, DMD_BIG_CY_Top, "EXPLODE");
		sprintf ("%d SEC LEFT,  %d HIT", explode_mode_timer, explode_mode_shots_made);
		font_render_string_center (&font_mono5, DMD_SMALL_CX_3, DMD_SMALL_CY_3, sprintf_buffer);

		//DMD size is 128x32
		for (i = 10; i < (118 - (explode_mode_timer * ( 118/explode_mode_timer_value) ) ); i++) {
		sprintf ("I");
		font_render_string_left (&font_mono5, i, DMD_SMALL_CY_4, sprintf_buffer);
		}
/*score count down
		score_zero(explode_mode_temp_score);
		score_add (explode_mode_temp_score, score_table[SC_1M]);
		score_mul (explode_mode_temp_score, explode_mode_timer);
*/
		dmd_show_low ();
		task_sleep (TIME_200MS);
	}//END OF ENDLESS LOOP
}//end of mode_effect_deff




void explode_end_deff(void) {
	dmd_alloc_low_clean ();
	font_render_string_center (&font_lithograph, DMD_BIG_CX_Top, DMD_BIG_CY_Top, "EXPLODE");
	font_render_string_center (&font_lithograph, DMD_BIG_CX_Bot, DMD_BIG_CY_Bot, "COMPLETED");
	dmd_show_low ();
	task_sleep_sec (2);
	deff_exit ();
}//end of mode_effect_deff
