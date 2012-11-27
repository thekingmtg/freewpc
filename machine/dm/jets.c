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
/* CALLSET_SECTION (jets, __machine5__) */


#include <freewpc.h>
#include "dm/global_constants.h"


//constants
const U8 JETS_EASY_GOAL = 25;
const U8 JETS_MED_GOAL = 45;
const U8 JETS_HARD_GOAL = 65;
const U8 JETS_GOAL_STEP = 15;
const U8 JETS_GOAL_MAX = 150;

//local variables
U8 			jet_shots_made;//external ref to eyeball_explode.c
U8 			jet_goal;
U8			jets_modes_achieved;

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
	jet_shots_made = 0;
}//end of function

void player_jet_goal_reset  (void) {
	jets_modes_achieved = 0;
	jet_goal = JETS_EASY_GOAL;
	jet_goal_reset();
}//end of function

CALLSET_ENTRY (jets, start_player) 	{  player_jet_goal_reset (); }
CALLSET_ENTRY (jets, start_ball) 	{ jet_goal_reset (); }


/****************************************************************************
 * body
 ***************************************************************************/
void jet_goal_award (void) {
	jet_shots_made = 0;
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
}//END OF FUNCTION



CALLSET_ENTRY (jets, sw_jet) {
	if (flag_test (FLAG_IS_PBREAK_RUNNING) ) { prison_break_made(); }
	else if (!flag_test (FLAG_IS_SUPERJETS_RUNNING) ) {//not in super jets mode
		++jet_shots_made;
		score(JETS_HIT_SCORE);
		if (jet_shots_made >= jet_goal)  jet_goal_award ();//sound played in call
		else deff_start (DEFF_JETS_EFFECT);//under /kernel/deff.c
		}//end of not in superjets mode
		flasher_pulse (FLASH_JETS_FLASHER);
}//end of function


/****************************************************************************
 * DMD display effects - normal jets
 ****************************************************************************/
void jets_effect_deff(void) {
	U8 i = 0;
	do {
		U8 x = random_scaled (6);
		U8 y = random_scaled (4);
		dmd_alloc_low_clean ();
		psprintf ("1 JET", "%d JETS", jet_shots_made);
		if (i%4 == 0)		font_render_string_center (&font_fixed6, DMD_MIDDLE_X + x, DMD_BIG_CY_Top - y, sprintf_buffer);
		else if (i%4 == 1)	font_render_string_center (&font_fixed6, DMD_MIDDLE_X + x, DMD_BIG_CY_Top + y, sprintf_buffer);
		else if (i%4 == 2)	font_render_string_center (&font_fixed6, DMD_MIDDLE_X - x, DMD_BIG_CY_Top - y, sprintf_buffer);
		else 				font_render_string_center (&font_fixed6, DMD_MIDDLE_X - x, DMD_BIG_CY_Top + y, sprintf_buffer);
		dmd_show_low ();
		task_sleep (TIME_66MS);
	} while (i++ < 8);//about .75sec
	/* Redraw it so text is centered */
	dmd_sched_transition (&trans_bitfade_fastest);
	dmd_alloc_low_clean ();
	psprintf ("1 JET", "%d JETS", jet_shots_made);
	font_render_string_center (&font_fixed6, DMD_MIDDLE_X, DMD_BIG_CY_Top, sprintf_buffer);
	sprintf ("%d FOR BONUS", jet_goal);
	font_render_string_center (&font_fixed6, DMD_MIDDLE_X, DMD_BIG_CY_Bot, sprintf_buffer);
	dmd_show_low ();
	task_sleep (TIME_1S);
	deff_exit ();
}//end of jets_effect_deff



void jets_completed_effect_deff(void) {
	dmd_alloc_low_clean ();
	dmd_sched_transition (&trans_bitfade_fastest);
	font_render_string_center (&font_fixed6, DMD_MIDDLE_X, DMD_BIG_CY_Top, "JETS");
	font_render_string_center (&font_fixed6, DMD_MIDDLE_X, DMD_BIG_CY_Bot, "COMPLETED");
	dmd_show_low ();
	task_sleep (TIME_1S);
	deff_exit ();
}//end of mode_effect_deff


