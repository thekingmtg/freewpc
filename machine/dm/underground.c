/*
 * demolition man
 * orbits.c
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
 */

#include <freewpc.h>

//constants

//local variables
U8		underground_SoundCounter;
U8 		underground_counter;
U8 		underground_goal;
__boolean 		underground_Jackpot_activated;
__boolean 		underground_Arrow_activated;

//external variables
extern __boolean 		prison_break_mode_activated; //from prison_break.c
extern  __boolean 		capture_simon_mode_activated; //from capture_simon.c

//prototypes
void underground_reset (void);

/****************************************************************************
 * initialize  and exit
 ***************************************************************************/
void underground_reset (void) {
	underground_counter = 0;
	underground_goal = 0;
	underground_Jackpot_activated = FALSE;
	underground_Arrow_activated = FALSE;
	}//end of function

void player_underground_reset(void) {
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
	underground_Jackpot_activated = TRUE;
	lamp_tristate_on (LM_UNDERGROUND_JACKPOT);
	}

CALLSET_ENTRY (underground, underground_Jackpot_Light_Off) {
	underground_Jackpot_activated = FALSE;
	lamp_tristate_off (LM_UNDERGROUND_JACKPOT);
	}

//lit by combo shots --TODO:
CALLSET_ENTRY (underground, underground_Arrow_Light_On) {
	underground_Arrow_activated = TRUE;
	lamp_tristate_on (LM_UNDERGROUND_ARROW);
	}

CALLSET_ENTRY (underground, underground_Arrow_Light_Off) {
	underground_Arrow_activated = FALSE;
	lamp_tristate_off (LM_UNDERGROUND_ARROW);
	}

/****************************************************************************
 * body
 *
 ****************************************************************************/
CALLSET_ENTRY (underground, sw_bottom_popper) {
	++underground_counter;
	score (SC_100K);//located in kernal/score.c
	if ( (underground_SoundCounter++ % 2) == 0 )//check if even
		sound_start (ST_EFFECT, SUBWAY, SL_1S, SP_NORMAL);
	else
		sound_start (ST_EFFECT, SUBWAY2, SL_1S, SP_NORMAL);
	if(prison_break_mode_activated)  callset_invoke(prison_break_made);
	if(capture_simon_mode_activated)  callset_invoke(capture_simon_made);
	//TODO: jackpot and combo shot detection
	//	if (underground_Jackpot_activated)  ;
	//	if(underground_Arrow_activated) callset_invoke();
	}//end of function



/****************************************************************************
 * DMD display and sound effects
 ****************************************************************************/
