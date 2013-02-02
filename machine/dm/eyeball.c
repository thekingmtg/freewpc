/*
 * demolition man
 * eyeball.c
 *
 * written by James Cardona
 *
 * Location Description:
 * Retina Scan: This is the leftmost shot of the game. The ball passes through a gate
 * and into an eject that feeds the left inlane.
 * If hit hard enough through the gate, it will hit the captive eyeball.
 *
 * Scoring Description: (original game)
 * Shots that knock the captive eyeball into the upper target award the Retina Scan value.
 * The Jet Bumpers increase the Retina Value.(eyeball)
 * It starts at 5M and goes up 100K per jet.
 *
 * At certain numbers of Retina Scans hits, Explode Hurry Up is activated.
 *
 */
/* CALLSET_SECTION (eyeball, __machine5__) */

#include <freewpc.h>
#include "dm/global_constants.h"
#include "eyeball.h"

//constants
const U8 EYE_EASY_GOAL = 1;
const U8 EYE_HARD_GOAL = 5;
const U8 EYE_GOAL_STEP = 3;
const U8 EYE_GOAL_MAX = 11;

const U8 EYE_EB_EASY_GOAL = 10;
const U8 EYE_EB_HARD_GOAL = 22;

//local variables
U8	eyeball_MessageCounter;
__local__ U8 eyeball_shots_made;
__local__ U8 eyeball_eb_shots_made;
__local__ U8 total_eyeball_shots_made; //for entire game
__local__ U8 eyeball_goal; //num of hits to start explode
__local__ U8 eyeball_eb_goal; //num of hits to light extraball
__local__ __boolean eyeball_eb_made; //num of hits to light extraball
U8	eyeball_switch_debounce;



//internally called function prototypes  --external found at protos.h
void eyeball_reset (void);
void player_eyeball_reset (void);
void eyeball_goal_award (void);
void eyeball_eb_award (void);

/****************************************************************************
 * initialize  and exit
 ***************************************************************************/
void player_eyeball_reset (void) {
	total_eyeball_shots_made = 0;
	eyeball_shots_made = 0;
	eyeball_eb_shots_made = 0;
	eyeball_goal = EYE_EASY_GOAL;
	eyeball_eb_goal = EYE_EB_EASY_GOAL;
	eyeball_eb_made = FALSE;
	eyeball_switch_debounce = 0;
	flag_on(FLAG_IS_EXPLODE_MODE_ENABLED);

	#ifdef CONFIG_DIFFICULTY_LEVEL
	if (system_config.difficulty == EASY) {
		eyeball_goal = EYE_EASY_GOAL;
		eyeball_eb_goal = EYE_EB_EASY_GOAL;
	}//end of if
	else	{
		eyeball_goal = EYE_HARD_GOAL;
		eyeball_eb_goal = EYE_EB_HARD_GOAL;
	} //END OF ELSE
#endif
}//end of function

/****************************************************************************
 * external event listeners
 ****************************************************************************/
CALLSET_ENTRY (eyeball, start_player) 	{ player_eyeball_reset(); }

/****************************************************************************
 *
 * body
 *
 ***************************************************************************/
void eyeball_goal_award (void) {
	flag_off(FLAG_IS_EXPLODE_MODE_ENABLED);
	eyeball_shots_made = 0;
	if (eyeball_goal < EYE_GOAL_MAX)  eyeball_goal += EYE_GOAL_STEP;
	sound_start (ST_SPEECH, SPCH_LOVE_WHEN_THAT_HAPPENS, SL_2S, PRI_GAME_QUICK5);
	start_explode();//start explode mode
}//end of function



void eyeball_eb_award (void) {
		eyeball_eb_made = TRUE;
		sound_start (ST_SPEECH, SPCH_LOVE_WHEN_THAT_HAPPENS, SL_2S, PRI_GAME_QUICK5);
		start_extraball();
}//end of function




void eyeball_switch_debounce_task (void) {
	task_sleep_sec(2);
	eyeball_switch_debounce = 0;
	task_exit();
}//end of function



CALLSET_ENTRY (eyeball, sw_eyeball_standup) {
	if (in_game) {
			if (++eyeball_switch_debounce == 1) {
						ballsave_add_time (1);
						++eyeball_shots_made;
						++eyeball_eb_shots_made;
						sound_start (ST_SAMPLE, EXPLOSION1_MED, SL_2S, PRI_GAME_QUICK1);
						leff_start(LEFF_EYEBALL);

						if (	!eyeball_eb_made
							&& (eyeball_eb_shots_made >= eyeball_eb_goal) )  	eyeball_eb_award();//award extraball
						else if (!flag_test(FLAG_BACK_IN_THE_FRIDGE_RUNNING)
								&& eyeball_shots_made >= eyeball_goal)  		eyeball_goal_award();//start explode
						else 													{
							deff_start (DEFF_EYEBALL_EFFECT);
							if (eyeball_goal - eyeball_shots_made <= 1) flag_on(FLAG_IS_EXPLODE_MODE_ENABLED);//set flag if one shot away
						}

						score (EYEBALL_SCORE);
			}//end of if DEBOUNCER
			task_create_gid1 (GID_EYEBALL_DEBOUNCE, eyeball_switch_debounce_task);
	}
}//end of function





/****************************************************************************
 *
 * lighting effects
 *
 ****************************************************************************/
void eyeball_leff1 (void) {
	U8 i;
	for (i = 0; i < 20; i++) {
		leff_toggle(LM_RETINA_SCAN);
		task_sleep (TIME_100MS);
	}// end of loop
}//end of function

void eyeball_leff (void){
	leff_create_peer (eyeball_leff1);
	flasher_pulse (FLASH_EYEBALL_FLASHER);
	flasher_pulse (FLASH_EYEBALL_FLASHER);
	flasher_pulse (FLASH_EYEBALL_FLASHER);
	flasher_pulse (FLASH_EYEBALL_FLASHER);
	task_sleep (TIME_1S);
	flasher_pulse (FLASH_EYEBALL_FLASHER);
	flasher_pulse (FLASH_EYEBALL_FLASHER);
	flasher_pulse (FLASH_EYEBALL_FLASHER);
	flasher_pulse (FLASH_EYEBALL_FLASHER);
	flasher_pulse (FLASH_EYEBALL_FLASHER);
	task_sleep (TIME_1S);
	leff_exit();
}//end of function





/****************************************************************************
 *
 * display effects
 *
 ****************************************************************************/
void eyeball_effect_deff(void) {
	dmd_alloc_pair_clean ();
	dmd_sched_transition (&trans_scroll_up);

	switch (++eyeball_MessageCounter % 5) {
		case 4:
			dmd_map_overlay ();
			dmd_clean_page_low ();
			font_render_string_center (&font_term6, DMD_MIDDLE_X, DMD_MED_CY_1, "EXPLODE AND CHASE");
			font_render_string_center (&font_var5, DMD_MIDDLE_X, DMD_MED_CY_2, "LEAD TO");
			font_render_string_center (&font_fipps, DMD_MIDDLE_X, DMD_BIG_CY_Bot, "HUXLEY");
			dmd_text_outline ();
			dmd_alloc_pair ();
			frame_draw(IMG_HUXLEY_B2_START);
			dmd_overlay_outline ();
			dmd_show2 ();
			break;
		case 1:
			font_render_string_center (&font_term6, DMD_MIDDLE_X + 20, DMD_MED_CY_1, "JETS");
			font_render_string_center (&font_term6, DMD_MIDDLE_X + 20, DMD_MED_CY_2, "INCREASE");
			font_render_string_center (&font_lithograph, DMD_MIDDLE_X, DMD_BIG_CY_Bot, "RETINA SCAN");
			bitmap_blit (eye3SM_bits, 15, 2);
			dmd_show_low ();
			break;
		case 2:
			font_render_string_center (&font_term6, DMD_MIDDLE_X - 30, DMD_MED_CY_1, "EXTRA BALL");
			font_render_string_center (&font_term6, DMD_MIDDLE_X - 30, DMD_MED_CY_2, "AT");
			sprintf ("%d HITS", eyeball_eb_goal - eyeball_eb_shots_made);
			font_render_string_center (&font_lithograph, DMD_MIDDLE_X - 30, DMD_BIG_CY_Bot, sprintf_buffer);
			bitmap_blit (eye5_bits, 80, 2);
			dmd_show_low ();
			break;
		case 3:
			font_render_string_center (&font_term6, DMD_MIDDLE_X - 25, DMD_MED_CY_1, "EXPLODE");
			font_render_string_center (&font_term6, DMD_MIDDLE_X - 25, DMD_MED_CY_2, "AT");
			sprintf ("%d HITS", eyeball_goal - eyeball_shots_made);
			font_render_string_center (&font_lithograph, DMD_MIDDLE_X - 25, DMD_BIG_CY_Bot, sprintf_buffer);
			bitmap_blit (eye3_bits, 86, 2);
			dmd_show_low ();
			break;
		case 0:
			sprintf ("OOOHHH");
			font_render_string_center (&font_lithograph, DMD_MIDDLE_X + 25, DMD_BIG_CY_Top, sprintf_buffer);
			sprintf ("MY EYE");
			font_render_string_center (&font_lithograph, DMD_MIDDLE_X + 25, DMD_BIG_CY_Bot, sprintf_buffer);
			bitmap_blit (eye3_bits, 2, 2);
			dmd_show_low ();
			break;
	}//END OF SWITCH

	task_sleep (TIME_500MS);
	deff_exit ();
}//end of mode_effect_deff


