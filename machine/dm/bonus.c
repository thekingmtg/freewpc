/*
 * bonus.c
 */
/* CALLSET_SECTION (bonus, __machine3__) */
#include <freewpc.h>


//constants

//local variables
score_t total_bonus;	/* Total bonus score */
score_t bonus_scored;	/* Temp variable used to calculate bonus per item etc */
bool buttons_held;

//external variables
/*
extern U8 		car_chase_mode_shots_made;			//from car_chase.c
extern U8 		car_chase_modes_achieved;
extern U8 		car_chase_modes_completed;
extern score_t 	car_chase_mode_score_total_score;

extern U8 		capture_simon_mode_shots_made;		//from capture_simon.c
extern U8 		capture_simon_modes_achieved;
extern U8 		capture_simon_modes_completed;
extern score_t 	capture_simon_mode_score_total_score;

extern U8 		prison_break_mode_shots_made;		//from prison_break.c
extern U8 		prison_break_modes_achieved;
extern U8 		prison_break_modes_completed;
extern score_t 	prison_break_mode_score_total_score;

extern U8 		acmag_mode_shots_made;				//from acmag.c
extern U8 		acmag_modes_achieved;
extern U8 		acmag_modes_completed;
extern score_t 	acmag_mode_score_total_score;

extern U8 		explode_mode_shots_made;				//from acmag.c
extern score_t 	explode_mode_score;

extern U8 		jet_shots_made;
extern U8 		superjets_modes_achieved;
extern U8 		super_jet_shots_made;

extern U8		standupFrenzy_modes_achieved;
extern U8 		standupFrenzyNumHits;
extern score_t 	standupFrenzyTotalScore;
extern U8 		standup_num_of_hits;

extern U8 		rollover_bonus_multiplier; 			//from rollovers.c
extern U8 		total_eyeball_shots_made; 			//from eyeball.c
extern U8		computerAwards;						//from underground.c
extern U8 		combo_counter;
extern U8		fortress_jackpot_shots_made;
*/

extern U8 		car_crash_shots_made; 				//from car_crash.c

//prototypes
static void bonus_button_monitor (void);
static void bonus_sched_transition (void);
static void bonus_pause (void);



void bonus_deff (void) {
	/* 
	 * Taunts.....
	 * */

	task_sleep (TIME_100MS);	/* Wait a bit so the previous music_stop doesn't kill the taunt sounds */
	score_zero (total_bonus);	/* Clear the bonus score */
	//	sample_start (MUS_FADE_BONUS, SL_500MS);
	sound_send (BONUS_SHORT);
	/* Show Initial bonus screen */
	dmd_alloc_low_clean ();
	font_render_string_center (&font_times8, 64, 16, "BONUS");
	dmd_sched_transition (&trans_bitfade_fast);
	dmd_show_low ();
	task_sleep_sec (1);

	task_recreate_gid (GID_BONUS_BUTTON_MONITOR, bonus_button_monitor); /* Start a task to monitor the buttons */
	/***car crash scoring and display ***/
	bonus_sched_transition ();
	if (car_crash_shots_made > 0) {
			dmd_alloc_low_clean ();
			score_zero (bonus_scored);
			score_add (bonus_scored, score_table[SC_100K]);
			score_mul (bonus_scored, car_crash_shots_made);
			score_add (total_bonus, bonus_scored);

			sprintf_score (bonus_scored);
			font_render_string_center (&font_fixed10, 64, 16, sprintf_buffer);
			sprintf ("CAR CRASH");
			font_render_string_center (&font_mono5, 64, 4, sprintf_buffer);
			bonus_sched_transition ();
			dmd_show_low ();
			sound_send (BONUS_SHORT);
			bonus_pause ();
	}/***end of car crash scoring and display ***/



	/***car chase scoring and display ***/
/*	bonus_sched_transition ();
	if (car_chase_modes_achieved > 0) {
			dmd_alloc_low_clean ();
			score_zero (bonus_scored);
			score_add (bonus_scored, score_table[SC_100K]);
			score_mul (bonus_scored, car_chase_mode_shots_made);
			score_add (total_bonus, bonus_scored);

			score_zero (bonus_scored);
			score_add (bonus_scored, score_table[SC_1M]);
			score_mul (bonus_scored, car_chase_modes_achieved);
			score_add (total_bonus, bonus_scored);

			sprintf_score (bonus_scored);
			font_render_string_center (&font_fixed10, 64, 16, sprintf_buffer);
			sprintf ("CAR CHASE");
			font_render_string_center (&font_mono5, 64, 4, sprintf_buffer);
			bonus_sched_transition ();
			dmd_show_low ();
			sound_send (BONUS_SHORT);
			bonus_pause ();
	}//end of car chase scoring and display
*/

		
	
	score_long (total_bonus); // Add to total bonus to player score

/*
	if (check_for_puny_score ())
			sound_send (SND_BUYIN_CANCELLED);
		else if (check_for_big_score ())
			task_sleep_sec (2);
		else
			sound_send (SND_GREED_MODE_BOOM);
		
		task_sleep_sec (2);
	}
*/
		
	/* Show final score */
	dmd_alloc_low_clean ();
	sound_send (BONUS_LONG);
	scores_draw ();
	dmd_sched_transition (&trans_scroll_up);
	dmd_show_low ();
	task_sleep_sec (2);
	task_kill_gid (GID_BONUS_BUTTON_MONITOR);
//	task_kill_gid (GID_BONUS_TALKING);
	deff_exit ();
}//END OF FUNCTION




/* Speed up the bonus if both flipper buttons are pressed */
static void bonus_button_monitor (void) {
	buttons_held = FALSE;
	for (;;) {
		if ((switch_poll_logical (SW_LEFT_BUTTON)
			&& switch_poll_logical (SW_RIGHT_BUTTON))
			&& buttons_held == FALSE) {
			buttons_held = TRUE;
			//sound_send (SND_FIST_BOOM1);
		}
		task_sleep (TIME_100MS);
	}
}//end of function




/* Function so we can call two different transistions
 * depending on whether the buttons were pressed */
static void bonus_sched_transition (void) {
	if (buttons_held == TRUE)
		dmd_sched_transition (&trans_scroll_down_fast);
	else
		dmd_sched_transition (&trans_scroll_down);
}//end of function

static void bonus_pause (void) {
	if (buttons_held) 	task_sleep (TIME_100MS);
	else 				task_sleep_sec (1);
}//end of function



/*
static void bonus_talking_task (void) {
	sound_send (SND_NOT_AN_ORDINARY_GAME);
	task_sleep_sec (2);
	sound_send (SND_OR_AN_ORDINARY_PLAYER);
	task_sleep_sec (2);

	switch (find_player_ranked (1) + 1)
	{
		case 1:
			sound_send (SND_PLAYER_ONE);
			break;
		case 2:
			sound_send (SND_PLAYER_TWO);
			break;
		case 3:
			sound_send (SND_PLAYER_THREE);
			break;
		case 4:
			sound_send (SND_PLAYER_FOUR);
			break;
		default:
			break;
	}
	task_exit ();
}

*/



/* Used to announce if the player has played well */
/*
static bool check_for_big_score (void)
{
	if (score_compare (points_this_ball, score_table[SC_100M]) == 1)
		return TRUE;
	else
		return FALSE;
}
*/

/*
static bool check_for_puny_score (void)
{
	if (score_compare (score_table[SC_10M], points_this_ball) == 1)
		return TRUE;
	else
		return FALSE;

}
*/





/* See if it's the players last ball */
/*
static bool check_if_last_ball_for_multiplayer (void)
{
	if (ball_up == system_config.balls_per_game && num_players > 1 && extra_balls == 0)
		return TRUE;
	else
		return FALSE;
}
*/

/*
static bool check_if_last_ball_of_multiplayer_game (void)
{
	if (ball_up == system_config.balls_per_game && player_up == num_players &&
		num_players > 1 && extra_balls == 0)
		return TRUE;
	else
		return FALSE;
}
*/


/* Function to calculate bonus score */
/*
static void bonus_add_up_score (U8 award_count, score_id_t award_amount) {
	score_zero (bonus_scored);// Zero the temporary score
	score_add (bonus_scored, score_table[award_amount]);	// Add the award amount to the temp score
	score_mul (bonus_scored, award_count);					// Multiply it by award_count
	score_add (total_bonus, bonus_scored);					// Add the temp score to the total bonus
	sprintf_score (bonus_scored);
}

*/
