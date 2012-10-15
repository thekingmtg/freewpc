/*
 * demolition man
 * car_crash.c
 *
 * written by James Cardona
 *
 * Location Description:
 * Located to the left and above the retina scan is the Car Crash target.
 * the Car Crash contains two captive cars (yes, that's right ... captive cars) behind the impact target.
 * Triggering the switch causes two miniature cars to crash.
 * The ball will rocket off this target, but there's a ballsaver if it goes STDM.
 *
 * Scoring Description: (original game)
 * Hitting the target adds to the Car Crash value and lights an additional Car Crash light.
 * Hitting the target with three lights lit award the Car Crash value.
 * Car Crash is worth three, then six, then ten million (for 1/2/3 hits) and is doubled
 * if the top car hits the target at the end of the captive lane.
 * Car Crash lights Car Chase on the left/right lamps.
 * once 10 million shot is lit, it stays lit
 * TODO: The Car Crash target has a terrible habit of deflecting the ball straight down the middle,
 * but the software does compensate with a short duration ball saver.
 *
 * TODO: It's also quite easy to shake the cars across their opto switches if your tilt is liberal enough
 * so we need to check for that in tilt checking.
 *
 * Scoring Description: (my rules)
 * same as above except:
 * first switch hit is 250K, first car (2nd switch) is three, then six, then ten million.
 * Second car (Third switch) still doubles
 *
 * Also, car chase is not a single lit ramp after a car crash hit but an entire mode
 * The car chase mode is lit after a predefined number of car crashes where the second car makes the switch.  Then
 * the mode starts and lights both ramps for car chases.  The car chase stays lit until ball drain
 * or until car crash is hit.  Car chase ramps score 15 million per shot.
 *
 * once 10 million shot is lit, it starts mode and car crash lights reset to lowest value
 *
 * estimate of average carcrash mode score: 45 million to 90 million
 * estimate of average car score: 9 million to 21 million
 *
 */
/* CALLSET_SECTION (car_crash, __machine2__) */

#include <freewpc.h>
#include "dm/global_constants.h"

//constants
const U8 	CAR_CRASH_EASY_GOAL = 2;
const U8 	CAR_CRASH_GOAL_STEP = 4; //increment for later goals
const U8 	CAR_CRASH_GOAL_MAX = 50;

//local variables
U8 			carCrashFirstSwitchDebouncer;
U8 			carCrashSecondSwitchDebouncer;
U8			carCrashThirdSwitchDebouncer;
U8			car_crash_multiplier;
U8 			car_crash_shots_made;	//non-mode shots made counter
__local__ U8 			car_crash_goal;		//goal to reach mode
__boolean 	is_car_crash_six_lit; 		//tracks which score to be awarded
__boolean 	is_car_crash_ten_lit; 		//tracks which score to be awarded
__boolean 	is_car_crash_three_lit; 		//tracks which score to be awarded

//external variables

//internally called function prototypes  --external found at protos.h
void car_crash_reset (void);
void player_car_crash_reset (void);
void car_crash_first_switch_task (void);
void car_crash_second_switch_task (void);
void car_crash_third_switch_task (void);
void carcrash_mode_effect_deff(void);
void carcrash_effect_deff(void);

/****************************************************************************
 * initialize  and exit
 ***************************************************************************/
void car_crash_reset (void) {
	carCrashFirstSwitchDebouncer = 0;
	carCrashSecondSwitchDebouncer = 0;
	carCrashThirdSwitchDebouncer = 0;
	car_crash_multiplier = 1;
	car_crash_shots_made = 0;
	car_crash_goal = CAR_CRASH_EASY_GOAL;
	is_car_crash_six_lit = FALSE;
	is_car_crash_ten_lit = FALSE;
	is_car_crash_three_lit = FALSE;
	lamp_tristate_off (LM_CAR_CRASH_TOP);
	lamp_tristate_off (LM_CAR_CRASH_CENTER);
	lamp_tristate_off (LM_CAR_CRASH_BOTTOM);
}//end of function

void player_car_crash_reset (void) {
	car_crash_goal = CAR_CRASH_EASY_GOAL;
	car_crash_reset();
}//end of function

CALLSET_ENTRY (car_crash, start_ball) { car_crash_reset (); }
CALLSET_ENTRY (car_crash, start_player) { player_car_crash_reset (); }


/****************************************************************************
 * playfield lights and flags
 ***************************************************************************/
void carcrash_three_on(void) {
	is_car_crash_three_lit = TRUE;
	lamp_tristate_flash(LM_CAR_CRASH_BOTTOM);
	task_sleep (TIME_1S);
	lamp_tristate_on (LM_CAR_CRASH_BOTTOM);
}//end of function

void carcrash_three_off(void) {
	is_car_crash_three_lit = FALSE;
	lamp_tristate_off (LM_CAR_CRASH_BOTTOM);
}//end of function

void carcrash_six_on(void) {
	is_car_crash_six_lit  = TRUE;
	lamp_tristate_flash(LM_CAR_CRASH_CENTER);
	task_sleep (TIME_1S);
	lamp_tristate_on (LM_CAR_CRASH_CENTER);
}//end of function

void carcrash_six_off(void) {
	is_car_crash_six_lit = FALSE;
	lamp_tristate_off (LM_CAR_CRASH_CENTER);
}//end of function

void carcrash_ten_on(void) {
	is_car_crash_ten_lit = TRUE;
	lamp_tristate_flash(LM_CAR_CRASH_TOP);
	task_sleep (TIME_1S);
	lamp_tristate_on (LM_CAR_CRASH_TOP);
}//end of function

void carcrash_ten_off(void) {
	is_car_crash_ten_lit = FALSE;
	lamp_tristate_off (LM_CAR_CRASH_TOP);
}//end of function

/****************************************************************************
 * body
 *
 ***************************************************************************/
void car_crash_first_switch_task (void) {
	task_sleep_sec(2);
	carCrashFirstSwitchDebouncer = 0;
	task_exit();
}//end of function


//first car switch at bottom of car lane
CALLSET_ENTRY (car_crash, sw_chase_car_1) {
	if (++carCrashFirstSwitchDebouncer == 1) {
		sound_start (ST_SAMPLE, CAR_SKID, SL_2S, PRI_GAME_QUICK5);
		score (SC_250K);
		flasher_pulse (FLASH_CAR_CHASE_LOWER_FLASHER);
	}//end of if DEBOUNCER
	task_create_gid1 (GID_CAR_CRASH_1, car_crash_first_switch_task);
}//end of function



void car_crash_second_switch_task (void) {
	task_sleep_sec(4);
	carCrashSecondSwitchDebouncer = 0;
	task_exit();
}//end of function


//second car switch at bottom of car lane
CALLSET_ENTRY (car_crash, sw_chase_car_2) {
	if (++carCrashSecondSwitchDebouncer == 1) {
		flasher_pulse (FLASH_CAR_CHASE_CENTER_FLASHER);
		//TODO: check for tilt here
		U8 i;
		for (i = 1; i <= car_crash_multiplier; i++){
			if (is_car_crash_ten_lit)	score (SC_10M);
			else if (is_car_crash_six_lit)	score (SC_6M);
			else if (is_car_crash_three_lit) score (SC_3M);
			else score (SC_1M);
		}//end of loop
	}//end of if DEBOUNCER
	task_create_gid1 (GID_CAR_CRASH_2, car_crash_second_switch_task);
}//end of function



void car_crash_third_switch_task (void) {
	task_sleep_sec(3);
	carCrashThirdSwitchDebouncer = 0;
	task_exit();
}//end of function


//second car switch at top of car lane
CALLSET_ENTRY (car_crash, sw_car_chase_standup) {
	if (++carCrashThirdSwitchDebouncer == 1) {
			++car_crash_shots_made;
			sound_start (ST_SAMPLE, CAR_CRASH, SL_2S, PRI_GAME_QUICK5);
			flasher_pulse (FLASH_CAR_CHASE_UPPER_FLASHER);
			//goal made then start mode
			if (car_crash_shots_made >= car_crash_goal)  {
				car_crash_shots_made = 0;
				//increment goal for next time
				if (car_crash_goal < CAR_CRASH_GOAL_MAX)  car_crash_goal += CAR_CRASH_GOAL_STEP;
				//start ramp mode
				start_car_chase(); //at car_chase_mode.c
				//turn off car crash lights
				carcrash_three_off();
				carcrash_six_off();
				carcrash_ten_off();
			}//end of if (car_crash_shots_made == car_crash_goal)
			else {//no car chase yet
				deff_start (DEFF_CARCRASH_EFFECT);
				//set point values and lights for car crash
				if (car_crash_shots_made == 1) {
					if (!is_car_crash_six_lit) 				carcrash_three_on();
				} else if (car_crash_shots_made == 2) {
					if (!is_car_crash_six_lit) 				carcrash_six_on();
					if (!is_car_crash_three_lit) 			carcrash_three_on();
				} else if (car_crash_shots_made >= 3) {
					if (!is_car_crash_ten_lit) 				carcrash_ten_on();
					if (!is_car_crash_six_lit) 				carcrash_six_on();
					if (!is_car_crash_three_lit) 			carcrash_three_on();
				}
			}//end of else not >= goal
			//effectively doubles the score
			U8 i;
			for (i = 1; i <= car_crash_multiplier; i++){
				if (is_car_crash_six_lit)	score (SC_6M);
				else if (is_car_crash_ten_lit)	score (SC_10M);
				else if (is_car_crash_three_lit) score (SC_3M);
				else score (SC_1M);
			}//end of loop
		}//end of if (++carCrashThirdSwitchDebouncer == 1)
	task_create_gid1 (GID_CAR_CRASH_3, car_crash_third_switch_task);
}//end of function


//called from comp award at underground.c
void comp_award_trip_car_crash(void) {
	car_crash_multiplier = 3;
}//end of function



/****************************************************************************
 * DMD display and sound effects
 ****************************************************************************/

void carcrash_effect_deff(void) {
	U8 i = 1;
	do {
		dmd_alloc_low_clean ();
		font_render_string_center( &font_fipps, DMD_MIDDLE_X - (i*8), 	DMD_BIG_CY_Cent, "C");
		font_render_string_center( &font_fipps, DMD_MIDDLE_X - (i*4), 	DMD_BIG_CY_Cent, "R");
		font_render_string_center( &font_fipps, DMD_MIDDLE_X, 			DMD_BIG_CY_Cent, "A"); //right in middle
		font_render_string_center( &font_fipps, DMD_MIDDLE_X + (i*4), 	DMD_BIG_CY_Cent, "S");
		font_render_string_center( &font_fipps, DMD_MIDDLE_X + (i*8), 	DMD_BIG_CY_Cent, "H");
		dmd_show_low ();
		if (i < 3) 		task_sleep (TIME_100MS);
		else if (i < 6) task_sleep (TIME_200MS);
		else if (i < 9) task_sleep (TIME_500MS);
	} while (i++ < 7);

	dmd_alloc_low_clean ();
	sprintf ("CRASH %d MORE", car_crash_goal - car_crash_shots_made );
	font_render_string_center( &font_term6, DMD_MIDDLE_X, DMD_BIG_CY_Top, sprintf_buffer);
	sprintf ("FOR CHASE");
	font_render_string_center( &font_fipps, DMD_MIDDLE_X, DMD_BIG_CY_Bot, sprintf_buffer);
	dmd_show_low ();
	task_sleep_sec (2);
	deff_exit ();
}//end of standard_effect_deff

