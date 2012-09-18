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


#include <freewpc.h>
#include "dm/global_constants.h"

//local variables
__boolean 	is_fortress_mode_activated;
score_t		fortress_score;
U8			fortress_jackpot_shots_made;


//external variables
extern U8			NumBallsFrozen; //from lock_freeze_mbstart.c

//prototypes
void fortress_player_reset (void);
void fortress_start_effect_deff(void);
void fortress_jackpot_effect_deff(void);
void fortress_effect_deff (void);


/****************************************************************************
 * multiball definition structure
 ***************************************************************************/
struct mb_mode_ops fortress_mode = {
	DEFAULT_MBMODE,
	.music = MUS_MB,
	.deff_starting = DEFF_FORTRESS_START_EFFECT,
	.deff_running = DEFF_FORTRESS_EFFECT,
//.deff_ending = , 	//	default => .deff_ending = ,
//.update = , 		//	default => .update = NULL,
//.active_task = 	//	default => .active_task = mb_mode_active_task,
	.prio = PRI_MULTIBALL,
//.grace_period = 	//default => .grace_period = 500ms
};




/****************************************************************************
 * initialize  and exit
 ***************************************************************************/
void fortress_player_reset (void) {
	is_fortress_mode_activated  = FALSE;
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
	mb_mode_end_ball (&fortress_mode);
}//end of function



/****************************************************************************
 * body
 *
 ***************************************************************************/

//this is called from left loop shot at orbits.c

CALLSET_ENTRY (fortress, fortress_start) {
	music_disable();
	sound_start (ST_EFFECT, HELICOPTER, SL_4S, SP_NORMAL);
	task_sleep (TIME_2S);
	sound_start (ST_EFFECT, HELICOPTER, SL_4S, SP_NORMAL);
	U8 	fortress_SoundCounter;
	fortress_SoundCounter = random_scaled(2);//from kernal/random.c - pick number from 0 to 2
	if (fortress_SoundCounter == 0)
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
	is_fortress_mode_activated  = TRUE;
	mb_mode_start(&fortress_mode);
	music_enable();
	callset_invoke(cramp_jackpot_light_on);
	set_ball_count (2); //NumBallsFrozen
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
	deff_start (DEFF_FORTRESS_JACKPOT_EFFECT);//under /kernel/deff.c
}//end of function





/****************************************************************************
 * DMD display and sound effects
 ****************************************************************************/
void fortress_start_effect_deff(void) {
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



void fortress_jackpot_effect_deff(void) {
	U8 	jackpot_randomizer;
	jackpot_randomizer = random_scaled(4);//from kernal/random.c - pick number from 0 to N
	U8 i;
	if (jackpot_randomizer == 0) {
		for (i=1; i < 8; i++) {			//slid from side
			dmd_alloc_low_clean ();
			sprintf ("JACKPOT");
			if (i < 7)
				sprintf_buffer[i] = '\0';
			font_render_string_center (&font_fixed10, 64, 16, sprintf_buffer);
			dmd_show_low ();
			task_sleep (TIME_300MS);
		}
	} //end of jackpot_randomizer == 0
	if (jackpot_randomizer == 1) {		//roll up single
			for (i=0; i < 8; i++) {
				dmd_sched_transition (&trans_scroll_up);
				dmd_alloc_low_clean ();
				font_render_string_center (&font_fixed10, 64, 16, "JACKPOT");
				dmd_show_low ();
			}
	} //end of jackpot_randomizer == 1
	if (jackpot_randomizer == 2) {		//roll up double
		for (i=0; i < 8; i++) {
			dmd_sched_transition (&trans_scroll_up);
			dmd_alloc_low_clean ();
			font_render_string_center (&font_fixed10, 64, 8, "JACKPOT");
			font_render_string_center (&font_fixed10, 64, 24, "JACKPOT");
			dmd_show_low ();
		}
	} //end of jackpot_randomizer == 2
	if (jackpot_randomizer == 3) {		//flash in center
		dmd_alloc_pair ();
		dmd_clean_page_low ();
		font_render_string_center (&font_fixed10, 64, 16, "JACKPOT");
		dmd_copy_low_to_high ();
		dmd_show_low ();
		dmd_invert_page (dmd_low_buffer);
		deff_swap_low_high (25, TIME_100MS);
	} //end of jackpot_randomizer == 3
	task_sleep (TIME_500MS);
	deff_exit ();
}//end of mode_effect_deff



void fortress_effect_deff (void) {
	for (;;) {
		dmd_alloc_low_clean ();
		font_render_string_center (&font_fixed10, DMD_BIG_CX_Top, DMD_BIG_CY_Top, "FORTRESS");
		sprintf ("%d JACKPOTS", fortress_jackpot_shots_made);
		font_render_string_center (&font_mono5, DMD_SMALL_CX_3, DMD_SMALL_CY_3, sprintf_buffer);
		dmd_show_low ();
		task_sleep (TIME_200MS);
	}//END OF ENDLESS LOOP
}//end of function
