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
#include "dm/global_constants.h"

//constants
const U8 	car_chase_TotalNumOfSounds = 8; //num between 0 and N-1 == N total
const sound_code_t car_chase_SoundsArray[] = {	SPCH_WHAT_ARE_YOU_DOING, 	SPCH_STOP_THIS, 		SPCH_WEEAHOO,
												SPCH_OHH_YEAH3,				SPCH_LETS_GO1, 			SPCH_YEEAHHAA,
												SPCH_WOOH, 					SPCH_WOW };

//local variables
U8 			car_chase_mode_shots_made;
U8 			car_chase_modes_achieved;
U8 			car_chase_modes_completed;
U8			car_chase_mode_timer;
score_t 	car_chase_mode_score;
score_t 	car_chase_mode_temp_score;
score_t 	car_chase_mode_score_total_score;


//external variables

//prototypes
void car_chase_reset (void);
void car_chase_player_reset (void);
void car_chase_effect_deff(void);
void car_chase_mode_init (void);
void car_chase_mode_expire (void);
void car_chase_mode_exit (void);



/****************************************************************************
 * mode definition structure
 ***************************************************************************/
struct timed_mode_ops car_chase_mode = {
	DEFAULT_MODE,
	.init = car_chase_mode_init,
	.exit = car_chase_mode_exit,
	.gid = GID_CAR_CHASE_MODE_RUNNING,
	.music = MUS_MD_CAR_CRASH,
	.deff_starting = DEFF_CAR_CHASE_START_EFFECT,
	.deff_running = DEFF_CAR_CHASE_EFFECT,
//	.deff_ending = DEFF_CAR_CHASE_END_EFFECT,
	.prio = PRI_GAME_MODE1,
	.init_timer = 23,
	.timer = &car_chase_mode_timer,
	.grace_timer = 2,
//	.pause = system_timer_pause,
};



/****************************************************************************
 * initialize  and exit
 ***************************************************************************/
void car_chase_reset (void) {
	flag_off (FLAG_IS_CARCHASE_MODE_ACTIVATED);
}//end of function



void car_chase_player_reset (void) {
	car_chase_reset();
	car_chase_modes_achieved = 0;
	car_chase_modes_completed = 0;
	score_zero(car_chase_mode_score_total_score);
}//end of function



void car_chase_mode_init (void) {
	car_chase_mode_shots_made = 0;
	flag_on (FLAG_IS_CARCHASE_MODE_ACTIVATED);
	++car_chase_modes_achieved;
	callset_invoke (carchase_mode_on); //at ramps.c
	sound_start (ST_SPEECH, SPCH_WUSS_SPARTAN, SL_4S, PRI_GAME_QUICK5);
	task_sleep_sec (2);
	sound_start (ST_SPEECH, SPCH_EXCUSE_ME, SL_4S, PRI_GAME_QUICK5);
	task_sleep_sec (1);
	sound_start (ST_SPEECH, SPCH_BETWEEN_YOU_N_ME, SL_4S, PRI_GAME_QUICK5);
	//flash lamp for a time
	score_zero(car_chase_mode_score);
	switch (car_chase_modes_achieved ){
		case 1: score_add(car_chase_mode_score, score_table[SC_15M]); break;
		case 2: score_add(car_chase_mode_score, score_table[SC_20M]); break;
		case 3: score_add(car_chase_mode_score, score_table[SC_25M]); break;
		default: score_add(car_chase_mode_score, score_table[SC_25M]);
	}//end of switch
}//end of function



void car_chase_mode_expire (void) {
	flag_off (FLAG_IS_CARCHASE_MODE_ACTIVATED);
}//end of function

void car_chase_mode_exit (void) { car_chase_mode_expire();}


/****************************************************************************
 * external event listeners
 ****************************************************************************/
CALLSET_ENTRY (car_chase, music_refresh)  	{ timed_mode_music_refresh (&car_chase_mode); }
CALLSET_ENTRY (car_chase, end_ball) 		{ if (timed_mode_running_p(&car_chase_mode) ) timed_mode_end (&car_chase_mode); }
CALLSET_ENTRY (car_chase, display_update) 	{ timed_mode_display_update (&car_chase_mode); }

CALLSET_ENTRY (car_chase, start_player) 	{ car_chase_player_reset(); }
CALLSET_ENTRY (car_chase, start_ball) 		{ car_chase_reset(); }




/****************************************************************************
 * body
 *
 ***************************************************************************/
//this is called from car_crash.c
CALLSET_ENTRY (car_chase, start_car_chase) {
	timed_mode_begin (&car_chase_mode);//start mode
}//end of function


 //center ramp shot made during car_chase mode
CALLSET_ENTRY (car_chase, car_chase_ramp_made) {
	++car_chase_mode_shots_made;
	U8 car_chase_SoundCounter;
	car_chase_SoundCounter = random_scaled(car_chase_TotalNumOfSounds);//from kernal/random.c
	sound_start (ST_SPEECH, car_chase_SoundsArray[car_chase_SoundCounter], SL_4S, PRI_GAME_QUICK2);
	deff_start (DEFF_CAR_CHASE_HIT_EFFECT);//under /kernel/deff.c

	//TODO: score rolls up as time goes?
	score_zero(car_chase_mode_temp_score);
	switch (car_chase_modes_achieved ){
		case 1:
			score (SC_15M);
			score_add(car_chase_mode_temp_score, score_table[SC_15M]);
			break;
		case 2:
			//2nd time we are in car_chase - score differently
			score (SC_20M);
			score_add(car_chase_mode_temp_score, score_table[SC_20M]);
			break;
		case 3:
			//3rd time we are in car_chase - score differently
			score (SC_25M);
			score_add(car_chase_mode_temp_score, score_table[SC_25M]);
			break;
		default:
			//all cases past 3rd time we are in car_chase
			score (SC_25M);
			score_add(car_chase_mode_temp_score, score_table[SC_25M]);
			break;
	}//end of switch
	score_add (car_chase_mode_score, car_chase_mode_temp_score);
	score_add (car_chase_mode_score_total_score, car_chase_mode_temp_score);
}//end of function


/****************************************************************************
 * DMD display and sound effects
 ****************************************************************************/
void car_chase_start_effect_deff(void) {
	dmd_alloc_low_clean ();
	font_render_string_center (&font_term6, DMD_BIG_CX_Top, DMD_BIG_CY_Top, "CAR_CHASE");
	font_render_string_center (&font_mono5, DMD_BIG_CX_Bot, DMD_BIG_CY_Bot, "SHOOT RAMPS");
	dmd_show_low ();
	task_sleep_sec (2);
	deff_exit ();
}//end of mode_effect_deff


void car_chase_hit_effect_deff(void) {
	dmd_alloc_low_clean ();
	font_render_string_center (&font_term6, DMD_BIG_CX_Top, DMD_BIG_CY_Top, "CAR_CHASE");
	sprintf ("15,000,000");
	font_render_string_center (&font_term6, DMD_BIG_CX_Bot, DMD_BIG_CY_Bot, sprintf_buffer);
	dmd_show_low ();
	task_sleep_sec (2);
	deff_exit ();
}//end of mode_effect_deff



void car_chase_effect_deff(void) {
	for (;;) {
		dmd_alloc_low_clean ();
		font_render_string_center (&font_term6, DMD_BIG_CX_Top, DMD_BIG_CY_Top, "CAR_CHASE");
		sprintf ("%d SEC LEFT,  %d HIT", car_chase_mode_timer, car_chase_mode_shots_made);
		font_render_string_center (&font_mono5, DMD_SMALL_CX_3, DMD_SMALL_CY_3, sprintf_buffer);
		sprintf_score (car_chase_mode_score);
		font_render_string_center (&font_mono5, DMD_SMALL_CX_4, DMD_SMALL_CY_4, sprintf_buffer);
		dmd_show_low ();
		task_sleep (TIME_200MS);
	}//END OF ENDLESS LOOP
}//end of mode_effect_deff





void car_chase_end_effect_deff(void) {
	dmd_alloc_low_clean ();
	font_render_string_center (&font_term6, DMD_BIG_CX_Top, DMD_BIG_CY_Top, "CAR_CHASE");
	sprintf("COMPLETED");
	font_render_string_center (&font_term6, DMD_BIG_CX_Bot, DMD_BIG_CY_Bot, sprintf_buffer);
	dmd_show_low ();
	task_sleep_sec (2);
	deff_exit ();
	}//end of mode_effect_deff

