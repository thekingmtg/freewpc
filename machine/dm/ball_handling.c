/*
 * demolition man
 * ball_handling.c
 *
 * written by James Cardona
 *
 * */
#include <freewpc.h>
#include "dm/global_constants.h"

//constants

//local variables

//external variables

//prototypes
void dm_launch_ball_task (void);



/****************************************************************************
 * firing of ball work around
 * see plunger.c and serve.c - code doesn't work for me
 *
 * what we know so far
 * the following are defined properly in the md and show up in the game logic:
 * MACHINE_LAUNCH_SWITCH
 * MACHINE_LAUNCH_SOLENOID
 * MACHINE_SHOOTER_SWITCH
 *
 * the following are supposed to be defined in plunger.c and serve.c
 * but they do not show up in the game logic:
 * INCLUDE_AUTOPLUNGER
 * HAVE_AUTO_SERVE
 *
 * So then I have defined them outright in the md as a temporary work around
 *
 * finally, the serve code in serve.c looks for the ball to be in the shooter lane
 * before it will fire the auto plunger sol, by checking a global flag
 * GLOBAL_FLAG_BALL_AT_PLUNGER
 *
 * another problem is that the ball autofires on the first ball only
 * - autofire should work only for ballsaves, multiballs or the case where
 * the playfield ball somehow falls back into the shooter lane
 *
 *
 *
 ***************************************************************************/
CALLSET_ENTRY (ball_handling, sw_ball_launch_button) {
	//copied from plunger.c and serve.c
	if (in_live_game)
		task_create_gid1 (GID_BALL_LAUNCH_BALL, dm_launch_ball_task);
}


void dm_launch_ball_task (void) {
	if (!switch_poll_logical (MACHINE_SHOOTER_SWITCH))
		task_sleep (TIME_500MS);
	do {
		sol_request_async (MACHINE_LAUNCH_SOLENOID);
		task_sleep (TIME_3S);
	} while (switch_poll_logical (MACHINE_SHOOTER_SWITCH));
	task_exit ();
}//end of function
