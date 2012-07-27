/*
 * demolition man
 * eyeball_explode.c
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
 * It takes 1 hit for the first Hurry-Up, four for the next on the easiest level.
 *
 * All four Explode ramps are lit at a value of 15M that begins counting down.
 * Hit any explode lamp to collect and add 10M to the value for the next Explode shot.
 * The round times out when the countdown reaches 5M (or higher if you've collected a few of the shots).
 * An extra ball is lit at three Retina Scans.
 * Retina Scan shots feed to the left saucer (also called eject) which drops the ball into the left inlane.
 *
 *
 *
 *
 * Scoring Description: (my rules)
 * Shots that knock the captive eyeball into the upper target award the Retina Scan value.
 * The Jet Bumpers increase the Retina Value.(eyeball)
 * TODO: It starts at 5M and goes up 100K per jet.
 *
 * At certain numbers of Retina Scans hits, Explode Hurry Up is activated.
 * It takes 1 hit for the first Hurry-Up, four for the next on the easiest level.
 *
 * All four Explode ramps are lit at a value of 15M that begins counting down.
 * Hit any explode lamp to collect and add 10M to the value for the next Explode shot.
 * The round times out after 30 seconds or ball drain.
 *
 * An extra ball is lit at three Retina Scans.
 * TODO: Retina Scan shots feed to the left saucer (also called eject) which drops the ball into the left inlane.
 *
 *
 */

#include <freewpc.h>
//constants
U8 EYE_EASY_GOAL = 1;
U8 EYE_PREDEFINED_GOAL_INCREMENT = 1;
U8 EYE_GOAL_STEP = 3;
U8 EYE_GOAL_MAX = 50;

//local variables
U8 eyeball_counter;//for current ball only
U8 total_eyeball_counter;//for entire game
U8 eyeball_goal;//num of hits to start explode
U8 explode_mode_timer;
U8 explode_mode_counter;//number of times an explode arrow or eyeball is hit
U8 explode_modes_achieved_counter;//number of times mode achieved
score_t explode_mode_score;
__boolean explode_activated;

//prototypes
void explode_mode_init (void);
void explode_mode_expire (void);
void explode_mode_exit (void);
void eyeball_reset (void);
void player_eyeball_reset (void);
void eyeball_flasher (char * flasher);
void eyeball_goal_award (void);

//mode definition structure
struct timed_mode_ops explode_mode = {
	DEFAULT_MODE,
	.init = explode_mode_init,
	.exit = explode_mode_exit,
	.gid = GID_EXPLODE_MODE_RUNNING,
	.music = MUS_MD_EXPLODE,
//	.deff_running = DEFF_EXPLODE_MODE,
//	.deff_ending = DEFF_EXPLODE_MODE_TOTAL,
	.prio = PRI_GAME_MODE1,
	.init_timer = 30,
	.timer = &explode_mode_timer,
	.grace_timer = 3,
	.pause = system_timer_pause,
};


/****************************************************************************
 * initialize  and exit
 ***************************************************************************/
void player_eyeball_reset (void) {
	total_eyeball_counter = 0;
	eyeball_counter = 0;
	eyeball_goal = EYE_EASY_GOAL;
	explode_activated = FALSE;
	explode_modes_achieved_counter = 0;
	explode_mode_counter = 0;
	}

void eyeball_reset (void) {
	eyeball_counter = 0;
	eyeball_goal = EYE_EASY_GOAL;
	explode_activated = FALSE;
	explode_modes_achieved_counter = 0;
	explode_mode_counter = 0;
	}

CALLSET_ENTRY (eyeball_explode, end_ball) { timed_mode_end (&explode_mode); }
CALLSET_ENTRY (eyeball_explode, display_update) { timed_mode_display_update (&explode_mode); }
CALLSET_ENTRY (eyeball_explode, start_player) {  player_eyeball_reset(); }
CALLSET_ENTRY (eyeball_explode, start_ball) { eyeball_reset(); }
void explode_mode_exit (void) { explode_mode_expire();}

void explode_mode_init (void) {
	explode_activated = TRUE;
	explode_mode_counter = 0;
	score_zero (explode_mode_score);
	callset_invoke(Activate_Explode_Inserts);
	++explode_modes_achieved_counter;
	}//end of function

void explode_mode_expire (void) {
	explode_activated = FALSE;
	callset_invoke(DeActivate_Explode_Inserts);
}




/*
 *
 *eyeball hits outside of explode mode
 */
void eyeball_goal_award (void) {
		eyeball_counter = 0;
		timed_mode_begin (&explode_mode);//start explode mode
		sound_start (ST_SAMPLE, SPCH_LOVE_WHEN_THAT_HAPPENS, SL_2S, PRI_GAME_QUICK5);
		if (eyeball_goal < EYE_GOAL_MAX)  eyeball_goal += EYE_GOAL_STEP;
	}

CALLSET_ENTRY (eyeball_explode, eyeball_standup) {
	eyeball_flasher ("FLASH_EYEBALL_FLASHER");
	sound_start (ST_SAMPLE, EXPLOSION1_MED, SL_1S, PRI_GAME_QUICK1);
	score (SC_5M);
	//need to add in 100k per jet hit here

	//light extra ball on 3rd eyeball hit
	if (total_eyeball_counter == 3) callset_invoke(ExtraBall_Light_On);

	//explode mode not activated
	if ( !timed_mode_running_p(&explode_mode) ) {
		++eyeball_counter;
		if (eyeball_counter == eyeball_goal)  eyeball_goal_award();
		}

	else { //else explode activated
		++explode_mode_counter;
		callset_invoke(explode_ramp_made);
		}
	}




CALLSET_ENTRY (eyeball_explode, explode_ramp_made) {
	//score 15 million counting down to 5 million at
	//end of mode  + 10 million for bonus # of explodes hit
	if (&explode_mode_timer > 25) score (SC_15M);
	else if (&explode_mode_timer > 20) score (SC_13M);
	else if (&explode_mode_timer > 15) score (SC_11M);
	else if (&explode_mode_timer > 10) score (SC_9M);
	else if (&explode_mode_timer > 5) score (SC_7M);
	else score (SC_5M);
	switch (explode_mode_counter) {
	case 0: break;
	case 1: {score (SC_10M); break;}
	case 2: {score (SC_20M); break;}
	case 3: {score (SC_30M); break;}
	case 4: score (SC_40M);
	}
}


/****************************************************************************
 * DMD display and sound effects
 ****************************************************************************/
//LIGHTING EFFECTS
void eyeball_flasher (char *flasher) {
	flasher_pulse (flasher); //FLASH followed by name of flasher in caps
	task_sleep (TIME_500MS);
	task_exit ();
	}
