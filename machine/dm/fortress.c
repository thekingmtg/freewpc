/*
 * demolition man
 * fortress.c
 * 
 * written by James Cardona
 *
 * handles the lock freezes and the trigger of multiballs
 *
 *
 * */
/* CALLSET_SECTION (fortress, __machine2__) */


#include <freewpc.h>
#include "dm/global_constants.h"

//local variables
score_t		fortress_score;
U8			fortress_jackpot_shots_made;
U8			fortress_MessageCounter;

//external variables
extern U8			NumBallsFrozen; //from lock_freeze_mbstart.c

/****************************************************************************
 * multiball definition structure
 ***************************************************************************/
struct mb_mode_ops fortress_mode = {
	DEFAULT_MBMODE,
	//.update = , /* The update callback is invoked whenever the state of the multiball changes. */
	.music = MUS_MB,
	.deff_starting = DEFF_FORTRESS_START_EFFECT,
	.deff_running = DEFF_FORTRESS_EFFECT,
	//.deff_ending = ,
//.active_task = 				//	default => .active_task = mb_mode_active_task,
	.prio = PRI_MULTIBALL,		//default => .prio = PRI_NULL,
//.grace_period = 				//default => .grace_period = 500ms
};
//task_gid_t gid_running;
//task_gid_t gid_in_grace;


/****************************************************************************
 * initialize  and exit
 ***************************************************************************/
void fortress_player_reset (void) {
	flag_off(FLAG_IS_FORTRESS_ACTIVATED);
	score_zero(fortress_score);
	fortress_jackpot_shots_made = 0;
}//end of function

CALLSET_ENTRY (fortress, start_player) { fortress_player_reset(); }



/****************************************************************************
 * external event listeners
 ****************************************************************************/
CALLSET_ENTRY (fortress, music_refresh)  { mb_mode_music_refresh (&fortress_mode); }
CALLSET_ENTRY (fortress, display_update) { mb_mode_display_update (&fortress_mode); }

CALLSET_ENTRY (fortress, end_ball) {
	callset_invoke(cramp_jackpot_light_off);
	if (flag_test(FLAG_IS_FORTRESS_ACTIVATED))
				mb_mode_end_ball (&fortress_mode);
}//end of function

/****************************************************************************
 * body
 *
 ***************************************************************************/
CALLSET_ENTRY (fortress, fortress_start) {
	//SOUNDS
			//music_timed_disable(5000);
			sound_start (ST_EFFECT, HELICOPTER, SL_4S, SP_NORMAL);
			task_sleep (TIME_2S);
			sound_start (ST_EFFECT, HELICOPTER, SL_4S, SP_NORMAL);
			U8 	fortress_SoundCounter;
			fortress_SoundCounter = random_scaled(2);//from kernal/random.c - pick number from 0 to 2
			if (fortress_SoundCounter == 0)
				sound_start (ST_SPEECH, SPCH_SOMETHING_RIGHT_PREV_LIFE, SL_4S, PRI_GAME_QUICK5);
			else
				sound_start (ST_SPEECH, SPCH_SEND_MANIAC, SL_4S, PRI_GAME_QUICK5);
	//LIGHTS
			lamp_tristate_flash(LM_FORTRESS_MULTIBALL);
			task_sleep (TIME_2S);
			lamp_tristate_on (LM_FORTRESS_MULTIBALL);
			lamp_tristate_off (LM_FREEZE_1);
			lamp_tristate_off (LM_FREEZE_2);
			lamp_tristate_off (LM_FREEZE_3);
			lamp_tristate_off (LM_FREEZE_4);
	flag_on(FLAG_IS_FORTRESS_ACTIVATED);
	mb_mode_start(&fortress_mode);
	//music_enable();
	callset_invoke(jackpot_init);//randomize which jackpot shot to make
	set_ball_count (2); //NumBallsFrozen
	callset_invoke(multiball_started);//reset all MB start criteria for next time
}//end of function



//jackpot shot
CALLSET_ENTRY (fortress, fortress_jackpot_made) {
	++fortress_jackpot_shots_made;
	U8 	fortress_SoundCounter;
	fortress_SoundCounter = random_scaled(4);//from kernal/random.c - pick number from 0 to 2
	if (fortress_SoundCounter == 0) 		sound_start (ST_SPEECH, SPCH_AHHHGGG, SL_4S, PRI_GAME_QUICK5);
	else if (fortress_SoundCounter == 1) 	sound_start (ST_SPEECH, SPCH_JACKPOT, SL_4S, PRI_GAME_QUICK5);
	else if (fortress_SoundCounter == 2) 	sound_start (ST_SPEECH, SPCH_DOUBLE_JACKPOT_WES, SL_4S, PRI_GAME_QUICK5);
	else 									sound_start (ST_SPEECH, SPCH_DOUBLE_JACKPOT_SLY, SL_4S, PRI_GAME_QUICK5);
	score (SC_15M);
	score_add (fortress_score, score_table[SC_15M]);
	fortress_jackpot_effect();//under /kernel/deff.c
}//end of function

/****************************************************************************
 * DMD display and sound effects
 ****************************************************************************/
void fortress_start_effect_deff(void) {
		U8 count = 8;
		dmd_alloc_pair_clean ();
		font_render_string_center (&font_amiga4ever, DMD_MIDDLE_X, DMD_BIG_CY_Top, "FORTRESS");
		font_render_string_center (&font_amiga4ever, DMD_MIDDLE_X, DMD_BIG_CY_Bot, "MULTIBALL");
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



void fortress_jackpot_effect(void) {
	switch (++fortress_MessageCounter % 3) {
		case 0:  deff_start (DEFF_FORTRESS_JACKPOT1_EFFECT); break;
		case 1:  deff_start (DEFF_FORTRESS_JACKPOT2_EFFECT); break;
		case 2:  deff_start (DEFF_FORTRESS_JACKPOT3_EFFECT); break;
		case 3:  deff_start (DEFF_FORTRESS_JACKPOT4_EFFECT); break;
		default: deff_start (DEFF_FORTRESS_JACKPOT1_EFFECT); break;
		}//end of switch
}//end of FUNCTION



void fortress_jackpot1_effect_deff(void) {
	U8 i;
	for (i=1; i < 8; i++) {			//SPELL OUT LETTRS
			dmd_alloc_low_clean ();
			sprintf ("JACKPOT");
			if (i < 7)
				sprintf_buffer[i] = '\0';
			font_render_string_center (&font_amiga4ever, 64, 16, sprintf_buffer);
			dmd_show_low ();
			task_sleep (TIME_300MS);
	}
	task_sleep (TIME_500MS);
	deff_exit ();
}//end of mode_effect_deff



void fortress_jackpot2_effect_deff(void) {
	U8 i;
	for (i=0; i < 8; i++) {
		dmd_sched_transition (&trans_scroll_up);
		dmd_alloc_low_clean ();
		font_render_string_center (&font_amiga4ever, 64, 16, "JACKPOT");
		dmd_show_low ();
	}
	task_sleep (TIME_500MS);
	deff_exit ();
}//end of mode_effect_deff



void fortress_jackpot3_effect_deff(void) {
	U8 i;
	for (i=0; i < 8; i++) {
		dmd_sched_transition (&trans_scroll_up);
		dmd_alloc_low_clean ();
		font_render_string_center (&font_amiga4ever, 64, 8, "JACKPOT");
		font_render_string_center (&font_amiga4ever, 64, 24, "JACKPOT");
		dmd_show_low ();
	}
	task_sleep (TIME_500MS);
	deff_exit ();
}//end of mode_effect_deff



void fortress_jackpot4_effect_deff(void) {
	dmd_alloc_pair ();
	dmd_clean_page_low ();
	font_render_string_center (&font_amiga4ever, 64, 16, "JACKPOT");
	dmd_copy_low_to_high ();
	dmd_show_low ();
	dmd_invert_page (dmd_low_buffer);
	deff_swap_low_high (25, TIME_100MS);
	task_sleep (TIME_500MS);
	deff_exit ();
}//end of mode_effect_deff



void fortress_effect_deff (void) {
	for (;;) {
		dmd_alloc_low_clean ();
		font_render_string_center (&font_amiga4ever, DMD_MIDDLE_X, DMD_BIG_CY_Top, "FORTRESS");
		sprintf ("%d JACKPOTS", fortress_jackpot_shots_made);
		font_render_string_center (&font_var5, DMD_MIDDLE_X, DMD_SMALL_CY_4, sprintf_buffer);
		dmd_show_low ();
		task_sleep (TIME_200MS);
	}//END OF ENDLESS LOOP
}//end of function
