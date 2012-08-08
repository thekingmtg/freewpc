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
//constants
const U8 	CAR_CRASH_EASY_GOAL = 3;
const U8 	CAR_CRASH_PREDEFINED_GOAL_INCREMENT = 1;//TODO: this is for making different levels besides easy
const U8 	CAR_CRASH_GOAL_STEP = 5; //increment for later goals
const U8 	CAR_CRASH_GOAL_MAX = 50;

//local variables
U8 			car_crash_counter;	//non-mode shots made counter
U8			car_chase_mode_counter; //mode  shots made counter
U8 			car_crash_goal;		//goal to reach mode
__boolean 	car_chase_mode_activated;
__boolean 	car_crash_six; 		//tracks which score to be awarded
__boolean 	car_crash_ten; 		//tracks which score to be awarded
__boolean 	car_crash_three; 		//tracks which score to be awarded

//prototypes
void car_crash_reset (void);
void car_crash_task (void);
void car_crash1_task (void);

/****************************************************************************
 * initialize  and exit
 ***************************************************************************/
void car_crash_reset (void) {
	car_crash_counter = 0;
	car_chase_mode_counter = 0;
	car_crash_goal = CAR_CRASH_EASY_GOAL;
	car_chase_mode_activated = FALSE;
	car_crash_six = FALSE;
	car_crash_ten = FALSE;
	car_crash_three = FALSE;
	lamp_tristate_off (LM_CAR_CRASH_TOP);
	lamp_tristate_off (LM_CAR_CRASH_CENTER);
	lamp_tristate_off (LM_CAR_CRASH_BOTTOM);
	}

void car_crash_mode_reset (void) {
	car_crash_counter = 0;
	car_chase_mode_counter = 0;
	car_chase_mode_activated = FALSE;
	car_crash_six = FALSE;
	car_crash_ten = FALSE;
	car_crash_three = FALSE;
	lamp_tristate_off (LM_CAR_CRASH_TOP);
	lamp_tristate_off (LM_CAR_CRASH_CENTER);
	lamp_tristate_off (LM_CAR_CRASH_BOTTOM);
	}

CALLSET_ENTRY (car_crash, start_player) { car_crash_reset (); }
CALLSET_ENTRY (car_crash, start_ball) { car_crash_reset (); }


/****************************************************************************
 * playfield lights and flags
 ***************************************************************************/
CALLSET_ENTRY (car_crash, carcrash_three_on) {
	car_crash_six = FALSE;
	car_crash_ten = FALSE;
	car_crash_three = TRUE;
	lamp_tristate_off (LM_CAR_CRASH_TOP);
	lamp_tristate_off (LM_CAR_CRASH_CENTER);
	lamp_tristate_flash(LM_CAR_CRASH_BOTTOM);
	task_sleep (TIME_300MS);
	lamp_tristate_on (LM_CAR_CRASH_BOTTOM);
	}

CALLSET_ENTRY (car_crash, carcrash_three_off) {
	car_crash_three = FALSE;
	lamp_tristate_off (LM_CAR_CRASH_BOTTOM);
	}

CALLSET_ENTRY (car_crash, carcrash_six_on) {
	car_crash_six  = TRUE;
	car_crash_ten = FALSE;
	car_crash_three = TRUE;
	lamp_tristate_off (LM_CAR_CRASH_TOP);
	lamp_tristate_flash(LM_CAR_CRASH_CENTER);
	task_sleep (TIME_300MS);
	lamp_tristate_on (LM_CAR_CRASH_CENTER);
	lamp_tristate_on (LM_CAR_CRASH_BOTTOM);
	}

CALLSET_ENTRY (car_crash, carcrash_six_off) {
	car_crash_six = FALSE;
	lamp_tristate_off (LM_CAR_CRASH_CENTER);
	}

CALLSET_ENTRY (car_crash, carcrash_ten_on) {
	car_crash_six  = TRUE;
	car_crash_ten = TRUE;
	car_crash_three = TRUE;
	lamp_tristate_flash(LM_CAR_CRASH_TOP);
	task_sleep (TIME_300MS);
	lamp_tristate_on (LM_CAR_CRASH_TOP);
	lamp_tristate_on (LM_CAR_CRASH_CENTER);
	lamp_tristate_on (LM_CAR_CRASH_BOTTOM);
	}

CALLSET_ENTRY (car_crash, carcrash_ten_off) {
	car_crash_ten = FALSE;
	lamp_tristate_off (LM_CAR_CRASH_TOP);
	}

/****************************************************************************
 * body
 *
 * like ramps requires up to 3 contacts to register
 *
 ***************************************************************************/
void car_crash_task (void) { task_sleep_sec(2); task_exit(); }
void car_crash1_task (void) { task_sleep_sec(2); task_exit(); }

//first switch hit
CALLSET_ENTRY (car_crash, sw_car_chase_standup) {
	score (SC_250K);
	flasher_pulse (FLASH_CAR_CHASE_LOWER_FLASHER);
	if (car_chase_mode_activated)  {//crash the car and end mode
		callset_invoke(carcrash_mode_off); //at ramps.c
		car_crash_mode_reset();
		sound_start (ST_SAMPLE, CAR_CRASH, SL_2S, PRI_GAME_QUICK5);
		//return to normal music
		sound_start (ST_MUSIC, MUS_BG, 0, SP_NORMAL);
		}
	else task_create_gid1 (GID_CAR_CRASH_ENTERED, car_crash_task);
	}//end of function

//second switch hit
CALLSET_ENTRY (car_crash, sw_chase_car_1) {
	//TODO: check for tilt here
	task_create_gid1 (GID_CAR_CRASH_ENTERED1, car_crash1_task);
	if (car_crash_six)	score (SC_6M);
		else if (car_crash_ten)	score (SC_10M);
			else if (car_crash_three) score (SC_3M);
				else score (SC_1M);
	sound_start (ST_SAMPLE, CAR_SKID, SL_2S, PRI_GAME_QUICK5);
	flasher_pulse (FLASH_CAR_CHASE_CENTER_FLASHER);
}//end of function

//final switch hit
CALLSET_ENTRY (car_crash, sw_chase_car_2) {
	//TODO: check for tilt here
	sound_start (ST_SAMPLE, CAR_CRASH, SL_2S, PRI_GAME_QUICK5);
	flasher_pulse (FLASH_CAR_CHASE_UPPER_FLASHER);
	//effectively doubles the score
	if (car_crash_six)	score (SC_6M);
		else if (car_crash_ten)	score (SC_10M);
			else if (car_crash_three) score (SC_3M);
				else score (SC_1M);
	if ( task_kill_gid(GID_CAR_CRASH1_ENTERED) ) callset_invoke(chase_car_made);
}//end of function

//final car makes switch so do bonuses
CALLSET_ENTRY (car_crash, chase_car_made) {
	++car_crash_counter;
	//set point values and lights for car crash
	if (car_crash_counter == 1) callset_invoke(carcrash_three_on);
	else if (car_crash_counter == 2) callset_invoke(carcrash_six_on);
	else if (car_crash_counter == 3) callset_invoke(carcrash_ten_on);

	//goal made then start mode
	if (car_crash_counter == car_crash_goal)  {
		car_crash_counter = 0;
		//increment goal for next time
		if (car_crash_goal < CAR_CRASH_GOAL_MAX)  car_crash_goal += CAR_CRASH_GOAL_STEP;
		//start ramp mode
		car_chase_mode_activated = TRUE;
		callset_invoke(carcrash_mode_on); //at ramps.c
		sound_start (ST_MUSIC, MUS_MD_CAR_CRASH, 0, SP_NORMAL);
		//turn off car crash lights
		callset_invoke(carcrash_three_off);
		callset_invoke(carcrash_six_off);
		callset_invoke(carcrash_ten_off);
		}
	}//end of function


//this is called from ramps.c
CALLSET_ENTRY (car_crash, car_chase_ramp_made) {
	score (SC_15M);
	++car_chase_mode_counter;
	//TODO: DEFF here
	sound_start (ST_SAMPLE, CAR_CRASH, SL_2S, PRI_GAME_QUICK5);
	}


/****************************************************************************
 * DMD display and sound effects
 ****************************************************************************/
