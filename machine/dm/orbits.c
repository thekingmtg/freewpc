/*
 * demolition man
 * orbits.c
 *
 * written by James Cardona
 *
 * Location Description:
 * Also called loops.
 * Left Loop: This loop is just to the right of the upper flipper.
 * It feeds to a sinkhole when hit hard enough; otherwise, the ball passes
 * behind the top ramp into the MTL rollovers or can pass all the way to
 * the right loop -making a complete orbit.
 * This shot can be lit to start multiball and award extra balls, otherwise it
 * awards a mystery bonus.
 * Lights are, from bottom up, Extra Ball, Start Multi-Ball, Explode, Jackpot, Combo arrow.
 *
 * Right Loop: This shot feeds the ball past the MTL rollovers and all
 * the way around to the upper left flipper. If it doesn't make it
 * (ie: weak shot) the ball will fall into the MTL rollovers.
 * Lights are, from bottom up, Explode, Jackpot, Combo Arrow.
 *
 * Scoring Description: (original game)
 *
 * depending upon # of freeze required, shoot left loop will start MB with X num of balls,
 * ends when 1 ball left if shoot light freeze from claw will grant instant 5 ball MB
 * if at least 1 ball already frozen, else will light a ball
 * and shoot left loop to start MB
 *
 *
 *
 */

#include <freewpc.h>

//constants
const U8 ORBITS_EASY_GOAL = 5;
const U8 ORBITS_PREDEFINED_GOAL_INCREMENT = 1;
const U8 ORBITS_GOAL_STEP = 5;
const U8 ORBITS_GOAL_MAX = 50;

//local variables
U8		orbits_SoundCounter;
U8 		left_loop_counter;
U8 		right_loop_counter;
U8 		all_loop_counter;
U8 			right_loop_goal;
U8 			left_loop_goal;
U8 			all_loop_goal;
__boolean 		left_Loop_ExtraBall_activated;
__boolean 		left_Loop_MultiBall_activated;
__boolean 		left_Loop_Explode_activated;
__boolean 		left_Loop_Jackpot_activated;
__boolean 		left_Loop_Arrow_activated;
__boolean 			right_Loop_Explode_activated;
__boolean 			right_Loop_Jackpot_activated;
__boolean 			right_Loop_Arrow_activated;

//external variables

//prototypes
void orbits_reset (void);
void left_orbit_task (void);
void right_orbit_task (void);
void right_loop_goal_award (void);
void left_loop_goal_award (void);

/****************************************************************************
 * initialize  and exit
 ***************************************************************************/
void orbits_reset (void) {
	left_loop_counter = 0;
	right_loop_counter = 0;
	all_loop_counter = 0;

	left_Loop_ExtraBall_activated = FALSE;
	left_Loop_MultiBall_activated = FALSE;
	left_Loop_Explode_activated = FALSE;
	left_Loop_Jackpot_activated = FALSE;
	left_Loop_Arrow_activated = FALSE;

	right_Loop_Explode_activated = FALSE;
	right_Loop_Jackpot_activated = FALSE;
	right_Loop_Arrow_activated = FALSE;
}//end of function

void player_orbits_reset (void) {
	orbits_SoundCounter = 0;
	right_loop_goal = 0;
	left_loop_goal=ORBITS_EASY_GOAL;
	all_loop_goal=(ORBITS_EASY_GOAL * 4);
	orbits_reset();
}//end of function

CALLSET_ENTRY (orbits, start_player) { player_orbits_reset(); }
CALLSET_ENTRY (orbits, start_ball) { orbits_reset(); }


/****************************************************************************
 * playfield lights and flags
 ***************************************************************************/
//lit by 5 MTL rollovers --see rollovers.c
CALLSET_ENTRY (orbits, ExtraBall_Light_On) {
	left_Loop_ExtraBall_activated = TRUE;
	lamp_tristate_on (LM_EXTRA_BALL);
	if ( (orbits_SoundCounter++ % 2) == 0 )//check if even
		sound_start (ST_SPEECH, SPCH_GET_THE_EXTRABALL, SL_4S, PRI_GAME_QUICK5);
	else
		sound_start (ST_SPEECH, SPCH_NEED_EXTRABALL, SL_4S, PRI_GAME_QUICK5);
}//end of function

CALLSET_ENTRY (orbits, ExtraBall_Light_Off) {
	left_Loop_ExtraBall_activated = FALSE;
	lamp_tristate_off (LM_EXTRA_BALL);
}//end of function

//lit by required number of freezes accomplished - called by lock_freeze_mbstart.c
//note well: capitalizing this will make some errors in the compiler
CALLSET_ENTRY (orbits, multiball_light_on) {
	left_Loop_MultiBall_activated = TRUE;
	lamp_tristate_on (LM_START_MULTIBALL);
}//end of function

CALLSET_ENTRY (orbits, multiball_light_off) {
	left_Loop_MultiBall_activated = FALSE;
	lamp_tristate_off (LM_START_MULTIBALL);
}//end of function

//lit by eyeball mode start --see eyeball_explode.c
CALLSET_ENTRY (orbits, Activate_Explode_Inserts) {
	left_Loop_Arrow_activated = TRUE;
	right_Loop_Arrow_activated = TRUE;
	lamp_tristate_flash (LM_LEFT_LOOP_EXPLODE);
	lamp_tristate_flash (LM_RIGHT_LOOP_EXPLODE);
}//end of function

CALLSET_ENTRY (orbits, DeActivate_Explode_Inserts) {
	left_Loop_Arrow_activated = FALSE;
	right_Loop_Arrow_activated = FALSE;
	lamp_tristate_off (LM_LEFT_LOOP_EXPLODE);
	lamp_tristate_off (LM_RIGHT_LOOP_EXPLODE);
}//end of function

//lit by multiball modes --TODO:
CALLSET_ENTRY (orbits, LL_Jackpot_Light_On) {
	left_Loop_Jackpot_activated = TRUE;
	lamp_tristate_on (LM_LEFT_LOOP_JACKPOT);
}//end of function

CALLSET_ENTRY (orbits, LL_Jackpot_Light_Off) {
	left_Loop_Jackpot_activated = FALSE;
	lamp_tristate_off (LM_LEFT_LOOP_JACKPOT);
}//end of function

//lit by multiball modes --TODO:
CALLSET_ENTRY (orbits, RL_Jackpot_Light_On) {
	right_Loop_Jackpot_activated = TRUE;
	lamp_tristate_on (LM_RIGHT_LOOP_JACKPOT);
}//end of function

CALLSET_ENTRY (orbits, RL_Jackpot_Light_Off) {
	right_Loop_Jackpot_activated = FALSE;
	lamp_tristate_off (LM_RIGHT_LOOP_JACKPOT);
}//end of function

//lit by combo shots --TODO:
CALLSET_ENTRY (orbits, LL_Arrow_Light_On) {
	left_Loop_Arrow_activated = TRUE;
	lamp_tristate_on (LM_LEFT_LOOP_ARROW);
}//end of function

CALLSET_ENTRY (orbits, LL_Arrow_Light_Off) {
	left_Loop_Arrow_activated = FALSE;
	lamp_tristate_off (LM_LEFT_LOOP_ARROW);
}//end of function

//lit by combo shots --TODO:
CALLSET_ENTRY (orbits, RL_Arrow_Light_On) {
	right_Loop_Arrow_activated = TRUE;
	lamp_tristate_on (LM_RIGHT_LOOP_ARROW);
}//end of function

CALLSET_ENTRY (orbits, RL_Arrow_Light_Off) {
	right_Loop_Arrow_activated = FALSE;
	lamp_tristate_off (LM_RIGHT_LOOP_ARROW);
}//end of function


/****************************************************************************
 * body
 *
 ****************************************************************************/
void left_orbit_task (void) { task_sleep_sec(2); task_exit(); }
void right_orbit_task (void) { task_sleep_sec(2); task_exit(); }

// full orbit right to left
// or start left to right check
CALLSET_ENTRY (orbits, sw_left_loop) {
//	if ( !single_ball_play() ) return;  //turn off during multiball?
	if ( task_kill_gid(GID_RIGHT_ORBIT_MADE) ) callset_invoke(right_orbit_shot_made);
	else {
		task_create_gid1 (GID_LEFT_ORBIT_MADE, left_orbit_task);
		if ( (orbits_SoundCounter++ % 2) == 0 )//check if even
			sound_start (ST_EFFECT, RACE_BY, SL_2S, PRI_GAME_QUICK5);
		else
			sound_start (ST_EFFECT, RACE_BY_2, SL_2S, PRI_GAME_QUICK5);
		}//end of else
}//end of function

// full orbit left to right
// or start right to left check
CALLSET_ENTRY (orbits, sw_right_freeway) {
//	if ( !single_ball_play () ) return;  //turn off during multiball?
	if ( task_kill_gid(GID_LEFT_ORBIT_MADE) ) callset_invoke(left_orbit_shot_made);
	else {
		task_create_gid1 (GID_RIGHT_ORBIT_MADE, right_orbit_task);
		if ( (orbits_SoundCounter++ % 2) == 0 )//check if even
			sound_start (ST_EFFECT, RACE_BY, SL_2S, PRI_GAME_QUICK5);
		else
			sound_start (ST_EFFECT, RACE_BY_2, SL_2S, PRI_GAME_QUICK5);
		}//end of else
}//end of function

// full orbit left to hole
CALLSET_ENTRY (orbits, sw_top_popper) {
	if ( task_kill_gid(GID_LEFT_ORBIT_MADE) ) callset_invoke(orbit_to_popper_made);
}//end of orbits_sw_top_popper



CALLSET_ENTRY (orbits, orbit_to_popper_made) {
	sound_start (ST_EFFECT, RACE_BY_3, SL_2S, PRI_GAME_QUICK5);
	//extra ball shot made
	if (left_Loop_ExtraBall_activated ) {
			callset_invoke(ExtraBall_Light_Off);
			sol_request(SOL_KNOCKER);
			sound_start (ST_SAMPLE, EXTRA_BALL_SOUND, SL_2S, PRI_GAME_QUICK5);
			if ( (orbits_SoundCounter++ % 2) == 0 )//check if even
				sound_start (ST_SPEECH, SPCH_EXTRABALL_WES, SL_4S, PRI_GAME_QUICK5);
			else
				sound_start (ST_SPEECH, SPCH_EXTRABALL_SLY, SL_4S, PRI_GAME_QUICK5);
			//TODO: add an extra ball here
			}//end of left_Loop_ExtraBall_activated
	if (left_Loop_MultiBall_activated) callset_invoke(multiball_start);
	//TODO: random top popper award
}//end of function

CALLSET_ENTRY (orbits, left_orbit_shot_made) {
	++left_loop_counter;
	++all_loop_counter;
	sound_start (ST_SAMPLE, MACHINE12, SL_2S, PRI_GAME_QUICK1);
	score (SC_100K);//located in kernal/score.c
	if(flag_test (FLAG_IS_EXPLODE_MODE_ACTIVATED) ) callset_invoke(explode_made);
	if(flag_test (FLAG_IS_CAPSIM_LEFTORB_ACTIVATED) )  callset_invoke(capture_simon_made);

	//TODO: jackpot and combo shot detection
	if (left_loop_counter == left_loop_goal)  left_loop_goal_award ();
}//end of function

CALLSET_ENTRY (orbits, right_orbit_shot_made) {
	++right_loop_counter;
	++all_loop_counter;
	score (SC_100K);//located in kernal/score.c
	sound_start (ST_SAMPLE, MACHINE12, SL_2S, PRI_GAME_QUICK1);
	if(flag_test (FLAG_IS_EXPLODE_MODE_ACTIVATED) ) callset_invoke(explode_made);
	if(flag_test (FLAG_IS_CAPSIM_RIGHTORB_ACTIVATED) )  callset_invoke(capture_simon_made);
		//TODO: jackpot and combo shot detection
	if (right_loop_counter == right_loop_goal)  right_loop_goal_award ();
}//end of function



void right_loop_goal_award (void) {
	sound_start (ST_SAMPLE, EXPLOSION, SL_2S, PRI_GAME_QUICK5);
	score (SC_250K);
	right_loop_counter = 0;
	if (right_loop_goal < ORBITS_GOAL_MAX)  right_loop_goal += ORBITS_GOAL_STEP;
}//end of function

void left_loop_goal_award (void) {
	sound_start (ST_SAMPLE, EXPLOSION, SL_2S, PRI_GAME_QUICK5);
	score (SC_250K);
	left_loop_counter = 0;
	if (left_loop_goal < ORBITS_GOAL_MAX)  left_loop_goal += ORBITS_GOAL_STEP;
}//end of function

