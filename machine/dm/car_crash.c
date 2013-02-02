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
/* CALLSET_SECTION (car_crash, __machine5__) */

#include <freewpc.h>
#include "dm/global_constants.h"



//constants
const U8 	CAR_CRASH_EASY_GOAL = 1;
const U8 	CAR_CRASH_HARD_GOAL = 5;
const U8 	CAR_CRASH_GOAL_STEP = 4; //increment for later goals
const U8 	CAR_CRASH_GOAL_MAX = 50;

//local variables
U8 			carCrashFirstSwitchDebouncer;
U8 			carCrashSecondSwitchDebouncer;
U8			carCrashThirdSwitchDebouncer;
U8			car_crash_multiplier;
__local__ U8 			car_crash_shots_made;	//non-mode shots made counter
__local__ U8 			car_crash_goal;		//goal to reach mode
__local__ __boolean 	is_car_crash_six_lit; 		//tracks which score to be awarded
__local__ __boolean 	is_car_crash_ten_lit; 		//tracks which score to be awarded
__local__ __boolean 	is_car_crash_three_lit; 		//tracks which score to be awarded
__boolean 	carcrash_ballsave;

//external variables

//internally called function prototypes  --external found at protos.h
void car_crash_reset (void);
void player_car_crash_reset (void);
void car_crash_first_switch_task (void);
void car_crash_second_switch_task (void);
void car_crash_third_switch_task (void);
void cc_lights_task (void);

/****************************************************************************
 * initialize  and exit
 ***************************************************************************/
void car_crash_reset (void) {
	carCrashFirstSwitchDebouncer = 0;
	carCrashSecondSwitchDebouncer = 0;
	carCrashThirdSwitchDebouncer = 0;
	car_crash_multiplier = 1;
	lamp_tristate_off (LM_CAR_CRASH_TOP);
	lamp_tristate_off (LM_CAR_CRASH_CENTER);
	lamp_tristate_off (LM_CAR_CRASH_BOTTOM);
	if (is_car_crash_ten_lit) lamp_tristate_on (LM_CAR_CRASH_TOP);
	if (is_car_crash_six_lit) lamp_tristate_on (LM_CAR_CRASH_CENTER);
	if (is_car_crash_three_lit) lamp_tristate_on (LM_CAR_CRASH_BOTTOM);
	carcrash_ballsave = FALSE;

	task_sleep (TIME_1S);
	if (flag_test(FLAG_IS_CARCHASE_MODE_ENABLED) )		leff_start (LEFF_CAR_CRASH_RUNNER);
	else 												leff_stop (LEFF_CAR_CRASH_RUNNER);
}//end of function



void player_car_crash_reset (void) {
	car_crash_shots_made = 0;
	car_crash_goal = CAR_CRASH_EASY_GOAL;
	car_crash_reset();
	flag_on(FLAG_IS_CARCHASE_MODE_ENABLED);
#ifdef CONFIG_DIFFICULTY_LEVEL
if (system_config.difficulty == EASY) {
	car_crash_goal = CAR_CRASH_EASY_GOAL;
}//end of if
else	{
	car_crash_goal = CAR_CRASH_HARD_GOAL;
} //END OF ELSE
#endif
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


void restore_carcrash_lights_state(void) {
	if (is_car_crash_six_lit) 	lamp_tristate_on (LM_CAR_CRASH_CENTER);
	else 	lamp_tristate_off (LM_CAR_CRASH_CENTER);

	if (is_car_crash_ten_lit) 	lamp_tristate_on (LM_CAR_CRASH_TOP);
	else 	lamp_tristate_off (LM_CAR_CRASH_TOP);

	if (is_car_crash_three_lit) 	lamp_tristate_on (LM_CAR_CRASH_BOTTOM);
	else 	lamp_tristate_off (LM_CAR_CRASH_BOTTOM);
}//end of function

/****************************************************************************
 *
 * body
 *
 ***************************************************************************/
CALLSET_BOOL_ENTRY (car_crash, ball_drain) { //thrown by device.c
	if	(carcrash_ballsave)	{
		serve_ball_auto ();
		sound_start (ST_SPEECH, CAR_SKID, SL_2S, PRI_GAME_QUICK5);
		return FALSE; //this is not a valid drain, don't count it
	}
	else return TRUE; //this is a valid drain
}//end of callset



void car_crash_first_switch_task (void) {
	task_sleep (TIME_500MS);
	carcrash_ballsave = FALSE;
	task_sleep_sec(1);
	task_sleep (TIME_500MS);
	carCrashFirstSwitchDebouncer = 0;
	task_exit();
}//end of function



//first car switch at bottom of car lane
CALLSET_ENTRY (car_crash, sw_chase_car_1) {
	if (in_game) {
			if (++carCrashFirstSwitchDebouncer == 1) {
				carcrash_ballsave = TRUE;
				sound_start (ST_SAMPLE, CAR_SKID, SL_2S, PRI_GAME_QUICK5);
				score (CARCRASH_SCORE_CAR1);
				flasher_pulse (FLASH_CAR_CHASE_LOWER_FLASHER);
			}//end of if DEBOUNCER
			task_create_gid1 (GID_CAR_CRASH_1, car_crash_first_switch_task);
	}
}//end of function



void car_crash_second_switch_task (void) {
	task_sleep_sec(4);
	carCrashSecondSwitchDebouncer = 0;
	task_exit();
}//end of function



//second car switch at bottom of car lane
CALLSET_ENTRY (car_crash, sw_chase_car_2) {
	if (in_game) {
			if (++carCrashSecondSwitchDebouncer == 1) {
				flasher_pulse (FLASH_CAR_CHASE_CENTER_FLASHER);
				//TODO: check for tilt here
				U8 i;
				for (i = 1; i <= car_crash_multiplier; i++){
					if (is_car_crash_ten_lit)	score (SC_10M);
					else if (is_car_crash_six_lit)	score (SC_6M);
					else if (is_car_crash_three_lit) score (SC_3M);
					else score (CARCRASH_SCORE_CAR2);
				}//end of loop
			}//end of if DEBOUNCER
			task_create_gid1 (GID_CAR_CRASH_2, car_crash_second_switch_task);
	}
}//end of function



void car_crash_third_switch_task (void) {
	task_sleep_sec(3);
	carCrashThirdSwitchDebouncer = 0;
	task_exit();
}//end of function


//second car switch at top of car lane
CALLSET_ENTRY (car_crash, sw_car_chase_standup) {
	if (in_game) {
			if (++carCrashThirdSwitchDebouncer == 1) {
					++car_crash_shots_made;
					sound_start (ST_SAMPLE, CAR_CRASH, SL_2S, PRI_GAME_QUICK5);
					flasher_pulse (FLASH_CAR_CHASE_UPPER_FLASHER);
					//goal made then start mode
					if (!flag_test(FLAG_BACK_IN_THE_FRIDGE_RUNNING)
						&&	car_crash_shots_made >= car_crash_goal)  {
						flag_off(FLAG_IS_CARCHASE_MODE_ENABLED);
						leff_stop (	LEFF_CAR_CRASH_RUNNER);
						car_crash_shots_made = 0;
						//increment goal for next time
						if (car_crash_goal < CAR_CRASH_GOAL_MAX)  car_crash_goal += CAR_CRASH_GOAL_STEP;

						huxley_increment();
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
						//set up flag if one shot away from car chase
							if (car_crash_goal - car_crash_shots_made <= 1) {
								flag_on(FLAG_IS_CARCHASE_MODE_ENABLED);
								leff_start (LEFF_CAR_CRASH_RUNNER);
							}//end of if
						}//end of else no chase yet
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
	}
}//end of function


//called from comp award at underground.c
void comp_award_trip_car_crash(void) {
	car_crash_multiplier = 3;
}//end of function




/****************************************************************************
 *
 * lighting effects
 *
 ****************************************************************************/
void car_crash_runner_leff (void) {
for (;;) {
			//listed from botton to top
			leff_on (LM_CAR_CRASH_BOTTOM);
			task_sleep (TIME_200MS);
			leff_off (LM_CAR_CRASH_BOTTOM);
			leff_on (LM_CAR_CRASH_CENTER);
			task_sleep (TIME_200MS);
			leff_off (LM_CAR_CRASH_CENTER);
			leff_on (LM_CAR_CRASH_TOP);
			task_sleep (TIME_200MS);
			leff_off (LM_CAR_CRASH_TOP);
			task_sleep (TIME_200MS);

			if (is_car_crash_three_lit) leff_on (LM_CAR_CRASH_BOTTOM);
			if (is_car_crash_six_lit) leff_on (LM_CAR_CRASH_CENTER);
			if (is_car_crash_ten_lit) leff_on (LM_CAR_CRASH_TOP);
			task_sleep (TIME_1S);

			leff_off (LM_CAR_CRASH_BOTTOM);
			leff_off (LM_CAR_CRASH_CENTER);
			leff_off (LM_CAR_CRASH_TOP);
			task_sleep (TIME_200MS);
	}//END OF LOOP
	leff_exit();
}//end of function


/****************************************************************************
 * DMD display and sound effects
 ****************************************************************************/
U8	carcrash_MessageCounter;
void carcrash_effect_deff(void) {
	U16 fno;
	dmd_alloc_pair_clean ();

	switch (++carcrash_MessageCounter % 5) {
		default:
				for (fno = IMG_CAR_CRASH_START; fno <= IMG_CAR_CRASH_END; fno += 2) {
					dmd_alloc_pair ();
					frame_draw(fno);
					dmd_show2 ();
					task_sleep (TIME_100MS);
				}//end of inner loop

				dmd_alloc_pair_clean ();// Clean both pages
				dmd_map_overlay ();
				dmd_clean_page_low ();
				sprintf ("CRASH %d MORE", car_crash_goal - car_crash_shots_made );
				font_render_string_center( &font_fipps, DMD_MIDDLE_X, DMD_BIG_CY_Top, sprintf_buffer);
				sprintf ("FOR CHASE");
				font_render_string_center( &font_fipps, DMD_MIDDLE_X, DMD_BIG_CY_Bot, sprintf_buffer);
				dmd_text_outline ();
				dmd_alloc_pair ();
				frame_draw(IMG_CAR_CRASH_END);
				dmd_overlay_outline ();
				dmd_show2 ();
				break;
		case 1:
			dmd_map_overlay ();
			dmd_clean_page_low ();
			font_render_string_center (&font_term6, DMD_MIDDLE_X, DMD_MED_CY_1, "EXPLODE AND CHASE");
			font_render_string_center (&font_var5, DMD_MIDDLE_X, DMD_MED_CY_2, "LEAD TO");
			font_render_string_center (&font_fipps, DMD_MIDDLE_X, DMD_BIG_CY_Bot, "HUXLEY");
			dmd_text_outline ();
			dmd_alloc_pair ();
			frame_draw(IMG_HUXLEY_B1_END);
			dmd_overlay_outline ();
			dmd_show2 ();
			break;
	}//end of switch
	task_sleep_sec (2);
	deff_exit ();
}//end of standard_effect_deff




