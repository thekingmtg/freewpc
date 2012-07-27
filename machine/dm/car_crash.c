/*
 * demolition man
 * car_crash.c
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
 * The Car Crash target has a terrible habit of deflecting the ball straight down the middle,
 * but the software does compensate with a short duration ball saver.
 * It's also quite easy to shake the cars across their opto switches if your tilt is liberal enough
 * so we need to check for that in tilt checking.
 *
 * Scoring Description: (my rules)
 *
 */
#include <freewpc.h>
//constants
U8 CAR_CRASH_EASY_GOAL = 3;
U8 CAR_CRASH_PREDEFINED_GOAL_INCREMENT = 1;
U8 CAR_CRASH_GOAL_STEP = 5;
U8 CAR_CRASH_GOAL_MAX = 50;

//local variables
U8 car_crash_counter;
U8 car_crash_goal;
__boolean car_chase_mode_activated;

//prototypes
void car_crash_reset (void);
void car_crash_task (void);
void car_crash1_task (void);

/****************************************************************************
 * initialize  and exit
 ***************************************************************************/
void car_crash_reset (void) {
	car_crash_counter = 0;
	car_crash_goal = CAR_CRASH_EASY_GOAL;
	car_chase_mode_activated = FALSE;
	}

CALLSET_ENTRY (car_crash, start_player) { car_crash_reset (); }
CALLSET_ENTRY (car_crash, start_ball) { car_crash_reset (); }




/*
 *
 * car_crash hits outside of mode
 * like ramps requires up to 3 contacts to register
 */
void car_crash_task (void) { task_sleep_sec(2); task_exit(); }
void car_crash1_task (void) { task_sleep_sec(2); task_exit(); }

CALLSET_ENTRY (car_crash, sw_car_chase_standup) {
	task_create_gid1 (GID_CAR_CRASH_ENTERED, car_crash_task);
	score (SC_50K);
	flasher_pulse (FLASH_CAR_CHASE_LOWER_FLASHER);
	if (car_chase_mode_activated)  {//crash the car and end mode
		car_chase_mode_activated = FALSE;
		sound_start (ST_SAMPLE, CAR_CRASH, SL_2S, PRI_GAME_QUICK5);
		}
}

CALLSET_ENTRY (car_crash, sw_chase_car_1) {
	task_create_gid1 (GID_CAR_CRASH_ENTERED1, car_crash1_task);
	sound_start (ST_SAMPLE, CAR_SKID, SL_2S, PRI_GAME_QUICK5);
	score (SC_50K);
	flasher_pulse (FLASH_CAR_CHASE_CENTER_FLASHER);
	}

CALLSET_ENTRY (car_crash, sw_chase_car_2) {
	sound_start (ST_SAMPLE, CAR_CRASH, SL_2S, PRI_GAME_QUICK5);
	flasher_pulse (FLASH_CAR_CHASE_UPPER_FLASHER);
	if ( task_kill_gid(GID_CAR_CRASH1_ENTERED) ) callset_invoke(chase_car_made);
	}

CALLSET_ENTRY (car_crash, chase_car_made) {
	++car_crash_counter;
	if (car_crash_counter == car_crash_goal)  {
		score (SC_50K);
		car_crash_counter = 0;
		if (car_crash_goal < CAR_CRASH_GOAL_MAX)  car_crash_goal += CAR_CRASH_GOAL_STEP;
		car_chase_mode_activated = TRUE; //start ramp mode
		}
	}



CALLSET_ENTRY (car_crash, car_chase_ramp_made) {
	score (SC_50K);
	}


/****************************************************************************
 * DMD display and sound effects
 ****************************************************************************/
