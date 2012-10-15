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
const U8		SSTotalNumOfSounds	= 12;
const sound_code_t outlaneSoundArray[] = {	SPCH_TOO_BAD, 			SPCH_PATHETIC,		SPCH_HATE_WHEN_THAT_HAPPENS,
											SPCH_OHH_NO,			SPCH_BAD_AIM,		SPCH_DULCH,
											SPCH_WES_LAUGH_SHORT1, SPCH_SIMON_SNICKER, 	SPCH_WES_LAUGH_SHORT2,
											SPCH_WES_LAUGH_SHORT3, SPCH_WOAH, 			SPCH_JUST_LOVELY };


//local variables

//external variables

//prototypes
void simple_sounds(void);

/****************************************************************************
 * body
 *
 ****************************************************************************/
CALLSET_ENTRY (simple_switches, sw_right_outlane, sw_left_outlane) {
	score(SC_5770);
	U8		outlaneSoundCounter;
	outlaneSoundCounter = random_scaled(SSTotalNumOfSounds);//from kernal/random.c
	sound_start (ST_SPEECH, outlaneSoundArray[outlaneSoundCounter], SL_4S, PRI_GAME_QUICK5);
}//end of function


CALLSET_ENTRY (simple_switches, sw_upper_left_flipper_gate) {
	if (flag_test (FLAG_IS_PBREAK_ACTIVATED) ) { prison_break_made(); }
	else score(SC_1010);
}

CALLSET_ENTRY (simple_switches, sw_upper_rebound) {
	if (flag_test (FLAG_IS_PBREAK_ACTIVATED) ) { prison_break_made(); }
	else {
		score(SC_1010);
		simple_sounds();
	}
}

CALLSET_ENTRY (simple_switches, sw_lower_rebound) {
	if (flag_test (FLAG_IS_PBREAK_ACTIVATED) ) { prison_break_made(); }
	else {
		simple_sounds();
		score(SC_1010);
		flasher_pulse (FLASH_LOWER_REBOUND_FLASHER); //FLASH followed by name of flasher in caps
	}
}//end of function


CALLSET_ENTRY (simple_switches, sw_sling) {
	score(SC_540);
	simple_sounds();
}//end of function

/****************************************************************************
 * sound effects
 ****************************************************************************/
void simple_sounds (void) {
	U8		simple_SoundCounter;
	simple_SoundCounter = random_scaled(4);//from kernal/random.c
	if ( simple_SoundCounter == 0 )
	sound_start (ST_EFFECT, ZAPP_1, SL_2S, PRI_GAME_QUICK5);
else if ( simple_SoundCounter == 1 )
	sound_start (ST_EFFECT, JOINK, SL_2S, PRI_GAME_QUICK5);
else if ( simple_SoundCounter == 2 )
	sound_start (ST_EFFECT, JOINK_2, SL_2S, PRI_GAME_QUICK5);
else if ( simple_SoundCounter == 3 )
	sound_start (ST_EFFECT, ZAPP_2, SL_2S, PRI_GAME_QUICK5);
}//end of function

