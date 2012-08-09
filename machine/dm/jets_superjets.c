/*
 * demolition man
 * jets_superjets.c
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

#include <freewpc.h>
//constants
const U8 JETS_EASY_GOAL = 25;
const U8 JETS_PREDEFINED_GOAL_INCREMENT = 1;
const U8 JETS_GOAL_STEP = 15;
const U8 JETS_GOAL_MAX = 150;
const U8 SUPERJETS_GOAL_MAX = 100;
const U8 SUPERJETS_EASY_GOAL = 20;
const U8 SUPERJETS_GOAL_STEP = 10;
score_t superjets_mode_score;

//local variables
U8 jet_count;
U8 jet_goal;
U8 super_jet_count;
U8 super_jet_goal;
U8 superjets_mode_counter; // number of times we entered the mode
U8 superjets_SoundCounter;
__boolean super_jets_activated;

//prototypes
void jet_flasher(void);
void jets_effect_deff(void);
void superjets_effect_deff(void);
void jet_goal_reset(void);
void jet_goal_award(void);
void super_jet_goal_award(void);
void player_jet_goal_reset(void);

/****************************************************************************
 * initialize  and exit
 ***************************************************************************/
void jet_goal_reset (void) {
	jet_count = 0;
	super_jet_count = 0;
	super_jets_activated = FALSE;
	superjets_SoundCounter = 0;
	}

void player_jet_goal_reset  (void) {
superjets_mode_counter = 0;
jet_goal = JETS_EASY_GOAL;
super_jet_goal = SUPERJETS_EASY_GOAL;
jet_goal_reset();
}

CALLSET_ENTRY (jets_superjets, start_player) {  player_jet_goal_reset (); }
CALLSET_ENTRY (jets_superjets, start_ball) { jet_goal_reset (); }



/****************************************************************************
 * body
 ***************************************************************************/

void jet_goal_award (void) {
	sound_start (ST_SAMPLE, EXPLOSION, SL_1S, PRI_GAME_QUICK5);
	jet_count = 0;
	score(SC_1M);//only once
	if (jet_goal < JETS_GOAL_MAX)  jet_goal += JETS_GOAL_STEP;
	}

void super_jet_goal_award (void) {
	sound_start (ST_SPEECH, SPCH_SUPERJETS_COMPLETED, SL_2S, PRI_GAME_QUICK5);
	jet_count = 0;
	super_jet_count = 0;
	//score higher if mode done more than once
	switch (superjets_mode_counter) {
		case 0: 	break; //never entered mode
		case 1:  { score (SC_40M);  score_add (superjets_mode_score, score_table[SC_40M]); break;}
		case 2:  { score (SC_45M);  score_add (superjets_mode_score, score_table[SC_45M]); break;}
		case 3:  { score (SC_50M);  score_add (superjets_mode_score, score_table[SC_50M]); break;}
		case 4:	 { score (SC_55M);  score_add (superjets_mode_score, score_table[SC_55M]); break;}
		default: { score (SC_60M);  score_add (superjets_mode_score, score_table[SC_60M]); break;}
		}//end of switch
	super_jets_activated = FALSE;
	if (super_jet_goal < SUPERJETS_GOAL_MAX)  super_jet_goal += SUPERJETS_GOAL_STEP;
	//TODO: deff
	//return to normal music
	sound_start (ST_MUSIC, MUS_BG, 0, SP_NORMAL);
	}

CALLSET_ENTRY (jets_superjets, sw_left_jet, sw_right_jet, sw_top_sling) {
	if (super_jets_activated){
		++super_jet_count;
		superjets_SoundCounter = random_scaled(3);//from kernal/random.c - pick number from 0 to 2
		if ( (superjets_SoundCounter) == 0 ) 		sound_start (ST_SPEECH, SPCH_DULCH, SL_500MS, PRI_GAME_QUICK5);
		else if ( (superjets_SoundCounter) == 1 ) 	sound_start (ST_SPEECH, SPCH_WOOH, SL_500MS, PRI_GAME_QUICK5);
		else 										sound_start (ST_SPEECH, SPCH_WOW, SL_500MS, PRI_GAME_QUICK5);

		if (super_jet_count == super_jet_goal)  super_jet_goal_award();

		// deff must be listed in md file or will crash here
			deff_start (DEFF_SUPERJETS_EFFECT);//under /kernel/deff.c
		//score higher if mode done more than once
		switch (superjets_mode_counter) {
			case 0: 	break; //never entered mode
			case 1:  { score (SC_2M);  score_add (superjets_mode_score, score_table[SC_2M]); break;}
			case 2:  { score (SC_3M);  score_add (superjets_mode_score, score_table[SC_3M]); break;}
			case 3:  { score (SC_4M);  score_add (superjets_mode_score, score_table[SC_4M]); break;}
			case 4:	 { score (SC_5M);  score_add (superjets_mode_score, score_table[SC_5M]); break;}
			default: { score (SC_5M);  score_add (superjets_mode_score, score_table[SC_5M]); break;}
			}//end of switch
	}
	else {//not in super jets mode
		++jet_count;
		score(SC_250K);
		deff_start (DEFF_JETS_EFFECT);//under /kernel/deff.c
		if (jet_count == jet_goal)  jet_goal_award ();//sound played in call
		//TODO: do we want a sound here?
		//	else
	//		sound_start (ST_SAMPLE, EXPLOSION, SL_1S, PRI_GAME_QUICK5);
		}
	task_create_gid1 (GID_JET_FLASHER, jet_flasher);
	}

//claw switch starts superjets mode
CALLSET_ENTRY (jets_superjets, claw_super_jets) {
	super_jets_activated = TRUE;
	++superjets_mode_counter;
	score_zero (superjets_mode_score);
	sound_start (ST_MUSIC, MUS_MD_SUPERJETS, 0, SP_NORMAL);
	sound_start (ST_SAMPLE, SPCH_SUPERJETS_ACTIVATED, SL_1S, PRI_GAME_QUICK5);
	//flash lamp for a time
	lamp_tristate_flash(LM_CLAW_SUPER_JETS);
	task_sleep (TIME_500MS);
	lamp_tristate_off(LM_CLAW_SUPER_JETS);
	}




/****************************************************************************
 * DMD display and sound effects
 ****************************************************************************/
//LIGHTING EFFECTS
void jet_flasher (void) {
	flasher_pulse (FLASH_JETS_FLASHER);//FLASH followed by name of flasher in caps
	task_sleep (TIME_500MS);
	task_exit ();
	}

//DMD DISPLAY EFFECTS
void jets_effect_deff(void) {
	dmd_alloc_low_clean ();
	font_render_string_center (&font_mono5, 96, 5, "JET BUMPERS");
	sprintf ("%d", jet_count);
	font_render_string_center (&font_fixed10, 96, 16, sprintf_buffer);
	if (jet_count == jet_goal)
		sprintf ("JET BONUS");
	else
		sprintf ("BONUS AT %d", jet_goal);

	font_render_string_center (&font_var5, 64, 26, sprintf_buffer);
	dmd_show_low ();
	task_sleep_sec (2);
	deff_exit ();
	}//end of jets_effect_deff

void superjets_effect_deff(void) {
	dmd_alloc_low_clean ();
	font_render_string_center (&font_mono5, 96, 5, "SUPER JETS");
	sprintf ("%d", super_jet_count);
	font_render_string_center (&font_fixed10, 96, 16, sprintf_buffer);
	dmd_show_low ();
	task_sleep_sec (2);
	deff_exit ();
	}//end of superjets_effect_deff

