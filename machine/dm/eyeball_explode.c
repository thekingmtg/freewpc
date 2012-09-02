/*
 * demolition man
 * eyeball_explode.c
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
 * It takes 1 hit for the first Hurry-Up, four for the next on the easiest level.
 *
 * All four Explode ramps are lit at a value of 15M that begins counting down.
 * Hit any explode lamp to collect and add 10M to the value for the next Explode shot.
 * The round times out when the countdown reaches 5M (or higher if you've collected a few of the shots).
 * An extra ball is lit at three Retina Scans.
 * TODO: Retina Scan shots feed to the left saucer (also called eject) which drops the ball into the left inlane.
 *
 *
 * Scoring Description: (my rules)
 * same as above except explode mode times out after 30 seconds or ball drain
 * and eyeball shot can be hit in addition to explode arrows
 *
 * estimate of average explode mode score: 80 million to 120 million
 * estimate of average eyeball score: 10 million to 20 million
 *
 *
 */

#include <freewpc.h>
#include "dm/global_constants.h"

//constants
const U8 EYE_EASY_GOAL = 3;
const U8 EYE_PREDEFINED_GOAL_INCREMENT = 1;
const U8 EYE_GOAL_STEP = 3;
const U8 EYE_GOAL_MAX = 50;
const U8 EYE_EB_EASY_GOAL = 5;
const U8 EYE_EB_GOAL_STEP = 3;
const U8 EYE_EB_GOAL_MAX = 50;
const U8 explode_mode_timer_value = 20;

//local variables
U8 explode_temp_counter; //temporary counter
U8 explode_SoundCounter;
U8 eyeball_shots_made;//for current ball only
U8 eyeball_eb_shots_made;//for current ball only
U8 total_eyeball_shots_made;//for entire game
U8 eyeball_goal;//num of hits to start explode
U8 eyeball_eb_goal;//num of hits to light extraball
U8 explode_mode_timer;
U8 explode_mode_shots_made;//number of times an explode arrow or eyeball is hit
U8 explode_modes_achieved_counter;//number of times mode achieved
U8 eject_killer_counter;
score_t explode_mode_score;
score_t explode_mode_last_score;
score_t temp_score;
__boolean 	is_explode_activated;//external in orbits and ramps

//external variables
extern 	U8 				jet_shots_made;//found in jets_superjets.c

//prototypes
void explode_mode_init (void);
void explode_mode_expire (void);
void explode_mode_exit (void);
void eyeball_reset (void);
void player_eyeball_reset (void);
void eyeball_goal_award (void);
void eyeball_eb_award (void);
void eyeball_effect_deff(void);
void explode_effect_deff(void);
void eject_killer_task (void);



/****************************************************************************
 * mode definition structure
 ***************************************************************************/
struct timed_mode_ops explode_mode = {
	DEFAULT_MODE,
	.init = explode_mode_init,
	.exit = explode_mode_exit,
	.gid = GID_EXPLODE_MODE_RUNNING,
	.music = MUS_MD_EXPLODE,
	.deff_starting = DEFF_EXPLODE_START_EFFECT,
	.deff_running = DEFF_EXPLODE_EFFECT,
//	.deff_ending = DEFF_EXPLODE_END,
	.prio = PRI_GAME_MODE1,
	.init_timer = 20, //explode_mode_timer_value should equal this
	.timer = &explode_mode_timer,
	.grace_timer = 0,
	.pause = system_timer_pause,
};


/****************************************************************************
 * initialize  and exit
 ***************************************************************************/
void player_eyeball_reset (void) {
	total_eyeball_shots_made = 0;
	eyeball_shots_made = 0;
	eyeball_goal = EYE_EASY_GOAL;
	eyeball_eb_goal = EYE_EB_EASY_GOAL;
	is_explode_activated = FALSE;
	explode_modes_achieved_counter = 0;
	explode_mode_shots_made = 0;
	explode_SoundCounter = 0;
	eject_killer_counter = 0;
}

void eyeball_reset (void) {
	eyeball_shots_made = 0;
	is_explode_activated = FALSE;
}



void explode_mode_init (void) {
	is_explode_activated = TRUE;
	explode_mode_shots_made = 0;
	if ( (explode_SoundCounter++ % 2) == 0 )//check if even
		sound_start (ST_SPEECH, SPCH_EXPLODE_ACTIVATED, SL_5S, PRI_GAME_QUICK5);
	else
		sound_start (ST_SPEECH, SPCH_EXPLODE_HURRYUP, SL_5S, PRI_GAME_QUICK5);
	score_zero(explode_mode_score);
	callset_invoke(Activate_Explode_Inserts);
	++explode_modes_achieved_counter;
	}//end of function



void explode_mode_expire (void) {
	is_explode_activated = FALSE;
	callset_invoke(DeActivate_Explode_Inserts);//sent to ramps.c and orbits.c
}


void explode_mode_exit (void) { explode_mode_expire();}


/****************************************************************************
 * external event listeners
 ****************************************************************************/
CALLSET_ENTRY (eyeball_explode, music_refresh)  { timed_mode_music_refresh (&explode_mode); }
CALLSET_ENTRY (eyeball_explode, end_ball)	 	{ if (timed_mode_running_p(&explode_mode) ) timed_mode_end (&explode_mode); }
CALLSET_ENTRY (eyeball_explode, display_update) { timed_mode_display_update (&explode_mode); }
CALLSET_ENTRY (eyeball_explode, start_player) 	{ player_eyeball_reset(); }
CALLSET_ENTRY (eyeball_explode, start_ball) 	{ eyeball_reset(); }

/****************************************************************************
 * playfield lights and flags
 ***************************************************************************/



/****************************************************************************
 * body
 *
 ***************************************************************************/
void eyeball_goal_award (void) {
		eyeball_shots_made = 0;
		sound_start (ST_SPEECH, SPCH_LOVE_WHEN_THAT_HAPPENS, SL_2S, PRI_GAME_QUICK5);
		timed_mode_begin (&explode_mode);//start explode mode
		if (eyeball_goal < EYE_GOAL_MAX)  eyeball_goal += EYE_GOAL_STEP;
	}


void eyeball_eb_award (void) {
		callset_invoke(ExtraBall_Light_On);
		eyeball_eb_shots_made = 0;
		sound_start (ST_SPEECH, SPCH_LOVE_WHEN_THAT_HAPPENS, SL_2S, PRI_GAME_QUICK5);
		if (eyeball_eb_goal < EYE_EB_GOAL_MAX)  eyeball_eb_goal += EYE_EB_GOAL_STEP;
	}


CALLSET_ENTRY (eyeball_explode, sw_eyeball_standup) {
	flasher_pulse (FLASH_EYEBALL_FLASHER); //FLASH followed by name of flasher in caps
	flasher_pulse (FLASH_EYEBALL_FLASHER); //FLASH followed by name of flasher in caps
	flasher_pulse (FLASH_EYEBALL_FLASHER); //FLASH followed by name of flasher in caps
	sound_start (ST_SAMPLE, EXPLOSION1_MED, SL_2S, PRI_GAME_QUICK1);
	score (SC_5M);
	//100k per jet hit here
	if (jet_shots_made > 0) {
		score_zero (temp_score);//zero out temp score
		 explode_temp_counter = jet_shots_made;
		do {
			score_add (temp_score, score_table[SC_100K]);//multiply 100K by jet count
		} while (--explode_temp_counter > 1);
		score_long_unmultiplied (temp_score); //add temp score to player's score
	}//end of if

	//light extra ball
	if (eyeball_eb_shots_made == eyeball_eb_goal)  eyeball_eb_award();

	//if explode mode not activated
	if ( !timed_mode_running_p(&explode_mode) ) {
		++eyeball_shots_made;
		if (eyeball_shots_made == eyeball_goal)  eyeball_goal_award();//start explode
		}//end of if timed mode not running
	else { //else explode activated
		callset_invoke(explode_made);
		}//end of else
	}//end of function




CALLSET_ENTRY (eyeball_explode, explode_made) {
	++explode_mode_shots_made;
	sound_start (ST_SAMPLE, EXPLOSION, SL_2S, PRI_GAME_QUICK1);
	score_zero(explode_mode_last_score);
	//score 15 million counting down to 5 million at
	//end of mode  + 10 million for bonus # of explodes hit
	if (explode_mode_timer > 25)  		{ score (SC_15M); score_add (explode_mode_last_score, score_table[SC_15M]);}
	else if (explode_mode_timer > 20)	{ score (SC_13M); score_add (explode_mode_last_score, score_table[SC_13M]); }
	else if (explode_mode_timer > 15) 	{ score (SC_11M); score_add (explode_mode_last_score, score_table[SC_11M]); }
	else if (explode_mode_timer > 10)	{ score (SC_9M); score_add (explode_mode_last_score, score_table[SC_9M]); }
	else if (explode_mode_timer > 5) 	{ score (SC_7M); score_add (explode_mode_last_score, score_table[SC_7M]); }
	else 								{ score (SC_5M); score_add (explode_mode_last_score, score_table[SC_5M]); }
	//bonus here

	switch (explode_mode_shots_made) {
	case 0: 	break;
	case 1:  { score (SC_1M); score_add (explode_mode_last_score, score_table[SC_1M]); break;}
	case 2:  { score (SC_2M); score_add (explode_mode_last_score, score_table[SC_2M]); break;}
	case 3:  { score (SC_3M); score_add (explode_mode_last_score, score_table[SC_3M]); break;}
	case 4:	 { score (SC_4M); score_add (explode_mode_last_score, score_table[SC_4M]); break;}
	default: { score (SC_4M); score_add (explode_mode_last_score, score_table[SC_4M]); break;}
	}//end of switch
	score_add (explode_mode_score, explode_mode_last_score);
	deff_start (DEFF_EXPLODE_HIT_EFFECT);
}//end of function



/****************************************************************************
 * eject saucer
 ****************************************************************************/
//this is to prevent a retrigger of the eject switch as soon as ball exits
void eject_killer_task (void){
	task_sleep (TIME_4S);
	eject_killer_counter = 0;
}//end of function


CALLSET_ENTRY (eyeball_explode, sw_eject) {
	if (eject_killer_counter++ == 1) {
		sound_start (ST_SAMPLE, RETINA_SCAN_LONG, SL_4S, PRI_GAME_QUICK1);
		score (SC_5M);
		deff_start (DEFF_EYEBALL_EFFECT);
		lamp_tristate_flash(LM_RETINA_SCAN);
		task_sleep (TIME_500MS);
		task_sleep (TIME_500MS);
		task_sleep (TIME_500MS);
		task_sleep (TIME_500MS);
		lamp_tristate_off(LM_RETINA_SCAN);
		flasher_pulse (FLASH_EJECT_FLASHER);
		flasher_pulse (FLASH_EJECT_FLASHER);
		flasher_pulse (FLASH_EJECT_FLASHER);
		flasher_pulse (FLASH_EJECT_FLASHER);
		sol_request (SOL_EJECT); //request to fire the eject sol
	}//end of if
	eject_killer_task();
}//end of function


/****************************************************************************
 * DMD display and sound effects
 ****************************************************************************/
void eyeball_effect_deff(void) {
	dmd_alloc_low_clean ();
	font_render_string_center (&font_term6, DMD_BIG_CX_Top, DMD_BIG_CY_Top, "RETINA");
	font_render_string_center (&font_term6, DMD_BIG_CX_Bot, DMD_BIG_CY_Bot, "SCAN");
	dmd_show_low ();
	task_sleep_sec (2);
	deff_exit ();
	}//end of mode_effect_deff


void explode_start_effect_deff(void) {
	U8 count = 8;
	dmd_alloc_pair_clean ();
	font_render_string_center (&font_lithograph, 64, 9, "EXPLODE");
	/* low = text, high = blank */
	while (--count > 0){
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
	}
	deff_exit ();
}//end of mode_effect_deff


void explode_hit_effect_deff(void) {
	dmd_alloc_low_clean ();
	font_render_string_center (&font_lithograph, DMD_BIG_CX_Top, DMD_BIG_CY_Top, "BOOM BABY");
	sprintf_score (explode_mode_last_score);
	font_render_string_center (&font_term6, DMD_BIG_CX_Bot, DMD_BIG_CY_Bot, sprintf_buffer);
	dmd_show_low ();
	task_sleep_sec (2);
	deff_exit ();
	}//end of mode_effect_deff



void explode_effect_deff(void) {
	U8 i;
	for (;;) {
		dmd_alloc_low_clean ();
		font_render_string_center (&font_lithograph, DMD_BIG_CX_Top, DMD_BIG_CY_Top, "EXPLODE");
		sprintf ("%d SEC LEFT,  %d HIT", explode_mode_timer, explode_mode_shots_made);
		font_render_string_center (&font_mono5, DMD_SMALL_CX_3, DMD_SMALL_CY_3, sprintf_buffer);


		for (i = 10; i < (140 - (explode_mode_timer * ( 140/explode_mode_timer_value) ) ); i++) {
		sprintf ("I");
		font_render_string_left (&font_mono5, i, DMD_SMALL_CY_4, sprintf_buffer);
		}

		dmd_show_low ();
		task_sleep (TIME_200MS);
	}//END OF ENDLESS LOOP
}//end of mode_effect_deff







void explode_end_deff(void) {
	dmd_alloc_low_clean ();
	font_render_string_center (&font_lithograph, DMD_BIG_CX_Top, DMD_BIG_CY_Top, "EXPLODE");
	font_render_string_center (&font_lithograph, DMD_BIG_CX_Bot, DMD_BIG_CY_Bot, "COMPLETED");
	dmd_show_low ();
	task_sleep_sec (2);
	deff_exit ();
	}//end of mode_effect_deff


/****************************************************************************
 * status display
 *
 * called from common/status.c automatically whenever either flipper button
 * is held for 4 seconds or longer.  since called by callset, order of
 * various status reports will be random depending upon call stack
****************************************************************************/
