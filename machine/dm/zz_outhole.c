/*
 */

/* CALLSET_SECTION (outhole, __machine2__) */

#include <freewpc.h>
/* How many balls have drained in three seconds */
U8 multidrain_count;
bool multidrain_awarded;
bool powerball_death;

CALLSET_ENTRY (outhole, ball_search)
{
	while (switch_poll_logical (SW_OUTHOLE))
	{
		sol_request (SOL_OUTHOLE);
		task_sleep_sec (1);
	}
}

CALLSET_ENTRY (outhole, single_ball_play)
{
	if (!timer_find_gid (GID_MULTIDRAIN))
		multidrain_awarded = FALSE;
}

CALLSET_ENTRY (outhole, sw_outhole)
{
	if (in_live_game)
	{
		if (ballsave_test_active ())
		{
			/* Start a timer so we grab the ball after its
			 * fired to help the player */
			timer_restart_free (GID_BALL_LAUNCH_DEATH, TIME_6S);
		}

		
		/* Start a timer to check if 3 balls drain quickly */
		if (!timer_find_gid (GID_MULTIDRAIN) && !ballsave_test_active ())
		{
			multidrain_count = 0;
			if (multi_ball_play ())
				timer_restart_free (GID_MULTIDRAIN, TIME_7S);
		}
	
		if (timer_find_gid (GID_MULTIDRAIN))
		{
			/* There are 6 balls installed normally */
			bounded_increment (multidrain_count, 6);
			if (multidrain_count >= 3)
				multidrain_awarded = TRUE;
		}
		
		/* Whoops, lost the powerball before getting it in the gumball */
		if (!multi_ball_play () &&
			 global_flag_test (GLOBAL_FLAG_POWERBALL_IN_PLAY) &&
			 !ballsave_test_active ())
		{
			sound_send (SND_NOOOOOOOO);
			powerball_death = TRUE;
			task_sleep (TIME_500MS);
		}
		deff_start (DEFF_BALL_EXPLODE);
		leff_start (LEFF_STROBE_UP);
	}
	
}

CALLSET_ENTRY (outhole, valid_playfield)
{
	powerball_death = FALSE;
}

CALLSET_ENTRY (outhole, serve_ball)
{
	powerball_death = FALSE;
	multidrain_count = 0;
	multidrain_awarded = FALSE;
	timer_kill_gid (GID_MULTIDRAIN);
}
