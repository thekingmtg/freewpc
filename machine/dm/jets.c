/*
 * demolition man
 * jets.c
 *
 * written by James Cardona
 *
 * Location Description:
 * Jet Bumpers: There are two regular jet bumpers and a top slingshot
 * which scores the same as a jet bumper. They advance the
 * Retina Scan value. The left jet bumper can also be hit from
 * the center ramp -- it acts as the right wall of the center ramp.
 *
 * Scoring Description: (original game)
 * The Jet Bumpers increase the Retina Value.(eyeball)
 * It starts at 5M and goes up 100K per jet.
 * There are two Jet Bumpers and one slingshot ... the slingshot counts as a jet bumper.
 * Jet Bumpers award 250k a Pop.
 * superjets mode award 1 million each
 *
 * Scoring Description: (my rules)
 * same as above except
 * superjets mode award 2 million each
 * successive modes award more
 *
 * estimate of average superjets mode score: 20 million to 80 million
 * estimate of average jets score: 2.5 million
 *
 */
/*this is not needed ---> ALLSET_SECTION (jets, __common__) */
//this file is currently being compiled into the common page since we have ran out of
//room on the other 5 pages.  check common/makefile and platform/wpc/generic.md



#include <freewpc.h>
#include "dm/global_constants.h"
#include "jets.h"


//constants
const U8 JETS_EASY_GOAL = 25;
const U8 JETS_HARD_GOAL = 50;
const U8 JETS_GOAL_STEP = 15;
const U8 JETS_GOAL_MAX = 150;

//local variables
U8 			jet_goal;
U8 			mode_jet_shots_made;
U8			jets_modes_achieved;
__boolean 	jets_inBall_search;
score_t jets_temp_score;
__local__ U8 			jet_shots_made;//external ref to eject.c

//external variables

//internally called function prototypes  --external found at protos.h
void jet_flasher(void);
void jets_effect_deff(void);
void jet_goal_reset(void);
void jet_goal_award(void);
void player_jet_goal_reset(void);


/****************************************************************************
 * initialize  and exit
 ***************************************************************************/
void jet_goal_reset (void) {
	jets_inBall_search = FALSE;
}//end of function

void player_jet_goal_reset  (void) {
	mode_jet_shots_made = 0;
	jet_shots_made = 0;
	jets_modes_achieved = 0;
	jet_goal = JETS_EASY_GOAL;
	jet_goal_reset();

#ifdef CONFIG_DIFFICULTY_LEVEL
	if (system_config.difficulty == EASY) 	jet_goal = JETS_EASY_GOAL;
	else									jet_goal = JETS_HARD_GOAL;
#endif
}//end of function

CALLSET_ENTRY (jets, start_player) 	{  player_jet_goal_reset (); }
CALLSET_ENTRY (jets, start_ball) 	{ jet_goal_reset (); }
CALLSET_ENTRY (jets, ball_search) { jets_inBall_search = TRUE; }
CALLSET_ENTRY (jets, ball_search_end) { jets_inBall_search = FALSE; }


/****************************************************************************
 *
 * body
 *
 ***************************************************************************/
void jet_goal_award (void) {
	mode_jet_shots_made = 0;
	switch (++jets_modes_achieved) {  //score higher if mode done more than once
		case 0: 	break; //never entered mode
		case 1:  score (JETS_GOAL_SCORE1);  break;
		case 2:  score (JETS_GOAL_SCORE2);  break;
		case 3:  score (JETS_GOAL_SCORE3);  break;
		case 4:	 score (JETS_GOAL_SCORE4);  break;
		default: score (JETS_GOAL_SCORE5);  break;
		}//end of switch
	sound_start (ST_SAMPLE, EXPLOSION, SL_2S, PRI_GAME_QUICK5);
	deff_start (DEFF_JETS_COMPLETED_EFFECT);
	if (jet_goal < JETS_GOAL_MAX)  jet_goal += JETS_GOAL_STEP;
	//light access claw
	increment_access_claw_light_on();//at inlanes.c
}//END OF FUNCTION



CALLSET_ENTRY (jets, sw_jet) {
	if (!jets_inBall_search) {
			if (flag_test (FLAG_IS_PBREAK_RUNNING) ) { prison_break_made(); }
			else if (!flag_test (FLAG_IS_SUPERJETS_RUNNING) ) {//not in super jets mode
				++jet_shots_made;
				++mode_jet_shots_made;
				score(JETS_HIT_SCORE);
				if (mode_jet_shots_made >= jet_goal)  jet_goal_award ();//sound played in call
				else deff_start (DEFF_JETS_EFFECT);//under /kernel/deff.c
				}//end of not in superjets mode
			sound_start (ST_EFFECT, MACHINE3, SL_1S, PRI_GAME_QUICK5);
			flasher_pulse (FLASH_JETS_FLASHER);
	}
}//end of function




/****************************************************************************
 *
 * DMD display effects
 *
 ****************************************************************************/
void jets_effect_deff(void) {
	U8 i = 0;
	if (IN_TEST) {
		jet_shots_made += 4;
		jet_goal = 20;
		jets_modes_achieved++;
	}
	score_zero (jets_temp_score);						//zero out temp score

	if (jets_modes_achieved % 2 == 0) { //DISPLAY THIS ON EVEN NUMBERED JETS MODES
		switch (jets_modes_achieved) {  //score higher if mode done more than once
			case 0:  score_add (jets_temp_score, score_table[JETS_GOAL_SCORE1]); break;
			case 1:  score_add (jets_temp_score, score_table[JETS_GOAL_SCORE2]); break;
			case 2:  score_add (jets_temp_score, score_table[JETS_GOAL_SCORE3]); break;
			case 3:  score_add (jets_temp_score, score_table[JETS_GOAL_SCORE4]); break;
			default: score_add (jets_temp_score, score_table[JETS_GOAL_SCORE5]); break;
			}//end of switch
		do {
			dmd_alloc_low_clean ();
			dmd_draw_thin_border (dmd_low_buffer);

			if (i < 2 ) 		bitmap_blit (box1_bits, 95, 0);
			else if (i < 4 )    bitmap_blit (box2_bits, 95, 0);
			else if (i < 6 )    bitmap_blit (box3_bits, 95, 0);
			else if (i < 8 )    bitmap_blit (box4_bits, 95, 0);
			else if (i < 10 )   bitmap_blit (box5_bits, 95, 0);
			else if (i < 12 )   bitmap_blit (box6_bits, 95, 0);
			else if (i < 14 )   bitmap_blit (box5_bits, 95, 0);
			else if (i < 16 )   bitmap_blit (box4_bits, 95, 0);
			else if (i < 18 )   bitmap_blit (box3_bits, 95, 0);
			else if (i < 20 )   bitmap_blit (box2_bits, 95, 0);
			else				bitmap_blit (box1_bits, 95, 0);

			psprintf ("1 JET", "%d JETS", mode_jet_shots_made);
			font_render_string_center (&font_fixed6, DMD_MIDDLE_X - 20, DMD_MED_CY_1, sprintf_buffer);
			sprintf ("%d FOR BONUS", jet_goal-mode_jet_shots_made);
			font_render_string_center (&font_term6, DMD_MIDDLE_X - 20, DMD_MED_CY_2, sprintf_buffer);
			sprintf_score (jets_temp_score);
			font_render_string_center (&font_fixed6, DMD_MIDDLE_X - 20, DMD_MED_CY_3, sprintf_buffer);
			dmd_show_low ();
			task_sleep (TIME_66MS);
		} while (i++ < 22);
	}//end of if
	else { // DISPLAY THIS ON ODD NUMBERED JETS MODES
		score_add (jets_temp_score, score_table[SC_100K]);
		score_mul (jets_temp_score, jet_shots_made);		//multiply 100K by jet count
		score_add (jets_temp_score, score_table[EJECT_SCORE]);			//add in normal retina scan score

		do {
			dmd_alloc_low_clean ();
			dmd_draw_thin_border (dmd_low_buffer);
			if (i < 3 ) 		bitmap_blit (jets_eye3_bits, 2, 4);
			else if (i < 6 )    bitmap_blit (jets_eye4_bits, 2, 4);
			else if (i < 9 )    bitmap_blit (jets_eye5_bits, 2, 4);
			else if (i < 12 )   bitmap_blit (jets_eye6_bits, 2, 4);
			else if (i < 15 )   bitmap_blit (jets_eye5_bits, 2, 4);
			else if (i < 18 )   bitmap_blit (jets_eye4_bits, 2, 4);
			else				bitmap_blit (jets_eye3_bits, 2, 4);

			sprintf ("JETS");
			font_render_string_center (&font_fixed6, DMD_MIDDLE_X, DMD_MED_CY_1, sprintf_buffer);

			sprintf ("INCREASE");
			font_render_string_center (&font_var5, DMD_MIDDLE_X + 39, DMD_MED_CY_1, sprintf_buffer);

			sprintf ("RETINA SCAN");
			font_render_string_center (&font_fixed6, DMD_MIDDLE_X + 18, DMD_MED_CY_2 + 1, sprintf_buffer);

			sprintf_score (jets_temp_score);
			font_render_string_center (&font_term6, DMD_MIDDLE_X + 20, DMD_MED_CY_3, sprintf_buffer);

			dmd_show_low ();
			task_sleep (TIME_66MS);
		} while (i++ < 21);
	}//end of else
	deff_exit ();
}//end of jets_effect_deff






void jets_completed_effect_deff(void) {
	U8 i = 0;
	score_zero (jets_temp_score);						//zero out temp score
	switch (jets_modes_achieved) {  //score higher if mode done more than once
		case 1:  score_add (jets_temp_score, score_table[JETS_GOAL_SCORE1]); break;
		case 2:  score_add (jets_temp_score, score_table[JETS_GOAL_SCORE2]); break;
		case 3:  score_add (jets_temp_score, score_table[JETS_GOAL_SCORE3]); break;
		case 4:  score_add (jets_temp_score, score_table[JETS_GOAL_SCORE4]); break;
		default: score_add (jets_temp_score, score_table[JETS_GOAL_SCORE5]); break;
		}//end of switch


	if (jets_modes_achieved % 2 == 0) { //DISPLAY THIS ON EVEN NUMBERED JETS MODES
		do {
			dmd_alloc_low_clean ();
			dmd_draw_thin_border (dmd_low_buffer);
			if (i < 3 ) 		bitmap_blit (jets_eye3_bits, 2, 4);
			else if (i < 6 )    bitmap_blit (jets_eye4_bits, 2, 4);
			else if (i < 9 )    bitmap_blit (jets_eye5_bits, 2, 4);
			else if (i < 12 )   bitmap_blit (jets_eye6_bits, 2, 4);
			else if (i < 15 )   bitmap_blit (jets_eye5_bits, 2, 4);
			else if (i < 18 )   bitmap_blit (jets_eye4_bits, 2, 4);
			else				bitmap_blit (jets_eye3_bits, 2, 4);

			sprintf ("JETS");
			font_render_string_center (&font_fixed6, DMD_MIDDLE_X + 18, DMD_MED_CY_1, sprintf_buffer);

			sprintf ("COMPLETED");
			font_render_string_center (&font_fixed6, DMD_MIDDLE_X + 18, DMD_MED_CY_2 + 1, sprintf_buffer);

			sprintf_score (jets_temp_score);
			font_render_string_center (&font_term6, DMD_MIDDLE_X + 20, DMD_MED_CY_3, sprintf_buffer);

			dmd_show_low ();
			task_sleep (TIME_66MS);
		} while (i++ < 21);
	}//end of if
	else { // DISPLAY THIS ON ODD NUMBERED JETS MODES
		do {
				dmd_alloc_low_clean ();
				dmd_draw_thin_border (dmd_low_buffer);

				if (i < 2 ) 		bitmap_blit (box1_bits, 95, 0);
				else if (i < 4 )    bitmap_blit (box2_bits, 95, 0);
				else if (i < 6 )    bitmap_blit (box3_bits, 95, 0);
				else if (i < 8 )    bitmap_blit (box4_bits, 95, 0);
				else if (i < 10 )   bitmap_blit (box5_bits, 95, 0);
				else if (i < 12 )   bitmap_blit (box6_bits, 95, 0);
				else if (i < 14 )   bitmap_blit (box5_bits, 95, 0);
				else if (i < 16 )   bitmap_blit (box4_bits, 95, 0);
				else if (i < 18 )   bitmap_blit (box3_bits, 95, 0);
				else if (i < 20 )   bitmap_blit (box2_bits, 95, 0);
				else				bitmap_blit (box1_bits, 95, 0);

				sprintf ("JETS");
				font_render_string_center (&font_fixed6, DMD_MIDDLE_X - 20, DMD_MED_CY_1, sprintf_buffer);
				sprintf ("COMPLETED");
				font_render_string_center (&font_term6, DMD_MIDDLE_X - 20, DMD_MED_CY_2, sprintf_buffer);
				sprintf_score (jets_temp_score);
				font_render_string_center (&font_fixed6, DMD_MIDDLE_X - 20, DMD_MED_CY_3, sprintf_buffer);
				dmd_show_low ();
				task_sleep (TIME_66MS);
			} while (i++ < 22);
	}//end of else
	deff_exit ();
}//end of mode_effect_deff


