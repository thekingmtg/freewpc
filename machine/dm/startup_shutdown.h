/*
 * demolition man
 * startup_shutdown.h
 *
 * written by James Cardona
 *
 */
#include <freewpc.h>
#include "dm/global_constants.h"



void kill_all_flashers (void){
	//turn off all flashers
	sol_stop (FLASH_CLAW_FLASHER);
	sol_stop (FLASH_JETS_FLASHER);
	sol_stop (FLASH_SIDE_RAMP_FLASHER);
	sol_stop (FLASH_LEFT_RAMP_UP_FLASHER);
	sol_stop (FLASH_LEFT_RAMP_LOWER_FLASHER);
	sol_stop (FLASH_CAR_CHASE_CENTER_FLASHER);
	sol_stop (FLASH_CAR_CHASE_LOWER_FLASHER);
	sol_stop (FLASH_RIGHT_RAMP_FLASHER);
	sol_stop (FLASH_EJECT_FLASHER);
	sol_stop (FLASH_CAR_CHASE_UPPER_FLASHER);
	sol_stop (FLASH_LOWER_REBOUND_FLASHER);
	sol_stop (FLASH_EYEBALL_FLASHER);
	sol_stop (FLASH_CENTER_RAMP_FLASHER);
	sol_stop (FLASH_ELEVATOR_2_FLASHER);
	sol_stop (FLASH_ELEVATOR_1_FLASHER);
	sol_stop (FLASH_DIVERTER_FLASHER);
	sol_stop (FLASH_RIGHT_RAMP_UP_FLASHER);
}//end of function



void flash_all_flashers (void){
	flasher_pulse_short (FLASH_CLAW_FLASHER);
	flasher_pulse_short (FLASH_JETS_FLASHER);
	flasher_pulse_short (FLASH_SIDE_RAMP_FLASHER);
	flasher_pulse_short (FLASH_LEFT_RAMP_UP_FLASHER);
	flasher_pulse_short (FLASH_LEFT_RAMP_LOWER_FLASHER);
	flasher_pulse_short (FLASH_CAR_CHASE_CENTER_FLASHER);
	flasher_pulse_short (FLASH_CAR_CHASE_LOWER_FLASHER);
	flasher_pulse_short (FLASH_RIGHT_RAMP_FLASHER);
	flasher_pulse_short (FLASH_EJECT_FLASHER);
	flasher_pulse_short (FLASH_CAR_CHASE_UPPER_FLASHER);
	flasher_pulse_short (FLASH_LOWER_REBOUND_FLASHER);
	flasher_pulse_short (FLASH_EYEBALL_FLASHER);
	flasher_pulse_short (FLASH_CENTER_RAMP_FLASHER);
	flasher_pulse_short (FLASH_ELEVATOR_2_FLASHER);
	flasher_pulse_short (FLASH_ELEVATOR_1_FLASHER);
	flasher_pulse_short (FLASH_DIVERTER_FLASHER);
	flasher_pulse_short (FLASH_RIGHT_RAMP_UP_FLASHER);
}//end of function



void kill_all_sols (void){
	//turn off all solenoids
		sol_stop (SOL_BALLSERVE);
		sol_stop (SOL_BOTTOM_POPPER);
		sol_stop (SOL_LAUNCH);
		sol_stop (SOL_TOP_POPPER);
		sol_stop (SOL_DIVERTER_POWER);
		sol_stop (SOL_KNOCKER);
		sol_stop (SOL_LEFT_SLING);
		sol_stop (SOL_RIGHT_SLING);
		sol_stop (SOL_LEFT_JET);
		sol_stop (SOL_TOP_SLING);
		sol_stop (SOL_RIGHT_JET);
		sol_stop (SOL_EJECT);
		sol_stop (SOL_DIVERTER_HOLD);
		sol_stop (SOL_CLAW_MAGNET);
}//end of function



void kill_all_flippers (void){
	//turn off all flippers
	sol_stop (SOL_LR_FLIP_POWER);
	sol_stop (SOL_LR_FLIP_HOLD);
	sol_stop (SOL_LL_FLIP_POWER);
	sol_stop (SOL_LL_FLIP_HOLD);
	sol_stop (SOL_UL_FLIP_POWER);
	sol_stop (SOL_UL_FLIP_HOLD);
}//end of function
