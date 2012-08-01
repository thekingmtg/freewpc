/*
 * demolition man
 * inlanes.c
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
 * */

#include <freewpc.h>

//local variables
U8		simple_SoundCounter = 0;

//prototypes
void simple_sounds(void);



/****************************************************************************
 * body
 *
 ****************************************************************************/
CALLSET_ENTRY (simple_switches, sw_right_outlane) {
	score(SC_5770);
	simple_sounds();
}

CALLSET_ENTRY (simple_switches, sw_left_outlane) {
	score(SC_5770);
	simple_sounds();
}

CALLSET_ENTRY (simple_switches, sw_upper_rebound) {
	score(SC_1010);
	simple_sounds();
}

CALLSET_ENTRY (simple_switches, sw_upper_left_flipper_gate) {
	score(SC_1010);
	simple_sounds();
}

CALLSET_ENTRY (simple_switches, sw_lower_rebound) {
	score(SC_1010);
	flasher_pulse (FLASH_LOWER_REBOUND_FLASHER); //FLASH followed by name of flasher in caps
	simple_sounds();
}


void simple_sounds (void) {
	simple_SoundCounter = random_scaled(4);//from kernal/random.c

	if ( simple_SoundCounter == 0 )
	sound_start (ST_EFFECT, ZAPP_1, SL_1S, PRI_GAME_QUICK5);
else if ( simple_SoundCounter == 1 )
	sound_start (ST_EFFECT, JOINK, SL_1S, PRI_GAME_QUICK5);
else if ( simple_SoundCounter == 2 )
	sound_start (ST_EFFECT, JOINK_2, SL_1S, PRI_GAME_QUICK5);
else if ( simple_SoundCounter == 3 )
	sound_start (ST_EFFECT, ZAPP_2, SL_1S, PRI_GAME_QUICK5);
}
