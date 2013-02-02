/*
 * demolition man
 * simple_swithces.c
 *
 * written by James Cardona
 * 
 * Location Description:
 * Upper Rebound : This is a three inch piece of rubber forming the left wall of
 * the Center Ramp that is stretched across a switch ... hitting the rubber
 * activates the switch, just like the left and right 110 switches on Whirlwind.
 *
 * Right Rebound : This is a three inch piece of rubber on the right side
 * of the playfield, just above the right outlane. Just like the Upper Rebound,
 * it is rubber stretched across a switch.
 *
 *
 * Scoring Description: (original game)
 * rebounds score 1010
 * slings score 1000
 *
 * Scoring Description: (my rules)
 * same as original
 *
 * */

#include <freewpc.h>
#include "dm/global_constants.h"

//constants
const U8 SSTotalNumOfSounds	= 11;

const sound_code_t outlaneSoundArray[] = {	SPCH_TOO_BAD, 			SPCH_PATHETIC,		SPCH_HATE_WHEN_THAT_HAPPENS,
											SPCH_OHH_NO,			SPCH_BAD_AIM,		SPCH_DULCH,
											SPCH_WES_LAUGH_SHORT1, SPCH_SIMON_SNICKER, 	SPCH_WES_LAUGH_SHORT2,
											SPCH_WES_LAUGH_SHORT3,		SPCH_JUST_LOVELY };


//local variables
U8	secretCodeCount;
__boolean simple_ball_search;

//external variables

//prototypes
void simple_sounds(void);

/****************************************************************************
 * body
 *
 ****************************************************************************/
CALLSET_ENTRY (simple_switches, start_ball) { simple_ball_search = FALSE; }



CALLSET_ENTRY (simple_switches, sw_right_outlane, sw_left_outlane) {
	score(OUTLANE_SCORE);
	U8		outlaneSoundCounter;
	outlaneSoundCounter = random_scaled(SSTotalNumOfSounds);//from kernal/random.c
	sound_start (ST_SPEECH, outlaneSoundArray[outlaneSoundCounter], SL_4S, PRI_GAME_QUICK5);
}//end of function



CALLSET_ENTRY (simple_switches, sw_upper_left_flipper_gate) {
	if (flag_test(FLAG_IS_HUXLEY_RUNNING) )	huxley_mode_shot_made();
	else if (flag_test (FLAG_IS_PBREAK_RUNNING) )  prison_break_made();
	else score(UP_LEFT_FLIP_GATE_SCORE);
}//end of function


CALLSET_ENTRY (simple_switches, sw_upper_rebound) {
	leff_start (LEFF_UPPER_REBOUND);
	if (flag_test(FLAG_IS_HUXLEY_RUNNING) )	huxley_mode_shot_made();
	else if (flag_test (FLAG_IS_PBREAK_RUNNING) ) { prison_break_made(); }
	else {
		score(UPPER_REBOUND_SCORE);
		simple_sounds();
	}//end of else
}//end of function





CALLSET_ENTRY (simple_switches, sw_lower_rebound) {
	leff_start (LEFF_LOWER_REBOUND);
	if (flag_test(FLAG_IS_HUXLEY_RUNNING) )	huxley_mode_shot_made();
	else if (flag_test (FLAG_IS_PBREAK_RUNNING) ) { prison_break_made(); }
	else {
		simple_sounds();
		score(LOWER_REBOUND_SCORE);
	}//end of else
}//end of function



CALLSET_ENTRY (simple_switches, sw_sling) {
	if (!simple_ball_search) {
		score(SLING_SCORE);
		simple_sounds();
	}
}//end of function



CALLSET_ENTRY (simple_switches, ball_search){
	//VERIFY WE DON'T HAVE ALL THE BALLS IN THE TROUGH AND JUST LOST COUNT
	U8 temp_live_balls;
	temp_live_balls = (5 - device_recount(device_entry (DEVNO_TROUGH) ));
	if (live_balls > temp_live_balls) live_balls = temp_live_balls;
	if (live_balls == 0) {
		live_balls++;
		device_remove_live (); //this will remove only 1
	}
	simple_ball_search = TRUE;

	//the smart thing to do would be to actually be a "real" programmer
	//and figure out why search.c is not working - maybe I'll do that
	//someday.  but for now, here is some really crappy code!
	if (simple_ball_search) { sol_request_async(SOL_LEFT_SLING); task_sleep (TIME_300MS); }
	if (simple_ball_search) { sol_request_async(SOL_RIGHT_SLING); task_sleep (TIME_300MS); }
	if (simple_ball_search) { sol_request_async(SOL_LEFT_JET); task_sleep (TIME_300MS); }
	if (simple_ball_search) { sol_request_async(SOL_RIGHT_JET); task_sleep (TIME_300MS); }
	if (simple_ball_search) { sol_request_async(SOL_TOP_SLING); task_sleep (TIME_300MS); }
	if (simple_ball_search) { sol_request (SOL_EJECT); task_sleep (TIME_300MS); }
	if (simple_ball_search) { sol_request_async(SOL_TOP_POPPER); task_sleep (TIME_300MS); }
	if (simple_ball_search) { sol_request_async(SOL_BOTTOM_POPPER); }
}//end of function



CALLSET_ENTRY (simple_switches, ball_search_end){ simple_ball_search = FALSE; }
CALLSET_ENTRY (simple_switches, start_player) { secretCodeCount = 0; }


//secret testing button
CALLSET_ENTRY (simple_switches, sw_left_handle_button) {
	if (IN_TEST) {
		secretCodeCount++;
//		if (secretCodeCount == 3) huxley_increment_all();
//		if (secretCodeCount == 6) demotime_increment_all();
		if (secretCodeCount == 5) enable_back_in_the_fridge();

//		increment_freeze();
	}//END OF IN TEST
}//end of function


//secret testing button
CALLSET_ENTRY (simple_switches, sw_escape) { if (IN_TEST)  scores_reset (); }

/****************************************************************************
 * lighting effects
 ****************************************************************************/
void upper_rebound_leff (void) {
	flasher_pulse (FLASH_LEFT_RAMP_UP_FLASHER);
	task_sleep (TIME_33MS);
	flasher_pulse (FLASH_DIVERTER_FLASHER);
	task_sleep (TIME_33MS);
	flasher_pulse (FLASH_RIGHT_RAMP_UP_FLASHER);
	task_sleep (TIME_100MS);

	flasher_pulse (FLASH_LEFT_RAMP_UP_FLASHER);
	task_sleep (TIME_33MS);
	flasher_pulse (FLASH_DIVERTER_FLASHER);
	task_sleep (TIME_33MS);
	flasher_pulse (FLASH_RIGHT_RAMP_UP_FLASHER);
	task_sleep (TIME_100MS);

	flasher_pulse (FLASH_LEFT_RAMP_UP_FLASHER);
	task_sleep (TIME_33MS);
	flasher_pulse (FLASH_DIVERTER_FLASHER);
	task_sleep (TIME_33MS);
	flasher_pulse (FLASH_RIGHT_RAMP_UP_FLASHER);
	leff_exit ();
}//end of function



void lower_rebound_leff (void) {
	flasher_pulse (FLASH_LOWER_REBOUND_FLASHER);
	task_sleep (TIME_100MS);
	flasher_pulse (FLASH_LOWER_REBOUND_FLASHER);
	task_sleep (TIME_100MS);
	flasher_pulse (FLASH_LOWER_REBOUND_FLASHER);
	task_sleep (TIME_100MS);
	flasher_pulse (FLASH_LOWER_REBOUND_FLASHER);
	leff_exit ();
}//end of function




/****************************************************************************
 * sound effects
 ****************************************************************************/
void simple_sounds (void) {
	U8		simple_SoundCounter;
	simple_SoundCounter = random_scaled(4);//from kernal/random.c
	if ( simple_SoundCounter == 0 )			sound_start (ST_EFFECT, ZAPP_1, SL_2S, PRI_GAME_QUICK5);
	else if ( simple_SoundCounter == 1 )	sound_start (ST_EFFECT, JOINK, SL_2S, PRI_GAME_QUICK5);
	else if ( simple_SoundCounter == 2 )	sound_start (ST_EFFECT, JOINK_2, SL_2S, PRI_GAME_QUICK5);
	else if ( simple_SoundCounter == 3 )	sound_start (ST_EFFECT, ZAPP_2, SL_2S, PRI_GAME_QUICK5);
}//end of function

