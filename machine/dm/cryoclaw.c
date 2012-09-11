/*
 * demolition man
 * cryoclaw.c
 *
 * written by James Cardona
 *
 * Location Description:
 *
 *
 */
#include <freewpc.h>
#include "dm/global_constants.h"

//local variables

//prototypes

/****************************************************************************
 * initialize  and exit
 ***************************************************************************/
CALLSET_ENTRY (cryoclaw, start_ball) {
		elevator_move_down();
}//end of function


//void player_cryoclaw_reset (void) {
//}//end of function

//ALLSET_ENTRY (cryoclaw, start_player) 	{ player_cryoclaw_reset(); }


/****************************************************************************
 * elevator
 *
 * this switch is an opto and is triggered by the elevator
 * when it is up and the ball, when the elevator is down
 ****************************************************************************/
CALLSET_ENTRY (cryoclaw, sw_elevator_hold) {
	if (valid_playfield) {

	//sound_start (ST_SAMPLE, RETINA_SCAN_LONG, SL_4S, PRI_GAME_QUICK1);
		task_sleep (TIME_500MS);
		//turn on magnet
		//sol_enable
		sol_request (SOL_CLAW_MAGNET);
		elevator_move_up();

		elevator_move_down();
	}//end of if valid playfield
}//end of function



