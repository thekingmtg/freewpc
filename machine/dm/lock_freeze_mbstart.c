/*
 * demolition man
 * lock_freeze_mbstart.c
 * 
 * handles the lock freezes and the trigger of multiballs
 *
 * */

#include <freewpc.h>

//local variables
__boolean 	ball_one_frozen;
__boolean 	ball_two_frozen;
__boolean 	ball_three_frozen;
__boolean 	ball_four_frozen;
U8			NumBallsFrozen;
U8			NumMBsDone;
U8			lock_SoundCounter;
//prototypes
void lock_reset (void);


/****************************************************************************
 * initialize  and exit
 ***************************************************************************/
void lock_reset (void) {
	ball_one_frozen = FALSE;
	ball_two_frozen = FALSE;
	ball_three_frozen = FALSE;
	ball_four_frozen = FALSE;
	NumBallsFrozen = 0;
}//end of reset

void player_reset (void) {
	lock_SoundCounter = 0;
	NumMBsDone = 0;
	lock_reset();
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
			ball_one_frozen = TRUE;
			lamp_tristate_flash(LM_FREEZE_1);
			task_sleep (TIME_500MS);
			lamp_tristate_on (LM_FREEZE_1);
			break;
	case 2:
			ball_two_frozen = TRUE;
			lamp_tristate_flash(LM_FREEZE_2);
			task_sleep (TIME_500MS);
			lamp_tristate_on (LM_FREEZE_2);
			break;
	case 3:
			ball_three_frozen = TRUE;
			lamp_tristate_flash(LM_FREEZE_3);
			task_sleep (TIME_500MS);
			lamp_tristate_on (LM_FREEZE_3);
			break;
	case 4:
			ball_four_frozen = TRUE;
			lamp_tristate_flash(LM_FREEZE_4);
			task_sleep (TIME_500MS);
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
	if (NumBallsFrozen > (NumMBsDone % 4) ) { //% is modulus
				callset_invoke (Multiball_Light_On);//goto orbits.c
				sound_start (ST_MUSIC, MUS_MB_READY, 0, SP_NORMAL);
				if ( (lock_SoundCounter++ % 2) == 0 )//check if even
					sound_start (ST_SPEECH, SPCH_MULTIBALL_ACTIVATED, SL_1S, PRI_GAME_QUICK5);
				else
					sound_start (ST_SPEECH, SPCH_SHOOT_LEFT_LOOP, SL_1S, PRI_GAME_QUICK5);

	}//end of if
}//end of function


//this is called from left loop shot at orbits.c
CALLSET_ENTRY (lock_freeze_mbstart, multiball_start) {
	NumMBsDone++;
	callset_invoke (Multiball_Light_Off);//goto orbits.c to turn off light and flag
	if ( (NumMBsDone % 4) == 1) {
		sound_start (ST_MUSIC, MUS_MB, 0, SP_NORMAL);
		sound_start (ST_EFFECT, HELICOPTER, SL_2S, SP_NORMAL);
		if ( (lock_SoundCounter++ % 2) == 0 )//check if even
			sound_start (ST_SPEECH, SPCH_SOMETHING_RIGHT_PREV_LIFE, SL_2S, PRI_GAME_QUICK5);
		else
			sound_start (ST_SPEECH, SPCH_SEND_MANIAC, SL_2S, PRI_GAME_QUICK5);
		lamp_tristate_flash(LM_FORTRESS_MULTIBALL);
		task_sleep (TIME_500MS);
		lamp_tristate_on (LM_FORTRESS_MULTIBALL);
		//here we call the appropriate multiball start function
		}
	if ((NumMBsDone % 4) == 2) {
		sound_start (ST_MUSIC, MUS_MB, 0, SP_NORMAL);
		sound_start (ST_EFFECT, EXPLOSION, SL_2S, SP_NORMAL);
		task_sleep (TIME_500MS);
		if ( (lock_SoundCounter++ % 2) == 0 )//check if even
			sound_start (ST_SPEECH, SPCH_BLAST_FROM_THE_PAST, SL_2S, PRI_GAME_QUICK5);
		else
			sound_start (ST_SPEECH, SPCH_BEST_DAY_OF_MY_LIFE, SL_2S, PRI_GAME_QUICK5);
		lamp_tristate_flash(LM_MUSEUM_MULTIBALL);
		task_sleep (TIME_500MS);
		lamp_tristate_on (LM_MUSEUM_MULTIBALL);
		//here we call the appropriate multiball start function
		}
	if ((NumMBsDone % 4) == 3) {
		sound_start (ST_MUSIC, MUS_MB, 0, SP_NORMAL);
		sound_start (ST_EFFECT, SIREN, SL_2S, SP_NORMAL);
		if ( (lock_SoundCounter++ % 2) == 0 )//check if even
			sound_start (ST_SPEECH, SPCH_MEN_DEFROSTED, SL_1S, PRI_GAME_QUICK5);
		else
			sound_start (ST_SPEECH, SPCH_IS_IT_COLD_IN_HERE, SL_2S, PRI_GAME_QUICK5);
		lamp_tristate_flash(LM_CRYOPRISON_MULTIBALL);
		task_sleep (TIME_500MS);
		lamp_tristate_on (LM_CRYOPRISON_MULTIBALL);
		//here we call the appropriate multiball start function
		}
	if ((NumMBsDone % 4) == 0) {//since we use mod, 4 = 0
		sound_start (ST_MUSIC, MUS_MB, 0, SP_NORMAL);
		sound_start (ST_EFFECT, SIREN, SL_2S, SP_NORMAL);
		if ( (lock_SoundCounter++ % 2) == 0 )//check if even
			sound_start (ST_SPEECH, SPCH_PHOENIX_LONG, SL_1S, PRI_GAME_QUICK5);
		else
			sound_start (ST_SPEECH, SPCH_PREPARE_FOR_SIMON, SL_2S, PRI_GAME_QUICK5);
		lamp_tristate_flash(LM_WASTELAND_MULTIBALL);
		task_sleep (TIME_500MS);
		lamp_tristate_on (LM_WASTELAND_MULTIBALL);
		//here we call the appropriate multiball start function
		}

	//TODO:write multiball functions - send out right num of frozen balls
	//TODO:reset all MB lights at end of wasteland MB

	//turn off freeze light and reset counter for next MB
	lock_reset();
}//end of function
