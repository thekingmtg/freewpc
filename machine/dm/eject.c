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
 *
 */
#include <freewpc.h>
#include "dm/global_constants.h"

//local variables
U8 eject_killer_counter;
U8 retina_scan_multiplier;

//prototypes
void eject_reset (void);
void player_eject_reset (void);
void eject_killer_task (void);
void eyeball_effect_deff(void);

/****************************************************************************
 * initialize  and exit
 ***************************************************************************/
void eject_reset (void) {
	retina_scan_multiplier = 1;
}//end of function


void player_eject_reset (void) {
	eject_killer_counter = 0;
	eject_reset ();
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
	if (eject_killer_counter++ == 1) {
		sound_start (ST_SAMPLE, RETINA_SCAN_LONG, SL_4S, PRI_GAME_QUICK1);
		U8 i;
		for (i = 1; i <= retina_scan_multiplier; i++){
			score (SC_5M);
		}//end of loop
		deff_start (DEFF_EJECT_EFFECT);
		lamp_tristate_flash(LM_RETINA_SCAN);
		task_sleep (TIME_500MS);
		task_sleep (TIME_500MS);
		task_sleep (TIME_500MS);
		task_sleep (TIME_500MS);
		lamp_tristate_off(LM_RETINA_SCAN);
		flasher_pulse (FLASH_EJECT_FLASHER);
		flasher_pulse (FLASH_EJECT_FLASHER);
		flasher_pulse (FLASH_EJECT_FLASHER);
		flasher_pulse (FLASH_EJECT_FLASHER);
		sol_request (SOL_EJECT); //request to fire the eject sol
	}//end of if
	eject_killer_task();
}//end of function


//called from comp award at underground.c
CALLSET_ENTRY (eject, comp_award_doub_retina) {
	retina_scan_multiplier = 2;
}//end of function

/****************************************************************************
 *
 * DISPLAY EFFECTS
 *
 ****************************************************************************/
void eject_effect_deff(void) {
	dmd_alloc_low_clean ();
	font_render_string_center (&font_term6, DMD_MIDDLE_X, DMD_BIG_CY_Top, "RETINA");
	font_render_string_center (&font_term6, DMD_MIDDLE_X, DMD_BIG_CY_Bot, "SCAN");
	dmd_show_low ();
	task_sleep_sec (2);
	deff_exit ();
}//end of mode_effect_deff



