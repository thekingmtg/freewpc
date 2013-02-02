/*
 * demolition man
 * eject.c
 *
 * written by James Cardona
 *
 * Location Description:
 * Retina Scan: This is the leftmost shot of the game. The ball passes through a gate
 * and into an eject that feeds the left inlane.
 * If hit hard enough through the gate, it will hit the captive eyeball.
 *
 */
/* CALLSET_SECTION (eject, __machine4__) */

#include <freewpc.h>
#include "dm/global_constants.h"
#include "search.h"

//local variables
U8 eject_killer_counter;
U8 retina_scan_multiplier;
U8 ejectDeffCounter;
score_t temp_score;

//external variables
extern __local__ U8 jet_shots_made;//external ref to eject.c

//internally called function prototypes  --external found at protos.h
void eject_reset (void);
void player_eject_reset (void);
void eject_killer_task (void);



/****************************************************************************
 * initialize  and exit
 ***************************************************************************/
void eject_reset (void) {
	retina_scan_multiplier = 1;
	flag_off(FLAG_KILL_NORMAL_EJECT);

}//end of function


void player_eject_reset (void) {
	eject_killer_counter = 0;
	eject_reset ();
	ejectDeffCounter = 0;
}//end of function

CALLSET_ENTRY (eject, start_player) 	{ player_eject_reset(); }
CALLSET_ENTRY (eject, start_ball) 		{ eject_reset(); }



/****************************************************************************
 * eject saucer
 ****************************************************************************/
//this is to prevent a retrigger of the eject switch as soon as ball exits
void eject_killer_task (void){
	task_sleep (TIME_2S);
	eject_killer_counter = 0;
	task_exit();
}//end of function



CALLSET_ENTRY (eject, dev_eyeball_eject_enter) {
	//HUXLEY MADE
	if ( !flag_test(FLAG_BACK_IN_THE_FRIDGE_RUNNING)
		&&	task_kill_gid(GID_LEFT_INLANE_MADE) ) {
//		flag_on(FLAG_KILL_NORMAL_EJECT);
		huxley_made();
		return;
//		task_sleep (TIME_300MS);//give time for huxley to catch up
	}

	//START VIDEO MODE
	else if (  !flag_test(FLAG_BACK_IN_THE_FRIDGE_RUNNING)
			&& !flag_test(FLAG_IS_FORTRESS_MB_RUNNING)
			&& !flag_test(FLAG_IS_MUSEUM_MB_RUNNING)
			&& !flag_test(FLAG_IS_CRYOPRISON_MB_RUNNING)
			&& !flag_test(FLAG_IS_WASTELAND_MB_RUNNING)
			&& !flag_test(FLAG_IS_DEMOTIME_RUNNING)
			&&	flag_test (FLAG_VIDEO_MODE_ENABLED) ) {
//						flag_on(FLAG_KILL_NORMAL_EJECT);
						start_video_mode(3);

						sound_start (ST_ANY, SPCH_PLAYER_TWO, SL_1S, PRI_GAME_QUICK5);
						task_sleep (TIME_500MS);
						task_sleep (TIME_500MS);
						task_sleep (TIME_500MS);
						task_sleep (TIME_500MS);


						combo_rehit_check (); //check to see if enough combos to relight video mode
						sol_request (SOL_EJECT); //request to fire the eject sol
						ball_search_monitor_start ();
//						return;
						task_sleep (TIME_300MS);
	}//end of start video mode

	//NORMAL RETINA SCAN
	else //if (!flag_test(FLAG_KILL_NORMAL_EJECT) )
	{
						leff_start (LEFF_EJECT);

						sound_start (ST_SAMPLE, RETINA_SCAN_LONG, SL_4S, PRI_GAME_QUICK1);
						score (EJECT_SCORE);
						//100k per jet hit here
						if (jet_shots_made > 0) {
							score_zero (temp_score);//zero out temp score
							score_add (temp_score, score_table[SC_100K]);//multiply 100K by jet count
							score_mul (temp_score, jet_shots_made);//multiply 100K by jet count
							score_long (temp_score); //add temp score to player's score
						}//end of if

						if (retina_scan_multiplier == 2)	{
							sound_start(ST_SPEECH, SPCH_DOUBLE_RETINA_SCAN, SL_4S, PRI_GAME_QUICK5);
							score (EJECT_SCORE);
							score_long (temp_score); //add temp score to player's score
						}

						deff_start (DEFF_EJECT_EFFECT);
						task_sleep_sec (1);

						sol_request (SOL_EJECT); //request to fire the eject sol
	}//end of else !flag_test(FLAG_KILL_NORMAL_EJECT)
}//end of function




//called from comp award at underground.c
void comp_award_doub_retina(void) {
	retina_scan_multiplier = 2;
}//end of function



void eject_video_mode_finished(void) {
//	task_create_gid1(GID_EJECT_LEFF, eject_leff);
	sol_request (SOL_EJECT); //request to fire the eject sol
	ball_search_monitor_start ();
//	task_sleep (TIME_300MS);
//	flag_off(FLAG_KILL_NORMAL_EJECT);
} //end of function




/****************************************************************************
 *
 * lighting effects
 *
 ****************************************************************************/
void eject_leff (void){
	lamp_tristate_flash(LM_RETINA_SCAN);
	flasher_pulse (FLASH_EJECT_FLASHER);
	task_sleep (TIME_100MS);
	flasher_pulse (FLASH_EJECT_FLASHER);
	task_sleep (TIME_100MS);
	flasher_pulse (FLASH_EJECT_FLASHER);
	task_sleep (TIME_100MS);
	flasher_pulse (FLASH_EJECT_FLASHER);
	task_sleep (TIME_100MS);
	flasher_pulse (FLASH_EJECT_FLASHER);
	task_sleep (TIME_100MS);
	flasher_pulse (FLASH_EJECT_FLASHER);
	task_sleep (TIME_100MS);
	flasher_pulse (FLASH_EJECT_FLASHER);
	task_sleep (TIME_100MS);
	flasher_pulse (FLASH_EJECT_FLASHER);
	task_sleep (TIME_100MS);
	flasher_pulse (FLASH_EJECT_FLASHER);
	task_sleep (TIME_100MS);
	flasher_pulse (FLASH_EJECT_FLASHER);
	task_sleep (TIME_100MS);
	flasher_pulse (FLASH_EJECT_FLASHER);
	task_sleep (TIME_100MS);
	flasher_pulse (FLASH_EJECT_FLASHER);
	task_sleep (TIME_100MS);
	flasher_pulse (FLASH_EJECT_FLASHER);
	task_sleep (TIME_100MS);
	flasher_pulse (FLASH_EJECT_FLASHER);
	task_sleep (TIME_100MS);
	lamp_tristate_off(LM_RETINA_SCAN);
	leff_exit();
}//end of function




/****************************************************************************
 *
 * DISPLAY EFFECTS
 *
 ****************************************************************************/
void eject_effect_deff(void) {
	U16 fno;
	dmd_clean_page_high ();//
	dmd_clean_page_low ();//
	switch (++ejectDeffCounter % 2) {
		default:
		case 0:
					for (fno = IMG_EYE_B_START; fno <= IMG_EYE_B_END; fno += 2) {
							dmd_alloc_pair ();
							frame_draw(fno);
							dmd_show2 ();
							task_sleep (TIME_66MS);
						}//end of inner loop

					for (fno = IMG_EYE_B_END - 2; fno >= IMG_EYE_B_START + 6; fno -= 2) {
							dmd_alloc_pair ();
							frame_draw(fno);
							dmd_show2 ();
							task_sleep (TIME_66MS);
						}//end of inner loop

					for (fno = IMG_EYE_B_START + 4; fno >= IMG_EYE_B_START + 2; fno -= 2) {
						dmd_map_overlay ();
						dmd_clean_page_low ();
						font_render_string_center (&font_fireball, DMD_MIDDLE_X, DMD_BIG_CY_Top, "RETINA");
						font_render_string_center (&font_fireball, DMD_MIDDLE_X, DMD_BIG_CY_Bot, "SCAN");
						dmd_text_outline ();
						dmd_alloc_pair ();
						frame_draw(fno);
						dmd_overlay_outline ();
						dmd_show2 ();
						task_sleep (TIME_66MS);
					}//end of inner loop
			break;
		case 1:
					for (fno = IMG_EYE_START; fno <= IMG_EYE_27; fno += 2) {
						dmd_alloc_pair ();
						frame_draw(fno);
						dmd_show2 ();
						task_sleep (TIME_66MS);
					}//end of inner loop

				dmd_alloc_pair_clean ();// Clean both pages
				for (fno = IMG_EYE_28; fno <= IMG_EYE_END; fno += 2) {
					dmd_map_overlay ();
					dmd_clean_page_low ();
					font_render_string_center (&font_fireball, DMD_MIDDLE_X, DMD_BIG_CY_Top, "RETINA");
					font_render_string_center (&font_fireball, DMD_MIDDLE_X, DMD_BIG_CY_Bot, "SCAN");
					dmd_text_outline ();
					dmd_alloc_pair ();
					frame_draw(fno);
					dmd_overlay_outline ();
					dmd_show2 ();
					task_sleep (TIME_66MS);
				}//end of inner loop
			break;
		}//end of switch
	deff_exit ();
}//end of mode_effect_deff



