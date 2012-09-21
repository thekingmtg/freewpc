/*
 */

#include <freewpc.h>
extern __fastram__ enum magnet_state {
	MAG_DISABLED,
	MAG_ENABLED,
	MAG_ON_POWER,
	MAG_ON_HOLD,
	MAG_THROW_DROP,
} claw_magnet_state;

extern U8 claw_magnet_hold_timer;

//extern struct timed_mode_ops spiral_mode;

__fastram__ bool magnet_enabled;

/* Check to see if a magnet is enabled. */
bool magnet_enabled (U8 magnet) {
	enum magnet_state *magstates = (enum magnet_state *)&claw_magnet_state;
	if (magstates[magnet] != MAG_DISABLED) 	return TRUE;
	else									return FALSE;
}//end of function

/* Check whether the magnet is busy holding/grabbing/throwing but NOT enabled */
bool magnet_busy (U8 magnet) {
	enum magnet_state *magstates = (enum magnet_state *)&claw_magnet_state;
	if (magstates[magnet] 	== MAG_DISABLED
		|| magstates[magnet] == MAG_ENABLED)
			return FALSE;
	else	return TRUE;
}//end of function

void magnet_enable_monitor_task (void) {
	while (magnet_enabled) {
/*		// Left Magnet grabs
		if (magnet_busy (MAG_LEFT) || task_find_gid (GID_LEFT_BALL_GRABBED))
			;// Do nothing, magnet is busy
		// Enable catch from an ballsave death
		else if (task_find_gid (GID_BALL_LAUNCH_DEATH))
		{
			magnet_enable_catch_and_throw (MAG_LEFT);
		}
		// Enable catch for Piano jackpot Shot
		else if (global_flag_test (GLOBAL_FLAG_MB_JACKPOT_LIT))
		{
			magnet_enable_catch (MAG_LEFT);
		}
		task_sleep (TIME_200MS);
*/	}//end of while
	task_exit ();
}//end of function

/* Check to see if the ball is still being held */
static void monitor_grab_task (void) {
	callset_invoke (ball_grabbed);
	task_sleep (TIME_500MS);
	task_exit ();
}//end of function


/* Check to see if we have successfully grabbed a ball */
void magnet_ball_grab_monitor_rtt (void) {
	if (magnet_enabled) {
		enum magnet_state *magstates = (enum magnet_state *)&claw_magnet_state;
		if ((magstates[MAG] == MAG_ON_HOLD)
			&& !task_find_gid (GID_BALL_GRABBED))
		{
			task_recreate_gid (GID_BALL_GRABBED, monitor_grab_task);
		}
	}//end of if magnet enabled
}//end of function



CALLSET_ENTRY (maghelper, start_ball) {
	magnet_reset ();
	magnet_enabled = TRUE;
	task_recreate_gid (GID_MAGNET_ENABLE_MONITOR, magnet_enable_monitor_task);
}//end of function



CALLSET_ENTRY (maghelper, ball_search) {
/*	if (switch_poll_logical (SW_LOWER_RIGHT_MAGNET))
		magnet_enable_catch_and_throw (MAG_RIGHT);	
	if (switch_poll_logical (SW_LEFT_MAGNET))
		magnet_enable_catch_and_throw (MAG_LEFT);	
*/
}//end of function



CALLSET_ENTRY (maghelper, end_ball) {
	magnet_reset ();
	magnet_enabled = FALSE;
}//end of function
