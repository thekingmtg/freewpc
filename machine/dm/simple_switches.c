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

//local variables
U8		simple_SoundCounter = 0;
U8		reboundCounter = 0;
U8		outlaneSoundCounter = 0;
U8		SSTotalNumOfSounds	= 12;
sound_code_t outlaneSoundArray[] = {SPCH_TOO_BAD, 		SPCH_PATHETIC,		SPCH_HATE_WHEN_THAT_HAPPENS,
										SPCH_OHH_NO,		SPCH_BAD_AIM,		SPCH_DULCH,
										SPCH_WES_LAUGH_SHORT1, SPCH_SIMON_SNICKER, SPCH_WES_LAUGH_SHORT2,
										SPCH_WES_LAUGH_SHORT3, SPCH_WOAH, 			SPCH_JUST_LOVELY };

//external variables

//prototypes
void simple_sounds(void);


/****************************************************************************
 * body
 *
 ****************************************************************************/
CALLSET_ENTRY (simple_switches, sw_right_outlane, sw_left_outlane) {
	score(SC_5770);
		outlaneSoundCounter = random_scaled(SSTotalNumOfSounds);//from kernal/random.c
		sound_start (ST_SPEECH, outlaneSoundArray[outlaneSoundCounter], SL_5S, PRI_GAME_QUICK5);
}



CALLSET_ENTRY (simple_switches, sw_upper_left_flipper_gate) { score(SC_1010); }


//this should already be coded somewhere in freewpc?
CALLSET_ENTRY (simple_switches, sw_ball_launch) { launch_ball();}



CALLSET_ENTRY (simple_switches, sw_upper_rebound) {
	score(SC_1010);
	simple_sounds();
	reboundCounter++;
	deff_start (DEFF_SIMPLE_EFFECT);
}



CALLSET_ENTRY (simple_switches, sw_lower_rebound) {
	reboundCounter++;
	deff_start (DEFF_SIMPLE_EFFECT);
	simple_sounds();
	score(SC_1010);
	flasher_pulse (FLASH_LOWER_REBOUND_FLASHER); //FLASH followed by name of flasher in caps
	flasher_pulse (FLASH_LOWER_REBOUND_FLASHER); //FLASH followed by name of flasher in caps
	flasher_pulse (FLASH_LOWER_REBOUND_FLASHER); //FLASH followed by name of flasher in caps
}



CALLSET_ENTRY (simple_switches, sw_sling) {
	score(SC_540);
	simple_sounds();
}



/****************************************************************************
 * DMD display and sound effects
 ****************************************************************************/
void simple_sounds (void) {
	simple_SoundCounter = random_scaled(4);//from kernal/random.c
	if ( simple_SoundCounter == 0 )
	sound_start (ST_EFFECT, ZAPP_1, SL_2S, PRI_GAME_QUICK5);
else if ( simple_SoundCounter == 1 )
	sound_start (ST_EFFECT, JOINK, SL_2S, PRI_GAME_QUICK5);
else if ( simple_SoundCounter == 2 )
	sound_start (ST_EFFECT, JOINK_2, SL_2S, PRI_GAME_QUICK5);
else if ( simple_SoundCounter == 3 )
	sound_start (ST_EFFECT, ZAPP_2, SL_2S, PRI_GAME_QUICK5);
}



void simple_effect_deff(void) {
	dmd_alloc_low_clean ();
	if (reboundCounter == 1) sprintf ("1 REBOUND");
	else	sprintf ("%d REBOUNDS", reboundCounter);
	font_render_string_center (&font_var5, DMD_BIG_CX_Top, DMD_BIG_CY_Top, sprintf_buffer);
	dmd_show_low ();
	task_sleep_sec (1);
	deff_exit ();
	}//end of mode_effect_deff

