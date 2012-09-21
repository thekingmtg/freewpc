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
#include "elevator.h"
#include "cryoclawdrive.h"

//local variables
__boolean	ballOnClaw;

//prototypes
void magnet_task (void);

/****************************************************************************
 * initialize  and exit
 ***************************************************************************/
CALLSET_ENTRY (cryoclaw, start_ball) {
	ballOnClaw = FALSE;
}//end of function

/****************************************************************************
 ****************************************************************************/
CALLSET_ENTRY (cryoclaw, sw_elevator_hold) {
	if (valid_playfield) {
		flipper_disable ();
		task_sleep(TIME_2S);
//		clawmagnet_on ();							//turn on magnet
		task_create_gid1 (GID_MAGNET, magnet_task); //start 10 second timer
		elevator_move_bump();						//move elevator up off bottom limit switch
		elevator_move_bump();
		elevator_move();							//then up and back down back to home
		ballOnClaw = TRUE;							//ball "should" be on magnet
		task_sleep(TIME_2S);
		cryoclawdrive_go_left();  			//move claw to other side
		callset_invoke(rramp_clawready_off);		//turn off diverter
	}//end of if valid playfield
}//end of function



//turn off magnet if not used in XX secs
void magnet_task (void) {
	task_sleep_sec(14);
//	clawmagnet_off ();
	flipper_enable ();
	task_exit();
}//end of function



//release ball
CALLSET_ENTRY (cryoclaw, sw_left_handle_button, sw_launch_button) {
	if (ballOnClaw) {
//		clawmagnet_off ();					//turn off magnet
		ballOnClaw = FALSE;
		flipper_enable ();
		task_sleep(TIME_2S);
		cryoclawdrive_go_right();		//move claw back to home position
	}//end of if
}//end of function




//move claw left
CALLSET_ENTRY (cryoclaw, sw_left_button, sw_upper_left_button) {
	if (	ballOnClaw
		 &&	!switch_poll_logical (SW_CLAW_POSITION_1)
		 &&	!cryoclawdrive_get_location() == CRYOCLAWDRIVE_LEFT) {
		cryoclawdrive_bump_left ();
	}//end of if
}//end of function



//move claw right
CALLSET_ENTRY (cryoclaw, sw_right_button, sw_upper_right_button) {
	if (	ballOnClaw
		&& 	!switch_poll_logical (SW_CLAW_POSITION_2)
		&&	!cryoclawdrive_get_location() == CRYOCLAWDRIVE_RIGHT) {
		cryoclawdrive_bump_right ();
	}//end of if
}//end of function



//ensure claw is all the way to the right
CALLSET_ENTRY (cryoclaw, amode_start, player_start) {
//	clawmagnet_off ();					//turn off magnet
	flipper_enable ();
	task_sleep(TIME_2S);
	cryoclawdrive_go_left ();
	task_sleep(TIME_2S);
	cryoclawdrive_go_right();
}//end of function


