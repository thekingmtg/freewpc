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
const sound_code_t outlaneSoundArray[] = {	SPCH_TOO_BAD, 		SPCH_PATHETIC,		SPCH_HATE_WHEN_THAT_HAPPENS,
											SPCH_OHH_NO,		SPCH_BAD_AIM,		SPCH_DULCH,
											SPCH_WES_LAUGH_SHORT1, SPCH_SIMON_SNICKER, SPCH_WES_LAUGH_SHORT2,
											SPCH_WES_LAUGH_SHORT3, SPCH_WOAH, 			SPCH_JUST_LOVELY };


const U8 	ball_save_TotalNumOfSounds = 6; //num between 0 and 24 == 25 total

sound_code_t ball_save_SoundsArray[] = {	SPCH_AINT_OVER_YET,		SPCH_DONT_MOVE_PHOENIX,		SPCH_DONT_MOVE,
											SPCH_SECOND_CHANCE,		SPCH_HEADS_UP,				SPCH_NEXT_SHOT};



//local variables

//external variables

//prototypes
void simple_sounds(void);
void dm_launch_ball_task (void);

/****************************************************************************
 * initialize  and exit
 ***************************************************************************/

/****************************************************************************
 * body
 *
 ****************************************************************************/
CALLSET_ENTRY (simple_switches, sw_right_outlane, sw_left_outlane) {
	score(SC_5770);
	U8		outlaneSoundCounter;
	outlaneSoundCounter = random_scaled(SSTotalNumOfSounds);//from kernal/random.c
	sound_start (ST_SPEECH, outlaneSoundArray[outlaneSoundCounter], SL_5S, PRI_GAME_QUICK5);
}


CALLSET_ENTRY (simple_switches, sw_upper_left_flipper_gate) { score(SC_1010); }

/****************************************************************************
 * firing of ball work around
 * see plunger.c and serve.c - code doesn't work
 ***************************************************************************/
CALLSET_ENTRY (simple_switches, sw_ball_launch_button) {
	//copied from plunger.c and serve.c
	if (in_live_game)
		task_create_gid1 (GID_BALL_LAUNCH_BALL, dm_launch_ball_task);
}


void dm_launch_ball_task (void) {
	if (!switch_poll_logical (MACHINE_SHOOTER_SWITCH))
		task_sleep (TIME_500MS);
	do {
		sol_request_async (MACHINE_LAUNCH_SOLENOID);
		task_sleep (TIME_3S);
	} while (switch_poll_logical (MACHINE_SHOOTER_SWITCH));
	task_exit ();
}//end of function
















CALLSET_ENTRY (simple_switches, sw_upper_rebound) {
	score(SC_1010);
	simple_sounds();
}



CALLSET_ENTRY (simple_switches, sw_lower_rebound) {
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
}



//DEFF_BALL_SAVE
CALLSET_ENTRY (simple_switches, start_custom_ball_save_deff) {	deff_start (DEFF_BALL_SAVE_EFFECT);}

//DEFF_BALL_SAVE
void ball_save_effect_deff(void) {
	U8 ball_save_SoundCounter;
	ball_save_SoundCounter = random_scaled(ball_save_TotalNumOfSounds);//from kernal/random.c
	sound_start (ST_SPEECH, ball_save_SoundsArray[ball_save_SoundCounter], SL_5S, PRI_GAME_QUICK5);
	dmd_alloc_low_clean ();
	sprintf ("BALL SAVE");
	font_render_string_center (&font_var5, DMD_BIG_CX_Top, DMD_BIG_CY_Top, sprintf_buffer);
	dmd_show_low ();
	task_sleep_sec (2);
	deff_exit ();
}//end of deff
















//this is used for in game trouble shooting
CALLSET_ENTRY (simple_switches, sw_left_handle_button) {deff_start (DEFF_TROUBLESHOOTING);}


//DEFF_BALL_SAVE
void troubleshooting_deff(void) {
	dmd_alloc_low_clean ();
	if (in_game) sprintf ("IG"); else sprintf ("NT IG");
	font_render_string_center (&font_var5, 20 , 5, sprintf_buffer);



	if (in_live_game) sprintf ("ILG"); else sprintf ("NT ILG");
	font_render_string_center (&font_var5, 60 , 5, sprintf_buffer);


#if defined(MACHINE_LAUNCH_SWITCH)
	sprintf ("MLSW");
#else sprintf ("NT MLSW");
#endif
	font_render_string_center (&font_var5, 100 , 5, sprintf_buffer);


#if defined(MACHINE_LAUNCH_SOLENOID)
	sprintf ("SOL");
#else sprintf ("NT SOL");
#endif
	font_render_string_center (&font_var5, 30 , 11, sprintf_buffer);


#if 	defined(MACHINE_SHOOTER_SWITCH)
	sprintf ("SHT");
#else sprintf ("NT SHT");
#endif
	font_render_string_center (&font_var5, 80 , 11, sprintf_buffer);



#if defined(INCLUDE_AUTOPLUNGER)
	sprintf ("PL");
#else sprintf ("NT PL");
#endif
	font_render_string_center (&font_var5, 30 , 17, sprintf_buffer);


#if defined(HAVE_AUTO_SERVE)
	sprintf ("SRV");
#else sprintf ("NT SRV");
#endif
	font_render_string_center (&font_var5, 80 , 17, sprintf_buffer);
dmd_show_low ();
	task_sleep_sec (4);
	deff_exit ();
}//end of deff

