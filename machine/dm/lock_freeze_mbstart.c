/*
 * demolition man
 * lock_freeze_mbstart.c
 * 
 * written by James Cardona
 *
 * handles the lock freezes and the trigger of multiballs
 *
 *
 * */
/* CALLSET_SECTION (lock_freeze_mbstart, __machine3__) */

#include <freewpc.h>
#include "dm/global_constants.h"


//local variables
__local__ U8			NumBallsFrozen;
__local__ U8			NumMBsDone;
U8			lock_SoundCounter;
__local__ U8			NumBallsNeededForNextMB;

//external variables

//internally called function prototypes  --external found at protos.h
void lock_reset (void);
void lfmb_ball_reset (void);
void lfmb_player_reset (void);
void check_multiball_requirements(void);

/****************************************************************************
 * initialize  and exit
 ***************************************************************************/
void lock_reset (void) {
	multiball_light_off();//goto orbits.c to turn off light and flag
}//end of reset

void lfmb_ball_reset (void) {
	switch (NumBallsFrozen) {
case 1:	lamp_tristate_on (LM_FREEZE_1);	break;
case 2:
		lamp_tristate_on (LM_FREEZE_1);
		lamp_tristate_on (LM_FREEZE_2);
		break;
case 3:
		lamp_tristate_on (LM_FREEZE_1);
		lamp_tristate_on (LM_FREEZE_2);
		lamp_tristate_on (LM_FREEZE_3);
		break;
case 4:
		lamp_tristate_on (LM_FREEZE_1);
		lamp_tristate_on (LM_FREEZE_2);
		lamp_tristate_on (LM_FREEZE_3);
		lamp_tristate_on (LM_FREEZE_4);
		break;
}//end of switch
}



void lfmb_player_reset (void) {
	lock_reset();
	NumBallsFrozen = 0;
	lock_SoundCounter = 0;
	NumMBsDone = 0;
	NumBallsNeededForNextMB = 1;
}//end of function



CALLSET_ENTRY (lock_freeze_mbstart, start_player) { lfmb_player_reset(); }
CALLSET_ENTRY (lock_freeze_mbstart, start_ball) { lfmb_ball_reset(); }

/****************************************************************************
 * body
 *
 ***************************************************************************/
CALLSET_ENTRY (lock_freeze_mbstart, music_refresh) {
	if (in_live_game && flag_test(FLAG_IS_MULTIBALL_ACTIVATED) )	music_request (MUS_MB_READY, PRI_GAME_MODE1);//must be higher priority than PRI_SCORES
}//end of function



void maximize_freeze(void) {
	NumBallsFrozen = 3;
	increment_freeze(); //will add another here for 4 total
}//end of function


void increment_freeze(void) {
	sound_start (ST_EFFECT, LOCK_FREEZE_PLOINK, SL_1S, PRI_GAME_QUICK1);
	if (NumBallsFrozen < 4)
			switch (++NumBallsFrozen) {
			case 1:
					lamp_tristate_flash(LM_FREEZE_1);
					task_sleep (TIME_500MS);
					lamp_tristate_on (LM_FREEZE_1);
					break;
			case 2:
					lamp_tristate_flash(LM_FREEZE_2);
					task_sleep (TIME_500MS);
					lamp_tristate_on (LM_FREEZE_1);
					lamp_tristate_on (LM_FREEZE_2);
					break;
			case 3:
					lamp_tristate_flash(LM_FREEZE_3);
					task_sleep (TIME_500MS);
					lamp_tristate_on (LM_FREEZE_1);
					lamp_tristate_on (LM_FREEZE_2);
					lamp_tristate_on (LM_FREEZE_3);
					break;
			case 4:
					lamp_tristate_flash(LM_FREEZE_4);
					task_sleep (TIME_500MS);
					lamp_tristate_on (LM_FREEZE_1);
					lamp_tristate_on (LM_FREEZE_2);
					lamp_tristate_on (LM_FREEZE_3);
					lamp_tristate_on (LM_FREEZE_4);
					break;
			}//end of switch
	deff_start (DEFF_FREEZE_EFFECT);
	deactivate_left_ramp_quickfreeze();//goto ramps.c
	light_quick_freeze_light_off();//goto inlanes.c
	check_multiball_requirements();
}//end of function




void check_multiball_requirements(void) {
	//if requirements met for next mb - light left loop shot to start mb
	//Fortress Multiball 	= 1 ball min needs to be frozen
	//Museum Multiball	 	= 2 ball min needs to be frozen
	//Cryoprison Multiball	= 3 ball min needs to be frozen
	//Wasteland Multiball 	= 4 ball min needs to be frozen
	if (NumBallsFrozen > (NumMBsDone % 4) ) { // % is modulus
				multiball_light_on();//goto orbits.c
				flag_on(FLAG_IS_MULTIBALL_ACTIVATED);
				if ( (lock_SoundCounter++ % 2) == 0 )//check if even
					sound_start (ST_SPEECH, SPCH_MULTIBALL_ACTIVATED, SL_4S, PRI_GAME_QUICK5);
				else
					sound_start (ST_SPEECH, SPCH_SHOOT_LEFT_LOOP, SL_4S, PRI_GAME_QUICK5);
	}  //end of if (NumBallsFrozen > (NumMBsDone % 4) )
}//end of function




//this is called from left loop shot at orbits.c
void multiball_start (void) {
	if 		( (NumMBsDone % 4) == 0) 	fortress_start();
	else if ( (NumMBsDone % 4) == 1) 	museum_start();
	else if ( (NumMBsDone % 4) == 2) 	cryoprison_start();
	else if ( (NumMBsDone % 4) == 3) 	wasteland_start();
}//end of function


//after we are sure we have a valid MB start, we reset everything for next time
void multiball_started(void) {
	NumMBsDone++;
	if (NumBallsNeededForNextMB < 4) ++NumBallsNeededForNextMB;
	flag_off(FLAG_IS_MULTIBALL_ACTIVATED);
	lock_reset();
}//end of function


/****************************************************************************
 *
 * display effects
 *
 ****************************************************************************/
void freeze_effect_deff(void) {
	U8 i = 12;
	do {
		dmd_alloc_pair_clean();
		font_render_string_center( &font_fixed10, DMD_MIDDLE_X - (i*10), 	DMD_BIG_CY_Top, "F");
		font_render_string_center( &font_fixed10, DMD_MIDDLE_X - (i*5), 	DMD_BIG_CY_Top, "R");
		font_render_string_center( &font_fixed10, DMD_MIDDLE_X, 			DMD_BIG_CY_Top, "EE"); //right in middle
		font_render_string_center( &font_fixed10, DMD_MIDDLE_X + (i*5), 	DMD_BIG_CY_Top, "Z");
		font_render_string_center( &font_fixed10, DMD_MIDDLE_X + (i*10), 	DMD_BIG_CY_Top, "E");
		dmd_show_low ();
		task_sleep (TIME_100MS);
	} while (i-- > 4);//about XXsec
	task_sleep (TIME_500MS);


	dmd_alloc_pair_clean();
	dmd_sched_transition (&trans_bitfade_fast);
	sprintf ("%d FROZEN", NumBallsFrozen);
	font_render_string_center (&font_term6, DMD_MIDDLE_X, DMD_BIG_CY_Top, sprintf_buffer);
	if (NumBallsFrozen >= NumBallsNeededForNextMB)
				sprintf ("MULTIBALL READY");
	else		sprintf ("%d MORE FOR MB", NumBallsNeededForNextMB - NumBallsFrozen);
	font_render_string_center (&font_term6, DMD_MIDDLE_X, DMD_BIG_CY_Bot, sprintf_buffer);
	dmd_show_low ();
	task_sleep_sec (2);
	deff_exit ();
}//end of mode_effect_deff
