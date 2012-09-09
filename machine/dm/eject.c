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

//prototypes
void eject_killer_task (void);
void eyeball_effect_deff(void);

/****************************************************************************
 * initialize  and exit
 ***************************************************************************/
void player_eject_reset (void) {
	eject_killer_counter = 0;
}//end of function

CALLSET_ENTRY (eject, start_player) 	{ player_eject_reset(); }


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
		score (SC_5M);
		callset_invoke(start_eject_deff);
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

