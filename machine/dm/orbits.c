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
 */

#include <freewpc.h>
#include "dm/global_constants.h"

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
__boolean 					left_Loop_Explode_activated;
__boolean 					right_Loop_Explode_activated;

//external variables

//prototypes
void orbits_reset (void);
void left_orbit_task (void);
void right_orbit_task (void);
void right_loop_goal_award (void);
void left_loop_goal_award (void);
void left_orbit_shot_made(void);
void right_orbit_shot_made(void);
/****************************************************************************
 * initialize  and exit
 ***************************************************************************/
void orbits_reset (void) {
	left_loop_counter = 0;
	right_loop_counter = 0;
	all_loop_counter = 0;
	left_Loop_Explode_activated = FALSE;
	right_Loop_Explode_activated = FALSE;
}//end of function

void player_orbits_reset (void) {
	orbits_SoundCounter = 0;
	right_loop_goal = 0;
	left_loop_goal=ORBITS_EASY_GOAL;
	all_loop_goal=(ORBITS_EASY_GOAL * 4);
	flag_off(FLAG_IS_MULTIBALL_ENABLED);
	flag_off(FLAG_IS_EXTRABALL_LIT);
	orbits_reset();
}//end of function

CALLSET_ENTRY (orbits, start_player) { player_orbits_reset(); }
CALLSET_ENTRY (orbits, start_ball) { orbits_reset(); }


/****************************************************************************
 * playfield lights and flags
 ***************************************************************************/
void extraball_light_on(void) {
	flag_on(FLAG_IS_EXTRABALL_LIT);
	lamp_tristate_on (LM_EXTRA_BALL);
	if ( (orbits_SoundCounter++ % 2) == 0 )//check if even
		sound_start (ST_SPEECH, SPCH_GET_THE_EXTRABALL, SL_4S, PRI_GAME_QUICK5);
	else
		sound_start (ST_SPEECH, SPCH_NEED_EXTRABALL, SL_4S, PRI_GAME_QUICK5);
}//end of function

void extraball_light_off(void) {
	flag_off(FLAG_IS_EXTRABALL_LIT);
	lamp_tristate_off (LM_EXTRA_BALL);
}//end of function

void multiball_light_on(void) {
	lamp_tristate_on (LM_START_MULTIBALL);
}//end of function

void multiball_light_off(void) {
	flag_off(FLAG_IS_MULTIBALL_ENABLED);
	lamp_tristate_off (LM_START_MULTIBALL);
}//end of function

void activate_explode_inserts_orbits(void) {
	left_Loop_Explode_activated = TRUE;
	right_Loop_Explode_activated = TRUE;
	lamp_tristate_flash (LM_LEFT_LOOP_EXPLODE);
	lamp_tristate_flash (LM_RIGHT_LOOP_EXPLODE);
}//end of function

void deactivate_explode_inserts_orbits(void) {
	left_Loop_Explode_activated = FALSE;
	right_Loop_Explode_activated = FALSE;
	lamp_tristate_off (LM_LEFT_LOOP_EXPLODE);
	lamp_tristate_off (LM_RIGHT_LOOP_EXPLODE);
}//end of function

void ll_jackpot_light_on(void) {
	flag_on (FLAG_IS_L_LOOP_JACKPOT_ACTIVATED);
	lamp_tristate_on (LM_LEFT_LOOP_JACKPOT);
}//end of function

void ll_jackpot_light_off(void) {
	flag_off (FLAG_IS_L_LOOP_JACKPOT_ACTIVATED);
	lamp_tristate_off (LM_LEFT_LOOP_JACKPOT);
}//end of function

void rl_jackpot_light_on(void) {
	flag_on (FLAG_IS_R_LOOP_JACKPOT_ACTIVATED);
	lamp_tristate_on (LM_RIGHT_LOOP_JACKPOT);
}//end of function

void rl_jackpot_light_off(void) {
	flag_off (FLAG_IS_R_LOOP_JACKPOT_ACTIVATED);
	lamp_tristate_off (LM_RIGHT_LOOP_JACKPOT);
}//end of function

void ll_arrow_light_on(void) {
	flag_on (FLAG_IS_L_LOOP_ARROW_ACTIVATED);
	lamp_tristate_on (LM_LEFT_LOOP_ARROW);
}//end of function

void ll_arrow_light_off(void) {
	flag_off (FLAG_IS_L_LOOP_ARROW_ACTIVATED);
	lamp_tristate_off (LM_LEFT_LOOP_ARROW);
}//end of function

void rl_arrow_light_on(void) {
	flag_on (FLAG_IS_R_LOOP_ARROW_ACTIVATED);
	lamp_tristate_on (LM_RIGHT_LOOP_ARROW);
}//end of function

void rl_arrow_light_off(void) {
	flag_off (FLAG_IS_R_LOOP_ARROW_ACTIVATED);
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
	if (flag_test(FLAG_IS_L_LOOP_JACKPOT_ACTIVATED) ) 			score_jackpot();
	else if (flag_test(FLAG_IS_HUXLEY_RUNNING) )				huxley_mode_shot_made();
	else if (flag_test (FLAG_IS_CAPSIM_LEFTORB_ACTIVATED) )  	capture_simon_made();
	else if (flag_test (FLAG_IS_PBREAK_RUNNING) ) 				prison_break_made();
	else if(flag_test (FLAG_IS_EXPLODE_MODE_RUNNING) ) 			explode_made();
	else if (flag_test(FLAG_IS_COMBOS_KILLED) ) 				combo_init();
	else if ( flag_test(FLAG_IS_COMBO_LEFTORB_ACTIVATED) ) 		combo_hit();

	if ( task_kill_gid(GID_RIGHT_ORBIT_MADE) ) right_orbit_shot_made();
	else {
		task_create_gid1 (GID_LEFT_ORBIT_MADE, left_orbit_task);
		if (!flag_test (FLAG_IS_PBREAK_RUNNING) ) {
				if ( (orbits_SoundCounter++ % 2) == 0 )//check if even
					sound_start (ST_EFFECT, RACE_BY, SL_2S, PRI_GAME_QUICK5);
				else
					sound_start (ST_EFFECT, RACE_BY_2, SL_2S, PRI_GAME_QUICK5);
		}//end of if ! pbreak
	}//end of else
}//end of function



// full orbit left to right
// or start right to left check
CALLSET_ENTRY (orbits, sw_right_freeway) {
	if (flag_test(FLAG_IS_R_LOOP_JACKPOT_ACTIVATED) ) 			score_jackpot();
	else if (flag_test(FLAG_IS_HUXLEY_RUNNING) )				huxley_mode_shot_made();
	else if(flag_test (FLAG_IS_CAPSIM_RIGHTORB_ACTIVATED) )  	capture_simon_made();
	else if (flag_test (FLAG_IS_PBREAK_RUNNING) )   			prison_break_made();
	else if(flag_test (FLAG_IS_EXPLODE_MODE_RUNNING) ) 			explode_made();
	else if (flag_test(FLAG_IS_COMBOS_KILLED) ) 				combo_init();
	else if ( flag_test(FLAG_IS_COMBO_RIGHTORB_ACTIVATED) ) 	combo_hit();

	if ( task_kill_gid(GID_LEFT_ORBIT_MADE) ) left_orbit_shot_made();
	else {
		task_create_gid1 (GID_RIGHT_ORBIT_MADE, right_orbit_task);
		if (!flag_test (FLAG_IS_PBREAK_RUNNING) ) {
				if ( (orbits_SoundCounter++ % 2) == 0 )//check if even
					sound_start (ST_EFFECT, RACE_BY, SL_2S, PRI_GAME_QUICK5);
				else
					sound_start (ST_EFFECT, RACE_BY_2, SL_2S, PRI_GAME_QUICK5);
		}//end of if ! pbreak
	}//end of else
}//end of function



void left_orbit_shot_made(void) {
	++left_loop_counter;
	++all_loop_counter;
	score (ORBIT_SCORE);//located in kernal/score.c
	if (!flag_test (FLAG_IS_PBREAK_RUNNING) ) sound_start (ST_SAMPLE, MACHINE12, SL_2S, PRI_GAME_QUICK1);
	if (left_loop_counter == left_loop_goal)  left_loop_goal_award ();
}//end of function



void right_orbit_shot_made(void) {
	++right_loop_counter;
	++all_loop_counter;
	score (ORBIT_SCORE);//located in kernal/score.c
	if (!flag_test (FLAG_IS_PBREAK_RUNNING) ) sound_start (ST_SAMPLE, MACHINE12, SL_2S, PRI_GAME_QUICK1);
	if (right_loop_counter == right_loop_goal)  right_loop_goal_award ();
}//end of function



void right_loop_goal_award (void) {
	sound_start (ST_SAMPLE, EXPLOSION, SL_2S, PRI_GAME_QUICK5);
	score (ORBIT_GOAL_SCORE);
	right_loop_counter = 0;
	if (right_loop_goal < ORBITS_GOAL_MAX)  right_loop_goal += ORBITS_GOAL_STEP;
}//end of function



void left_loop_goal_award (void) {
	sound_start (ST_SAMPLE, EXPLOSION, SL_2S, PRI_GAME_QUICK5);
	score (ORBIT_GOAL_SCORE);
	left_loop_counter = 0;
	if (left_loop_goal < ORBITS_GOAL_MAX)  left_loop_goal += ORBITS_GOAL_STEP;
}//end of function

