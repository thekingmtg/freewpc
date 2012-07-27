/*
 * demolition man
 * ramps.c
 *
 * Location Description:
 * There are 4 ramps total
 *
 * Left Ramp: Directly to the right of the left loop is this ramp which always
 * feeds to the left inlane. It can be lit for "Quick Freeze" and the "Car Chase" round.
 * Lights are, from bottom up, Quick Freeze, Car Chase, Explode, Jackpot, Combo arrow.
 *
 * Center Ramp (the ACMag): Located at the very top of the playfield, it is
 * a wide ramp (three pinball widths wide) that is triggered only by a gate.
 * Balls drop immediately and roll either down the left loop or into
 * the MTL rollovers. The center ramp has a Jackpot lamp and a Combo arrow.
 * The Center Ramp is bounded on the left by the Upper Rebound and on the right
 * by the left jet bumper.
 *
 * Side Ramp: This is a small, twisting, and difficult ramp just below the
 * Computer. It is used for the skill shot. The Side Ramp has a jackpot
 * lamp and a Combo Arrow.
 *
 * Right Ramp: This is a tall ramp similar to the right ramp in Judge Dredd.
 * It has a diverter which normally sends the ball to the right inlane,
 * but it can send the ball to the Cryo-Claw when that feature is available.
 * It's also used during the Car Chase round, and to score Super Jackpots.
 * Lights are, from bottom up, Claw Ready, Car Chase, Explode, Jackpot, Combo
 * Arrow. There is a Super Jackpot light mounted over the ramp.
 *
 * Scoring Description: (original game)
 * Ramps score depending on mode running as follows:
 * no mode running:
 * car crash running: scores only on left and right ramps-
 * explode running: scores only on left and right ramps- 5 to 15 million depending on time left
 * Multiball: any ramp may be lit for jackpot shot - award of
 * Combo: if successive arrow shots are made in a row -scores award of
 *
 *
 */
#include <freewpc.h>
//constants
U8 RAMPS_EASY_GOAL = 5;
U8 RAMPS_PREDEFINED_GOAL_INCREMENT = 1;
U8 RAMPS_GOAL_STEP = 5;
U8 RAMPS_GOAL_MAX = 50;

//local variables
U8 	left_ramp_counter;
U8 	right_ramp_counter;
U8 	center_ramp_counter;
U8 	all_ramp_counter;
U8 		left_ramp_goal;
U8 		right_ramp_goal;
U8 		center_ramp_goal;
U8 		all_ramp_goal;
__boolean 	left_Ramp_QuickFreeze_activated;
__boolean 	left_Ramp_CarChase_activated;
__boolean 	left_Ramp_Explode_activated;
__boolean 	left_Ramp_Jackpot_activated;
__boolean 	left_Ramp_Arrow_activated;
__boolean 		center_Ramp_Jackpot_activated;
__boolean 		center_Ramp_Arrow_activated;
__boolean 			side_Ramp_Jackpot_activated;
__boolean 			side_Ramp_Arrow_activated;
__boolean 	right_Ramp_ClawReady_activated;
__boolean 	right_Ramp_CarChase_activated;
__boolean 	right_Ramp_Explode_activated;
__boolean 	right_Ramp_Jackpot_activated;
__boolean 	right_Ramp_Arrow_activated;
extern __boolean 		explode_activated;

//external variables
//located in eyeball_explode.c
extern	__boolean explode_activated;
//extern	explode_mode_timer;
//located in car_chase.c
extern	__boolean car_chase_mode_activated;


//prototypes
void ramps_reset (void);
void right_ramp_task (void);
void right_ramp_goal_award (void);
void left_ramp_task (void);
void left_ramp_goal_award (void);
void center_ramp_goal_award (void);
void ramps_flasher (char * flasher);



/****************************************************************************
 * initialize  and exit
 ***************************************************************************/
void ramps_reset (void) {
	left_ramp_counter = 0;
	right_ramp_counter = 0;
	center_ramp_counter = 0;
	all_ramp_counter = 0;

	right_ramp_goal = RAMPS_EASY_GOAL;
	left_ramp_goal = RAMPS_EASY_GOAL;
	center_ramp_goal = RAMPS_EASY_GOAL;
	all_ramp_goal = (RAMPS_EASY_GOAL * 5);
	}

//TODO: modify options to allow saving ramps/loops across balls
CALLSET_ENTRY (ramps, start_player) { ramps_reset (); }
CALLSET_ENTRY (ramps, start_ball) { ramps_reset (); }


CALLSET_ENTRY (ramps, Activate_Explode_Inserts) {
	left_Ramp_Arrow_activated = TRUE;
	right_Ramp_Arrow_activated = TRUE;
	lamp_tristate_flash (LM_LEFT_RAMP_EXPLODE);
	lamp_tristate_flash (LM_RIGHT_RAMP_EXPLODE);
	}


CALLSET_ENTRY (ramps, DeActivate_Explode_Inserts) {
	left_Ramp_Arrow_activated = FALSE;
	right_Ramp_Arrow_activated = FALSE;
	lamp_tristate_off (LM_LEFT_RAMP_EXPLODE);
	lamp_tristate_off (LM_RIGHT_RAMP_EXPLODE);
	}



/*
 *
 * right ramp
 */
void right_ramp_task (void) { task_sleep_sec(2); task_exit(); }
CALLSET_ENTRY (ramps, sw_right_ramp_enter) {
	task_create_gid1 (GID_RIGHT_RAMP_ENTERED, right_ramp_task);
	ramps_flasher("FLASH_RIGHT_RAMP_UP_FLASHER");
	}
CALLSET_ENTRY (ramps, sw_right_ramp_exit) {
	if ( task_kill_gid(GID_RIGHT_RAMP_ENTERED) ) callset_invoke(right_ramp_made);
	}

void right_ramp_goal_award (void) {
	sound_start (ST_SAMPLE, EXPLOSION, SL_1S, PRI_GAME_QUICK5);
	score (SC_50K);
	right_ramp_counter = 0;
	if (right_ramp_goal < RAMPS_GOAL_MAX)  right_ramp_goal += RAMPS_GOAL_STEP;
	}

CALLSET_ENTRY (ramps, right_ramp_made) {
	++right_ramp_counter;
	++all_ramp_counter;
	ramps_flasher("FLASH_RIGHT_RAMP_FLASHER");
	if (explode_activated) callset_invoke(explode_ramp_made);
	if (car_chase_mode_activated) callset_invoke(car_chase_ramp_made);

	sound_start (ST_SAMPLE, MACHINE14_SHORT, SL_1S, PRI_GAME_QUICK1);
	score (SC_50K);
	if (right_ramp_counter == right_ramp_goal)  right_ramp_goal_award ();
	}//end of function

/*
 *
 * left ramp
 */
void left_ramp_task (void) { task_sleep_sec(2); task_exit(); }
CALLSET_ENTRY (ramps, sw_left_ramp_enter) {
	task_create_gid1 (GID_LEFT_RAMP_ENTERED, left_ramp_task);
	ramps_flasher("FLASH_LEFT_RAMP_UP_FLASHER");
	}

CALLSET_ENTRY (ramps, sw_left_ramp_exit) {
	if ( task_kill_gid(GID_LEFT_RAMP_ENTERED) ) callset_invoke(left_ramp_made);
	}

void left_ramp_goal_award (void) {
	sound_start (ST_SAMPLE, EXPLOSION, SL_1S, PRI_GAME_QUICK5);
	score (SC_50K);
	left_ramp_counter = 0;
	if (left_ramp_goal < RAMPS_GOAL_MAX)  left_ramp_goal += RAMPS_GOAL_STEP;
	}

CALLSET_ENTRY (ramps, left_ramp_made) {
	++left_ramp_counter;
	++all_ramp_counter;
	ramps_flasher("FLASH_LEFT_RAMP_LOWER_FLASHER");
	if (explode_activated) callset_invoke(explode_ramp_made);
	if (car_chase_mode_activated) callset_invoke(car_chase_ramp_made);

	sound_start (ST_SAMPLE, EXPLOSION1_SHORT, SL_1S, PRI_GAME_QUICK1);
		score (SC_50K);
	if (left_ramp_counter == left_ramp_goal)  left_ramp_goal_award();
	}//end of function

/*
 *
 * center ramp
 * note: center ramp has only 1 switch
 */
void center_ramp_goal_award (void) {
	sound_start (ST_SAMPLE, EXPLOSION, SL_1S, PRI_GAME_QUICK5);
	ramps_flasher("FLASH_CENTER_RAMP_FLASHER");
	score (SC_50K);
	center_ramp_counter = 0;
	if (center_ramp_goal < RAMPS_GOAL_MAX)  center_ramp_goal += RAMPS_GOAL_STEP;
	}

CALLSET_ENTRY (ramps, center_ramp) {
	++center_ramp_counter;
	++all_ramp_counter;
	sound_start (ST_SAMPLE, EXPLOSION1_MED, SL_1S, PRI_GAME_QUICK1);
	score (SC_50K);
	if (center_ramp_counter == center_ramp_goal)  center_ramp_goal_award ();
	}//end of function



/****************************************************************************
 * DMD display and sound effects
 ****************************************************************************/
//LIGHTING EFFECTS
void ramps_flasher (char * flasher) {
	flasher_pulse (flasher); //FLASH followed by name of flasher in caps
	task_sleep (TIME_500MS);
	task_exit ();
	}
