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

//constants
const U8 		computerAwardsNumOfSounds = 7; //num between 0 and 6

//local variables
U8		underground_SoundCounter;
U8 		underground_shots_made;
U8 		underground_goal;
__boolean 		is_underground_Jackpot_activated;
__boolean 		is_underground_Arrow_activated;
//this will be external and in combo function when written
__boolean 		next_combo_total;
U8				computerAwards;
sound_code_t computerAwardsSoundsArray[] = {	COMPUTER1, 				COMPUTER_ADDING,   COMPUTER_AWARD_SMALL,
												COMPUTER_AWARD_LONG,	COMPUTER2, 			SPCH_ACCESSING,
												SPCH_COMPUTER_AWARD};

//external variables
extern 	__boolean 		is_prison_break_mode_activated; //from prison_break.c
extern  __boolean 		is_capture_simon_mode_activated; //from capture_simon.c
extern 	__boolean 		inTest; //located in global_constants.c

//prototypes
void underground_reset (void);

/****************************************************************************
 * initialize  and exit
 ***************************************************************************/
void underground_reset (void) {
	underground_shots_made = 0;
	underground_goal = 0;
	is_underground_Jackpot_activated = FALSE;
	is_underground_Arrow_activated = FALSE;
	}//end of function

void player_underground_reset(void) {
	//this will be external and in combo function when written
	next_combo_total = FALSE;
	underground_SoundCounter = 0;
	underground_reset();
}

CALLSET_ENTRY (underground, start_player) { player_underground_reset(); }
CALLSET_ENTRY (underground, start_ball) { underground_reset(); }


/****************************************************************************
 * playfield lights and flags
 ***************************************************************************/
//lit by multiball modes --TODO:
CALLSET_ENTRY (underground, underground_Jackpot_Light_On) {
	is_underground_Jackpot_activated = TRUE;
	lamp_tristate_on (LM_UNDERGROUND_JACKPOT);
	}

CALLSET_ENTRY (underground, underground_Jackpot_Light_Off) {
	is_underground_Jackpot_activated = FALSE;
	lamp_tristate_off (LM_UNDERGROUND_JACKPOT);
	}

//lit by combo shots --TODO:
CALLSET_ENTRY (underground, underground_Arrow_Light_On) {
	is_underground_Arrow_activated = TRUE;
	lamp_tristate_on (LM_UNDERGROUND_ARROW);
	}

CALLSET_ENTRY (underground, underground_Arrow_Light_Off) {
	is_underground_Arrow_activated = FALSE;
	lamp_tristate_off (LM_UNDERGROUND_ARROW);
	}

/****************************************************************************
 * body
 *
 ****************************************************************************/
CALLSET_ENTRY (underground, sw_bottom_popper) {
	++underground_shots_made;
	score (SC_100K);//located in kernal/score.c
	if ( (underground_SoundCounter++ % 2) == 0 )//check if even
		sound_start (ST_EFFECT, SUBWAY, SL_1S, SP_NORMAL);
	else
		sound_start (ST_EFFECT, SUBWAY2, SL_1S, SP_NORMAL);
	if(is_prison_break_mode_activated)  callset_invoke(prison_break_made);
	if(is_capture_simon_mode_activated)  callset_invoke(capture_simon_made);
	//TODO: jackpot and combo shot detection
	//	if (is_underground_Jackpot_activated)  ;
	//	if(is_underground_Arrow_activated) allset_invoke();
	if (next_combo_total ==TRUE) callset_invoke(computer_award);
	}//end of function


//called after minimum of every 10 combos and underground shot made
CALLSET_ENTRY (underground, computer_award) {
	computerAwards = random_scaled(computerAwardsNumOfSounds);//from kernal/random.c
	sound_start (ST_SPEECH, computerAwardsSoundsArray[computerAwards], SL_2S, PRI_GAME_QUICK5);
	switch (computerAwards) {
	case 0 :{
			sound_start(ST_SPEECH, SPCH_COLLECT_BONUS, SL_2S, PRI_GAME_QUICK5);
			//TODO: put bonus routine here
			}
	case 1 :{
			sound_start(ST_SPEECH, SPCH_TRIPLE_CAR_CRASH, SL_2S, PRI_GAME_QUICK5);
			//TODO: put routine here
			}
	case 2 :{
			sound_start(ST_SPEECH, SPCH_COLLECT_STANDUPS, SL_2S, PRI_GAME_QUICK5);
			//TODO: put routine here
			}
	case 3 :{
			sound_start(ST_SPEECH, SPCH_LIGHT_ARROWS, SL_2S, PRI_GAME_QUICK5);
			//TODO: put routine here
			}
	case 4 :{
			sound_start(ST_SPEECH, SPCH_LIGHT_EXTRA_BALL, SL_2S, PRI_GAME_QUICK5);
			//TODO: put routine here
			}
	case 5 :{
			sound_start(ST_SPEECH, SPCH_MAXIMIZE_FREEZES, SL_2S, PRI_GAME_QUICK5);
			//TODO: put routine here
			}
	case 6 :{
			sound_start(ST_SPEECH, SPCH_DOUBLE_RETINA_SCAN, SL_2S, PRI_GAME_QUICK5);
			//TODO: put routine here
			}
	}//end of switch
}//end of function


/****************************************************************************
 * DMD display and sound effects
 ****************************************************************************/


/****************************************************************************
 * status display
 *
 * called from common/status.c automatically whenever either flipper button
 * is held for 4 seconds or longer.  since called by callset, order of
 * various status reports will be random depending upon call stack
****************************************************************************/
CALLSET_ENTRY (underground, status_report){
	if (inTest) {
		sprintf ("%d underground shots made", underground_shots_made);
		font_render_string_center (&font_mono5, 64, 19, sprintf_buffer);
	}//end of 	if (inTest)
	//deff_exit (); is called at end of calling function - not needed here?
}//end of function

