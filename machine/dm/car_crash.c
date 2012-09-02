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
#include <freewpc.h>
#include "dm/global_constants.h"

//constants
const U8 	CAR_CRASH_EASY_GOAL = 6;
const U8 	CAR_CRASH_PREDEFINED_GOAL_INCREMENT = 1;//TODO: this is for making different levels besides easy
const U8 	CAR_CRASH_GOAL_STEP = 5; //increment for later goals
const U8 	CAR_CRASH_GOAL_MAX = 50;

//local variables
U8 			carCrashFirstSwitchDebouncer;
U8 			carCrashSecondSwitchDebouncer;
U8			carCrashThirdSwitchDebouncer;
U8 			car_crash_shots_made;	//non-mode shots made counter
U8 			car_crash_goal;		//goal to reach mode
__boolean 	is_car_crash_six_lit; 		//tracks which score to be awarded
__boolean 	is_car_crash_ten_lit; 		//tracks which score to be awarded
__boolean 	is_car_crash_three_lit; 		//tracks which score to be awarded

//external variables
extern 	__boolean 		is_car_chase_mode_activated; //located in car_chase_mode.c

//prototypes
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
	car_crash_shots_made = 0;
	car_crash_goal = CAR_CRASH_EASY_GOAL;
	is_car_crash_six_lit = FALSE;
	is_car_crash_ten_lit = FALSE;
	is_car_crash_three_lit = FALSE;
	lamp_tristate_off (LM_CAR_CRASH_TOP);
	lamp_tristate_off (LM_CAR_CRASH_CENTER);
	lamp_tristate_off (LM_CAR_CRASH_BOTTOM);
}

void player_car_crash_reset (void) {
	car_crash_goal = CAR_CRASH_EASY_GOAL;
	car_crash_reset();
}

CALLSET_ENTRY (car_crash, start_ball) { car_crash_reset (); }
CALLSET_ENTRY (car_crash, start_player) { player_car_crash_reset (); }


/****************************************************************************
 * playfield lights and flags
 ***************************************************************************/
CALLSET_ENTRY (car_crash, carcrash_three_on) {
	is_car_crash_three_lit = TRUE;
	lamp_tristate_flash(LM_CAR_CRASH_BOTTOM);
	task_sleep (TIME_1S);
	lamp_tristate_on (LM_CAR_CRASH_BOTTOM);
	}

CALLSET_ENTRY (car_crash, carcrash_three_off) {
	is_car_crash_three_lit = FALSE;
	lamp_tristate_off (LM_CAR_CRASH_BOTTOM);
	}

CALLSET_ENTRY (car_crash, carcrash_six_on) {
	is_car_crash_six_lit  = TRUE;
	lamp_tristate_flash(LM_CAR_CRASH_CENTER);
	task_sleep (TIME_1S);
	lamp_tristate_on (LM_CAR_CRASH_CENTER);
	}

CALLSET_ENTRY (car_crash, carcrash_six_off) {
	is_car_crash_six_lit = FALSE;
	lamp_tristate_off (LM_CAR_CRASH_CENTER);
	}

CALLSET_ENTRY (car_crash, carcrash_ten_on) {
	is_car_crash_ten_lit = TRUE;
	lamp_tristate_flash(LM_CAR_CRASH_TOP);
	task_sleep (TIME_1S);
	lamp_tristate_on (LM_CAR_CRASH_TOP);
	}

CALLSET_ENTRY (car_crash, carcrash_ten_off) {
	is_car_crash_ten_lit = FALSE;
	lamp_tristate_off (LM_CAR_CRASH_TOP);
	}

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
		score (SC_250K);
		flasher_pulse (FLASH_CAR_CHASE_LOWER_FLASHER);
		flasher_pulse (FLASH_CAR_CHASE_LOWER_FLASHER);
		flasher_pulse (FLASH_CAR_CHASE_LOWER_FLASHER);
	}//end of if DEBOUNCER
	task_create_gid1 (GID_CAR_CRASH_1, car_crash_first_switch_task);
}//end of function



void car_crash_second_switch_task (void) {
	task_sleep_sec(3);
	carCrashSecondSwitchDebouncer = 0;
	task_exit();
}//end of function


//second car switch at bottom of car lane
CALLSET_ENTRY (car_crash, sw_chase_car_2) {
	if (++carCrashSecondSwitchDebouncer == 1) {
		flasher_pulse (FLASH_CAR_CHASE_CENTER_FLASHER);
		flasher_pulse (FLASH_CAR_CHASE_CENTER_FLASHER);
		flasher_pulse (FLASH_CAR_CHASE_CENTER_FLASHER);
		//TODO: check for tilt here
		if (is_car_crash_ten_lit)	score (SC_10M);
		else if (is_car_crash_six_lit)	score (SC_6M);
		else if (is_car_crash_three_lit) score (SC_3M);
		else score (SC_1M);
		sound_start (ST_SAMPLE, CAR_SKID, SL_2S, PRI_GAME_QUICK5);
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
			deff_start (DEFF_CARCRASH_EFFECT);
			++car_crash_shots_made;
			sound_start (ST_SAMPLE, CAR_CRASH, SL_2S, PRI_GAME_QUICK5);
			flasher_pulse (FLASH_CAR_CHASE_UPPER_FLASHER);
			flasher_pulse (FLASH_CAR_CHASE_UPPER_FLASHER);
			flasher_pulse (FLASH_CAR_CHASE_UPPER_FLASHER);
			//effectively doubles the score
			if (is_car_crash_six_lit)	score (SC_6M);
			else if (is_car_crash_ten_lit)	score (SC_10M);
			else if (is_car_crash_three_lit) score (SC_3M);
			else score (SC_1M);
			//set point values and lights for car crash
			if (car_crash_shots_made == 1) {
				if (!is_car_crash_six_lit) callset_invoke(carcrash_three_on);
			} else if (car_crash_shots_made == 2) {
				if (!is_car_crash_six_lit) callset_invoke(carcrash_six_on);
				if (!is_car_crash_three_lit) callset_invoke(carcrash_three_on);
			} else if (car_crash_shots_made >= 3) {
				if (!is_car_crash_ten_lit) callset_invoke(carcrash_ten_on);
				if (!is_car_crash_six_lit) callset_invoke(carcrash_six_on);
				if (!is_car_crash_three_lit) callset_invoke(carcrash_three_on);
			}
			//goal made then start mode
			if (car_crash_shots_made == car_crash_goal)  {
				car_crash_shots_made = 0;
				//increment goal for next time
				if (car_crash_goal < CAR_CRASH_GOAL_MAX)  car_crash_goal += CAR_CRASH_GOAL_STEP;
				//start ramp mode
				callset_invoke(start_car_chase); //at car_chase_mode.c
				//turn off car crash lights
				callset_invoke(carcrash_three_off);
				callset_invoke(carcrash_six_off);
				callset_invoke(carcrash_ten_off);
			}//end of if (car_crash_shots_made == car_crash_goal)
	}//end of if (++carCrashThirdSwitchDebouncer == 1)
	task_create_gid1 (GID_CAR_CRASH_3, car_crash_third_switch_task);
}//end of function

/****************************************************************************
 * DMD display and sound effects
 ****************************************************************************/
void carcrash_effect_deff(void) {
	dmd_alloc_low_clean ();
	font_render_string_center( &font_term6, DMD_BIG_CX_Top, DMD_BIG_CY_Top, "CRASH");
	sprintf ("%d FOR CHASE", (car_crash_goal - car_crash_shots_made) );
	font_render_string_center( &font_term6, DMD_BIG_CX_Bot, DMD_BIG_CY_Bot, sprintf_buffer);
	dmd_show_low ();
	task_sleep_sec (2);
	deff_exit ();
	}//end of standard_effect_deff


/****************************************************************************
 * status display
 *
 * called from common/status.c automatically whenever either flipper button
 * is held for 4 seconds or longer.  since called by callset, order of
 * various status reports will be random depending upon call stack
****************************************************************************/
//ALLSET_ENTRY (car_crash, status_report){
//	sprintf ("%d CRASH", car_crash_shots_made);
//	font_render_string_center(&font_fixed10, 1, 1, sprintf_buffer);

//	sprintf ("%d CAR CRASHES needed for next car chase", car_crash_goal - car_crash_shots_made);
//	font_render_string_center (&font_mono5, 64, 13, sprintf_buffer);
	//deff_exit (); is called at end of calling function - not needed here?
//}//end of function
