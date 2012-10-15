/*
 * demolition man
 * eyeball.c
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
 *
 */
/* CALLSET_SECTION (eyeball, __machine5__) */

#include <freewpc.h>
#include "dm/global_constants.h"
#include "eyeball.h"

//constants
const U8 EYE_EASY_GOAL = 2;
const U8 EYE_MED_GOAL = 3;
const U8 EYE_HARD_GOAL = 5;
const U8 EYE_GOAL_STEP = 3;
const U8 EYE_GOAL_MAX = 50;

const U8 EYE_EB_EASY_GOAL = 5;
const U8 EYE_EB_MED_GOAL = 8;
const U8 EYE_EB_HARD_GOAL = 12;
const U8 EYE_EB_GOAL_STEP = 5;
const U8 EYE_EB_GOAL_MAX = 50;

//local variables
U8	eyeball_MessageCounter;
U8 eyeball_shots_made;//for current ball only
U8 eyeball_eb_shots_made;//for current ball only
__local__ U8 total_eyeball_shots_made;//for entire game
__local__ U8 eyeball_goal;//num of hits to start explode
__local__ U8 eyeball_eb_goal;//num of hits to light extraball
score_t temp_score;

//external variables
extern 	U8 				jet_shots_made;//found in jets_superjets.c

//internally called function prototypes  --external found at protos.h
void eyeball_reset (void);
void player_eyeball_reset (void);
void eyeball_goal_award (void);
void eyeball_eb_award (void);
void eyeball_effect_chooser(void);

/****************************************************************************
 * initialize  and exit
 ***************************************************************************/
void player_eyeball_reset (void) {
	total_eyeball_shots_made = 0;
	eyeball_shots_made = 0;
	eyeball_goal = EYE_EASY_GOAL;
	eyeball_eb_goal = EYE_EB_EASY_GOAL;
}//end of function

void eyeball_reset (void) {
	eyeball_shots_made = 0;
}//end of function

/****************************************************************************
 * external event listeners
 ****************************************************************************/
CALLSET_ENTRY (eyeball, start_player) 	{ player_eyeball_reset(); }
CALLSET_ENTRY (eyeball, start_ball) 	{ eyeball_reset(); }

/****************************************************************************
 * body
 *
 ***************************************************************************/
void eyeball_goal_award (void) {
		eyeball_shots_made = 0;
		sound_start (ST_SPEECH, SPCH_LOVE_WHEN_THAT_HAPPENS, SL_2S, PRI_GAME_QUICK5);
		start_explode();//start explode mode
		if (eyeball_goal < EYE_GOAL_MAX)  eyeball_goal += EYE_GOAL_STEP;
}//end of function


void eyeball_eb_award (void) {
		extraball_light_on();
		eyeball_eb_shots_made = 0;
		sound_start (ST_SPEECH, SPCH_LOVE_WHEN_THAT_HAPPENS, SL_2S, PRI_GAME_QUICK5);
		if (eyeball_eb_goal < EYE_EB_GOAL_MAX)  eyeball_eb_goal += EYE_EB_GOAL_STEP;
}//end of function


CALLSET_ENTRY (eyeball, sw_eyeball_standup) {
	++eyeball_shots_made;
	sound_start (ST_SAMPLE, EXPLOSION1_MED, SL_2S, PRI_GAME_QUICK1);
	flasher_pulse (FLASH_EYEBALL_FLASHER); //FLASH followed by name of flasher in caps
	if (eyeball_eb_shots_made == eyeball_eb_goal)  eyeball_eb_award();
	if (eyeball_shots_made == eyeball_goal)  eyeball_goal_award();//start explode
	if ( 	(!eyeball_eb_shots_made == eyeball_eb_goal)
		&& 	(!eyeball_shots_made == eyeball_goal)  )
				eyeball_effect_chooser();
	score (SC_5M);
	//100k per jet hit here
	if (jet_shots_made > 0) {
		score_zero (temp_score);//zero out temp score
		score_add (temp_score, score_table[SC_100K]);//multiply 100K by jet count
		score_mul (temp_score, jet_shots_made);//multiply 100K by jet count
		score_long (temp_score); //add temp score to player's score
	}//end of if
	//light extra ball
}//end of function




/****************************************************************************
 * display effects
 ****************************************************************************/
void eyeball_effect_chooser(void) {
	switch (++eyeball_MessageCounter % 4) {
		case 0: deff_start (DEFF_EYEBALL1_EFFECT); break;
		case 1: deff_start (DEFF_EYEBALL2_EFFECT); break;
		case 2: deff_start (DEFF_EYEBALL3_EFFECT); break;
		case 3: deff_start (DEFF_EYEBALL4_EFFECT); break;
	}//END OF SWITCH
}//end of mode_effect_deff



void eyeball1_effect_deff(void) {
	dmd_alloc_low_clean ();
	dmd_sched_transition (&trans_scroll_up);
	sprintf ("OOOHHH");
	font_render_string_center (&font_lithograph, DMD_MIDDLE_X + 25, DMD_BIG_CY_Top, sprintf_buffer);
	sprintf ("MY EYE");
	font_render_string_center (&font_lithograph, DMD_MIDDLE_X + 25, DMD_BIG_CY_Bot, sprintf_buffer);
	bitmap_blit (eye3_bits, 2, 2);
	dmd_show_low ();
	task_sleep_sec (1);
	task_sleep (TIME_500MS);
	deff_exit ();
}//end of mode_effect_deff



void eyeball2_effect_deff(void) {
	dmd_alloc_low_clean ();
	dmd_sched_transition (&trans_scroll_up);
	font_render_string_center (&font_var5, DMD_MIDDLE_X + 20, DMD_SMALL_CY_1, "JETS");
	font_render_string_center (&font_var5, DMD_MIDDLE_X + 20, DMD_SMALL_CY_2, "INCREASE");
	font_render_string_center (&font_lithograph, DMD_MIDDLE_X, DMD_BIG_CY_Bot, "RETINA SCAN");

	bitmap_blit (eye3SM_bits, 15, 2);

	dmd_show_low ();
	task_sleep_sec (1);
	task_sleep (TIME_500MS);
	deff_exit ();
}//end of mode_effect_deff



void eyeball3_effect_deff(void) {
	dmd_alloc_low_clean ();
	dmd_sched_transition (&trans_scroll_up);
	font_render_string_center (&font_var5, DMD_MIDDLE_X - 30, DMD_SMALL_CY_1, "EXTRA BALL");
	font_render_string_center (&font_var5, DMD_MIDDLE_X - 30, DMD_SMALL_CY_2, "AT");
	sprintf ("%d HITS", eyeball_eb_goal-eyeball_eb_shots_made);
	font_render_string_center (&font_lithograph, DMD_MIDDLE_X - 30, DMD_BIG_CY_Bot, sprintf_buffer);
	bitmap_blit (eye5_bits, 80, 2);
	dmd_show_low ();
	task_sleep_sec (1);
	task_sleep (TIME_500MS);
	deff_exit ();
}//end of mode_effect_deff



void eyeball4_effect_deff(void) {
	dmd_alloc_pair_clean ();
	dmd_sched_transition (&trans_scroll_up);
	font_render_string_center (&font_var5, DMD_MIDDLE_X - 25, DMD_SMALL_CY_1, "EXPLODE");
	font_render_string_center (&font_var5, DMD_MIDDLE_X - 25, DMD_SMALL_CY_2, "AT");
	sprintf ("%d HITS", eyeball_goal-eyeball_shots_made);
	font_render_string_center (&font_lithograph, DMD_MIDDLE_X - 25, DMD_BIG_CY_Bot, sprintf_buffer);
	bitmap_blit (eye3_bits, 86, 2);
	dmd_show_low ();
	task_sleep_sec (1);
	task_sleep (TIME_500MS);
	deff_exit ();
}//end of mode_effect_deff





