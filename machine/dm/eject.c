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
/* CALLSET_SECTION (eject, __machine3__) */

#include <freewpc.h>
#include "dm/global_constants.h"

//local variables
U8 eject_killer_counter;
U8 retina_scan_multiplier;
U8 ejectDeffCounter;


//internally called function prototypes  --external found at protos.h
void eject_reset (void);
void player_eject_reset (void);
void eject_killer_task (void);



/****************************************************************************
 * initialize  and exit
 ***************************************************************************/
void eject_reset (void) {
	retina_scan_multiplier = 1;
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
	task_sleep (TIME_4S);
	eject_killer_counter = 0;
}//end of function



CALLSET_ENTRY (eject, sw_eject) {
	if ( task_kill_gid(GID_LEFT_INLANE_MADE) ) huxley_made();
	else {
		if (eject_killer_counter++ == 1) {
				//NORMAL RETINA SCAN
				sound_start (ST_SAMPLE, RETINA_SCAN_LONG, SL_4S, PRI_GAME_QUICK1);
				U8 i;
				for (i = 1; i <= retina_scan_multiplier; i++)	score (EJECT_SCORE);

				deff_start (DEFF_EJECT_EFFECT);

				lamp_tristate_flash(LM_RETINA_SCAN);
				task_sleep (TIME_4S);
				lamp_tristate_off(LM_RETINA_SCAN);
				flasher_pulse (FLASH_EJECT_FLASHER);
				flasher_pulse (FLASH_EJECT_FLASHER);
				flasher_pulse (FLASH_EJECT_FLASHER);
				flasher_pulse (FLASH_EJECT_FLASHER);

				sol_request (SOL_EJECT); //request to fire the eject sol
			}//end of if
	}//end of else
	eject_killer_task();
}//end of function



//called from comp award at underground.c
void comp_award_doub_retina(void) {
	retina_scan_multiplier = 2;
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
							task_sleep (TIME_100MS);
						}//end of inner loop

						for (fno = IMG_EYE_B_END; fno >= IMG_EYE_B_2; fno -= 2) {
						dmd_map_overlay ();
						dmd_clean_page_low ();
						font_render_string_center (&font_fireball, DMD_MIDDLE_X, DMD_BIG_CY_Top, "RETINA");
						font_render_string_center (&font_fireball, DMD_MIDDLE_X, DMD_BIG_CY_Bot, "SCAN");
						dmd_text_outline ();
						dmd_alloc_pair ();
						frame_draw(fno);
						dmd_overlay_outline ();
						dmd_show2 ();
						task_sleep (TIME_100MS);
					}//end of inner loop
			break;
		case 1:
					for (fno = IMG_EYE_START; fno <= IMG_EYE_27; fno += 2) {
						dmd_alloc_pair ();
						frame_draw(fno);
						dmd_show2 ();
						task_sleep (TIME_100MS);
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
					task_sleep (TIME_100MS);
				}//end of inner loop
			break;
		}//end of switch
	task_sleep_sec (2);
	deff_exit ();
}//end of mode_effect_deff



