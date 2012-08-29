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

#include <freewpc.h>
#include "dm/global_constants.h"


//local variables
__boolean 	is_ball_one_frozen;
__boolean 	is_ball_two_frozen;
__boolean 	is_ball_three_frozen;
__boolean 	is_ball_four_frozen;
__boolean 	fortressMB;
__boolean 	museumMB;
__boolean 	cryoMB;
__boolean 	wastelandMB;

U8			NumBallsFrozen;
U8			NumMBsDone;
U8			lock_SoundCounter;
U8			NumBallsNeededForNextMB;
//external variables

//prototypes
void lock_reset (void);

/****************************************************************************
 * initialize  and exit
 ***************************************************************************/
void lock_reset (void) {
	is_ball_one_frozen = FALSE;
	is_ball_two_frozen = FALSE;
	is_ball_three_frozen = FALSE;
	is_ball_four_frozen = FALSE;
	fortressMB = FALSE;
	museumMB = FALSE;
	cryoMB = FALSE;
	wastelandMB = FALSE;
	NumBallsFrozen = 0;
	callset_invoke(Multiball_Light_Off);
}//end of reset

void player_reset (void) {
	lock_SoundCounter = 0;
	NumMBsDone = 0;
	lock_reset();
	NumBallsNeededForNextMB = 1;
}


CALLSET_ENTRY (lock_freeze_mbstart, start_player) { player_reset(); }

// need to decide what we are doing for a new ball --start_ball
//ALLSET_ENTRY (lock_freeze_mbstart, mb_exit) { lock_reset(); }


/****************************************************************************
 * body
 *
 ***************************************************************************/
CALLSET_ENTRY (lock_freeze_mbstart, increment_freeze) {
	sound_start (ST_EFFECT, LOCK_FREEZE_PLOINK, SL_1S, PRI_GAME_QUICK1);
		switch (++NumBallsFrozen) {
	case 1:
			is_ball_one_frozen = TRUE;
			lamp_tristate_flash(LM_FREEZE_1);
			task_sleep (TIME_500MS);
			lamp_tristate_on (LM_FREEZE_1);
			break;
	case 2:
			is_ball_one_frozen = TRUE;
			is_ball_two_frozen = TRUE;
			lamp_tristate_flash(LM_FREEZE_2);
			task_sleep (TIME_500MS);
			lamp_tristate_on (LM_FREEZE_1);
			lamp_tristate_on (LM_FREEZE_2);
			break;
	case 3:
			is_ball_one_frozen = TRUE;
			is_ball_two_frozen = TRUE;
			is_ball_three_frozen = TRUE;
			lamp_tristate_flash(LM_FREEZE_3);
			task_sleep (TIME_500MS);
			lamp_tristate_on (LM_FREEZE_1);
			lamp_tristate_on (LM_FREEZE_2);
			lamp_tristate_on (LM_FREEZE_3);
			break;
	case 4:
			is_ball_one_frozen = TRUE;
			is_ball_two_frozen = TRUE;
			is_ball_three_frozen = TRUE;
			is_ball_four_frozen = TRUE;
			lamp_tristate_flash(LM_FREEZE_4);
			task_sleep (TIME_500MS);
			lamp_tristate_on (LM_FREEZE_1);
			lamp_tristate_on (LM_FREEZE_2);
			lamp_tristate_on (LM_FREEZE_3);
			lamp_tristate_on (LM_FREEZE_4);
			break;
	}//end of switch
	callset_invoke(DeActivate_left_Ramp_QuickFreeze);//goto ramps.c
	callset_invoke(light_quick_freeze_light_off);//goto inlanes.c
	callset_invoke(check_multiball_requirements);
}//end of function


CALLSET_ENTRY (lock_freeze_mbstart, check_multiball_requirements) {
	//if requirements met for next mb - light left loop shot to start mb
	//Fortress Multiball 	= 1 ball min needs to be frozen
	//Museum Multiball	 	= 2 ball min needs to be frozen
	//Cryoprison Multiball	= 3 ball min needs to be frozen
	//Wasteland Multiball 	= 4 ball min needs to be frozen
	if (NumBallsFrozen > (NumMBsDone % 4) ) { // % is modulus
				callset_invoke (Multiball_Light_On);//goto orbits.c
				music_disable();
				if ( (lock_SoundCounter++ % 2) == 0 )//check if even
					sound_start (ST_SPEECH, SPCH_MULTIBALL_ACTIVATED, SL_5S, PRI_GAME_QUICK5);
				else
					sound_start (ST_SPEECH, SPCH_SHOOT_LEFT_LOOP, SL_5S, PRI_GAME_QUICK5);
				music_set (MUS_MB_READY); //from sound_effect.c
	}//end of if (NumBallsFrozen > (NumMBsDone % 4) )
}//end of function



//this is called from left loop shot at orbits.c
CALLSET_ENTRY (lock_freeze_mbstart, multiball_start) {
	NumMBsDone++;
	//TODO: find out why this crashes the cpu
	//deff_start (DEFF_MULTIBALL_EFFECT);
	callset_invoke (Multiball_Light_Off);//goto orbits.c to turn off light and flag

	//Fortress Multiball 	= 1 ball min needs to be frozen
	if ( (NumMBsDone % 4) == 1) {
		music_disable();
		sound_start (ST_EFFECT, HELICOPTER, SL_5S, SP_NORMAL);
		task_sleep (TIME_1S);
		if ( (lock_SoundCounter++ % 2) == 0 )//check if even
			sound_start (ST_SPEECH, SPCH_SOMETHING_RIGHT_PREV_LIFE, SL_5S, PRI_GAME_QUICK5);
		else
			sound_start (ST_SPEECH, SPCH_SEND_MANIAC, SL_5S, PRI_GAME_QUICK5);
		fortressMB = TRUE;
		lamp_tristate_flash(LM_FORTRESS_MULTIBALL);
		task_sleep (TIME_2S);
		lamp_tristate_on (LM_FORTRESS_MULTIBALL);
		music_set(MUS_MB);
		lamp_tristate_off (LM_FREEZE_1);
		lamp_tristate_off (LM_FREEZE_2);
		lamp_tristate_off (LM_FREEZE_3);
		lamp_tristate_off (LM_FREEZE_4);
		++NumBallsNeededForNextMB;
		//TODO: here we call the Fortress multiball start function
		//Add COUNT balls into play from the trough
		add_ball_count(1); //located at /common/serve.c - will add balls to playfield from trough
		/**
		 * Set the total number of balls in play to COUNT.
		 */
		//set_ball_count (NumBallsFrozen + 1);
	}//end of Fortress Multiball

	//Museum Multiball	 	= 2 ball min needs to be frozen
	if ((NumMBsDone % 4) == 2) {
		music_disable();
		sound_start (ST_EFFECT, EXPLOSION, SL_2S, SP_NORMAL);
		task_sleep (TIME_500MS);
		if ( (lock_SoundCounter++ % 2) == 0 )//check if even
			sound_start (ST_SPEECH, SPCH_BLAST_FROM_THE_PAST, SL_5S, PRI_GAME_QUICK5);
		else
			sound_start (ST_SPEECH, SPCH_BEST_DAY_OF_MY_LIFE, SL_5S, PRI_GAME_QUICK5);
		lamp_tristate_flash(LM_MUSEUM_MULTIBALL);
		task_sleep (TIME_2S);
		museumMB = TRUE;
lamp_tristate_on (LM_MUSEUM_MULTIBALL);
		lamp_tristate_off (LM_FREEZE_1);
		lamp_tristate_off (LM_FREEZE_2);
		lamp_tristate_off (LM_FREEZE_3);
		lamp_tristate_off (LM_FREEZE_4);
		music_set(MUS_MB);
		++NumBallsNeededForNextMB;
		//TODO: here we call the Museum multiball start function
		add_ball_count(NumBallsFrozen + 1); //located at /common/serve.c - will add balls to playfield from trough
		}//end of Museum Multiball

	//Cryoprison Multiball	= 3 ball min needs to be frozen
	if ((NumMBsDone % 4) == 3) {
		music_disable();
		sound_start (ST_EFFECT, SIREN, SL_5S, SP_NORMAL);
		if ( (lock_SoundCounter++ % 2) == 0 )//check if even
			sound_start (ST_SPEECH, SPCH_MEN_DEFROSTED, SL_5S, PRI_GAME_QUICK5);
		else
			sound_start (ST_SPEECH, SPCH_IS_IT_COLD_IN_HERE, SL_5S, PRI_GAME_QUICK5);
		lamp_tristate_flash(LM_CRYOPRISON_MULTIBALL);
		task_sleep (TIME_2S);
		cryoMB = TRUE;
		lamp_tristate_on (LM_CRYOPRISON_MULTIBALL);
		music_set(MUS_MB);
		lamp_tristate_off (LM_FREEZE_1);
		lamp_tristate_off (LM_FREEZE_2);
		lamp_tristate_off (LM_FREEZE_3);
		lamp_tristate_off (LM_FREEZE_4);
		++NumBallsNeededForNextMB;
		//here we call the Cryoprison multiball start function
		add_ball_count(NumBallsFrozen + 1); //located at /common/serve.c - will add balls to playfield from trough
		}//end of Cryoprison Multiball

	//Wasteland Multiball 	= 4 ball min needs to be frozen
	if ((NumMBsDone % 4) == 0) {//since we use mod, 4 = 0
		music_disable();
		sound_start (ST_EFFECT, SIREN, SL_5S, SP_NORMAL);
		if ( (lock_SoundCounter++ % 2) == 0 )//check if even
			sound_start (ST_SPEECH, SPCH_PHOENIX_LONG, SL_5S, PRI_GAME_QUICK5);
		else
			sound_start (ST_SPEECH, SPCH_PREPARE_FOR_SIMON, SL_5S, PRI_GAME_QUICK5);
		wastelandMB = TRUE;
		lamp_tristate_flash(LM_WASTELAND_MULTIBALL);
		task_sleep (TIME_2S);
		lamp_tristate_on (LM_WASTELAND_MULTIBALL);
		lamp_tristate_off (LM_FREEZE_1);
		lamp_tristate_off (LM_FREEZE_2);
		lamp_tristate_off (LM_FREEZE_3);
		lamp_tristate_off (LM_FREEZE_4);
		music_set(MUS_MB);
		NumBallsNeededForNextMB = 1 ;
		//here we call the Wasteland multiball start function
		add_ball_count(NumBallsFrozen + 1); //located at /common/serve.c - will add balls to playfield from trough
		}//end of Wasteland Multiball

	//TODO: write multiball functions
	//TODO: reset all MB lights at end of wasteland MB

	//turn off freeze light and reset counter for next MB
	lock_reset();
}//end of function


/****************************************************************************
 * DMD display and sound effects
 ****************************************************************************/
void freeze_effect_deff(void) {
	dmd_alloc_low_clean ();
	sprintf ("%d FROZEN", NumBallsFrozen);
	font_render_string_center (&font_term6, DMD_BIG_CX_Top, DMD_BIG_CY_Top, sprintf_buffer);
	sprintf ("%d BALLS FOR MB", NumBallsNeededForNextMB);
	font_render_string_center (&font_term6, DMD_BIG_CX_Bot, DMD_BIG_CY_Bot, sprintf_buffer);
	dmd_show_low ();
	task_sleep_sec (2);
	deff_exit ();
	}//end of mode_effect_deff



void multiball_effect_deff(void) {
	dmd_alloc_low_clean ();
	font_render_string_center (&font_fixed10, DMD_BIG_CX_Top, DMD_BIG_CY_Top, "MULTIBALL");
	dmd_show_low ();
	task_sleep_sec (2);
	deff_exit ();
	}//end of mode_effect_deff



/****************************************************************************
 * status display
 *
 * called from common/status.c automatically whenever either flipper button
 * is held for 4 seconds or longer.  since called by callset, order of
 * various status reports will be random depending upon call stack
****************************************************************************/
CALLSET_ENTRY (lock_freeze_mbstart, status_report){
//	sprintf ("%d MULTIBALLS", NumMBsDone);
//	font_render_string_left (&font_fixed10, 1, 1, sprintf_buffer);

//	sprintf ("Balls Frozen: %d", NumBallsFrozen);
//	font_render_string_center (&font_mono5, 64, 7, sprintf_buffer);

//	if (NumBallsFrozen >= NumBallsNeededForNextMB) 	sprintf ("Multiball Ready");
//	else sprintf ("Freeze %d More Balls For Next MultiBall", NumBallsNeededForNextMB - NumBallsFrozen);
//	font_render_string_center (&font_mono5, 64, 13, sprintf_buffer);

/*	switch (NumMBsDone % 4 == 0) {
		case 0: 	sprintf ("Next MultiBall is Fortress"); 	break;
		case 1: 	sprintf ("Next MultiBall is Museum");  		break;
		case 2: 	sprintf ("Next MultiBall is Cryo Prison");	break;
		case 3: 	sprintf ("Next MultiBall is Wasteland");	break;
	}//end of switch
	font_render_string_center (&font_mono5, 64, 19, sprintf_buffer);
*/
}//end of function
