/*
 * demolition man
 * capture_simon.c
 *
 * written by James Cardona
 *
 * Location Description:
 * mode started by claw switch capture_simon
 *
 * Scoring Description: (original game)
 * The ball is dropped down a tunnel and served to the
 * right ramp just like the Underground. The seven
 * major shots are lit for 5 mil each for three shots.
 * A 25 million award for completing all three shots for a total of 40 million

 * Scoring Description: (my rules)
 * same as above except:
 * major shots are lit for 15 mil each for three shots.
 * A 50 million award for completing all three shots for a total of 95 million.
 *
 * estimate of average capture simon mode score: 30 million to 95 million
 *
 */

#include <freewpc.h>
#include "dm/global_constants.h"

//constants
const U8 			CAP_SIM_EASY_GOAL 	= 3;
const U8 			CAP_SIM_MED_GOAL 	= 4;
const U8 			CAP_SIM_HARD_GOAL 	= 5;
const U8 			CAP_SIM_GOAL_INCREMENT = 1;
const U8 			CAP_SIM_GOAL_MAX 	= 10;

//local variables
U8			capture_simon_SoundCounter;
U8 			capture_simon_mode_shots_made;
__local__ U8 			capture_simon_modes_achieved;
__local__ U8 			capture_simon_modes_completed;
U8			capture_simon_mode_timer;
score_t 	capture_simon_mode_score;
score_t 	capture_simon_mode_last_score;
score_t 	capture_simon_mode_next_score;
__local__ score_t 	capture_simon_mode_score_total_score;


//external variables

//prototypes
void capture_simon_reset (void);
void capture_simon_player_reset (void);
void capture_simon_effect_deff(void);
void capture_simon_mode_init (void);
void capture_simon_mode_expire (void);
void capture_simon_mode_exit (void);



/****************************************************************************
 * mode definition structure
 ***************************************************************************/
struct timed_mode_ops capture_simon_mode = {
	DEFAULT_MODE,
	.init = capture_simon_mode_init,
	.exit = capture_simon_mode_exit,
	.gid = GID_CAPTURE_SIMON_MODE_RUNNING,
	.music = MUS_MD_CAPTURE_SIMON,
	.deff_starting = DEFF_CAPTURE_SIMON_START_EFFECT,
	.deff_running = DEFF_CAPTURE_SIMON_EFFECT,
//	.deff_ending = DEFF_CAPTURE_SIMON_END_EFFECT,
	.prio = PRI_GAME_MODE1,
	.init_timer = 23,
	.timer = &capture_simon_mode_timer,
	.grace_timer = 2,
//	.pause = system_timer_pause,
};



/****************************************************************************
 * initialize  and exit
 ***************************************************************************/
void capture_simon_reset (void) {
	flag_off (FLAG_IS_CAPSIM_SIDERAMP_ACTIVATED);
	flag_off (FLAG_IS_CAPSIM_LEFTRAMP_ACTIVATED);
	flag_off (FLAG_IS_CAPSIM_RIGHTRAMP_ACTIVATED);
	flag_off (FLAG_IS_CAPSIM_UNDER_ACTIVATED);
	flag_off (FLAG_IS_CAPSIM_CENTERRAMP_ACTIVATED);
	flag_off (FLAG_IS_CAPSIM_LEFTORB_ACTIVATED);
	flag_off (FLAG_IS_CAPSIM_RIGHTORB_ACTIVATED);
}//end of function


void capture_simon_player_reset (void) {
	capture_simon_reset();
	capture_simon_modes_achieved = 0;
	capture_simon_modes_completed = 0;
	score_zero(capture_simon_mode_score_total_score);
	capture_simon_SoundCounter = 0;
}//end of function



void capture_simon_mode_init (void) {
	sound_start (ST_SPEECH, SPCH_UNDER_ARREST, SL_3S, PRI_GAME_QUICK1);
	score (SC_250K);
	capture_simon_mode_shots_made = 0;
	++capture_simon_modes_achieved;
	//flash lamp for a time
	lamp_tristate_flash(LM_CLAW_CAPTURE_SIMON);
	task_sleep(TIME_500MS);
	lamp_tristate_on(LM_CLAW_CAPTURE_SIMON);
	score_zero(capture_simon_mode_score);
	score_zero(capture_simon_mode_last_score);
	score_zero(capture_simon_mode_next_score);
	switch (capture_simon_modes_achieved ){
		case 1: score_add(capture_simon_mode_next_score, score_table[SC_15M]); break;
		case 2: score_add(capture_simon_mode_next_score, score_table[SC_20M]); break;
		case 3: score_add(capture_simon_mode_next_score, score_table[SC_25M]); break;
		default: score_add(capture_simon_mode_next_score, score_table[SC_25M]);
	}//end of switch
	//this pause is here to give sounds time to finish
	task_sleep(TIME_500MS);
	task_sleep(TIME_500MS);
	task_sleep(TIME_500MS);
	sound_start (ST_SPEECH, SPCH_SO_SCARED, SL_4S, PRI_GAME_QUICK1);
	//TODO: randomize which shots are to be made
	flag_on (FLAG_IS_CAPSIM_SIDERAMP_ACTIVATED);
	flag_on (FLAG_IS_CAPSIM_LEFTRAMP_ACTIVATED);
	flag_off (FLAG_IS_CAPSIM_RIGHTRAMP_ACTIVATED);
	flag_off (FLAG_IS_CAPSIM_UNDER_ACTIVATED);
	flag_on (FLAG_IS_CAPSIM_CENTERRAMP_ACTIVATED);
	flag_off (FLAG_IS_CAPSIM_LEFTORB_ACTIVATED);
	flag_off (FLAG_IS_CAPSIM_RIGHTORB_ACTIVATED);
	callset_invoke(all_arrow_update);
}//end of function




void capture_simon_mode_expire (void) {
	flag_off (FLAG_IS_CAPSIM_SIDERAMP_ACTIVATED);
	flag_off (FLAG_IS_CAPSIM_LEFTRAMP_ACTIVATED);
	flag_off (FLAG_IS_CAPSIM_RIGHTRAMP_ACTIVATED);
	flag_off (FLAG_IS_CAPSIM_UNDER_ACTIVATED);
	flag_off (FLAG_IS_CAPSIM_CENTERRAMP_ACTIVATED);
	flag_off (FLAG_IS_CAPSIM_LEFTORB_ACTIVATED);
	flag_off (FLAG_IS_CAPSIM_RIGHTORB_ACTIVATED);
	callset_invoke(all_arrow_update);
}//end of function


void capture_simon_mode_exit (void) { capture_simon_mode_expire();}


/****************************************************************************
 * external event listeners
 ****************************************************************************/
CALLSET_ENTRY (capture_simon, music_refresh)  	{ timed_mode_music_refresh (&capture_simon_mode); }
CALLSET_ENTRY (capture_simon, end_ball) 		{ if (timed_mode_running_p(&capture_simon_mode) ) timed_mode_end (&capture_simon_mode); }
CALLSET_ENTRY (capture_simon, display_update) 	{ timed_mode_display_update (&capture_simon_mode); }

CALLSET_ENTRY (capture_simon, start_player) 	{ capture_simon_player_reset(); }
CALLSET_ENTRY (capture_simon, start_ball) 		{ capture_simon_reset(); }




/****************************************************************************
 * body
 *
 ***************************************************************************/
CALLSET_ENTRY (capture_simon, sw_claw_capture_simon) {
	timed_mode_begin (&capture_simon_mode);//start mode
}//end of function



 //center ramp shot made during capture_simon mode
CALLSET_ENTRY (capture_simon, capture_simon_made) {
	if ( (capture_simon_SoundCounter++ % 3) == 0 )
		sound_start (ST_SPEECH, SPCH_WES_LAUGH1, SL_4S, PRI_GAME_QUICK5);
	else if ( (capture_simon_SoundCounter % 3) == 1 )
		sound_start (ST_SPEECH, SPCH_WES_LAUGH2, SL_4S, PRI_GAME_QUICK5);
	else if ( (capture_simon_SoundCounter % 3) == 2 )
		sound_start (ST_SPEECH, SPCH_WES_LAUGH3, SL_4S, PRI_GAME_QUICK5);

	++capture_simon_mode_shots_made;
	//TODO: score rolls up as time goes?
	score_zero(capture_simon_mode_last_score);
	switch (capture_simon_modes_achieved ){
		case 1:
			score (SC_15M);
			score_add(capture_simon_mode_last_score, score_table[SC_15M]);
			break;
		case 2:
			//2nd time we are in capture_simon - score differently
			score (SC_20M);
			score_add(capture_simon_mode_last_score, score_table[SC_20M]);
			break;
		case 3:
			//3rd time we are in capture_simon - score differently
			score (SC_25M);
			score_add(capture_simon_mode_last_score, score_table[SC_25M]);
			break;
		default:
			//all cases past 3rd time we are in capture_simon
			score (SC_25M);
			score_add(capture_simon_mode_last_score, score_table[SC_25M]);
			break;
	}//end of switch
	score_add (capture_simon_mode_score, capture_simon_mode_last_score);
	score_add (capture_simon_mode_score_total_score, capture_simon_mode_last_score);

	//TODO: complete mode if xx number of shots made?
	//TODO: not activate capture simon shot on right ramp if diverter is made
	if (capture_simon_mode_shots_made == 1) {
		flag_on (FLAG_IS_CAPSIM_LEFTRAMP_ACTIVATED);
		flag_off (FLAG_IS_CAPSIM_RIGHTRAMP_ACTIVATED);
		flag_off (FLAG_IS_CAPSIM_SIDERAMP_ACTIVATED);
		flag_off (FLAG_IS_CAPSIM_UNDER_ACTIVATED);
		flag_off (FLAG_IS_CAPSIM_CENTERRAMP_ACTIVATED);
		flag_off (FLAG_IS_CAPSIM_LEFTORB_ACTIVATED);
		flag_on (FLAG_IS_CAPSIM_RIGHTORB_ACTIVATED);
		callset_invoke(all_arrow_update);
		deff_start (DEFF_CAPTURE_SIMON_HIT_EFFECT);
	}
	else if (capture_simon_mode_shots_made == 2) {
		flag_on (FLAG_IS_CAPSIM_SIDERAMP_ACTIVATED);
		flag_off (FLAG_IS_CAPSIM_LEFTRAMP_ACTIVATED);
		flag_off (FLAG_IS_CAPSIM_RIGHTRAMP_ACTIVATED);
		flag_off (FLAG_IS_CAPSIM_UNDER_ACTIVATED);
		flag_off (FLAG_IS_CAPSIM_CENTERRAMP_ACTIVATED);
		flag_off (FLAG_IS_CAPSIM_LEFTORB_ACTIVATED);
		flag_off (FLAG_IS_CAPSIM_RIGHTORB_ACTIVATED);
		callset_invoke(all_arrow_update);
		deff_start (DEFF_CAPTURE_SIMON_HIT_EFFECT);
	}
		//IF FINAL CAPTURE SIMON SHOT MADE
	else if (capture_simon_mode_shots_made >= 3) {
		score (SC_50M);
		deff_start (DEFF_CAPTURE_SIMON_END_EFFECT);
		capture_simon_mode_exit();
		}

}//end of function


/****************************************************************************
 * DMD display and sound effects
 ****************************************************************************/
void capture_simon_start_effect_deff(void) {
	dmd_alloc_low_clean ();
	font_render_string_center (&font_times8, DMD_MIDDLE_X, DMD_BIG_CY_Top, "CAPTURE");
	font_render_string_center (&font_times8, DMD_MIDDLE_X, DMD_BIG_CY_Bot, "SIMON");
	dmd_show_low ();
	task_sleep_sec (2);
	deff_exit ();
}//end of mode_effect_deff



void capture_simon_hit_effect_deff(void) {
	dmd_alloc_low_clean ();
	font_render_string_center (&font_term6, DMD_MIDDLE_X, DMD_BIG_CY_Top, "CAPTURE SIMON");
	sprintf_score (capture_simon_mode_last_score);
	font_render_string_center (&font_term6, DMD_MIDDLE_X, DMD_BIG_CY_Bot, sprintf_buffer);
	dmd_show_low ();
	task_sleep_sec (2);
	deff_exit ();
}//end of mode_effect_deff



void capture_simon_effect_deff(void) {
	for (;;) {
		dmd_alloc_low_clean ();
		font_render_string_center (&font_term6, DMD_MIDDLE_X, DMD_BIG_CY_Top, "CAPTURE SIMON");
		sprintf ("%d SEC LEFT,  %d HIT", capture_simon_mode_timer, capture_simon_mode_shots_made);
		font_render_string_center (&font_mono5, DMD_MIDDLE_X, DMD_SMALL_CY_3, sprintf_buffer);
		sprintf_score (capture_simon_mode_next_score);
		font_render_string_center (&font_mono5, DMD_MIDDLE_X, DMD_SMALL_CY_4, sprintf_buffer);
		dmd_show_low ();
		task_sleep (TIME_200MS);
	}//END OF ENDLESS LOOP
}//end of mode_effect_deff





void capture_simon_end_effect_deff(void) {
	dmd_alloc_low_clean ();
	font_render_string_center (&font_term6, DMD_MIDDLE_X, DMD_BIG_CY_Top, "CAPTURE SIMON");
	sprintf("COMPLETED");
	font_render_string_center (&font_term6, DMD_MIDDLE_X, DMD_BIG_CY_Bot, sprintf_buffer);
	dmd_show_low ();
	task_sleep_sec (2);
	deff_exit ();
	}//end of mode_effect_deff

