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

U8			NumBallsFrozen;
U8			NumMBsDone;
U8			lock_SoundCounter;
U8			NumBallsNeededForNextMB;

//external variables

//prototypes
void lock_reset (void);
void player_reset (void);
/****************************************************************************
 * initialize  and exit
 ***************************************************************************/
void lock_reset (void) {
	is_ball_one_frozen = FALSE;
	is_ball_two_frozen = FALSE;
	is_ball_three_frozen = FALSE;
	is_ball_four_frozen = FALSE;
	NumBallsFrozen = 0;
	callset_invoke (Multiball_Light_Off);//goto orbits.c to turn off light and flag
}//end of reset

void player_reset (void) {
	lock_reset();
	lock_SoundCounter = 0;
	NumMBsDone = 0;
	NumBallsNeededForNextMB = 1;
}//end of function


CALLSET_ENTRY (lock_freeze_mbstart, start_player) { player_reset(); }


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
	callset_invoke(start_freeze_deff);
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


				//music_disable();
				music_request (MUS_MB_READY, PRI_GAME_MODE8);//must be higher priority than PRI_SCORES
				//no work - music_set (MUS_MB_READY); //from sound_effect.c
				//music_enable();


				if ( (lock_SoundCounter++ % 2) == 0 )//check if even
					sound_start (ST_SPEECH, SPCH_MULTIBALL_ACTIVATED, SL_4S, PRI_GAME_QUICK5);
				else
					sound_start (ST_SPEECH, SPCH_SHOOT_LEFT_LOOP, SL_4S, PRI_GAME_QUICK5);
	}  //end of if (NumBallsFrozen > (NumMBsDone % 4) )
}//end of function



//this is called from left loop shot at orbits.c
CALLSET_ENTRY (lock_freeze_mbstart, multiball_start) {
	if 		( (NumMBsDone % 4) == 1) 	callset_invoke (Fortress_start);//goto fortressMB.c
	else if ( (NumMBsDone % 4) == 2) 	callset_invoke (Museum_start);//goto museumMB.c
	else if ( (NumMBsDone % 4) == 3) 	callset_invoke (Cryoprison_start);//goto cryoprisonMB.c
	else 								callset_invoke (Wasteland_start);//goto wastelandMB.c
	//turn off freeze light and reset counter for next MB
	NumMBsDone++;
	++NumBallsNeededForNextMB;
	lock_reset();
}//end of function




