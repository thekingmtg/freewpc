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

//local variables
			U8 			top_popperSwitchDebouncer;
__local__ 	U8 			top_popper_shots_made;

//external variables

//internally called function prototypes  --external found at protos.h
void top_popper_lights_task (void);

/****************************************************************************
 * initialize  and exit
 ***************************************************************************/
CALLSET_ENTRY (top_popper, start_player, start_ball) {
	top_popperSwitchDebouncer = 0;
	top_popper_shots_made = 0;
}

/****************************************************************************
 *
 * body
 *
 ****************************************************************************/
//called from /common/replay.c
CALLSET_ENTRY (top_popper, award_extra_ball_effect) {
	leff_start (LEFF_TOP_POPPER);
	deff_start (DEFF_EXTRA_BALL_EFFECT);
}//end of function



//sw_top_popper
CALLSET_ENTRY (top_popper, dev_top_sol_enter) {
	ball_search_monitor_stop ();

				//BACK IN FRIDGE WIZARD MODE
				if (flag_test(FLAG_BACK_IN_THE_FRIDGE_ACTIVATED)) {
					start_back_in_the_fridge ();
					task_sleep_sec(1);
					leff_start (LEFF_TOP_POPPER);
					sol_request_async(SOL_TOP_POPPER);
				}

				//EXTRABALL AWARD OR MULTIBALL START OR BOTH
				else if (flag_test(FLAG_IS_EXTRABALL_LIT)
						|| (!flag_test(FLAG_BACK_IN_THE_FRIDGE_RUNNING)
								&&	flag_test (FLAG_IS_MULTIBALL_ENABLED) )) {

						//EXTRABALL AWARD
							if (flag_test(FLAG_IS_EXTRABALL_LIT) ) {
								increment_extra_balls();
								end_extraball();
								leff_start (LEFF_EXTRABALL);
								deff_start_sync (DEFF_EXTRA_BALL_EFFECT);
								task_sleep_sec(1);
							}//end of EB

						//START MULTIBALL
							if (	!flag_test(FLAG_BACK_IN_THE_FRIDGE_RUNNING)
								&&	flag_test (FLAG_IS_MULTIBALL_ENABLED) ) {
								multiball_start();
								task_sleep_sec(1);
							}//end of multiball start

						//do for both
							leff_start (LEFF_TOP_POPPER);
							sol_request_async(SOL_TOP_POPPER);
				}//END of EB or MB

				//START VIDEO MODE
				else if (  !flag_test(FLAG_BACK_IN_THE_FRIDGE_RUNNING)
						&& !flag_test(FLAG_IS_FORTRESS_MB_RUNNING)
						&& !flag_test(FLAG_IS_MUSEUM_MB_RUNNING)
						&& !flag_test(FLAG_IS_CRYOPRISON_MB_RUNNING)
						&& !flag_test(FLAG_IS_WASTELAND_MB_RUNNING)
						&& !flag_test(FLAG_IS_DEMOTIME_RUNNING)
						&&	flag_test (FLAG_VIDEO_MODE_ENABLED) )//this is set at combos
												start_video_mode(1);

				//NOTHING SPECIAL
				else {
					score (TOP_POP_SCORE);//located in kernal/score.c
					//LIGHTING EFFECTS
					//	leff_start (LEFF_UNDERGROUND_KICKOUT);
					deff_start_sync (DEFF_TOP_POPPER_EFFECT); //if nothing special, do normal display effects
					leff_start (LEFF_TOP_POPPER);
					sol_request_async(SOL_TOP_POPPER);
				}//end of else NOTHING SPECIAL

	ball_search_monitor_start ();
}//end of function



void top_popper_video_mode_finished (void) {
	leff_start (LEFF_TOP_POPPER);
	sol_request_async(SOL_TOP_POPPER);
}//end of function




/****************************************************************************
 *
 * lighting effects
 *
 ****************************************************************************/
void top_popper_leff (void) {
	U8 i;

	for (i = 0; i < 6; i++) {
		flasher_pulse (FLASH_DIVERTER_FLASHER);
		task_sleep (TIME_33MS);
		flasher_pulse(FLASH_CLAW_FLASHER);
		task_sleep (TIME_33MS);
		flasher_pulse(FLASH_ELEVATOR_1_FLASHER);
		task_sleep (TIME_33MS);
		flasher_pulse(FLASH_ELEVATOR_2_FLASHER);
		task_sleep (TIME_100MS);
	}

	for (i = 0; i < 6; i++) {
		flasher_pulse (FLASH_LEFT_RAMP_UP_FLASHER);
		task_sleep (TIME_33MS);
		flasher_pulse (FLASH_DIVERTER_FLASHER);
		task_sleep (TIME_33MS);
		flasher_pulse (FLASH_RIGHT_RAMP_UP_FLASHER);
		sound_start (ST_EFFECT, ZAPP_3_MED, SL_2S, PRI_GAME_QUICK2);
		task_sleep (TIME_100MS);
	}
	leff_exit();
}//end of function



void extraball_leff(void) {
	gi_leff_disable (PINIO_GI_STRINGS); //turn off gi
	leff_start(LEFF_FLASH_ALL);
	gi_leff_enable (PINIO_GI_STRINGS); //turn on gi
	task_sleep_sec (TIME_1S);
	leff_exit();
}//end of function







/****************************************************************************
 *
 * display effects
 *
 ****************************************************************************/
//nothing is going on effect
U8 	top_popper_MessageCounter;
void top_popper_effect_deff(void) {
	U16 fno;

	if (IN_TEST) {
		if (++top_popper_MessageCounter > 3) top_popper_MessageCounter = 0;
	}
	else top_popper_MessageCounter = random_scaled(3);

	dmd_alloc_pair_clean ();// Clean both pages

	switch (top_popper_MessageCounter) {
		default:
		case 0:
			sound_start (ST_EFFECT, TRAIN_PULL_IN, SL_2S, SP_NORMAL);
			for (fno = IMG_SIMON_E_END-2; fno >= IMG_SIMON_E_START; fno -= 2) {
						dmd_alloc_pair ();
						frame_draw(fno);
						dmd_show2 ();
						task_sleep (TIME_100MS);
			}//end of for loop

			sound_start (ST_EFFECT, TRAIN_PULL_IN, SL_2S, SP_NORMAL);
			for (fno = IMG_SIMON_E_START; fno <= IMG_SIMON_E_END; fno += 2) {
					dmd_map_overlay ();
					dmd_clean_page_low ();
					font_render_string_center (&font_term6, DMD_MIDDLE_X, DMD_MED_CY_1, "LIGHT ALL M T L");
					font_render_string_center (&font_var5, DMD_MIDDLE_X, DMD_MED_CY_2, "5 TIMES TO");
					font_render_string_center (&font_bitcube10, DMD_MIDDLE_X, DMD_MED_CY_3, "LIGHT EXTRA BALL");
						dmd_text_outline ();
						dmd_alloc_pair ();
						frame_draw(fno);
						dmd_overlay_outline ();
						dmd_show2 ();
						task_sleep (TIME_100MS);
			}//end of for loop
			break;
		case 1:
			sound_start (ST_EFFECT, COMPUTER3, SL_2S, SP_NORMAL);
			for (fno = IMG_CLAW_B_START; fno <= IMG_CLAW_B_END; fno += 2) {
						dmd_alloc_pair ();
						frame_draw(fno);
						dmd_show2 ();
						task_sleep (TIME_100MS);
			}//end of for loop

			sound_start (ST_EFFECT, COMPUTER3, SL_2S, SP_NORMAL);
					dmd_map_overlay ();
					dmd_clean_page_low ();
					font_render_string_center (&font_term6, DMD_MIDDLE_X, DMD_MED_CY_1, "SHOOT EYEBALL");
					font_render_string_center (&font_term6, DMD_MIDDLE_X, DMD_MED_CY_2, "TO LIGHT");
					font_render_string_center (&font_bitcube10, DMD_MIDDLE_X, DMD_MED_CY_3, "EXTRA BALL");
						dmd_text_outline ();
						dmd_alloc_pair ();
						frame_draw(IMG_CLAW_B_END);
						dmd_overlay_outline ();
						dmd_show2 ();
			break;
		case 2:
			sound_start (ST_EFFECT, COMPUTER3, SL_2S, SP_NORMAL);
			for (fno = IMG_CLAW_A_START; fno <= IMG_CLAW_A_END; fno += 2) {
						dmd_alloc_pair ();
						frame_draw(fno);
						dmd_show2 ();
						task_sleep (TIME_100MS);
			}//end of for loop

			sound_start (ST_EFFECT, COMPUTER3, SL_2S, SP_NORMAL);
					dmd_map_overlay ();
					dmd_clean_page_low ();
					font_render_string_center (&font_term6, DMD_MIDDLE_X, DMD_MED_CY_1, "SHOOT COMBOS");
					font_render_string_center (&font_term6, DMD_MIDDLE_X, DMD_MED_CY_2, "TO LIGHT");
					font_render_string_center (&font_bitcube10, DMD_MIDDLE_X, DMD_MED_CY_3, "VIDEO MODE");
						dmd_text_outline ();
						dmd_alloc_pair ();
						frame_draw(IMG_CLAW_A_END);
						dmd_overlay_outline ();
						dmd_show2 ();
			break;
	}//END OF SWITCH
	deff_exit ();
}//end of mode_effect_deff




void extra_ball_effect_deff(void) {
	U8 eb_rnd_sound;
	U16 fno;
	dmd_alloc_pair_clean ();// Clean both pages
	eb_rnd_sound = random_scaled(2);
	sound_start (ST_EFFECT, EXTRA_BALL_SOUND, SL_2S, SP_NORMAL);

	for (fno = IMG_FREEZER1_START; fno <= IMG_FREEZER1_END; fno += 2) {
				dmd_alloc_pair ();
				frame_draw(fno);
				dmd_show2 ();
				task_sleep (TIME_66MS);
	}//end of for loop

	for (fno = IMG_FREEZER2_START; fno <= IMG_FREEZER2_END; fno += 2) {
				dmd_alloc_pair ();
				frame_draw(fno);
				dmd_show2 ();
				task_sleep (TIME_66MS);
	}//end of for loop

	if (eb_rnd_sound) 	sound_start (ST_SPEECH, SPCH_EXTRABALL_WES, SL_2S, SP_NORMAL);
	else 				sound_start (ST_SPEECH, SPCH_EXTRABALL_SLY, SL_2S, SP_NORMAL);

	for (fno = IMG_FREEZER3_START; fno <= IMG_FREEZER3_END; fno += 2) {
					dmd_map_overlay ();
					dmd_clean_page_low ();
					font_render_string_center (&font_fireball, DMD_MIDDLE_X - 25, DMD_BIG_CY_Top, "EXTRA");
					font_render_string_center (&font_fireball, DMD_MIDDLE_X - 25, DMD_BIG_CY_Bot, "BALL");
						dmd_text_outline ();
						dmd_alloc_pair ();
						frame_draw(fno);
						dmd_overlay_outline ();
						dmd_show2 ();
						task_sleep (TIME_66MS);
			}//end of for loop

	deff_exit ();
}//end of mode_effect_deff


