/*
 * demolition man
 * underground.c
 *
 * written by James Cardona
 *
 * Location Description:
 * subway / underground is a scoop across from the upper flipper and to the left of the jets
 * and to the right of the center ramp, with the opening on the left side. It is an easy
 * shot from the upper left flipper, and it can also be made with a little luck from either
 * of the lower flippers. The Computer and Demolition Time are awarded here. The
 * Underground has a Jackpot lamp and a Combo arrow. The ball is returned, via a VUK
 * to the right ramp habitrail. The Computer and Demolition time are indicated by
 * yellow and blue lights mounted over the Underground entrance.
 *
 * The Computer is accessed via the UnderGround shot and is lit by a yellow lamp
 * immediately above. Lit every 10 combos, the Computer is a random award like
 * the Camera on Twilight Zone. Awards seen so far :
 * Light Extra Ball
 * Light Explodes (lights all four explode 10M lamps)
 * 3x Car Crash
 * 2x Retina Scan
 * Light Arrows (lights Laser millions on all combo shots)
 * Maximize Freezes (freezes four balls and lights multiball)
 * Collect Bonus (complete with bonus music!)
 * Collect Standups (awards all the lit yellow targets)
 *
 * Scoring Description: (original game)
 *
 *
 * Scoring Description: (my rules)
 * same as above except
 *
 */

#include <freewpc.h>
#include "dm/global_constants.h"

//constants
const U8 		computerAwardsNumOfSounds = 7; //num between 0 and 6
const sound_code_t computerAwardsSoundsArray[] = {	COMPUTER1, 				COMPUTER_ADDING,   COMPUTER_AWARD_SMALL,
												COMPUTER_AWARD_LONG,	COMPUTER2, 			SPCH_ACCESSING,
												SPCH_COMPUTER_AWARD};

//local variables
U8 		underground_shots_made;
U8 		underground_goal;
__boolean 		is_underground_Jackpot_activated;
//this will be external and in combo function when written
__boolean 		next_combo_total;
U8				computerAwards;

//external variables

//prototypes
void underground_reset (void);

/****************************************************************************
 * initialize  and exit
 ***************************************************************************/
void underground_reset (void) {
	underground_shots_made = 0;
	underground_goal = 0;
	is_underground_Jackpot_activated = FALSE;
}//end of function

void player_underground_reset(void) {
	//this will be external and in combo function when written
	next_combo_total = FALSE;
	underground_reset();
}//end of function

CALLSET_ENTRY (underground, start_player) { player_underground_reset(); }
CALLSET_ENTRY (underground, start_ball) { underground_reset(); }


/****************************************************************************
 * playfield lights and flags
 ***************************************************************************/
//lit by multiball modes --TODO:
CALLSET_ENTRY (underground, underground_jackpot_light_on) {
	is_underground_Jackpot_activated = TRUE;
	lamp_tristate_on (LM_UNDERGROUND_JACKPOT);
}//end of function

CALLSET_ENTRY (underground, underground_jackpot_light_off) {
	is_underground_Jackpot_activated = FALSE;
	lamp_tristate_off (LM_UNDERGROUND_JACKPOT);
}//end of function

CALLSET_ENTRY (underground, underground_arrow_light_on) {
	flag_on (FLAG_IS_UGROUND_ARROW_ACTIVATED);
	lamp_tristate_on (LM_UNDERGROUND_ARROW);
}//end of function

CALLSET_ENTRY (underground, underground_arrow_light_off) {
	flag_off (FLAG_IS_UGROUND_ARROW_ACTIVATED);
	lamp_tristate_off (LM_UNDERGROUND_ARROW);
}//end of function

/****************************************************************************
 * body
 *
 ****************************************************************************/
CALLSET_ENTRY (underground, sw_bottom_popper) {
	++underground_shots_made;
	score (SC_100K);//located in kernal/score.c
	U8	underground_SoundCounter;
	underground_SoundCounter = random_scaled(2);//from kernal/random.c
	if (underground_SoundCounter == 0 )
		sound_start (ST_EFFECT, SUBWAY, SL_2S, SP_NORMAL);
	else if (underground_SoundCounter == 1 )
		sound_start (ST_EFFECT, SUBWAY2, SL_2S, SP_NORMAL);
	if (flag_test (FLAG_IS_PBREAK_UNDER_ACTIVATED) )  	callset_invoke(prison_break_made);
	if (flag_test (FLAG_IS_CAPSIM_UNDER_ACTIVATED) )	callset_invoke(capture_simon_made);
	//TODO: jackpot and combo shot detection
	//	if (is_underground_Jackpot_activated) //during multiball


//	if (flag_test(FLAG_IS_COMBO_UNDER_ACTIVATED) ) {
	//call combo shot updater and underground_arrowhandler
//	}

	if (next_combo_total) callset_invoke(computer_award);
	//if nothing special, do normal display effects
	if(		!flag_test (FLAG_IS_PBREAK_UNDER_ACTIVATED)
		&& 	!flag_test (FLAG_IS_CAPSIM_UNDER_ACTIVATED)
		&& 	!next_combo_total
		&& 	!flag_test (FLAG_IS_UGROUND_ARROW_ACTIVATED)
		&& 	!is_underground_Jackpot_activated)
						callset_invoke(start_underground_deff);
}//end of function


//called after minimum of every 10 combos and underground shot made
CALLSET_ENTRY (underground, computer_award) {
	computerAwards = random_scaled(computerAwardsNumOfSounds);//from kernal/random.c
	sound_start (ST_SPEECH, computerAwardsSoundsArray[computerAwards], SL_2S, PRI_GAME_QUICK5);
	switch (computerAwards) {
	case 0 :{
			sound_start(ST_SPEECH, SPCH_COLLECT_BONUS, SL_4S, PRI_GAME_QUICK5);
			//TODO: put bonus routine here
			}
	case 1 :{
			sound_start(ST_SPEECH, SPCH_TRIPLE_CAR_CRASH, SL_4S, PRI_GAME_QUICK5);
			//TODO: put routine here
			}
	case 2 :{
			sound_start(ST_SPEECH, SPCH_COLLECT_STANDUPS, SL_4S, PRI_GAME_QUICK5);
			//TODO: put routine here
			}
	case 3 :{
			sound_start(ST_SPEECH, SPCH_LIGHT_ARROWS, SL_4S, PRI_GAME_QUICK5);
			//TODO: put routine here
			}
	case 4 :{
			sound_start(ST_SPEECH, SPCH_LIGHT_EXTRA_BALL, SL_4S, PRI_GAME_QUICK5);
			//TODO: put routine here
			}
	case 5 :{
			sound_start(ST_SPEECH, SPCH_MAXIMIZE_FREEZES, SL_4S, PRI_GAME_QUICK5);
			//TODO: put routine here
			}
	case 6 :{
			sound_start(ST_SPEECH, SPCH_DOUBLE_RETINA_SCAN, SL_4S, PRI_GAME_QUICK5);
			//TODO: put routine here
			}
	}//end of switch
}//end of function


