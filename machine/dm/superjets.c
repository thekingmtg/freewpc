/*
 * demolition man
 * superjets.c
 * written by James Cardona
 *
 * Location Description:
 * Jet Bumpers: There are two regular jet bumpers and a top slingshot
 * which scores the same as a jet bumper. They advance the
 * Retina Scan value. The left jet bumper can also be hit from
 * the center ramp -- it acts as the right wall of the center ramp.
 *
 * Scoring Description: (original game)
 * The Jet Bumpers increase the Retina Value.(eyeball)
 * It starts at 5M and goes up 100K per jet.
 * There are two Jet Bumpers and one slingshot ... the slingshot counts as a jet bumper.
 * Jet Bumpers award 250k a Pop.
 * superjets mode award 1 million each
 *
 * Scoring Description: (my rules)
 * same as above except
 * superjets mode award 2 million each
 * successive modes award more
 *
 * estimate of average superjets mode score: 20 million to 80 million
 *
 */
/* CALLSET_SECTION (superjets, __machine5__) */


#include <freewpc.h>
#include "dm/global_constants.h"
#include "clawmagnet.h"
#include "superjets.h"

//constants
const U8 SUPERJETS_EASY_GOAL = 20;
const U8 SUPERJETS_MED_GOAL = 30;
const U8 SUPERJETS_HARD_GOAL = 40;
const U8 SUPERJETS_GOAL_STEP = 10;
const U8 SUPERJETS_GOAL_MAX = 100;

//local variables
U8 						superjets_mode_shots_made;
__local__ U8 			superjets_modes_achieved;
__local__ U8 			superjets_modes_completed;
U8					superjets_mode_timer;
score_t 			superjets_mode_score;
score_t 			superjets_mode_next_score;
score_t 	superjets_mode_score_total_score;
U8 					superjets_goal;
U8 			sj_counter; //counter used for display effects


//external variables

//internally called function prototypes  --external found at protos.h
void superjets_reset (void);
void superjets_player_reset (void);
void superjets_effect_deff(void);
void superjets_mode_init (void);
void superjets_mode_expire (void);
void superjets_mode_exit (void);
void superjets_goal_award(void);

/****************************************************************************
 * mode definition structure
 ***************************************************************************/
struct timed_mode_ops superjets_mode = {
	DEFAULT_MODE,
	.init = superjets_mode_init,
	.exit = superjets_mode_exit,
	.gid = GID_SUPERJETS_MODE_RUNNING,
	.music = MUS_MD_SUPERJETS,
//	.deff_starting = DEFF_SUPERJETS_START_EFFECT,
	.deff_running = DEFF_SUPERJETS_EFFECT,
//	.deff_ending = DEFF_SUPERJETS_END_EFFECT,
	.prio = PRI_GAME_MODE3,
	.init_timer = 63,
	.timer = &superjets_mode_timer,
	.grace_timer = 2, //default is 2
//	.pause = system_timer_pause,
};



/****************************************************************************
 * initialize  and exit
 ***************************************************************************/
void superjets_reset (void) {
	flag_off (FLAG_IS_SUPERJETS_RUNNING);
}//end of function


void superjets_player_reset (void) {
	superjets_reset();
	superjets_mode_shots_made = 0; //these need to be zeroed in before we enter the mode so bonus doesn't fake trigger
	superjets_modes_achieved = 0;
	superjets_modes_completed = 0;
	score_zero(superjets_mode_score_total_score);
	superjets_goal = SUPERJETS_EASY_GOAL;
}//end of function



void superjets_mode_init (void) {
		//the claw mode can expire on its own and since it is a lower priority it will not display
		//	callset_invoke (end_claw_mode); // this seemed to cause occasional crashes
			clawmagnet_off ();
			flag_off(FLAG_IS_BALL_ON_CLAW);
			flipper_enable ();
	superjets_mode_shots_made = 0;
	flag_on (FLAG_IS_SUPERJETS_RUNNING);
	++superjets_modes_achieved;
	sound_start (ST_SPEECH, SPCH_SUPERJETS_ACTIVATED, SL_4S, PRI_GAME_QUICK5);
	//flash lamp for a time
	lamp_tristate_flash(LM_CLAW_SUPER_JETS);
	task_sleep(TIME_500MS);
	lamp_tristate_on(LM_CLAW_SUPER_JETS);
	score_zero(superjets_mode_score);
	score_zero(superjets_mode_next_score);
	switch (superjets_modes_achieved ){
		case 1:
			score_add(superjets_mode_next_score, score_table[SUPERJETS_HIT_SCORE1]);
			score_add(superjets_mode_score, score_table[SUPERJETS_START_SCORE1]);
			score(SUPERJETS_START_SCORE1);
			break;
		case 2:
			score_add(superjets_mode_next_score, score_table[SUPERJETS_HIT_SCORE2]);
			score_add(superjets_mode_score, score_table[SUPERJETS_START_SCORE2]);
			score(SUPERJETS_START_SCORE2);
			break;
		default:
		case 3:
			score_add(superjets_mode_next_score, score_table[SUPERJETS_HIT_SCORE3]);
			score_add(superjets_mode_score, score_table[SUPERJETS_START_SCORE3]);
			score(SUPERJETS_START_SCORE3);
			break;

	}//end of switch
}//end of function



void superjets_mode_expire (void) {
	flag_off (FLAG_IS_SUPERJETS_RUNNING);
}//end of function


void superjets_mode_exit (void) { superjets_mode_expire();}


/****************************************************************************
 * external event listeners
 ****************************************************************************/
CALLSET_ENTRY (superjets, music_refresh)  	{ timed_mode_music_refresh (&superjets_mode); }
CALLSET_ENTRY (superjets, end_ball) 		{ if (timed_mode_running_p(&superjets_mode) ) timed_mode_end (&superjets_mode); }
CALLSET_ENTRY (superjets, display_update) 	{ timed_mode_display_update (&superjets_mode); }

CALLSET_ENTRY (superjets, start_player) 	{ superjets_player_reset(); }
CALLSET_ENTRY (superjets, start_ball) 		{ superjets_reset(); }


/****************************************************************************
 * body
 *
 ***************************************************************************/
CALLSET_ENTRY (superjets, sw_claw_super_jets) {
	demotime_increment();
	timed_mode_begin (&superjets_mode);//start mode
}//end of function



CALLSET_ENTRY (superjets, sw_jet) {
	if (timed_mode_running_p(&superjets_mode) ) {
		++superjets_mode_shots_made;
		//score higher if mode done more than once
		switch (superjets_modes_achieved) {
			case 0: 	break; //never entered mode?
			case 1:
				score (SUPERJETS_HIT_SCORE1);
				score_add (superjets_mode_score, score_table[SUPERJETS_HIT_SCORE1]);
				break;
			case 2:
				score (SUPERJETS_HIT_SCORE2);
				score_add (superjets_mode_score, score_table[SUPERJETS_HIT_SCORE2]);
				break;
			default:
			case 3:
				score (SUPERJETS_HIT_SCORE3);
				score_add (superjets_mode_score, score_table[SUPERJETS_HIT_SCORE3]);
				break;
			}//end of switch
		if (superjets_mode_shots_made >= superjets_goal) 	superjets_goal_award();
		else /* goal not met yet */  					deff_start (DEFF_SUPERJETS_HIT_EFFECT);
	}//end of if
}//end of function



void superjets_goal_award (void) {
	sound_start (ST_SPEECH, SPCH_SUPERJETS_COMPLETED, SL_4S, PRI_GAME_QUICK5);
	score (SUPERJETS_GOAL_SCORE);
	score_add (superjets_mode_score, score_table[SUPERJETS_GOAL_SCORE]);
	if (superjets_goal < SUPERJETS_GOAL_MAX)  superjets_goal += SUPERJETS_GOAL_STEP;
	timed_mode_end (&superjets_mode);
}//END OF FUNCTION

/****************************************************************************
 * DMD display and sound effects
 ****************************************************************************/
void superjets_raceby (void){
	dmd_alloc_pair_clean ();

	if (sj_counter%2 == 0) 	sound_start (ST_ANY, RACE_BY_SHORT, SL_2S, PRI_GAME_QUICK5);
	else 					sound_start (ST_ANY, RACE_BY_LONG, SL_2S, PRI_GAME_QUICK5);

	bitmap_blit (superjets_across1_bits, 0, 32 - superjets_across1_height);
		dmd_show_low ();
		task_sleep (TIME_66MS);
		dmd_clean_page_low ();
	bitmap_blit (superjets_across2_bits, 0, 32 - superjets_across2_height);
	dmd_show_low ();
	task_sleep (TIME_66MS);
	dmd_clean_page_low ();
		bitmap_blit (superjets_across3_bits, 0, 32 - superjets_across3_height);
		dmd_show_low ();
		task_sleep (TIME_66MS);
		dmd_clean_page_low ();
	bitmap_blit (superjets_across4_bits, 0, 32 - superjets_across4_height);
	dmd_show_low ();
	task_sleep (TIME_66MS);
	dmd_clean_page_low ();
		bitmap_blit (superjets_across5_bits, 0, 32 - superjets_across5_height);
		dmd_show_low ();
		task_sleep (TIME_66MS);
		dmd_clean_page_low ();
	bitmap_blit (superjets_across6_bits, 0, 0);
	dmd_show_low ();
	task_sleep (TIME_66MS);
	dmd_clean_page_low ();
	bitmap_blit (superjets_across7_bits, 0, 0);
		dmd_show_low ();
		task_sleep (TIME_66MS);
		dmd_clean_page_low ();
	bitmap_blit (superjets_across8_bits, 0, 0);
	dmd_show_low ();
	task_sleep (TIME_66MS);
	dmd_clean_page_low ();
	bitmap_blit (superjets_across9_bits, 0, 0);
		dmd_show_low ();
		task_sleep (TIME_66MS);
		dmd_clean_page_low ();
		bitmap_blit (superjets_across10_bits, 0, 0);
	dmd_show_low ();
	task_sleep (TIME_66MS);
	dmd_clean_page_low ();
	bitmap_blit (superjets_across11_bits, 0, 0);
		dmd_show_low ();
		task_sleep (TIME_66MS);
		dmd_clean_page_low ();
		bitmap_blit (superjets_across12_bits, 0, 0);
	dmd_show_low ();
	task_sleep (TIME_66MS);
	dmd_clean_page_low ();
	bitmap_blit (superjets_across13_bits, 16, 0);
		dmd_show_low ();
		task_sleep (TIME_66MS);
		dmd_clean_page_low ();
		bitmap_blit (superjets_across14_bits, 32, 32 - superjets_across14_height);
	dmd_show_low ();
	task_sleep (TIME_66MS);
	dmd_clean_page_low ();
		bitmap_blit (superjets_across15_bits, 48, 32 - superjets_across15_height);
		dmd_show_low ();
		task_sleep (TIME_66MS);
		dmd_clean_page_low ();
	bitmap_blit (superjets_across16_bits, 64, 32 - superjets_across16_height);
	dmd_show_low ();
	task_sleep (TIME_66MS);
	dmd_clean_page_low ();
		bitmap_blit (superjets_across17_bits, 80, 32 - superjets_across17_height);
		dmd_show_low ();
		task_sleep (TIME_66MS);
		dmd_clean_page_low ();
	bitmap_blit (superjets_across18_bits, 96, 32 - superjets_across18_height);
	dmd_show_low ();
	task_sleep (TIME_66MS);
	dmd_clean_page_low ();
}//end of function



void superjets_hit_effect_deff(void) {
	if (superjets_mode_shots_made % 10 == 0) superjets_raceby();
	else {
			U8 i = 0;
			sj_counter = 0;
			U8 superjets_SoundCounter;
			//sounds 		*********************
			superjets_SoundCounter = random_scaled(3);//from kernal/random.c - pick number from 0 to 2
			if ( (superjets_SoundCounter) == 0 ) 		sound_start (ST_SPEECH, SPCH_DULCH, SL_2S, PRI_GAME_QUICK5);
			else if ( (superjets_SoundCounter) == 1 ) 	sound_start (ST_SPEECH, SPCH_WOOH, SL_2S, PRI_GAME_QUICK5);
			else 										sound_start (ST_SPEECH, SPCH_WOW, SL_2S, PRI_GAME_QUICK5);
			//display 		*********************
			do {
				U8 x = random_scaled (6);
				U8 y = random_scaled (4);
				sj_counter++;
				dmd_alloc_low_clean ();
				if (sj_counter < 2)			bitmap_blit (superjets_bounce0_bits, 0, 0);
				else if (sj_counter < 6)	bitmap_blit (superjets_bounce4_bits, 0, 0);
				else 						bitmap_blit (superjets_bounce0_bits, 0, 0);


				sprintf_score (superjets_mode_next_score);
				if ((i % 2) == 0) {
					font_render_string_center (&font_lithograph, DMD_MIDDLE_X + 30 + x, DMD_BIG_CY_Cent + y, sprintf_buffer);
				} else {
					font_render_string_center (&font_lithograph, DMD_MIDDLE_X + 30 - x, DMD_BIG_CY_Cent - y, sprintf_buffer);
				}
				dmd_show_low ();
				task_sleep (TIME_100MS);
			} while (i++ < 8);//about .8sec
			sj_counter = 0;//reset for return to other mode
	}//end of else
	deff_exit ();
}//end of mode_effect_deff


void superjets_effect_deff(void) {
U8 sj_swap = 0;
dmd_alloc_low_clean ();
for (;;) {
		dmd_clean_page_low ();
		sj_counter++;				//display coord in upper left corner - 40 bits wide at largest
		if (sj_counter % 5 == 1)	bitmap_blit (superjets_bounce1_bits, 0, 0);
		if (sj_counter % 5 == 2)	bitmap_blit (superjets_bounce2_bits, 0, 0);
		if (sj_counter % 5 == 3)	bitmap_blit (superjets_bounce3_bits, 0, 0);
		if (sj_counter % 5 == 4)	bitmap_blit (superjets_bounce2_bits, 0, 0);
		if (sj_counter % 5 == 0)	bitmap_blit (superjets_bounce1_bits, 0, 0);

		//128-40 / 2 +40 = 84

		if (sj_counter % 10 == 0) {
			if (sj_swap) 	sj_swap = 0; 		else sj_swap = 1;
		}
		if (sj_swap) 	{
			sprintf ("%d", superjets_mode_shots_made);
			font_render_string_center (&font_var5, 84, DMD_SMALL_CY_5, sprintf_buffer);
			font_render_string_center (&font_antiqua, 84, DMD_SMALL_CY_1, "SUPER");
			font_render_string_center (&font_antiqua, 84, DMD_SMALL_CY_3, "JETS");
		}
		else 			{
			sprintf ("%d TO GOAL", superjets_goal);
			font_render_string_center (&font_var5, 84, DMD_SMALL_CY_5, sprintf_buffer);
			font_render_string_center (&font_antiqua, 84, DMD_SMALL_CY_1, "SHOOT");
			font_render_string_center (&font_antiqua, 84, DMD_SMALL_CY_3, "BUMPERS");
		}

		dmd_show_low ();
		task_sleep (TIME_300MS);
	}//END OF ENDLESS LOOP
}//end of mode_effect_deff

