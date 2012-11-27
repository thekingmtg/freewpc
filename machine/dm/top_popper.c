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
__local__ 	U8 			top_popper_shots_made;
__local__ 	U8 			video_mode_goal;
__local__ 	U8 			video_mode_counter;

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
 * body
 *
 ****************************************************************************/
CALLSET_ENTRY (top_popper, sw_top_popper) {
	ball_search_monitor_stop ();

	//EXTRABALL AWARD
	if (flag_test(FLAG_IS_EXTRABALL_LIT) ) {
		increment_extra_balls();
		extraball_light_off();
		deff_start_sync (DEFF_EXTRA_BALL_EFFECT);
		task_sleep_sec(1);
		sol_request_async(SOL_TOP_POPPER);
	}

	//START MULTIBALL
	else if (flag_test (FLAG_IS_MULTIBALL_ENABLED) ) {
			multiball_start();
			task_sleep_sec(3);
			sol_request_async(SOL_TOP_POPPER);
		}//end of multiball start

	//START VIDEO MODE
	/*
	else if (++top_popper_shots_made >= video_mode_goal) {
				flag_on(FLAG_IN_VIDEO_MODE);
				start_video_mode();
				video_mode_goal += VIDEO_MODE_GOAL_INCREMENT;
		}
	*/

	//NOTHING SPECIAL
	else {
		score (TOP_POP_SCORE);//located in kernal/score.c
		//SOUNDS
		U8	top_popper_SoundCounter;
		top_popper_SoundCounter = random_scaled(2);//from kernal/random.c
		if (top_popper_SoundCounter == 0 )
			sound_start (ST_EFFECT, TRAIN_PULL_IN, SL_2S, SP_NORMAL);
		else if (top_popper_SoundCounter == 1 )
			sound_start (ST_EFFECT, COMPUTER3, SL_2S, SP_NORMAL);
		//LIGHTING EFFECTS
		//	leff_start (LEFF_UNDERGROUND_KICKOUT);
		deff_start_sync (DEFF_TOP_POPPER_EFFECT); //if nothing special, do normal display effects
		task_sleep_sec(1);
		sol_request_async(SOL_TOP_POPPER);
	}//end of else NOTHING SPECIAL
	ball_search_monitor_start ();
}//end of function



void start_video_mode(void) {
	video_mode_counter++;
	flipper_disable ();
	switch (video_mode_counter % 3) {
	default:
	case 0:
//		start_wheelie();
		break;
	case 1:
		break;
	case 2:
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
//nothing is going on effect
void top_popper_effect_deff(void) {
	U16 fno;
	U8 	MessageCounter;
	MessageCounter = random_scaled(3);
	dmd_alloc_pair_clean ();// Clean both pages

	switch (MessageCounter) {
		default:
		case 0:
			for (fno = IMG_SIMON_E_END; fno >= IMG_SIMON_E_START; fno -= 2) {
						dmd_alloc_pair ();
						frame_draw(fno);
						dmd_show2 ();
						task_sleep (TIME_100MS);
			}//end of for loop

			for (fno = IMG_SIMON_E_START; fno <= IMG_SIMON_E_END; fno += 2) {
					dmd_map_overlay ();
					dmd_clean_page_low ();
					font_render_string_center (&font_var5, DMD_MIDDLE_X, DMD_SMALL_CY_1, "LIGHT ALL M T L");
					font_render_string_center (&font_var5, DMD_MIDDLE_X, DMD_SMALL_CY_2, "5 TIMES");
					font_render_string_center (&font_var5, DMD_MIDDLE_X, DMD_SMALL_CY_3, "TO");
					font_render_string_center (&font_bitcube10, DMD_MIDDLE_X, DMD_SMALL_CY_4, "LIGHT EXTRA BALL");
						dmd_text_outline ();
						dmd_alloc_pair ();
						frame_draw(fno);
						dmd_overlay_outline ();
						dmd_show2 ();
						task_sleep (TIME_100MS);
			}//end of for loop
			break;
		case 1:
			for (fno = IMG_CLAW_B_END; fno >= IMG_CLAW_B_START; fno -= 2) {
						dmd_alloc_pair ();
						frame_draw(fno);
						dmd_show2 ();
						task_sleep (TIME_100MS);
			}//end of for loop

			for (fno = IMG_CLAW_B_START; fno <= IMG_CLAW_B_END; fno += 2) {
					dmd_map_overlay ();
					dmd_clean_page_low ();
					font_render_string_center (&font_var5, DMD_MIDDLE_X, DMD_SMALL_CY_1, "SHOOT EYEBALL");
					font_render_string_center (&font_var5, DMD_MIDDLE_X, DMD_SMALL_CY_3, "TO");
					font_render_string_center (&font_bitcube10, DMD_MIDDLE_X, DMD_SMALL_CY_4, "LIGHT EXTRA BALL");
						dmd_text_outline ();
						dmd_alloc_pair ();
						frame_draw(fno);
						dmd_overlay_outline ();
						dmd_show2 ();
						task_sleep (TIME_100MS);
			}//end of for loop
			break;
		case 2:
			for (fno = IMG_CLAW_A_END; fno >= IMG_CLAW_A_START; fno -= 2) {
						dmd_alloc_pair ();
						frame_draw(fno);
						dmd_show2 ();
						task_sleep (TIME_100MS);
			}//end of for loop

			for (fno = IMG_CLAW_A_START; fno <= IMG_CLAW_A_END; fno += 2) {
					dmd_map_overlay ();
					dmd_clean_page_low ();
					font_render_string_center (&font_var5, DMD_MIDDLE_X, DMD_SMALL_CY_1, "SHOOT TOP");
					font_render_string_center (&font_var5, DMD_MIDDLE_X, DMD_SMALL_CY_2, "POPPER");
					font_render_string_center (&font_var5, DMD_MIDDLE_X, DMD_SMALL_CY_3, "FOR");
					font_render_string_center (&font_bitcube10, DMD_MIDDLE_X, DMD_SMALL_CY_4, "VIDEO MODE");
						dmd_text_outline ();
						dmd_alloc_pair ();
						frame_draw(fno);
						dmd_overlay_outline ();
						dmd_show2 ();
						task_sleep (TIME_100MS);
			}//end of for loop
			break;
	}//END OF SWITCH
	task_sleep (TIME_500MS);
	deff_exit ();
}//end of mode_effect_deff




void extra_ball_effect_deff(void) {
	U16 fno;
	dmd_alloc_pair_clean ();// Clean both pages
	sound_start (ST_EFFECT, SPECIAL, SL_2S, SP_NORMAL);

	for (fno = IMG_SIMON_E_END; fno >= IMG_SIMON_E_START; fno -= 2) {
				dmd_alloc_pair ();
				frame_draw(fno);
				dmd_show2 ();
				task_sleep (TIME_100MS);
	}//end of for loop

			for (fno = IMG_SIMON_E_START; fno <= IMG_SIMON_E_END; fno += 2) {
					dmd_map_overlay ();
					dmd_clean_page_low ();
					font_render_string_center (&font_fireball, DMD_MIDDLE_X - 25, DMD_BIG_CY_Top, "EXTRA");
					font_render_string_center (&font_fireball, DMD_MIDDLE_X - 25, DMD_BIG_CY_Bot, "BALL");
						dmd_text_outline ();
						dmd_alloc_pair ();
						frame_draw(fno);
						dmd_overlay_outline ();
						dmd_show2 ();
						task_sleep (TIME_100MS);
			}//end of for loop
	task_sleep (TIME_3S);
	deff_exit ();

	deff_exit ();
}//end of mode_effect_deff



//this is a place holder for now
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


