/*
 * demolition man
 * car_chase_mode.c
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
 * Hitting the target with three lights lit award the Car Crash.
 * Car Crash lights Car Chase on the left/right lamps.
 *
 * Scoring Description: (my rules)
 * same as above except:
 * car chase is not a single lit ramp after a car crash hit but an entire mode
 * The car chase mode is lit after a predefined number of car crashes where the second car makes the switch.  Then
 * the mode starts and lights both ramps for car chases.  The car chase stays lit until ball drain
 * or until car crash is hit.  Car chase ramps score 15 million per shot.
 *
 * estimate of average carcrash mode score: 45 million to 90 million
 *
 */
#include <freewpc.h>
//constants

//local variables
U8			car_chase_mode_shots_made;   //mode  shots made counter
__boolean 	is_car_chase_mode_activated; //external at car_crash.c
U8			car_chase_modes_made;
score_t		car_chase_score;

//external variables
extern 	__boolean 		inTest; //located in global_constants.c

//prototypes

/****************************************************************************
 * initialize  and exit
 ***************************************************************************/
void car_chase_mode_reset (void) {
	car_chase_mode_shots_made = 0;
	is_car_chase_mode_activated = FALSE;
	score_zero(car_chase_score);
	}

CALLSET_ENTRY (car_chase_mode, start_player, start_ball) { car_chase_mode_reset(); }


/****************************************************************************
 * playfield lights and flags
 ***************************************************************************/



/****************************************************************************
 * body
 *
 ***************************************************************************/
//this is called from ramps.c
CALLSET_ENTRY (car_chase_mode, car_chase_ramp_made) {
	score (SC_15M);
	score_add(car_chase_score, score_table[SC_15M]);
	++car_chase_mode_shots_made;
	deff_start (DEFF_CARCHASE_MODE_EFFECT);//under /kernel/deff.c
	sound_start (ST_SAMPLE, CAR_CRASH, SL_2S, PRI_GAME_QUICK5);
	}

//this is called from car_crash.c
CALLSET_ENTRY (car_chase_mode, start_car_chase) {
	++car_chase_modes_made;
	is_car_chase_mode_activated = TRUE;
}

//this is called from car_crash.c
CALLSET_ENTRY (car_chase_mode, end_car_chase) {
	is_car_chase_mode_activated = FALSE;
}

/****************************************************************************
 * DMD display and sound effects
 ****************************************************************************/
//DMD DISPLAY EFFECTS
void carchase_mode_effect_deff(void) {
	dmd_alloc_low_clean ();
	font_render_string_center (&font_mono5, 96, 5, "car race");
	dmd_show_low ();
	task_sleep_sec (2);
	deff_exit ();
	}//end of mode_effect_deff


/****************************************************************************
 * status display
 *
 * called from common/status.c automatically whenever either flipper button
 * is held for 4 seconds or longer.  since called by callset, order of
 * various status reports will be random depending upon call stack
****************************************************************************/
CALLSET_ENTRY (car_chase_mode, status_report){
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
