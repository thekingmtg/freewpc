/*
 * demolition man
 * jets_superjets.c
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
 * same as above
 *
 * TODO: verify scoring
 *
 */

#include <freewpc.h>
//constants
const U8 JETS_EASY_GOAL = 25;
const U8 JETS_PREDEFINED_GOAL_INCREMENT = 1;
const U8 JETS_GOAL_STEP = 15;
const U8 JETS_GOAL_MAX = 150;
const U8 SUPERJETS_EASY_GOAL = 20;

//local variables
U8 jet_count;
U8 jet_goal;
U8 super_jet_count;
U8 super_jet_goal;
__boolean super_jets_activated;

//prototypes
void jet_flasher (void);
void jets_effect_deff(void);
void superjets_effect_deff(void);
void jet_goal_reset (void);
void jet_goal_award (void);
void super_jet_goal_award (void);

/****************************************************************************
 * initialize  and exit
 ***************************************************************************/
void jet_goal_reset (void) {
	jet_count = 0;
	jet_goal = JETS_EASY_GOAL;
	super_jet_goal = SUPERJETS_EASY_GOAL;
	super_jet_count = 0;
	super_jets_activated = FALSE;
	}

CALLSET_ENTRY (jets_superjets, start_player) {  jet_goal_reset (); }
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
	sound_start (ST_SAMPLE, SPCH_SUPERJETS_COMPLETED, SL_1S, PRI_GAME_QUICK5);
	jet_count = 0;
	score(SC_20M);
	super_jets_activated = FALSE;
	}

CALLSET_ENTRY (jets_superjets, sw_left_jet, sw_right_jet, sw_top_sling) {
	if (super_jets_activated){
		++super_jet_count;
		score(SC_1M);
		sound_start (ST_SAMPLE, SPCH_WOAH, SL_1S, PRI_GAME_QUICK5);
		if (super_jet_count == super_jet_goal)  super_jet_goal_award();

		//
		//
		//jets_effect_deff();
		//crash here?
			deff_start (DEFF_SUPERJETS_EFFECT);//under /kernel/deff.c


	}
	else {//not in super jets mode
		++jet_count;
		score(SC_250K);
		if (jet_count == jet_goal)  jet_goal_award ();
		else
			sound_start (ST_SAMPLE, EXPLOSION, SL_1S, PRI_GAME_QUICK5);
		}
	task_create_gid1 (GID_JET_FLASHER, jet_flasher);
	}

//claw switch starts superjets mode
CALLSET_ENTRY (jets_superjets, claw_super_jets) {
	super_jets_activated = TRUE;
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

