/*
 * demolition man
 * ramps.c
 *
 * written by James Cardona
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
 *
 * Scoring Description: (original game)
 * Ramps score depending on mode running as follows:
 * basic score - no mode running: ramp 100k enter - 250k exit
 * explode running: scores only on left and right ramps- 5 to 15 million depending on time left
 * car crash running: scores only on left and right ramps- 10 million
 * car chase in orig is not a mode but single lights that can be lit on either L or R ramp
 * MB quick freeze - right inlane activates quick freeze light from left ramp - shoot left ramp to freeze.

 * TODO: Multiball: any ramp may be lit for jackpot shot - award of 20 mill
 * TODO: combos - 4million first combo and extra 2 mill for each successive combo
 * TODO: capture simon: all arrows flash and remain for easiest lev - shoot 3 arrows 5 mill each
 * first 2, last one 25 mill
 *
 * TODO: right ramp to access claw - 1 million
 *
 * Scoring Description: (my rules)
 * same as original except:
 * car chase is a mode here (not in original) --L or R ramp for xx million shot --handled in carchase.c
 *
 *
 */

#include <freewpc.h>
#include <diverter.h> //autogenerated by divhold.ct
#include "dm/global_constants.h"

//constants
const U8 		RAMPS_EASY_GOAL = 5;
const U8 		RAMPS_PREDEFINED_GOAL_INCREMENT = 1;
const U8 		RAMPS_GOAL_STEP = 5;
const U8 		RAMPS_GOAL_MAX = 50;
const U8 		RampsTotalNumOfSounds = 25; //num between 0 and 24 == 25 total
const sound_code_t rampSoundsArray[] = {	CAR_GEAR_CHANGE, 	SPCH_PUNCH_IT, 				SPCH_PUSH_PEDAL_HARD,
											SPCH_GO_LEFT, 		SPCH_CATCH_UP, 				SPCH_STEP_ON_IT,
											SPCH_CATCH_UP, 		SPCH_NICE_SHOOTING_SLY, 	SPCH_NICE_SHOT_SLY,
											SPCH_GREAT_SHOT_SLY, SPCH_GREAT_SHOOTING, 		SPCH_NOW_THATS_WHAT_I_CALL_SHOOTING,
											SPCH_OHH_YEAH1, 	SPCH_OHH_YEAH2, 			SPCH_OUTSTANDING_SLY,
											SPCH_NICE, 			SPCH_VERY_NICE, 			SPCH_NICE_MOVE_SLY,
											SPCH_GOOD_ONE, 		SPCH_GREAT_SHOT, 			SPCH_GOOD_SHOT,
											SPCH_NICE_SHOT, 	SPCH_NICE_SHOOTING, 	SPCH_NICE_SHOOTING,
											SPCH_EXCELLENT };

const U8 		missedTotalNumOfSounds =12;
const sound_code_t missedRampSoundsArray[] = {SPCH_TOO_BAD, 		SPCH_PATHETIC,		SPCH_HATE_WHEN_THAT_HAPPENS,
											SPCH_OHH_NO,		SPCH_BAD_AIM,		SPCH_DULCH,
											SPCH_WES_LAUGH_SHORT1, SPCH_SIMON_SNICKER, SPCH_WES_LAUGH_SHORT2,
											SPCH_WES_LAUGH_SHORT3, SPCH_WOAH, 			SPCH_JUST_LOVELY };

//local variables
U8	ramp_SoundCounter;
U8 	left_ramp_counter;
U8 	right_ramp_counter;
U8 	center_ramp_counter;
U8 	side_ramp_counter;
U8 	all_ramp_counter;
U8 		left_ramp_goal;
U8 		right_ramp_goal;
U8 		center_ramp_goal;
U8 		side_ramp_goal;
U8 		all_ramp_goal;
__boolean 	left_Ramp_QuickFreeze_activated;
__boolean 	left_Ramp_CarChase_activated;
__boolean 	left_Ramp_Explode_activated;
__boolean 	left_Ramp_Jackpot_activated;
__boolean 	left_Ramp_Arrow_activated;
__boolean 		center_Ramp_Jackpot_activated;
__boolean 		center_Ramp_Arrow_activated;
__boolean 	side_Ramp_Jackpot_activated;
__boolean 	side_Ramp_Arrow_activated;
__boolean 		right_Ramp_ClawReady_activated;
__boolean 		right_Ramp_CarChase_activated;
__boolean 		right_Ramp_Explode_activated;
__boolean 		right_Ramp_Jackpot_activated;
__boolean 		right_Ramp_Arrow_activated;

//external variables

//prototypes
void ramps_reset (void);
void right_ramp_task (void);
void right_ramp_goal_award (void);
void left_ramp_task (void);
void left_ramp_goal_award (void);
void center_ramp_goal_award (void);
void ramps_flasher (char * flasher);
void side_ramp_task (void);
void side_ramp_goal_award (void);
void ramp_sounds (void);
void missed_ramp_sounds (void);


/****************************************************************************
 * initialize  and exit
 ***************************************************************************/
void ramps_reset (void) {
	left_ramp_counter = 0;
	right_ramp_counter = 0;
	center_ramp_counter = 0;
	side_ramp_counter = 0;
	all_ramp_counter = 0;
		left_Ramp_QuickFreeze_activated = FALSE;
		left_Ramp_CarChase_activated = FALSE;
		left_Ramp_Explode_activated = FALSE;
		left_Ramp_Jackpot_activated = FALSE;
		left_Ramp_Arrow_activated = FALSE;
	center_Ramp_Jackpot_activated = FALSE;
	center_Ramp_Arrow_activated = FALSE;
		side_Ramp_Jackpot_activated = FALSE;
		side_Ramp_Arrow_activated = FALSE;
	right_Ramp_ClawReady_activated = FALSE;
	right_Ramp_CarChase_activated = FALSE;
	right_Ramp_Explode_activated = FALSE;
	right_Ramp_Jackpot_activated = FALSE;
	right_Ramp_Arrow_activated = FALSE;
}//end of function

void players_ramps_reset (void) {
	ramp_SoundCounter = 0;
	right_ramp_goal = RAMPS_EASY_GOAL;
	left_ramp_goal = RAMPS_EASY_GOAL;
	center_ramp_goal = RAMPS_EASY_GOAL;
	side_ramp_goal = RAMPS_EASY_GOAL;
	all_ramp_goal = (RAMPS_EASY_GOAL * 5);
	ramps_reset();
}//end of function

CALLSET_ENTRY (ramps, start_player) { players_ramps_reset(); }
CALLSET_ENTRY (ramps, start_ball) { ramps_reset(); }


/****************************************************************************
 * playfield lights and flags -- MULTIPLE RAMPS AT SAME TIME
 ***************************************************************************/
//called from eyeball_explode.c for explode mode start
CALLSET_ENTRY (ramps, Activate_Explode_Inserts) {
	left_Ramp_Explode_activated = TRUE;
	right_Ramp_Explode_activated = TRUE;
	lamp_tristate_flash (LM_LEFT_RAMP_EXPLODE);
	lamp_tristate_flash (LM_RIGHT_RAMP_EXPLODE);
}//end of function

CALLSET_ENTRY (ramps, DeActivate_Explode_Inserts) {
	left_Ramp_Explode_activated = FALSE;
	right_Ramp_Explode_activated = FALSE;
	lamp_tristate_off (LM_LEFT_RAMP_EXPLODE);
	lamp_tristate_off (LM_RIGHT_RAMP_EXPLODE);
}//end of function


//called from carcrash.c for carcrash mode start
CALLSET_ENTRY (ramps, carchase_mode_on) {
	left_Ramp_CarChase_activated = TRUE;
	right_Ramp_CarChase_activated = TRUE;
	lamp_tristate_flash (LM_LEFT_RAMP_CAR_CHASE);
	lamp_tristate_flash (LM_RIGHT_RAMP_CAR_CHASE);
}//end of function

CALLSET_ENTRY (ramps, carcrash_mode_off) {
	left_Ramp_CarChase_activated = FALSE;
	right_Ramp_CarChase_activated = FALSE;
	lamp_tristate_off (LM_LEFT_RAMP_CAR_CHASE);
	lamp_tristate_off (LM_RIGHT_RAMP_CAR_CHASE);
}//end of function


/****************************************************************************
 * playfield lights and flags - LEFT RAMP
 ***************************************************************************/
// called by right inlane switch --see inlanes.c
// or
// called by certain number of standups made --see standupfrenzy.c
CALLSET_ENTRY (ramps, Activate_left_Ramp_QuickFreeze) {
	left_Ramp_QuickFreeze_activated = TRUE;
	lamp_tristate_flash (LM_QUICK_FREEZE);
}//end of function

CALLSET_ENTRY (ramps, DeActivate_left_Ramp_QuickFreeze) {
	left_Ramp_QuickFreeze_activated = FALSE;
	lamp_tristate_off (LM_QUICK_FREEZE);
}//end of function

//called by multiball modes --TODO:
CALLSET_ENTRY (ramps, LRamp_Jackpot_Light_On) {
	left_Ramp_Jackpot_activated = TRUE;
	lamp_tristate_on (LM_LEFT_RAMP_JACKPOT);
}//end of function

CALLSET_ENTRY (ramps, LRamp_Jackpot_Light_Off) {
	left_Ramp_Jackpot_activated = FALSE;
	lamp_tristate_off (LM_LEFT_RAMP_JACKPOT);
}//end of function

//called by combo shots --TODO:
CALLSET_ENTRY (ramps, LRamp_Arrow_Light_On) {
	left_Ramp_Arrow_activated = TRUE;
	lamp_tristate_on (LM_LEFT_RAMP_ARROW);
}//end of function

CALLSET_ENTRY (ramps, LRamp_Arrow_Light_Off) {
	left_Ramp_Arrow_activated = FALSE;
	lamp_tristate_off (LM_LEFT_RAMP_ARROW);
}//end of function


/****************************************************************************
 * playfield lights and flags - RIGHT RAMP
 ***************************************************************************/
//called by multiball modes --TODO:
CALLSET_ENTRY (ramps, RRamp_Jackpot_Light_On) {
	right_Ramp_Jackpot_activated = TRUE;
	lamp_tristate_on (LM_RIGHT_RAMP_JACKPOT);
}//end of function

CALLSET_ENTRY (ramps, RRamp_Jackpot_Light_Off) {
	right_Ramp_Jackpot_activated = FALSE;
	lamp_tristate_off (LM_RIGHT_RAMP_JACKPOT);
}//end of function

//called by combo shots --TODO:
CALLSET_ENTRY (ramps, RRamp_Arrow_Light_On) {
	right_Ramp_Arrow_activated = TRUE;
	lamp_tristate_on (LM_RIGHT_RAMP_ARROW);
}//end of function

CALLSET_ENTRY (ramps, RRamp_Arrow_Light_Off) {
	right_Ramp_Arrow_activated = FALSE;
	lamp_tristate_off (LM_RIGHT_RAMP_ARROW);
}//end of function

//called by left inlane --see inlanes.c
CALLSET_ENTRY (ramps, RRamp_ClawReady_On) {
	right_Ramp_ClawReady_activated = TRUE;
	lamp_tristate_on (LM_CLAW_READY);
	flasher_pulse (FLASH_DIVERTER_FLASHER);
	//TODO: THIS DOES NOT WORK!!!
	diverter_start();//defined in divhold.ct
	diverter_service();
}//end of function

CALLSET_ENTRY (ramps, RRamp_ClawReady_Off) {
	right_Ramp_ClawReady_activated = FALSE;
	lamp_tristate_off (LM_CLAW_READY);
	flasher_pulse (FLASH_DIVERTER_FLASHER);
	diverter_stop();//defined in divhold.ct
	diverter_service();
}//end of function


/****************************************************************************
 * playfield lights and flags - CENTER RAMP
 ***************************************************************************/
//called by multiball modes --TODO:
CALLSET_ENTRY (ramps, CRamp_Jackpot_Light_On) {
	center_Ramp_Jackpot_activated = TRUE;
	lamp_tristate_on (LM_CENTER_RAMP_JACKPOT);
}//end of function

CALLSET_ENTRY (ramps, CRamp_Jackpot_Light_Off) {
	center_Ramp_Jackpot_activated = FALSE;
	lamp_tristate_off (LM_CENTER_RAMP_JACKPOT);
}//end of function

//called by combo shots --TODO:
CALLSET_ENTRY (ramps, CRamp_Arrow_Light_On) {
	center_Ramp_Arrow_activated = TRUE;
	lamp_tristate_on (LM_CENTER_RAMP_ARROW);
}//end of function

CALLSET_ENTRY (ramps, CRamp_Arrow_Light_Off) {
	center_Ramp_Arrow_activated = FALSE;
	lamp_tristate_off (LM_CENTER_RAMP_ARROW);
}//end of function


/****************************************************************************
 * playfield lights and flags - SIDE RAMP
 ***************************************************************************/
//called by multiball modes --TODO:
CALLSET_ENTRY (ramps, SRamp_Jackpot_Light_On) {
	side_Ramp_Jackpot_activated = TRUE;
	lamp_tristate_on (LM_SIDE_RAMP_JACKPOT);
}//end of function

CALLSET_ENTRY (ramps, SRamp_Jackpot_Light_Off) {
	side_Ramp_Jackpot_activated = FALSE;
	lamp_tristate_off (LM_SIDE_RAMP_JACKPOT);
}//end of function

//called by combo shots --TODO:
CALLSET_ENTRY (ramps, SRamp_Arrow_Light_On) {
	side_Ramp_Arrow_activated = TRUE;
	lamp_tristate_on (LM_SIDE_RAMP_ARROW);
}//end of function

CALLSET_ENTRY (ramps, SRamp_Arrow_Light_Off) {
	side_Ramp_Arrow_activated = FALSE;
	lamp_tristate_off (LM_SIDE_RAMP_ARROW);
}//end of function

/****************************************************************************
 *
 * left ramp
 *
 ***************************************************************************/
void left_ramp_task (void) { task_sleep_sec(2); task_exit(); }


CALLSET_ENTRY (ramps, sw_left_ramp_enter) {
	task_create_gid1 (GID_LEFT_RAMP_ENTERED, left_ramp_task);
	flasher_pulse (FLASH_LEFT_RAMP_UP_FLASHER); //FLASH followed by name of flasher in caps
	score (SC_100K);
	if (flag_test (FLAG_IS_CARCHASE_MODE_ACTIVATED) )
			sound_start (ST_SAMPLE, CAR_SKID, SL_3S, PRI_GAME_QUICK1);
}//end of function


CALLSET_ENTRY (ramps, sw_left_ramp_exit) {
	if ( task_kill_gid(GID_LEFT_RAMP_ENTERED) ) callset_invoke(left_ramp_made);
}//end of function


CALLSET_ENTRY (ramps, left_ramp_made) {
	++left_ramp_counter;
	++all_ramp_counter;
	score (SC_250K);
	flasher_pulse (FLASH_LEFT_RAMP_UP_FLASHER); //FLASH followed by name of flasher in caps
	task_sleep (TIME_100MS);
	if (flag_test (FLAG_IS_EXPLODE_MODE_ACTIVATED) ) callset_invoke(explode_made); //goto eyeball_explode.c for scoring
	if (flag_test (FLAG_IS_CARCHASE_MODE_ACTIVATED) )
			callset_invoke(car_chase_ramp_made); //goto carchase.c for scoring
	if (left_Ramp_QuickFreeze_activated) callset_invoke(increment_freeze); //goto lock_freeze_mbstart.c
	if (flag_test (FLAG_IS_CAPSIM_LEFTRAMP_ACTIVATED) )
		callset_invoke(capture_simon_made);
	// TODO: check for multiball jackpots here
	// TODO: check for combo arrows here
	//if not in a mode then perform normal sounds and display effects
	if (	!flag_test (FLAG_IS_EXPLODE_MODE_ACTIVATED)
		&& 	!flag_test (FLAG_IS_CARCHASE_MODE_ACTIVATED)
		&& 	!flag_test (FLAG_IS_CAPSIM_LEFTRAMP_ACTIVATED) )
				ramp_sounds();
	// TODO: normal display effects call
	if (left_ramp_counter == left_ramp_goal)  left_ramp_goal_award();
}//end of function


void left_ramp_goal_award (void) {
	//sound_start (ST_SAMPLE, EXPLOSION, SL_1S, PRI_GAME_QUICK5);
	left_ramp_counter = 0;
	if (left_ramp_goal < RAMPS_GOAL_MAX)  left_ramp_goal += RAMPS_GOAL_STEP;
}//end of function



/****************************************************************************
 *
 * right ramp
 *
 ***************************************************************************/
void right_ramp_task (void) { task_sleep_sec(2); task_exit(); }


CALLSET_ENTRY (ramps, sw_right_ramp_enter) {
	task_create_gid1 (GID_RIGHT_RAMP_ENTERED, right_ramp_task);
	flasher_pulse (FLASH_RIGHT_RAMP_UP_FLASHER); //FLASH followed by name of flasher in caps
	score (SC_100K);
	if (flag_test (FLAG_IS_CARCHASE_MODE_ACTIVATED) )
			sound_start (ST_SAMPLE, CAR_SKID, SL_3S, PRI_GAME_QUICK1);
}//end of function


CALLSET_ENTRY (ramps, sw_right_ramp_exit) {
	if ( task_kill_gid(GID_RIGHT_RAMP_ENTERED) ) callset_invoke(right_ramp_made);
}//end of function


CALLSET_ENTRY (ramps, right_ramp_made) {
	++right_ramp_counter;
	++all_ramp_counter;
	score (SC_250K);
	flasher_pulse (FLASH_RIGHT_RAMP_UP_FLASHER); //FLASH followed by name of flasher in caps
	task_sleep (TIME_100MS);
	if (flag_test (FLAG_IS_EXPLODE_MODE_ACTIVATED) ) callset_invoke(explode_made); //goto eyeball_explode.c for scoring
	if (flag_test (FLAG_IS_CARCHASE_MODE_ACTIVATED) )
			callset_invoke(car_chase_ramp_made); //goto carchase.c for scoring
	if (flag_test (FLAG_IS_CAPSIM_RIGHTRAMP_ACTIVATED) )
			callset_invoke(capture_simon_made);
	// TODO: check for multiball jackpots here
	// TODO: check for combo arrows here
	//if not in a mode then perform normal sounds and display effects
	if (	!flag_test (FLAG_IS_EXPLODE_MODE_ACTIVATED)
		&& 	!flag_test (FLAG_IS_CARCHASE_MODE_ACTIVATED)
		&& 	!flag_test (FLAG_IS_CAPSIM_RIGHTRAMP_ACTIVATED) )
				ramp_sounds();
	// TODO: normal display effects call
	if (right_ramp_counter == right_ramp_goal)  right_ramp_goal_award ();
}//end of function


void right_ramp_goal_award (void) {
	right_ramp_counter = 0;
	if (right_ramp_goal < RAMPS_GOAL_MAX)  right_ramp_goal += RAMPS_GOAL_STEP;
}//end of function



/****************************************************************************
 *
 * center ramp
 * note: center ramp has only 1 switch
 *
 ***************************************************************************/
CALLSET_ENTRY (ramps, sw_center_ramp) {
	++center_ramp_counter;
	++all_ramp_counter;
	score (SC_250K);
	lamp_tristate_flash(LM_CENTER_RAMP_MIDDLE);
	lamp_tristate_flash(LM_CENTER_RAMP_OUTER);
	lamp_tristate_flash(LM_CENTER_RAMP_INNER);
	task_sleep (TIME_100MS);
	lamp_tristate_off(LM_CENTER_RAMP_MIDDLE);
	lamp_tristate_off(LM_CENTER_RAMP_OUTER);
	lamp_tristate_off(LM_CENTER_RAMP_INNER);
	//lamps will also flash at acmag call - need to see how that looks
	if (flag_test(FLAG_IS_ACMAG_ACTIVATED) ) callset_invoke(acmag_made);
	ramp_sounds();
	if (flag_test (FLAG_IS_CAPSIM_CENTERRAMP_ACTIVATED) )
		callset_invoke(capture_simon_made);
	if (center_Ramp_Jackpot_activated) callset_invoke(fortressMB_jackpot_made);

	// TODO: check for combo arrows here
	if (center_ramp_counter == center_ramp_goal)  center_ramp_goal_award ();
}//end of function


void center_ramp_goal_award (void) {
	//sound_start (ST_SAMPLE, EXPLOSION, SL_1S, PRI_GAME_QUICK5);
	center_ramp_counter = 0;
	if (center_ramp_goal < RAMPS_GOAL_MAX)  center_ramp_goal += RAMPS_GOAL_STEP;
}//end of function



/****************************************************************************
 *
 * side ramp
 *
 ***************************************************************************/
void side_ramp_task (void) { task_sleep_sec(2); task_exit(); }


CALLSET_ENTRY (ramps, sw_side_ramp_enter) {
	task_create_gid1 (GID_SIDE_RAMP_ENTERED, side_ramp_task);
	flasher_pulse (FLASH_SIDE_RAMP_FLASHER); //FLASH followed by name of flasher in caps
	task_sleep (TIME_100MS);
	score (SC_100K);
}//end of function


CALLSET_ENTRY (ramps, sw_side_ramp_exit) {
	if ( task_kill_gid(GID_SIDE_RAMP_ENTERED) ) callset_invoke(side_ramp_made);
}//end of function


CALLSET_ENTRY (ramps, side_ramp_made) {
	++side_ramp_counter;
	++all_ramp_counter;
	score (SC_250K);
	ramp_sounds();
	flasher_pulse (FLASH_SIDE_RAMP_FLASHER); //FLASH followed by name of flasher in caps
	task_sleep (TIME_100MS);
	if(flag_test (FLAG_IS_PRISON_BREAK_ACTIVATED) )  callset_invoke(prison_break_made);
	if (flag_test (FLAG_IS_CAPSIM_SIDERAMP_ACTIVATED) )
			callset_invoke(capture_simon_made);
	// TODO: check for multiball jackpots here
	// TODO: check for combo arrows here
	if (side_ramp_counter == side_ramp_goal)  side_ramp_goal_award();
}//end of function


void side_ramp_goal_award (void) {
	side_ramp_counter = 0;
	if (side_ramp_goal < RAMPS_GOAL_MAX)  side_ramp_goal += RAMPS_GOAL_STEP;
}//end of function



/****************************************************************************
 * sound effects
 ****************************************************************************/
void ramp_sounds (void) {
	ramp_SoundCounter = random_scaled(RampsTotalNumOfSounds);//from kernal/random.c
	sound_start (ST_SPEECH, rampSoundsArray[ramp_SoundCounter], SL_5S, PRI_GAME_QUICK5);
}//end of function

void missed_ramp_sounds (void) {
	ramp_SoundCounter = random_scaled(missedTotalNumOfSounds);//from kernal/random.c
	sound_start (ST_SPEECH, missedRampSoundsArray[ramp_SoundCounter], SL_5S, PRI_GAME_QUICK5);
}//end of function



