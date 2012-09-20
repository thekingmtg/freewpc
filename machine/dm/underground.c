/*
 * demolition man
 * underground.c
 *
 * written by James Cardona
 *
 * Location Description:
 * subway / underground is a scoop across from the upper flipper and to the left of the jets
 * and to the right of the center ramp, with the opening on the left side. It is an easy
 * shot from the upper left flipper, and it can also be made with a little luck from either
 * of the lower flippers. The Computer and Demolition Time are awarded here. The
 * Underground has a Jackpot lamp and a Combo arrow. The ball is returned, via a VUK
 * to the right ramp habitrail. The Computer and Demolition time are indicated by
 * yellow and blue lights mounted over the Underground entrance.
 *
 * The Computer is accessed via the UnderGround shot and is lit by a yellow lamp
 * immediately above. Lit every 10 combos, the Computer is a random award like
 * the Camera on Twilight Zone. Awards seen so far :
 * Light Extra Ball
 * Light Explodes (lights all four explode 10M lamps)
 * 3x Car Crash
 * 2x Retina Scan
 * Light Arrows (lights Laser millions on all combo shots)
 * Maximize Freezes (freezes four balls and lights multiball)
 * Collect Bonus (complete with bonus music!)
 * Collect Standups (awards all the lit yellow targets)
 *
 * Scoring Description: (original game)
 *
 *
 * Scoring Description: (my rules)
 * same as above except
 *
 */

#include <freewpc.h>
#include "dm/global_constants.h"

//constants
const U8 		computerAwardsNumOfSounds = 7; //num between 0 and 6
const sound_code_t computerAwardsSoundsArray[] = {	COMPUTER1, 				COMPUTER_ADDING,   COMPUTER_AWARD_SMALL,
													COMPUTER_AWARD_LONG,	COMPUTER2, 			SPCH_ACCESSING,
													SPCH_COMPUTER_AWARD};

//local variables
U8 			undergroundSwitchDebouncer;
U8 			underground_shots_made;
U8 			underground_goal;
__boolean 	is_underground_Jackpot_activated;
U8			computerAwards;

//external variables

//prototypes
void underground_reset (void);
void underground_task (void);

/****************************************************************************
 * initialize  and exit
 ***************************************************************************/
void underground_reset (void) {
	undergroundSwitchDebouncer = 0;
	underground_shots_made = 0;
	underground_goal = 0;
	is_underground_Jackpot_activated = FALSE;
}//end of function

void player_underground_reset(void) {
	underground_reset();
}//end of function

CALLSET_ENTRY (underground, start_player) { player_underground_reset(); }
CALLSET_ENTRY (underground, start_ball) { underground_reset(); }


/****************************************************************************
 * playfield lights and flags
 ***************************************************************************/
//lit by multiball modes --TODO:
CALLSET_ENTRY (underground, underground_jackpot_light_on) {
	is_underground_Jackpot_activated = TRUE;
	lamp_tristate_on (LM_UNDERGROUND_JACKPOT);
}//end of function

CALLSET_ENTRY (underground, underground_jackpot_light_off) {
	is_underground_Jackpot_activated = FALSE;
	lamp_tristate_off (LM_UNDERGROUND_JACKPOT);
}//end of function

CALLSET_ENTRY (underground, underground_arrow_light_on) {
	flag_on (FLAG_IS_UGROUND_ARROW_ACTIVATED);
	lamp_tristate_on (LM_UNDERGROUND_ARROW);
}//end of function

CALLSET_ENTRY (underground, underground_arrow_light_off) {
	flag_off (FLAG_IS_UGROUND_ARROW_ACTIVATED);
	lamp_tristate_off (LM_UNDERGROUND_ARROW);
}//end of function

/****************************************************************************
 * body
 *
 ****************************************************************************/
void underground_task (void) {
	task_sleep_sec(3);
	undergroundSwitchDebouncer = 0;
	task_exit();
}//end of function


CALLSET_ENTRY (underground, sw_bottom_popper) {
	if (++undergroundSwitchDebouncer == 1) {
			++underground_shots_made;
			score (SC_100K);//located in kernal/score.c
			U8	underground_SoundCounter;
			//SOUNDS
			underground_SoundCounter = random_scaled(2);//from kernal/random.c
			if (underground_SoundCounter == 0 )
				sound_start (ST_EFFECT, SUBWAY, SL_2S, SP_NORMAL);
			else if (underground_SoundCounter == 1 )
				sound_start (ST_EFFECT, SUBWAY2, SL_2S, SP_NORMAL);
			//LIGHTING EFFECTS
		//	leff_start (LEFF_UNDERGROUND_KICKOUT);
			//CALLS
			if (flag_test (FLAG_IS_PBREAK_UNDER_ACTIVATED) )  	callset_invoke(prison_break_made);
			if (flag_test (FLAG_IS_CAPSIM_UNDER_ACTIVATED) )	callset_invoke(capture_simon_made);
			if (flag_test(FLAG_IS_COMBOS_KILLED) ) 				callset_invoke(combo_init);
			else if ( flag_test(FLAG_IS_COMBO_UNDER_ACTIVATED)) callset_invoke(combo_hit);
			if (flag_test(FLAG_IS_COMPUTER_ACTIVATED) ) 		callset_invoke(computer_award);
			//TODO: jackpot shot detection
			//	if (is_underground_Jackpot_activated) //during multiball

			//if nothing special, do normal display effects
			if(		!flag_test (FLAG_IS_PBREAK_UNDER_ACTIVATED)
				&& 	!flag_test (FLAG_IS_CAPSIM_UNDER_ACTIVATED)
				&& 	!flag_test (FLAG_IS_COMPUTER_ACTIVATED)
				&& 	!is_underground_Jackpot_activated)
								deff_start (DEFF_UNDERGROUND_EFFECT);
			//fire sol
			if (!flag_test(FLAG_IS_COMPUTER_ACTIVATED) ) sol_request_async(SOL_BOTTOM_POPPER);
	}//end of if DEBOUNCER
	task_create_gid1 (GID_UG_DEBOUNCE, underground_task);
}//end of function


//called after minimum of every N combos and underground shot made
CALLSET_ENTRY (underground, computer_award) {
	deff_start (DEFF_COMPUTER_AWARD_EFFECT);
	computerAwards = random_scaled(computerAwardsNumOfSounds);//from kernal/random.c
	sound_start (ST_SPEECH, computerAwardsSoundsArray[computerAwards], SL_2S, PRI_GAME_QUICK5);
	task_sleep(TIME_2S);
	switch (computerAwards) {
	case 0 :
			sound_start(ST_SPEECH, SPCH_COLLECT_BONUS, SL_4S, PRI_GAME_QUICK5);
			task_sleep(TIME_2S);
			deff_start (DEFF_BONUS);
			break;
	case 1 :
			sound_start(ST_SPEECH, SPCH_TRIPLE_CAR_CRASH, SL_4S, PRI_GAME_QUICK5);
			callset_invoke (comp_award_trip_car_crash);
			break;
	case 2 :
			sound_start(ST_SPEECH, SPCH_COLLECT_STANDUPS, SL_4S, PRI_GAME_QUICK5);
			//TODO: put routine here
			break;
	case 3 :
			sound_start(ST_SPEECH, SPCH_LIGHT_ARROWS, SL_4S, PRI_GAME_QUICK5);
			callset_invoke (comp_award_light_arrows);
			break;
	case 4 :
			sound_start(ST_SPEECH, SPCH_LIGHT_EXTRA_BALL, SL_4S, PRI_GAME_QUICK5);
			//TODO: put routine here
			break;
	case 5 :
			sound_start(ST_SPEECH, SPCH_MAXIMIZE_FREEZES, SL_4S, PRI_GAME_QUICK5);
			//TODO: put routine here
			break;
	case 6 :
			sound_start(ST_SPEECH, SPCH_DOUBLE_RETINA_SCAN, SL_4S, PRI_GAME_QUICK5);
			callset_invoke (comp_award_doub_retina);
			break;
	}//end of switch
	task_sleep(TIME_3S);
	sol_request_async(SOL_BOTTOM_POPPER);
}//end of function




/****************************************************************************
 * lighting effects
 ****************************************************************************/
static void under_kickout_subtask (void) {
	U8 i;
	for (i = 0; i < 5; i++) {
		leff_toggle (LAMPLIST_UNDERGROUND_SCOOP);
		task_sleep (TIME_100MS);
	}//end of loop
	task_exit ();
}//end of function

void underground_kickout_leff (void) {
	gi_leff_enable (PINIO_GI_STRINGS);
	leff_create_peer (under_kickout_subtask);
	task_sleep (TIME_500MS);
	gi_leff_enable (PINIO_GI_STRINGS);
	leff_exit ();
}//end of function


/****************************************************************************
 * display effects
 ****************************************************************************/
void underground_effect_deff(void) {
	dmd_alloc_low_clean ();
	sprintf ("SUBWAY");
	font_render_string_center (&font_steel, DMD_MIDDLE_X, DMD_BIG_CY_Top, sprintf_buffer);
	sprintf ("%d MADE", underground_shots_made);
	font_render_string_center (&font_term6, DMD_MIDDLE_X, DMD_BIG_CY_Bot, sprintf_buffer);
	dmd_show_low ();
	task_sleep_sec (2);
	deff_exit ();
}//end of mode_effect_deff



void computer_award_effect_deff(void) {
	dmd_alloc_low_clean ();
	sprintf ("COMPUTER");
	font_render_string_center (&font_term6, DMD_MIDDLE_X, DMD_BIG_CY_Top, sprintf_buffer);
	sprintf ("AWARD");
	font_render_string_center (&font_term6, DMD_MIDDLE_X, DMD_BIG_CY_Bot, sprintf_buffer);
	dmd_show_low ();
	task_sleep_sec (2);
	deff_exit ();
}//end of mode_effect_deff


