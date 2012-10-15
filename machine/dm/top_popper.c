/*
 * demolition man
 * top_popper.c
 *
 * written by James Cardona
 *
 * Location Description:
 *
 * Scoring Description: (original game)
 *
 *
 * Scoring Description: (my rules)
 * same as above except
 *
 */
/* CALLSET_SECTION (top_popper, __machine4__) */


#include <freewpc.h>
#include <eb.h>
#include "search.h"
#include "dm/global_constants.h"

//constants
const U8 		top_popperAwardsNumOfSounds = 6; //num between 0 and N
const sound_code_t top_popperAwardsSoundsArray[] = {	SPCH_YOU_LOOK_GREAT_TODAY,	SPCH_ILUMINATE,
														SPCH_AMAZING_WES,			SPCH_AMAZING_SLY,
														SPCH_GLORIOUS,				SPCH_OUTSTANDING};

const U8 EASY_VIDEO_MODE_GOAL = 3;
const U8 VIDEO_MODE_GOAL_INCREMENT = 5;

//local variables
U8 			top_popperSwitchDebouncer;
__local__ U8 			top_popper_shots_made;
__local__ U8 			video_mode_goal;
__local__ U8 			video_mode_counter;

//external variables

//internally called function prototypes  --external found at protos.h
void top_popper_reset (void);
void start_video_mode(void);

/****************************************************************************
 * initialize  and exit
 ***************************************************************************/
void top_popper_reset (void) {
	top_popperSwitchDebouncer = 0;
	top_popper_shots_made = 0;
	flag_off(FLAG_IN_VIDEO_MODE);
}//end of function

void player_top_popper_reset(void) {
	video_mode_goal = EASY_VIDEO_MODE_GOAL;
	video_mode_counter = 0;
	top_popper_reset();
}//end of function

CALLSET_ENTRY (top_popper, start_player) { player_top_popper_reset(); }
CALLSET_ENTRY (top_popper, start_ball) { top_popper_reset(); }


/****************************************************************************
 * playfield lights and flags
 ***************************************************************************/


/****************************************************************************
 * body
 *
 ****************************************************************************/
CALLSET_ENTRY (top_popper, sw_top_popper) {
	ball_search_monitor_stop ();
	if (flag_test(FLAG_IS_EXTRABALL_LIT) ) {
		increment_extra_balls();
		extraball_light_off();
		//extra ball deff here
	}

		if (flag_test (FLAG_IS_MULTIBALL_ACTIVATED) ) {
			multiball_start();
			task_sleep_sec(5);
			task_sleep_sec(2);
			sol_request_async(SOL_TOP_POPPER);
			ball_search_monitor_start ();
		}//end of multiball start
		else
		if (++top_popper_shots_made >= video_mode_goal) {
				flag_on(FLAG_IN_VIDEO_MODE);
				flipper_disable ();
				start_video_mode();
				video_mode_goal += VIDEO_MODE_GOAL_INCREMENT;
		}
		else {
				score (SC_100K);//located in kernal/score.c
		//SOUNDS
				U8	top_popper_SoundCounter;
				top_popper_SoundCounter = random_scaled(2);//from kernal/random.c
				if (top_popper_SoundCounter == 0 )
					sound_start (ST_EFFECT, TRAIN_PULL_IN, SL_2S, SP_NORMAL);
				else if (top_popper_SoundCounter == 1 )
					sound_start (ST_EFFECT, COMPUTER3, SL_2S, SP_NORMAL);
		//LIGHTING EFFECTS
				//	leff_start (LEFF_UNDERGROUND_KICKOUT);
				deff_start (DEFF_TOP_POPPER_EFFECT); //if nothing special, do normal display effects
				task_sleep_sec(3);
				sol_request_async(SOL_TOP_POPPER);
				ball_search_monitor_start ();
		}//end of else
}//end of function



void start_video_mode(void) {
	video_mode_counter++;
	switch (video_mode_counter % 3) {
	case 0:
		start_wheelie();
		break;
	case 1:
	case 2:
	default:
		start_wheelie();
		break;
	}//end of switch
}//end of function


void video_mode_finished (void) {
	flipper_enable ();
	flag_off(FLAG_IN_VIDEO_MODE);
	sol_request_async(SOL_TOP_POPPER);
	ball_search_monitor_start ();
}//end of function


/****************************************************************************
 * lighting effects
 ****************************************************************************/


/****************************************************************************
 * display effects
 ****************************************************************************/
void top_popper_effect_deff(void) {
	dmd_alloc_pair_clean();
	frame_draw_plane (IMG_OLDCAR); //only to low page
	dmd_show_low ();
	task_sleep_sec (1);


	dmd_alloc_pair_clean();
	frame_draw_plane_test (IMG_CITY); //only to low page
	dmd_show_low ();
	task_sleep_sec (1);

	dmd_alloc_pair_clean();
	frame_draw_plane (IMG_OLDCAR);
	dmd_flip_low_high ();
	dmd_show2 ();
	task_sleep_sec (1);

//bmp_draw ();//x,y,id - for placing a small image
	dmd_alloc_low_clean ();
	bmp_draw (0, 0, IMG_OLDCAR);
	dmd_show_low ();
	task_sleep_sec (2);

	dmd_alloc_low_clean ();
	bmp_draw (0, 0, IMG_TEST);
	dmd_show_low ();
	task_sleep_sec (2);

	dmd_alloc_low_clean ();
	sprintf ("TOP");
	font_render_string_center (&font_v5prc, DMD_MIDDLE_X, DMD_BIG_CY_Top, sprintf_buffer);
	sprintf ("%d MADE", top_popper_shots_made);
	font_render_string_center (&font_v5prc, DMD_MIDDLE_X, DMD_BIG_CY_Bot, sprintf_buffer);

	dmd_show_low ();
//	dmd_show2 ();
	task_sleep_sec (2);
	deff_exit ();
}//end of mode_effect_deff



void top_popper_award_effect_deff(void) {
	dmd_alloc_low_clean ();
	sprintf ("VIDEO");
	font_render_string_center (&font_v5prc, DMD_MIDDLE_X, DMD_BIG_CY_Top, sprintf_buffer);
	sprintf ("MODE");
	font_render_string_center (&font_v5prc, DMD_MIDDLE_X, DMD_BIG_CY_Bot, sprintf_buffer);
	dmd_show_low ();
	task_sleep_sec (2);
	deff_exit ();
}//end of mode_effect_deff


