/*
 * demolition man
 * rollovers.c
 *
 * written by James Cardona
 *
 * Location Description:
 * MTL rollovers: In the top right corner are three rollovers which advance
 * the bonus multiplier. Balls leave the rollovers into the jet bumpers.
 * These are very hard to see from the player's point of view.
 * Especially the leftmost (M) rollover.
 *
 * MTL rollovers
 * are actually left, center, right but
 * are labeled middle, top, lower where
 * left is called middle
 * center is called top
 * right is called lower
 *
 * Scoring Description: (original game)
 * MTL Rollovers light an increasing bonus (2x, 3x, 4x, 5x) multiplier. The bonus
 * multiplier is not scored until the end of the ball.
 * Getting the 5x bonus multiplier also lights the extra ball on the left loop (orbit).
 * Each completed MTL rollover sequence lights Access Claw at the left inlane.
 * Hitting an unlit 250k, lit one 100k
 * all 3 500k bonus
 *
 * Scoring Description: (my rules)
 * same as original
 *
 */

#include <freewpc.h>
#include "dm/global_constants.h"

//constants
const U8 			max_rollover_bonus_multiplier = 5;

//local variables
__boolean 			middle_rollover_activated;
__boolean 			top_rollover_activated;
__boolean 			lower_rollover_activated;
U8 					rollover_bonus_multiplier; // 0 to 5
U8					rollover_SoundCounter;
U8					rollover_MessageCounter;

//external variables

//prototypes
void rollover_reset (void);
void all_rollover_made (void);
void rollover_sounds (void);
void rollover_sounds_all_rollovers (void);
void rollover_sounds_already_lit(void);
void rollovers_effect(void);

/****************************************************************************
 * initialize  and exit
 ***************************************************************************/
void rollover_reset (void) {
	rollover_SoundCounter = 0;
	rollover_MessageCounter = 0;
	middle_rollover_activated = TRUE;
	top_rollover_activated = FALSE;
	lower_rollover_activated = FALSE;
	rollover_bonus_multiplier = 1;

	lamp_tristate_on(LM_MIDDLE_ROLLOVER);
	lamp_tristate_off(LM_TOP_ROLLOVER);
	lamp_tristate_off(LM_LOWER_ROLLOVER);
}//end of rollover_reset

void player_rollover_reset (void) { }//end of function

CALLSET_ENTRY (rollovers, start_player) { player_rollover_reset(); }
CALLSET_ENTRY (rollovers, start_ball) { rollover_reset(); }


/****************************************************************************
 * body
 ***************************************************************************/
void all_rollover_made (void){
	lamp_tristate_flash(LM_MIDDLE_ROLLOVER);
	lamp_tristate_flash(LM_TOP_ROLLOVER);
	lamp_tristate_flash(LM_LOWER_ROLLOVER);
	rollover_sounds_all_rollovers();
	if (rollover_bonus_multiplier < max_rollover_bonus_multiplier) {
		++rollover_bonus_multiplier;
		deff_start (DEFF_ALL_ROLLOVERS_EFFECT);
	}
	else if (rollover_bonus_multiplier == max_rollover_bonus_multiplier) callset_invoke(extraball_light_on);
	task_sleep (TIME_2S);
	lamp_tristate_off(LM_MIDDLE_ROLLOVER);
	lamp_tristate_off(LM_TOP_ROLLOVER);
	lamp_tristate_off(LM_LOWER_ROLLOVER);
	middle_rollover_activated = FALSE;
	top_rollover_activated = FALSE;
	lower_rollover_activated = FALSE;
	score (SC_500K);
	//light access claw
	callset_invoke(access_claw_light_on);//at inlanes.c
}//end of function


//note that switch is called left rollover corresponds to light called middle rollover
CALLSET_ENTRY (rollovers, sw_left_rollover) {
	//if already lit
	if (middle_rollover_activated) {
		rollover_sounds_already_lit();
		score (SC_100K);
		}
	else { //else - not already lit, so activate rollover
		lamp_tristate_flash(LM_MIDDLE_ROLLOVER);
		task_sleep (TIME_500MS);
		lamp_tristate_on(LM_MIDDLE_ROLLOVER);
		middle_rollover_activated = TRUE;
		rollover_sounds();
		score (SC_250K);
		//check to see if this is the third rollover to activate
		if (top_rollover_activated && lower_rollover_activated) all_rollover_made();
		else rollovers_effect();
	}//end of else - not already lit, so activate rollover
}//end of function rollovers_sw_middle_rollover



//note that switch is called center rollover corresponds to light called top rollover
CALLSET_ENTRY (rollovers, sw_center_rollover) {
	if (top_rollover_activated) {
		rollover_sounds_already_lit();
		score (SC_100K);
		}
	else { //activate rollover
		lamp_tristate_flash(LM_TOP_ROLLOVER);
		task_sleep (TIME_500MS);
		lamp_tristate_on(LM_TOP_ROLLOVER);
		top_rollover_activated = TRUE;
		rollover_sounds();
		score (SC_250K);
		//check to see if this is the third rollover to activate
		if (middle_rollover_activated && lower_rollover_activated)  all_rollover_made();
		else rollovers_effect();
	}//end of else - not already lit, so activate rollover
}//end of function rollovers_sw_top_rollover



//note that switch called right rollover corresponds to light called lower rollover
CALLSET_ENTRY (rollovers, sw_right_rollover) {
	if (lower_rollover_activated) {
		rollover_sounds_already_lit();
		score (SC_100K);
		}
	else { //activate rollover
		lamp_tristate_flash(LM_LOWER_ROLLOVER);
		task_sleep (TIME_500MS);
		lamp_tristate_on(LM_LOWER_ROLLOVER);
		lower_rollover_activated = TRUE;
		rollover_sounds();
		score (SC_250K);
		//check to see if this is the third rollover to activate
		if (middle_rollover_activated && top_rollover_activated)  all_rollover_made();
		else rollovers_effect();
	}//end of else - not already lit, so activate rollover
}//end of function rollovers_sw_lower_rollover


	/****************************************************************************
	 * rotate rollovers when buttons pressed
	 ***************************************************************************/
CALLSET_ENTRY (rollovers, sw_left_button, sw_u_l_flipper_button) {
	if (in_live_game) {
			if (top_rollover_activated && lower_rollover_activated) { //left (M) not activated
				lamp_tristate_on(LM_MIDDLE_ROLLOVER);
				lamp_tristate_on(LM_TOP_ROLLOVER);
				lamp_tristate_off(LM_LOWER_ROLLOVER);
				middle_rollover_activated = TRUE;
				top_rollover_activated = TRUE;
				lower_rollover_activated = FALSE;
				}
			else if (middle_rollover_activated && lower_rollover_activated) { //center (T) not activated
				lamp_tristate_off(LM_MIDDLE_ROLLOVER);
				lamp_tristate_on(LM_TOP_ROLLOVER);
				lamp_tristate_on(LM_LOWER_ROLLOVER);
				middle_rollover_activated = FALSE;
				top_rollover_activated = TRUE;
				lower_rollover_activated = TRUE;
				}
			else if (middle_rollover_activated && top_rollover_activated) { //right (L) not activated
				lamp_tristate_on(LM_MIDDLE_ROLLOVER);
				lamp_tristate_off(LM_TOP_ROLLOVER);
				lamp_tristate_on(LM_LOWER_ROLLOVER);
				middle_rollover_activated = TRUE;
				top_rollover_activated = FALSE;
				lower_rollover_activated = TRUE;
				}
			else if (middle_rollover_activated) { //center and right not activated
				lamp_tristate_off(LM_MIDDLE_ROLLOVER);
				lamp_tristate_off(LM_TOP_ROLLOVER);
				lamp_tristate_on(LM_LOWER_ROLLOVER);
				middle_rollover_activated = FALSE;
				lower_rollover_activated = TRUE;
				top_rollover_activated = FALSE;
				}
			else if (top_rollover_activated) { //left and right not activated
				lamp_tristate_on(LM_MIDDLE_ROLLOVER);
				lamp_tristate_off(LM_TOP_ROLLOVER);
				lamp_tristate_off(LM_LOWER_ROLLOVER);
				middle_rollover_activated = TRUE;
				top_rollover_activated = FALSE;
				lower_rollover_activated = FALSE;
				}
			else if (lower_rollover_activated) { //left and center not activated
				lamp_tristate_off(LM_MIDDLE_ROLLOVER);
				lamp_tristate_on(LM_TOP_ROLLOVER);
				lamp_tristate_off(LM_LOWER_ROLLOVER);
				middle_rollover_activated = FALSE;
				top_rollover_activated = TRUE;
				lower_rollover_activated = FALSE;
				}
	}//end of if (in_live_game)
}//end of function to rotate left



//rotate rollovers when buttons pressed
CALLSET_ENTRY (rollovers, sw_right_button, sw_u_r_flipper_button) {
	if (in_live_game) {
			if (top_rollover_activated && lower_rollover_activated) { //left not activated
				lamp_tristate_on(LM_MIDDLE_ROLLOVER);
				lamp_tristate_off(LM_TOP_ROLLOVER);
				lamp_tristate_on(LM_LOWER_ROLLOVER);
				middle_rollover_activated = TRUE;
				top_rollover_activated = FALSE;
				lower_rollover_activated = TRUE;
				}
			else if (middle_rollover_activated && lower_rollover_activated) { //center not activated
				lamp_tristate_on(LM_MIDDLE_ROLLOVER);
				lamp_tristate_on(LM_TOP_ROLLOVER);
				lamp_tristate_off(LM_LOWER_ROLLOVER);
				middle_rollover_activated = TRUE;
				top_rollover_activated = TRUE;
				lower_rollover_activated = FALSE;
				}
			else if (middle_rollover_activated && top_rollover_activated) { //right not activated
				lamp_tristate_off(LM_MIDDLE_ROLLOVER);
				lamp_tristate_on(LM_TOP_ROLLOVER);
				lamp_tristate_on(LM_LOWER_ROLLOVER);
				middle_rollover_activated = FALSE;
				top_rollover_activated = TRUE;
				lower_rollover_activated = TRUE;
				}
			else if (middle_rollover_activated) { //center and right not activated
				lamp_tristate_off(LM_MIDDLE_ROLLOVER);
				lamp_tristate_on(LM_TOP_ROLLOVER);
				lamp_tristate_off(LM_LOWER_ROLLOVER);
				middle_rollover_activated = FALSE;
				top_rollover_activated = TRUE;
				lower_rollover_activated = FALSE;
				}
			else if (top_rollover_activated) { //left and right not activated
				lamp_tristate_off(LM_MIDDLE_ROLLOVER);
				lamp_tristate_off(LM_TOP_ROLLOVER);
				lamp_tristate_on(LM_LOWER_ROLLOVER);
				middle_rollover_activated = FALSE;
				top_rollover_activated = FALSE;
				lower_rollover_activated = TRUE;
				}
			else if (lower_rollover_activated) { //left and center not activated
				lamp_tristate_on(LM_MIDDLE_ROLLOVER);
				lamp_tristate_off(LM_TOP_ROLLOVER);
				lamp_tristate_off(LM_LOWER_ROLLOVER);
				middle_rollover_activated = TRUE;
				top_rollover_activated = FALSE;
				lower_rollover_activated = FALSE;
				}
	}//end of 	if (in_live_game)
}//end of function to rotate right






/****************************************************************************
 * sound effects
 ****************************************************************************/
void rollover_sounds (void) {
	rollover_SoundCounter = random_scaled(3);//from kernal/random.c
	if ( rollover_SoundCounter  == 0 )
	sound_start (ST_EFFECT, MACHINE1_SHORT, SL_2S, PRI_GAME_QUICK5);
else if ( rollover_SoundCounter  == 1 )
	sound_start (ST_EFFECT, MACHINE1_MED, SL_2S, PRI_GAME_QUICK5);
else if ( rollover_SoundCounter  == 2 )
	sound_start (ST_EFFECT, MACHINE1_LONG, SL_2S, PRI_GAME_QUICK5);
}//end of function



void rollover_sounds_all_rollovers (void) {
	rollover_SoundCounter = random_scaled(3);//from kernal/random.c
	if ( rollover_SoundCounter  == 0 )
	sound_start (ST_EFFECT, STORM1_SHORT, SL_2S, PRI_GAME_QUICK5);
else if ( rollover_SoundCounter  == 1 )
	sound_start (ST_EFFECT, STORM1_MED, SL_2S, PRI_GAME_QUICK5);
else if ( rollover_SoundCounter  == 2 )
	sound_start (ST_EFFECT, STORM1_LONG, SL_2S, PRI_GAME_QUICK5);
}//end of function



void rollover_sounds_already_lit(void) {
	rollover_SoundCounter = random_scaled(2);//from kernal/random.c
	if ( rollover_SoundCounter  == 0 )
	sound_start (ST_EFFECT, TOINK1, SL_2S, PRI_GAME_QUICK5);
else if ( rollover_SoundCounter  == 1 )
	sound_start (ST_EFFECT, TOINK2, SL_2S, PRI_GAME_QUICK5);
}//end of function


/****************************************************************************
 * display effects
 ****************************************************************************/
void all_rollovers_effect_deff(void) {
	dmd_clean_page_low ();
	switch (rollover_bonus_multiplier) {
		case 2:  sprintf ("2 X"); break;
		case 3:  sprintf ("3 X"); break;
		case 4:  sprintf ("4 X"); break;
		case 5:	 sprintf ("5 X"); break;
		default: sprintf ("ERR"); break; //this is for development testing
		}//end of switch
	font_render_string_center (&font_cu17, DMD_MIDDLE_X, DMD_BIG_CY_Cent, sprintf_buffer);
	dmd_show_low ();
	task_sleep_sec (1);
	dmd_sched_transition (&trans_sequential_boxfade);
	dmd_alloc_low_clean ();
	font_render_string_center (&font_emulogic, DMD_MIDDLE_X, DMD_BIG_CY_Cent, "BONUS");
	dmd_show_low ();
	task_sleep_sec (2);
	deff_exit ();
}//end of mode_effect_deff





void rollovers_effect(void) {
	switch (++rollover_MessageCounter % 3) {
		case 0:  deff_start (DEFF_ROLLOVERS1_EFFECT); break;
		case 1:  deff_start (DEFF_ROLLOVERS2_EFFECT); break;
		case 2:  deff_start (DEFF_ROLLOVERS3_EFFECT); break;
		default: deff_start (DEFF_ROLLOVERS1_EFFECT); break;
		}//end of switch
}//end of FUNCTION


void rollovers1_effect_deff(void) {
	dmd_alloc_low_clean ();
	font_render_string_center (&font_cu17, DMD_MIDDLE_X, DMD_BIG_CY_Top, "M  T  L");
	dmd_show_low ();
	task_sleep_sec (1);
	dmd_sched_transition (&trans_random_boxfade);
	dmd_clean_page_low ();
			font_render_string_center (&font_mono5, DMD_SMALL_CX_1, DMD_SMALL_CY_1, "LIGHT ALL M T L");
			font_render_string_center (&font_mono5, DMD_SMALL_CX_3, DMD_SMALL_CY_3, "TO");
			font_render_string_center (&font_mono5, DMD_SMALL_CX_4, DMD_SMALL_CY_4, "ADVANCE BONUS X");
	dmd_show_low ();
	task_sleep_sec (2);
	deff_exit ();
}//end of mode_effect_deff



void rollovers2_effect_deff(void) {
	dmd_alloc_low_clean ();
	font_render_string_center (&font_cu17, DMD_MIDDLE_X, DMD_BIG_CY_Top, "M  T  L");
	dmd_show_low ();
	task_sleep_sec (1);
	dmd_sched_transition (&trans_random_boxfade);
	dmd_clean_page_low ();
			font_render_string_center (&font_mono5, DMD_SMALL_CX_1, DMD_SMALL_CY_1, "LIGHT ALL M T L");
			font_render_string_center (&font_mono5, DMD_SMALL_CX_3, DMD_SMALL_CY_3, "TO");
			font_render_string_center (&font_mono5, DMD_SMALL_CX_4, DMD_SMALL_CY_4, "LIGHT CRYOCLAW");
	dmd_show_low ();
	task_sleep_sec (2);
	deff_exit ();
}//end of mode_effect_deff



void rollovers3_effect_deff(void) {
	dmd_alloc_low_clean ();
	font_render_string_center (&font_cu17, DMD_MIDDLE_X, DMD_BIG_CY_Top, "M  T  L");
	dmd_show_low ();
	task_sleep_sec (1);
	dmd_sched_transition (&trans_random_boxfade);
	dmd_clean_page_low ();
			font_render_string_center (&font_mono5, DMD_SMALL_CX_1, DMD_SMALL_CY_1, "LIGHT ALL M T L");
			font_render_string_center (&font_mono5, DMD_SMALL_CX_2, DMD_SMALL_CY_2, "5 TIMES");
			font_render_string_center (&font_mono5, DMD_SMALL_CX_3, DMD_SMALL_CY_3, "TO");
			font_render_string_center (&font_mono5, DMD_SMALL_CX_4, DMD_SMALL_CY_4, "LIGHT EXTRA BALL");
	dmd_show_low ();
	task_sleep_sec (2);
	deff_exit ();
}//end of mode_effect_deff



