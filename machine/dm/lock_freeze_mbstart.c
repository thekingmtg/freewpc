/*
 * demolition man
 * lock_freeze_mbstart.c
 * 
 * written by James Cardona
 *
 * handles the lock freezes and the trigger of multiballs
 *
 *
 */
/* CALLSET_SECTION (lock_freeze_mbstart, __machine3__) */

#include <freewpc.h>
#include "dm/global_constants.h"


//local variables
__local__ 	U8			NumBallsFrozen;
__local__ 	U8			NumMBsDone;
			U8			lock_SoundCounter;
__local__ 	U8			NumBallsNeededForNextMB;
			U8			freeze_MessageCounter;
//external variables

//internally called function prototypes  --external found at protos.h
void lock_reset (void);
void lfmb_ball_reset (void);
void lfmb_player_reset (void);
void check_multiball_requirements(void);

/****************************************************************************
 * initialize  and exit
 ***************************************************************************/
void lock_reset (void) {
	multiball_light_off();//goto orbits.c to turn off light and flag
}//end of reset



void lfmb_ball_reset (void) {
	switch (NumBallsFrozen) {
		case 1:	lamp_tristate_on (LM_FREEZE_1);	break;
		case 2:
				lamp_tristate_on (LM_FREEZE_1);
				lamp_tristate_on (LM_FREEZE_2);
				break;
		case 3:
				lamp_tristate_on (LM_FREEZE_1);
				lamp_tristate_on (LM_FREEZE_2);
				lamp_tristate_on (LM_FREEZE_3);
				break;
		case 4:
				lamp_tristate_on (LM_FREEZE_1);
				lamp_tristate_on (LM_FREEZE_2);
				lamp_tristate_on (LM_FREEZE_3);
				lamp_tristate_on (LM_FREEZE_4);
		break;
	}//end of switch
}



void lfmb_player_reset (void) {
	lock_reset();
	NumBallsFrozen = 0;
	lock_SoundCounter = 0;
	NumMBsDone = 0;
	NumBallsNeededForNextMB = 1;
	freeze_MessageCounter = 0;
}//end of function



CALLSET_ENTRY (lock_freeze_mbstart, start_player) { lfmb_player_reset(); }
CALLSET_ENTRY (lock_freeze_mbstart, start_ball) { lfmb_ball_reset(); }

/****************************************************************************
 * body
 *
 ***************************************************************************/
CALLSET_ENTRY (lock_freeze_mbstart, music_refresh) {
	if (in_live_game && flag_test(FLAG_IS_MULTIBALL_ENABLED) )	music_request (MUS_MB_READY, PRI_GAME_MODE1);//must be higher priority than PRI_SCORES
}//end of function



void maximize_freeze(void) {
	NumBallsFrozen = 4; //see note below
	increment_freeze(); //will add another here
}//end of function




void increment_freeze(void) {
	sound_start (ST_EFFECT, LOCK_FREEZE_PLOINK, SL_1S, PRI_GAME_QUICK1);
	if (NumBallsFrozen < 5)
			switch (++NumBallsFrozen) {
			case 1:
					lamp_tristate_flash(LM_FREEZE_1);
					task_sleep (TIME_500MS);
					lamp_tristate_on (LM_FREEZE_1);
					break;
			case 2:
					lamp_tristate_flash(LM_FREEZE_2);
					task_sleep (TIME_500MS);
					lamp_tristate_on (LM_FREEZE_1);
					lamp_tristate_on (LM_FREEZE_2);
					break;
			case 3:
					lamp_tristate_flash(LM_FREEZE_3);
					task_sleep (TIME_500MS);
					lamp_tristate_on (LM_FREEZE_1);
					lamp_tristate_on (LM_FREEZE_2);
					lamp_tristate_on (LM_FREEZE_3);
					break;
			case 4:
					lamp_tristate_flash(LM_FREEZE_4);
					task_sleep (TIME_500MS);
					lamp_tristate_on (LM_FREEZE_1);
					lamp_tristate_on (LM_FREEZE_2);
					lamp_tristate_on (LM_FREEZE_3);
					lamp_tristate_on (LM_FREEZE_4);
					break;
			case 5://THERE ARE ONLY 4 LIGHTS, this is just here of for the case of maximize freezes, to prevent any from flashing
					lamp_tristate_on (LM_FREEZE_1);
					lamp_tristate_on (LM_FREEZE_2);
					lamp_tristate_on (LM_FREEZE_3);
					lamp_tristate_on (LM_FREEZE_4);
					break;
			}//end of switch
	deactivate_left_ramp_quickfreeze();//goto ramps.c
	light_quick_freeze_light_off();//goto inlanes.c
	if (!flag_test(FLAG_BACK_IN_THE_FRIDGE_RUNNING) ) check_multiball_requirements();
}//end of function




void check_multiball_requirements(void) {
	//if requirements met for next mb - light left loop shot to start mb
	//Fortress Multiball 	= 1 ball min needs to be frozen
	//Museum Multiball	 	= 2 ball min needs to be frozen
	//Cryoprison Multiball	= 3 ball min needs to be frozen
	//Wasteland Multiball 	= 4 ball min needs to be frozen
	if (NumBallsFrozen > (NumMBsDone % 4) ) { // % is modulus
				multiball_light_on();//goto orbits.c
				flag_on(FLAG_IS_MULTIBALL_ENABLED);
				if ( (lock_SoundCounter++ % 2) == 0 )//check if even
					sound_start (ST_SPEECH, SPCH_MULTIBALL_ACTIVATED, SL_4S, PRI_GAME_QUICK5);
				else
				sound_start (ST_SPEECH, SPCH_SHOOT_LEFT_LOOP, SL_4S, PRI_GAME_QUICK5);
	}  //end of if (NumBallsFrozen > (NumMBsDone % 4) )

	leff_start(LEFF_FREEZE);
	deff_start (DEFF_FREEZE_EFFECT);
}//end of function



//this is called from left loop shot at orbits.c
void multiball_start (void) {
	if 		( (NumMBsDone % 4) == 0) 	fortress_start(NumBallsFrozen);
	else if ( (NumMBsDone % 4) == 1) 	museum_start(NumBallsFrozen);
	else if ( (NumMBsDone % 4) == 2) 	wasteland_start(NumBallsFrozen);
	else if ( (NumMBsDone % 4) == 3) 	cryoprison_start(NumBallsFrozen);
}//end of function



//after we are sure we have a valid MB start, we reset everything for next time
void multiball_started(void) {
	NumMBsDone++;
	if (NumBallsNeededForNextMB < 4) ++NumBallsNeededForNextMB;
	flag_off(FLAG_IS_MULTIBALL_ENABLED);
	NumBallsFrozen = 0;
	lock_reset();
}//end of function






/****************************************************************************
 *
 * lighting effects
 *
 ****************************************************************************/
void freeze_leff(void) {
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
void freeze_effect_deff(void) {
	U16 fno;
	dmd_alloc_pair_clean ();
	if (flag_test(FLAG_IS_MULTIBALL_ENABLED) ) {
		for (fno = IMG_CLAW_A_START; fno <= IMG_CLAW_A_END; fno += 2) {
			dmd_alloc_pair_clean ();
			frame_draw(fno);
			dmd_show2 ();
			task_sleep (TIME_100MS);
		}//end of inner loop

		for (fno = IMG_CLAW_B_START; fno <= IMG_CLAW_B_END; fno += 2) {
			dmd_alloc_pair_clean ();
			frame_draw(fno);
			dmd_show2 ();
			task_sleep (TIME_100MS);
		}//end of inner loop

		dmd_alloc_pair_clean ();// Clean both pages
			dmd_map_overlay ();
			dmd_clean_page_low ();
			font_render_string_center (&font_term6, DMD_MIDDLE_X - 30, DMD_BIG_CY_Top, "MULTIBALL");
			font_render_string_center (&font_term6, DMD_MIDDLE_X - 15, DMD_BIG_CY_Bot, "READY");
			dmd_text_outline ();
			dmd_alloc_pair ();
			frame_draw(IMG_CLAW_B_END);
			dmd_overlay_outline ();
			dmd_show2 ();
		}
	else
	switch (++freeze_MessageCounter % 2) {
		default:
		case 0:
			for (fno = IMG_FREEZE_A_START; fno <= IMG_FREEZE_A_END; fno += 2) {
				dmd_alloc_pair_clean ();
				frame_draw(fno);
				dmd_show2 ();
				task_sleep (TIME_100MS);
			}//end of inner loop
			dmd_alloc_pair_clean ();// Clean both pages
				dmd_map_overlay ();
				dmd_clean_page_low ();
				font_render_string_center (&font_term6, DMD_MIDDLE_X - 30, DMD_BIG_CY_Top, "FREEZE");
				dmd_text_outline ();
				dmd_alloc_pair ();
				frame_draw(IMG_FREEZE_A_END);
				dmd_overlay_outline ();
				dmd_show2 ();
			break;
		case 1:
			for (fno = IMG_WASTELAND_B1_START; fno <= IMG_WASTELAND_B1_END; fno += 2) {
				dmd_alloc_pair_clean ();
				frame_draw(fno);
				dmd_show2 ();
				task_sleep (TIME_100MS);
			}//end of inner loop
			for (fno = IMG_WASTELAND_B2_START; fno <= IMG_WASTELAND_B2_END; fno += 2) {
				dmd_alloc_pair_clean ();
				frame_draw(fno);
				dmd_show2 ();
				task_sleep (TIME_100MS);
			}//end of inner loop
			dmd_alloc_pair_clean ();// Clean both pages
				dmd_map_overlay ();
				dmd_clean_page_low ();
				font_render_string_center (&font_term6, DMD_MIDDLE_X - 30, DMD_BIG_CY_Top, "FREEZE");
				dmd_text_outline ();
				dmd_alloc_pair ();
				frame_draw(IMG_WASTELAND_B2_END);
				dmd_overlay_outline ();
				dmd_show2 ();
			break;
		}//end of switch
	task_sleep_sec (1);
	deff_exit ();
}//end of mode_effect_deff

