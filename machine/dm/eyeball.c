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

#include <freewpc.h>
#include "dm/global_constants.h"

//constants
const U8 EYE_EASY_GOAL = 3;
const U8 EYE_MED_GOAL = 4;
const U8 EYE_HARD_GOAL = 5;
const U8 EYE_GOAL_STEP = 3;
const U8 EYE_GOAL_MAX = 50;

const U8 EYE_EB_EASY_GOAL = 5;
const U8 EYE_EB_MED_GOAL = 8;
const U8 EYE_EB_HARD_GOAL = 12;
const U8 EYE_EB_GOAL_STEP = 5;
const U8 EYE_EB_GOAL_MAX = 50;

//local variables
U8 eyeball_shots_made;//for current ball only
U8 eyeball_eb_shots_made;//for current ball only
__local__ U8 total_eyeball_shots_made;//for entire game
__local__ U8 eyeball_goal;//num of hits to start explode
__local__ U8 eyeball_eb_goal;//num of hits to light extraball
score_t temp_score;

//external variables
extern 	U8 				jet_shots_made;//found in jets_superjets.c

//prototypes
void eyeball_reset (void);
void player_eyeball_reset (void);
void eyeball_goal_award (void);
void eyeball_eb_award (void);
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
		callset_invoke(start_explode);//start explode mode
		if (eyeball_goal < EYE_GOAL_MAX)  eyeball_goal += EYE_GOAL_STEP;
}//end of function


void eyeball_eb_award (void) {
		callset_invoke(extraball_light_on);
		eyeball_eb_shots_made = 0;
		sound_start (ST_SPEECH, SPCH_LOVE_WHEN_THAT_HAPPENS, SL_2S, PRI_GAME_QUICK5);
		if (eyeball_eb_goal < EYE_EB_GOAL_MAX)  eyeball_eb_goal += EYE_EB_GOAL_STEP;
}//end of function


CALLSET_ENTRY (eyeball, sw_eyeball_standup) {
	++eyeball_shots_made;
	sound_start (ST_SAMPLE, EXPLOSION1_MED, SL_2S, PRI_GAME_QUICK1);
	flasher_pulse (FLASH_EYEBALL_FLASHER); //FLASH followed by name of flasher in caps
	flasher_pulse (FLASH_EYEBALL_FLASHER); //FLASH followed by name of flasher in caps
	flasher_pulse (FLASH_EYEBALL_FLASHER); //FLASH followed by name of flasher in caps
	score (SC_5M);
	//100k per jet hit here
	if (jet_shots_made > 0) {
		score_zero (temp_score);//zero out temp score
		score_add (temp_score, score_table[SC_100K]);//multiply 100K by jet count
		score_mul (temp_score, jet_shots_made);//multiply 100K by jet count
		score_long (temp_score); //add temp score to player's score
	}//end of if
	//light extra ball
	if (eyeball_eb_shots_made == eyeball_eb_goal)  eyeball_eb_award();
	if (eyeball_shots_made == eyeball_goal)  eyeball_goal_award();//start explode
}//end of function




