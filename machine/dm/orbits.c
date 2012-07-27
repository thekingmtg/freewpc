/*
 * demolition man
 * orbits.c
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
 *
 */
#include <freewpc.h>
	/* In this file we will be using sounds for the first time so
	 * sound_start is located in audio.h as inline function
	 * format is: sound_start (U8 channels, sound_code_t code, U8 duration, U8 priority)
	 *
	 *	max possible of 4 channels of audio
	 *	here are the channels
	 *	MUSIC_CHANNEL 0
	 *	ST_MUSIC   0x1
	 *	ST_SPEECH  0x2
	 *	ST_SAMPLE  0x4
	 *	ST_EFFECT  0x8
	 *	ST_ANY     (ST_SAMPLE | ST_EFFECT)
	 *
	 *Here are the priorities
	 *  SP_NORMAL  PRI_NULL
	 *
	 *  Here are the possible duration codes
	 *
	 *  SL_100MS
	 *  SL_500MS
	 *  SL_1S
	 *  SL_2S
	 *  SL_3S
	 *  SL_4S
	*/

//constants
U8 ORBITS_EASY_GOAL = 5;
U8 ORBITS_PREDEFINED_GOAL_INCREMENT = 1;
U8 ORBITS_GOAL_STEP = 5;
U8 ORBITS_GOAL_MAX = 50;

//local variables
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
extern __boolean 		explode_activated;


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

	right_loop_goal = 0;
	left_loop_goal=ORBITS_EASY_GOAL;
	all_loop_goal=(ORBITS_EASY_GOAL * 4);

	left_Loop_ExtraBall_activated = FALSE;
	left_Loop_MultiBall_activated = FALSE;
	left_Loop_Explode_activated = FALSE;
	left_Loop_Jackpot_activated = FALSE;
	left_Loop_Arrow_activated = FALSE;
	right_Loop_Explode_activated = FALSE;
	right_Loop_Jackpot_activated = FALSE;
	right_Loop_Arrow_activated = FALSE;
	}

//TODO: modify options to allow saving ramps/loops across balls
CALLSET_ENTRY (orbits, start_player) { orbits_reset (); }
CALLSET_ENTRY (orbits, start_ball) { orbits_reset (); }


CALLSET_ENTRY (orbits, ExtraBall_Light_On) {
	left_Loop_ExtraBall_activated = TRUE;
	lamp_tristate_on (LM_EXTRA_BALL);
	}

CALLSET_ENTRY (orbits, ExtraBall_Light_Off) {
	left_Loop_ExtraBall_activated = FALSE;
	lamp_tristate_off (LM_EXTRA_BALL);
	}

CALLSET_ENTRY (orbits, Activate_Explode_Inserts) {
	left_Loop_Arrow_activated = TRUE;
	right_Loop_Arrow_activated = TRUE;
	lamp_tristate_flash (LM_LEFT_LOOP_EXPLODE);
	lamp_tristate_flash (LM_RIGHT_LOOP_EXPLODE);
	}


CALLSET_ENTRY (orbits, DeActivate_Explode_Inserts) {
	left_Loop_Arrow_activated = FALSE;
	right_Loop_Arrow_activated = FALSE;
	lamp_tristate_off (LM_LEFT_LOOP_EXPLODE);
	lamp_tristate_off (LM_RIGHT_LOOP_EXPLODE);
	}




/*
 *
 *left to right or right to left orbits
 */
void left_orbit_task (void) { task_sleep_sec(2); task_exit(); }
void right_orbit_task (void) { task_sleep_sec(2); task_exit(); }

// full orbit right to left
// or start left to right check
CALLSET_ENTRY (orbits, sw_left_loop) {
//	if ( !single_ball_play() ) return;  //turn off during multiball?
	if ( task_kill_gid(GID_RIGHT_ORBIT_MADE) ) {
		callset_invoke(right_orbit_shot);
		return;
		}
	task_create_gid1 (GID_LEFT_ORBIT_MADE, left_orbit_task);
	}

// full orbit left to right
// or start right to left check
CALLSET_ENTRY (orbits, sw_right_freeway) {
//	if ( !single_ball_play () ) return;  //turn off during multiball?
	if ( task_kill_gid(GID_LEFT_ORBIT_MADE) ) {
		callset_invoke(left_orbit_shot);
		return;
		}
	task_create_gid1 (GID_RIGHT_ORBIT_MADE, right_orbit_task);
	}


// full orbit left to hole
CALLSET_ENTRY (orbits, sw_top_popper) {
	if ( task_kill_gid(GID_LEFT_ORBIT_MADE) &&
			left_Loop_ExtraBall_activated ) {
		callset_invoke(ExtraBall_Light_Off);
		//add an extra ball here
		}
	}//end of orbits_sw_top_popper


void right_loop_goal_award (void) {
	sound_start (ST_SAMPLE, EXPLOSION, SL_1S, PRI_GAME_QUICK5);
	right_loop_counter = 0;
	if (right_loop_goal < ORBITS_GOAL_MAX)  right_loop_goal += ORBITS_GOAL_STEP;
	}

void left_loop_goal_award (void) {
	sound_start (ST_SAMPLE, EXPLOSION, SL_1S, PRI_GAME_QUICK5);
	score (SC_50K);
	left_loop_counter = 0;
	if (left_loop_goal < ORBITS_GOAL_MAX)  left_loop_goal += ORBITS_GOAL_STEP;
	}

CALLSET_ENTRY (orbits, left_orbit_shot) {
	++left_loop_counter;
	++all_loop_counter;
	if (left_loop_counter == left_loop_goal)  left_loop_goal_award ();
	sound_start (ST_SAMPLE, ZAPP_3_LONG, SL_1S, PRI_GAME_QUICK1);
	if(left_Loop_Arrow_activated && explode_activated) callset_invoke(explode_ramp_made);
	else score (SC_100K);//located in kernal/score.c
	}

CALLSET_ENTRY (orbits, right_orbit_shot) {
	++right_loop_counter;
	++all_loop_counter;
	if (right_loop_counter == right_loop_goal)  right_loop_goal_award ();
	sound_start (ST_SAMPLE, ZAPP_3_LONG, SL_1S, PRI_GAME_QUICK1);
	score (SC_50K);
	if(right_Loop_Arrow_activated && explode_activated) callset_invoke(explode_ramp_made);
	else score (SC_100K);//located in kernal/score.c
	}






/****************************************************************************
 * DMD display and sound effects
 ****************************************************************************/
