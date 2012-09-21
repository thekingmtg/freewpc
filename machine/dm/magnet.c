/*
 */
#include <freewpc.h>

//constants
#define MAG_SWITCH_RTT_FREQ 4 /* Magnet switch RTT runs every 4 ms */
#define MAG_DRIVE_RTT_FREQ 4
#define MAG_POWER_TIME (200 / MAG_DRIVE_RTT_FREQ)
#define DEFAULT_MAG_HOLD_TIME (200 / MAG_DRIVE_RTT_FREQ)

//local variables
__fastram__ U8 claw_magnet_timer;
__fastram__ U8 claw_magnet_hold_timer;
__fastram__ bool is_magnet_enabled;
__fastram__ enum magnet_state {
	MAG_DISABLED,
	MAG_ENABLED,
	MAG_ON_POWER,
	MAG_ON_HOLD,
	MAG_THROW_DROP,
} claw_magnet_state;

//external variables

//prototypes


/****************************************************************************
 * calls
 ***************************************************************************/
CALLSET_ENTRY (magnet, start_ball, single_ball_play, init) { magnet_reset (); 	is_magnet_enabled = TRUE; }
CALLSET_ENTRY (maghelper, ball_search) { }//end of function

CALLSET_ENTRY (maghelper, end_ball) {
	magnet_reset ();
	magnet_enabled = FALSE;
}//end of function



/****************************************************************************
 * body
 ***************************************************************************/
static inline void set_mag_hold_time (U8 magnet, U8 holdtime) { left_magnet_hold_timer = holdtime; }



/** The magnet switch handler is a frequently called function
 * that polls to see the magnet should be turned on.*/
static inline void magnet_rtt_switch_handler (
	const U8 sw_magnet,
	const U8 sol_magnet,
	enum magnet_state *state,
	U8 *power_timer ) {
	/* rt_switch_poll is inverted because it is an opto */
	if ((*state == MAG_ENABLED) &&
		 (!rt_switch_poll (sw_magnet)))
	{
		sol_enable (sol_magnet);
		*state = MAG_ON_POWER;
		*power_timer = MAG_POWER_TIME;
	}//end of if
}//end of function


/** The magnet duty handler is a less-frequently called
 * function that turns on/off the magnet as necessary.
 * When a ball is being held, it uses duty cycling to avoid
 * burnout. */
static inline void magnet_rtt_duty_handler (
	const U8 sw_magnet,
	const U8 sol_magnet,
	enum magnet_state *state,
	U8 *power_timer,
	U8 *hold_timer,
	bool *throw_enabled) {
	switch (*state) {
		case MAG_DISABLED:
		case MAG_ENABLED:
			sol_disable (sol_magnet);
			break;
		case MAG_ON_POWER:
			/* keep magnet on with high power */
			/* switch to MAG_ON_HOLD fairly quickly though */
			/* But leave solenoid enabled so it doesn't suffer 
			 * any drop */
			if (*power_timer == 0)
			{	
				if (rt_switch_poll (sw_magnet))
				{
					/* Grab failed */
					*throw_enabled = FALSE;
					*state = MAG_DISABLED;
				}
				else
				{
					/* switch to HOLD */
					*state = MAG_ON_HOLD;
				}
			}
			--*power_timer;
			break;

		case MAG_ON_HOLD:
			/* keep magnet on with low power */
			/* switch should remain closed in this state */
			if (*hold_timer == 0)
			{
				if (*throw_enabled == TRUE)
				{
					*throw_enabled = FALSE;
					switch (sol_magnet)
					{
						case SOL_RIGHT_MAGNET:
							*hold_timer = DEFAULT_MAG_DROP_TIME_RIGHT;
							break;
						case SOL_LEFT_MAGNET:
							*hold_timer = DEFAULT_MAG_DROP_TIME_LEFT;
							break;
					}
					/* switch to THROW_DROP */
					sol_disable (sol_magnet);
					*state = MAG_THROW_DROP;
				}
				else
				{
					/* switch to DISABLED */
					sol_disable (sol_magnet);
					*state = MAG_DISABLED;
				}
			}
			/* Hold the ball at 50% duty or 100% if held
			 * for throw (to minimize rattle */
			else if ((*hold_timer % 2) != 0)
			{
				sol_enable (sol_magnet);
			}
			else if (*throw_enabled == TRUE)
			{
				sol_enable (sol_magnet);
			}
			else
			{
				sol_disable (sol_magnet);
			}
			/* This has to go here for some reason */
			--*hold_timer;
			break;
	}//end of switch
}//end of function


/* Realtime function to poll the magnet switches. */
void magnet_switch_rtt (void) {
	magnet_rtt_switch_handler (SW_LEFT_MAGNET, SOL_LEFT_MAGNET,
		&left_magnet_state, &left_magnet_timer);
}//end of function



/* Realtime function to duty cycle the magnet drives */
inline void magnet_duty_rtt (void) {
	magnet_rtt_duty_handler (SW_LEFT_MAGNET, SOL_LEFT_MAGNET, 
		&left_magnet_state, &left_magnet_timer, &left_magnet_hold_timer, &left_magnet_enabled_to_throw);
}//end of function






void magnet_enable_catch (U8 magnet)
{
	enum magnet_state *magstates = (enum magnet_state *)&left_magnet_state;
	set_mag_hold_time (magnet, DEFAULT_MAG_HOLD_TIME);	
	magstates[magnet] = MAG_ENABLED;
}



void magnet_enable_catch_and_hold (U8 magnet, U8 secs)
{
	enum magnet_state *magstates = (enum magnet_state *)&left_magnet_state;
	
	/* Limit to 4 secs as we run at 16ms ticks in a U8 timer */
	if (secs > 2)
		secs = 2;
	set_mag_hold_time (magnet, (secs * (1000UL / MAG_DRIVE_RTT_FREQ)));
	magstates[magnet] = MAG_ENABLED;
}



void magnet_enable_catch_and_throw (U8 magnet)
{
	enum magnet_state *magstates = (enum magnet_state *)&left_magnet_state;
	/* Hold the ball for 1 sec@100% before we throw to stabilise it*/
	set_mag_hold_time (magnet, (32  / MAG_DRIVE_RTT_FREQ));
	enable_magnet_throw (magnet);
	magstates[magnet] = MAG_ENABLED;
}



void magnet_disable_catch (U8 magnet)
{
	enum magnet_state *magstates = (enum magnet_state *)&left_magnet_state;
	/* Don't allow disable if the magnet is currently holding/grabbing 
	 * it will switch to disabled after it's done anyway */
	if (!magnet_busy (magnet))
	magstates[magnet] = MAG_DISABLED;
}

void magnet_reset (void)
{
	left_magnet_state = lower_right_magnet_state = MAG_DISABLED;
	left_magnet_timer = lower_right_magnet_timer = 0;
	left_magnet_hold_timer = lower_right_magnet_hold_timer = 0;
	left_magnet_enabled_to_throw = lower_right_magnet_enabled_to_throw = FALSE;
}






/* Check to see if a magnet is enabled. */
bool is_magnet_enabled (U8 magnet) {
	enum magnet_state *magstates = (enum magnet_state *)&claw_magnet_state;
	if (magstates[magnet] != MAG_DISABLED) 	return TRUE;
	else									return FALSE;
}//end of function



/* Check whether the magnet is busy holding/grabbing/throwing */
bool magnet_busy (U8 magnet) {
	enum magnet_state *magstates = (enum magnet_state *)&claw_magnet_state;
	if (magstates[magnet] 	== MAG_DISABLED
		|| magstates[magnet] == MAG_ENABLED)
			return FALSE;
	else	return TRUE;
}//end of function



/* Check to see if the ball is still being held */
static void monitor_grab_task (void) {
	callset_invoke (ball_grabbed);
	task_sleep (TIME_500MS);
	task_exit ();
}//end of function



/* Check to see if we have successfully grabbed a ball */
void magnet_ball_grab_monitor_rtt (void) {
	if (is_magnet_enabled) {
		enum magnet_state *magstates = (enum magnet_state *)&claw_magnet_state;
		if ((magstates[MAG] == MAG_ON_HOLD)
			&& !task_find_gid (GID_BALL_GRABBED))
		{
			task_recreate_gid (GID_BALL_GRABBED, monitor_grab_task);
		}
	}//end of if magnet enabled
}//end of function



