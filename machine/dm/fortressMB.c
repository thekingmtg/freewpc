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
__boolean 	is_fortressMB_mode_activated;
score_t		fortressMB_score;
U8			fortressMB_jackpot_shots_made;


//external variables
extern U8			NumBallsFrozen; //from lock_freeze_mbstart.c

//prototypes
void fortressMB_player_reset (void);
void fortressMB_start_effect_deff(void);
void fortressMB_jackpot_effect_deff(void);
void fortressMB_effect_deff (void);


/****************************************************************************
 * multiball definition structure
 ***************************************************************************/
struct mb_mode_ops fortressMB_mode = {
	DEFAULT_MBMODE,
	.music = MUS_MB,
	.deff_starting = DEFF_FORTRESSMB_START_EFFECT,
	.deff_running = DEFF_FORTRESSMB_EFFECT,
//	.deff_ending = ,
//	.update = NULL,
//	.active_task = mb_mode_active_task,
	.prio = PRI_MULTIBALL,
//	.grace_period = 0, //default grace is 500ms
};




/****************************************************************************
 * initialize  and exit
 ***************************************************************************/
void fortressMB_player_reset (void) {
	is_fortressMB_mode_activated  = FALSE;
	score_zero(fortressMB_score);
	fortressMB_jackpot_shots_made = 0;
}//end of function

CALLSET_ENTRY (fortressMB, start_player) { fortressMB_player_reset(); }



/****************************************************************************
 * external event listeners
 ****************************************************************************/
CALLSET_ENTRY (fortressMB, music_refresh)  { mb_mode_music_refresh (&fortressMB_mode); }
CALLSET_ENTRY (fortressMB, display_update) { mb_mode_display_update (&fortressMB_mode); }

CALLSET_ENTRY (fortressMB, end_ball) {
	callset_invoke(CRamp_Jackpot_Light_Off);
	mb_mode_end_ball (&fortressMB_mode);
}//end of function



/****************************************************************************
 * body
 *
 ***************************************************************************/

//this is called from left loop shot at orbits.c
CALLSET_ENTRY (fortressMB, Fortress_start) {
	music_disable();
	sound_start (ST_EFFECT, HELICOPTER, SL_4S, SP_NORMAL);
	task_sleep (TIME_2S);
	sound_start (ST_EFFECT, HELICOPTER, SL_4S, SP_NORMAL);
	U8 	fortressMB_SoundCounter;
	fortressMB_SoundCounter = random_scaled(2);//from kernal/random.c - pick number from 0 to 2
	if (fortressMB_SoundCounter == 0)
		sound_start (ST_SPEECH, SPCH_SOMETHING_RIGHT_PREV_LIFE, SL_4S, PRI_GAME_QUICK5);
	else
		sound_start (ST_SPEECH, SPCH_SEND_MANIAC, SL_4S, PRI_GAME_QUICK5);

	lamp_tristate_flash(LM_FORTRESS_MULTIBALL);
	task_sleep (TIME_2S);
	lamp_tristate_on (LM_FORTRESS_MULTIBALL);
	lamp_tristate_off (LM_FREEZE_1);
	lamp_tristate_off (LM_FREEZE_2);
	lamp_tristate_off (LM_FREEZE_3);
	lamp_tristate_off (LM_FREEZE_4);
	is_fortressMB_mode_activated  = TRUE;
	mb_mode_start(&fortressMB_mode);
	music_enable();
	callset_invoke(CRamp_Jackpot_Light_On);
	set_ball_count (2); //NumBallsFrozen
}//end of function




//jackpot shot
CALLSET_ENTRY (fortressMB, fortressMB_jackpot_made) {
	++fortressMB_jackpot_shots_made;
	U8 	fortressMB_SoundCounter;
	fortressMB_SoundCounter = random_scaled(4);//from kernal/random.c - pick number from 0 to 2
	if (fortressMB_SoundCounter == 0) 		sound_start (ST_SPEECH, SPCH_AHHHGGG, SL_4S, PRI_GAME_QUICK5);
	else if (fortressMB_SoundCounter == 1) 	sound_start (ST_SPEECH, SPCH_JACKPOT, SL_4S, PRI_GAME_QUICK5);
	else if (fortressMB_SoundCounter == 2) 	sound_start (ST_SPEECH, SPCH_DOUBLE_JACKPOT_WES, SL_4S, PRI_GAME_QUICK5);
	else 									sound_start (ST_SPEECH, SPCH_DOUBLE_JACKPOT_SLY, SL_4S, PRI_GAME_QUICK5);
	score (SC_15M);
	score_add (fortressMB_score, score_table[SC_15M]);
	deff_start (DEFF_FORTRESSMB_JACKPOT_EFFECT);//under /kernel/deff.c
}//end of function





/****************************************************************************
 * DMD display and sound effects
 ****************************************************************************/
void fortressMB_start_effect_deff(void) {
		U8 count = 8;
		dmd_alloc_pair_clean ();
		font_render_string_center (&font_fixed10, DMD_BIG_CX_Top, DMD_BIG_CY_Top, "FORTRESS");
		font_render_string_center (&font_fixed10, DMD_BIG_CX_Bot, DMD_BIG_CY_Bot, "MULTIBALL");
		/* low = text, high = blank */
		while (--count > 0) {
			dmd_show2 ();
			task_sleep (TIME_100MS);
					dmd_flip_low_high ();
					dmd_show2 ();
					task_sleep (TIME_100MS);
			dmd_show_high ();
			task_sleep (TIME_200MS);
					dmd_show2 ();
					task_sleep (TIME_100MS);
					dmd_flip_low_high ();
		}//END OF WHILE LOOP
		deff_exit ();
}//end of mode_effect_deff



void fortressMB_jackpot_effect_deff(void) {
	U8 i;
	for (i=1; i < 8; i++) {
		dmd_alloc_low_clean ();
		sprintf ("JACKPOT");
		if (i < 7)
			sprintf_buffer[i] = '\0';
		font_render_string_center (&font_fixed10, 64, 16, sprintf_buffer);
		dmd_show_low ();
		task_sleep (TIME_300MS);
	}
	for (i=0; i < 8; i++) {
		dmd_sched_transition (&trans_scroll_up);
		dmd_alloc_low_clean ();
		font_render_string_center (&font_fixed10, 64, 16, "JACKPOT");
		dmd_show_low ();
	}
	for (i=0; i < 8; i++) {
		dmd_sched_transition (&trans_scroll_up);
		dmd_alloc_low_clean ();
		font_render_string_center (&font_fixed10, 64, 8, "JACKPOT");
		font_render_string_center (&font_fixed10, 64, 24, "JACKPOT");
		dmd_show_low ();
	}
	dmd_alloc_pair ();
	dmd_clean_page_low ();
	font_render_string_center (&font_fixed10, 64, 16, "JACKPOT");
	dmd_copy_low_to_high ();
	dmd_show_low ();
	dmd_invert_page (dmd_low_buffer);
	deff_swap_low_high (25, TIME_100MS);
	task_sleep (TIME_500MS);
	deff_exit ();
}//end of mode_effect_deff



void fortressMB_effect_deff (void) {
	for (;;) {
		dmd_alloc_low_clean ();
		font_render_string_center (&font_fixed10, DMD_BIG_CX_Top, DMD_BIG_CY_Top, "FORTRESS");
		sprintf ("%d JACKPOT", fortressMB_jackpot_shots_made);
		font_render_string_center (&font_mono5, DMD_SMALL_CX_3, DMD_SMALL_CY_3, sprintf_buffer);
		dmd_show_low ();
		task_sleep (TIME_200MS);
	}//END OF ENDLESS LOOP
}//end of function
