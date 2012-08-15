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
U8 			car_crash_shots_made;	//non-mode shots made counter
U8 			car_crash_goal;		//goal to reach mode
__boolean 	is_car_crash_six_lit; 		//tracks which score to be awarded
__boolean 	is_car_crash_ten_lit; 		//tracks which score to be awarded
__boolean 	is_car_crash_three_lit; 		//tracks which score to be awarded

U8			car_chase_mode_shots_made; //mode  shots made counter
__boolean 	is_car_chase_mode_activated;
U8			car_chase_modes_made;
score_t		car_chase_score;

//external variables
extern 	__boolean 		inTest; //located in global_constants.c

//prototypes
void car_crash_reset (void);
void car_crash_task (void);
void car_crash1_task (void);
void carcrash_mode_effect_deff(void);
void carcrash_effect_deff(void);

/****************************************************************************
 * initialize  and exit
 ***************************************************************************/
void car_crash_reset (void) {
	car_crash_shots_made = 0;
	car_chase_mode_shots_made = 0;
	car_crash_goal = CAR_CRASH_EASY_GOAL;
	is_car_chase_mode_activated = FALSE;
	is_car_crash_six_lit = FALSE;
	is_car_crash_ten_lit = FALSE;
	is_car_crash_three_lit = FALSE;
	lamp_tristate_off (LM_CAR_CRASH_TOP);
	lamp_tristate_off (LM_CAR_CRASH_CENTER);
	lamp_tristate_off (LM_CAR_CRASH_BOTTOM);
	car_chase_modes_made = 0;
	score_zero(car_chase_score);
	}

void car_crash_mode_reset (void) {
	car_crash_shots_made = 0;
	car_chase_mode_shots_made = 0;
	is_car_chase_mode_activated = FALSE;
	is_car_crash_six_lit = FALSE;
	is_car_crash_ten_lit = FALSE;
	is_car_crash_three_lit = FALSE;
	lamp_tristate_off (LM_CAR_CRASH_TOP);
	lamp_tristate_off (LM_CAR_CRASH_CENTER);
	lamp_tristate_off (LM_CAR_CRASH_BOTTOM);
	score_zero(car_chase_score);
	}

CALLSET_ENTRY (car_crash, start_player) { car_crash_reset (); }
CALLSET_ENTRY (car_crash, start_ball) { car_crash_reset (); }


/****************************************************************************
 * playfield lights and flags
 ***************************************************************************/
CALLSET_ENTRY (car_crash, carcrash_three_on) {
	is_car_crash_six_lit = FALSE;
	is_car_crash_ten_lit = FALSE;
	is_car_crash_three_lit = TRUE;
	lamp_tristate_off (LM_CAR_CRASH_TOP);
	lamp_tristate_off (LM_CAR_CRASH_CENTER);
	lamp_tristate_flash(LM_CAR_CRASH_BOTTOM);
	task_sleep (TIME_300MS);
	lamp_tristate_on (LM_CAR_CRASH_BOTTOM);
	}

CALLSET_ENTRY (car_crash, carcrash_three_off) {
	is_car_crash_three_lit = FALSE;
	lamp_tristate_off (LM_CAR_CRASH_BOTTOM);
	}

CALLSET_ENTRY (car_crash, carcrash_six_on) {
	is_car_crash_six_lit  = TRUE;
	is_car_crash_ten_lit = FALSE;
	is_car_crash_three_lit = TRUE;
	lamp_tristate_off (LM_CAR_CRASH_TOP);
	lamp_tristate_flash(LM_CAR_CRASH_CENTER);
	task_sleep (TIME_300MS);
	lamp_tristate_on (LM_CAR_CRASH_CENTER);
	lamp_tristate_on (LM_CAR_CRASH_BOTTOM);
	}

CALLSET_ENTRY (car_crash, carcrash_six_off) {
	is_car_crash_six_lit = FALSE;
	lamp_tristate_off (LM_CAR_CRASH_CENTER);
	}

CALLSET_ENTRY (car_crash, carcrash_ten_on) {
	is_car_crash_six_lit  = TRUE;
	is_car_crash_ten_lit = TRUE;
	is_car_crash_three_lit = TRUE;
	lamp_tristate_flash(LM_CAR_CRASH_TOP);
	task_sleep (TIME_300MS);
	lamp_tristate_on (LM_CAR_CRASH_TOP);
	lamp_tristate_on (LM_CAR_CRASH_CENTER);
	lamp_tristate_on (LM_CAR_CRASH_BOTTOM);
	}

CALLSET_ENTRY (car_crash, carcrash_ten_off) {
	is_car_crash_ten_lit = FALSE;
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
	if (is_car_chase_mode_activated)  {//crash the car and end mode
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
	if (is_car_crash_six_lit)	score (SC_6M);
		else if (is_car_crash_ten_lit)	score (SC_10M);
			else if (is_car_crash_three_lit) score (SC_3M);
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
	if (is_car_crash_six_lit)	score (SC_6M);
		else if (is_car_crash_ten_lit)	score (SC_10M);
			else if (is_car_crash_three_lit) score (SC_3M);
				else score (SC_1M);
	if ( task_kill_gid(GID_CAR_CRASH1_ENTERED) ) callset_invoke(chase_car_made);
}//end of function

//final car makes switch so do bonuses
CALLSET_ENTRY (car_crash, chase_car_made) {
	++car_crash_shots_made;
	deff_start (DEFF_CARCRASH_EFFECT);//under /kernel/deff.c
	//set point values and lights for car crash
	if (car_crash_shots_made == 1) callset_invoke(carcrash_three_on);
	else if (car_crash_shots_made == 2) callset_invoke(carcrash_six_on);
	else if (car_crash_shots_made == 3) callset_invoke(carcrash_ten_on);

	//goal made then start mode
	if (car_crash_shots_made == car_crash_goal)  {
		car_crash_shots_made = 0;
		//increment goal for next time
		if (car_crash_goal < CAR_CRASH_GOAL_MAX)  car_crash_goal += CAR_CRASH_GOAL_STEP;
		//start ramp mode
		is_car_chase_mode_activated = TRUE;
		++car_chase_modes_made;
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
	score_add(car_chase_score, score_table[SC_15M]);
	++car_chase_mode_shots_made;
	deff_start (DEFF_CARCRASH_MODE_EFFECT);//under /kernel/deff.c
	sound_start (ST_SAMPLE, CAR_CRASH, SL_2S, PRI_GAME_QUICK5);
	}


/****************************************************************************
 * DMD display and sound effects
 ****************************************************************************/
//DMD DISPLAY EFFECTS
void carcrash_effect_deff(void) {
	dmd_alloc_low_clean ();
	font_render_string_center (&font_mono5, 96, 5, "car crash");
	//sprintf ("%d", jet_count);
	//font_render_string_center (&font_fixed10, 96, 16, sprintf_buffer);
	//if (jet_count == jet_goal)
	//	sprintf ("JET BONUS");
	//else
	//	sprintf ("BONUS AT %d", jet_goal);
	//font_render_string_center (&font_var5, 64, 26, sprintf_buffer);
	dmd_show_low ();
	task_sleep_sec (2);
	deff_exit ();
	}//end of standard_effect_deff

void carcrash_mode_effect_deff(void) {
	dmd_alloc_low_clean ();
	font_render_string_center (&font_mono5, 96, 5, "car race");
	dmd_show_low ();
	task_sleep_sec (2);
	deff_exit ();
	}//end of mode_effect_deff


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
CALLSET_ENTRY (car_crash, status_report){
	if (inTest) {
		if (is_car_chase_mode_activated) sprintf ("car chase mode is activated");
		else sprintf ("car chase mode is not activated");
		font_render_string_center (&font_mono5, 64, 1, sprintf_buffer);
	}//end of 	if (inTest)

	sprintf ("%d car chase modes completed", car_chase_modes_made);
	font_render_string_center (&font_mono5, 64, 7, sprintf_buffer);

	sprintf ("car chase score: %d", car_chase_score);
	font_render_string_center (&font_mono5, 64, 13, sprintf_buffer);

	if (inTest) {
		sprintf ("%d car chase shots made", car_chase_mode_shots_made);
		font_render_string_center (&font_mono5, 64, 19, sprintf_buffer);
	}//end of 	if (inTest)
	//deff_exit (); is called at end of calling function - not needed here?
}//end of function

//number of shots to make to start next car chase mode
//car_crash_shots_made;	//non-mode shots made counter
//car_crash_goal;		//goal to reach mode

