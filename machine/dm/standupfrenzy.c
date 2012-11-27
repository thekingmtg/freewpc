/*
 * demolition man
 * standupfrenzy.c
 * 
 * written by James Cardona - based loosely on greed mode from TZ code
 *
 * Location Description:
 * Standups: There are 5 yellow standup targets scattered about the playfield,
 * like those in TZ. However, these targets are very important to game play.
 * One is located directly below the upper left flipper; another is to the
 * right of the left ramp; two more are angled to the left of the right ramp;
 * and one is to the right of the right ramp.
 *
 * Scoring Description: (original game)
 * These are similar to Twilight Zone's Greed targets.
 * They are worth 1 million plus 1 million per number of hits
 * (resets on each ball). Maximum of 20 hits.
 *
 * Depending on machine settings, One, Two, or Three sets of
 * targets must be completed to light Quick Freeze, depending on
 * whether Quick Freeze is set to easy, medium, or hard.
 *
 * an unlit target scores nothing
 * once all 5 hit - they extinguish and all 5 relight
 *
 * Scoring Description: (my rules)
 * same as above except
 * in frenzy mode worth 5 million plus 1 million per number of hits
 * no max number of hits - 20 second timer
 * outside frenzy worth 500k points
 *
 *
 * estimate of average standupfrenzy mode score: 21 million to 56 million
 * estimate of average standup score: 2.5 million
 *
 * */
/* CALLSET_SECTION (standupfrenzy, __machine4__) */

#include <freewpc.h>
#include "dm/global_constants.h"
#include "clawmagnet.h"
#include "standupfrenzy.h"

//constants
#define 	NO_TARGETS 	0x0/** Bitmask referring to all 5 standup targets */
#define 	ALL_TARGETS 0x1f/** Bitmask referring to all 5 standup targets */
const U8 			STANDUP_EASY_GOAL 	= 1;
const U8 			STANDUP_MED_GOAL 	= 4;
const U8 			STANDUP_HARD_GOAL 	= 7;
const U8 			STANDUP_GOAL_INCREMENT = 5;
const U8 			STANDUP_GOAL_MAX 	= 100;

//local variables
U8			standup_MessageCounter;
U8			standupFrenzy_temp_counter;//generic counter
U8			standupFrenzy_SoundCounter = 0;//used for randomizing sound calls
U8			standup_SoundCounter = 0;//used for randomizing sound calls
score_t 	standupFrenzy_temp_score;//generic score for calculations

__local__ 	U8		standupFrenzy_modes_achieved;
__local__ 	U8 		standupFrenzyNumHits;
			U8 		standupFrenzyTimer;
score_t 			standupFrenzyTotalScore;
score_t 			standupFrenzyLastScore;
score_t 			standupFrenzyNextScore;
			U8 		standupFrenzyLightsLit; //tracks which LIGHTS ARE LIT
			U8 		standupLightsLit; 		//tracks which LIGHTS ARE LIT
__local__ 	U8 		standup_num_of_hits;
__local__ 	U8 		standup_goal;
__boolean 			isStandupFrenzyActivated;
__local__ 	U8		exploded[5];

//external variables

//internally called function prototypes  --external found at protos.h
void standupFrenzy_mode_init (void);
void standupFrenzy_mode_exit (void);
void standupHandler (U8 target);
void standupHandler1 (U8 target, U8 lamp);
void frenzyHandler (U8 target, U8 lamp);
void standup_lamp_update1 (U8 mask, U8 lamp);
void standupFrenzy_sounds (void);
void standup_sounds (void);
void standup_effect(void);


/****************************************************************************
 * mode definition structure
 ***************************************************************************/
struct timed_mode_ops standupFrenzy_mode = {
	DEFAULT_MODE,
	.init = standupFrenzy_mode_init,
	.exit = standupFrenzy_mode_exit,
	.gid = GID_STANDUPFRENZY_MODE_RUNNING,
	.music = MUS_MD_STANDUP_FRENZY,
	.deff_starting = DEFF_STANDUPFRENZY_START_EFFECT,
	.deff_running = DEFF_STANDUPFRENZY_EFFECT,
//	.deff_ending = DEFF_STANDUPFRENZY_END_EFFECT,
	.prio = PRI_GAME_MODE5,
	.init_timer = 23, //time displayed plus length of starting deff
	.timer = &standupFrenzyTimer,
	.grace_timer = 3, //default is 2
	//.pause = , // default is null_false_function - other is system_timer_pause,
};


/****************************************************************************
 * initialize  and exit
 ***************************************************************************/
void standupFrenzy_mode_init (void) {
		//the claw mode can expire on its own and since it is a lower priority it will not display
		//callset_invoke (end_claw_mode); // this seemed to cause occasional crashes
		clawmagnet_off ();
		flag_off(FLAG_IS_BALL_ON_CLAW);
		flipper_enable ();
	isStandupFrenzyActivated = TRUE;
	++standupFrenzy_modes_achieved;
	standupFrenzyNumHits = 0;
	standupFrenzyLightsLit = ALL_TARGETS;
	callset_invoke (lamp_update);
	score_zero (standupFrenzyTotalScore);
	score_zero (standupFrenzyLastScore);
	score_zero (standupFrenzyNextScore);
	score_add (standupFrenzyNextScore, score_table[FRENZY_HIT_SCORE]);
	score_add (standupFrenzyTotalScore, score_table[FRENZY_START_SCORE]);
	score (FRENZY_START_SCORE);
	U8 i;
	for (i = 0; i < 5; i++) exploded[i] = 0;
}//end of standupFrenzy_mode_init 



void standupFrenzy_mode_exit (void) {
	/* Light all 'default' lamps */
	isStandupFrenzyActivated = FALSE;
	standupFrenzyLightsLit = NO_TARGETS;
	standupLightsLit  = ALL_TARGETS;
	callset_invoke (lamp_update);
}//end of standupFrenzy_mode_exit 



CALLSET_ENTRY (standupFrenzy, start_player) {
standup_MessageCounter = 0;
standupFrenzy_modes_achieved = 0;  //these need to be zeroed in before we enter the mode so bonus doesn't fake trigger
standupFrenzyNumHits = 0;
/* Light all 'default' lamps */
standupFrenzyLightsLit = NO_TARGETS;
standupLightsLit  = ALL_TARGETS;
standup_num_of_hits = 0;
standup_goal = STANDUP_EASY_GOAL;
isStandupFrenzyActivated = FALSE;
callset_invoke (lamp_update);
}//end of function


/****************************************************************************
 * external event listeners
 ****************************************************************************/
CALLSET_ENTRY (standupFrenzy, music_refresh)  	{ timed_mode_music_refresh (&standupFrenzy_mode); }
CALLSET_ENTRY (standupFrenzy, end_ball) 		{ if (timed_mode_running_p(&standupFrenzy_mode) ) timed_mode_end (&standupFrenzy_mode); }
CALLSET_ENTRY (standupFrenzy, display_update) 	{ timed_mode_display_update (&standupFrenzy_mode); }


/****************************************************************************
 * standup target hit
 ****************************************************************************/
/* declare event listeners for standup target hits*/
/* target is given as a bitmask to cover all the switches*/
CALLSET_ENTRY (standupFrenzy, sw_standup_1) { standupHandler(0x1); }
CALLSET_ENTRY (standupFrenzy, sw_standup_2) { standupHandler(0x2); }
CALLSET_ENTRY (standupFrenzy, sw_standup_3) { standupHandler(0x4); }
CALLSET_ENTRY (standupFrenzy, sw_standup_4) { standupHandler(0x8); }
CALLSET_ENTRY (standupFrenzy, sw_standup_5) { standupHandler(0x10); }



void standupHandler (U8 target) {
	const U8 sw = task_get_arg ();
	const U8 lamp = switch_lookup_lamp (sw);
	//verify target hit was a lit target and verify mode is running
	if ((standupFrenzyLightsLit & target) && timed_mode_running_p(&standupFrenzy_mode)) frenzyHandler (target, lamp);
	//else mode NOT running ---verify target hit was a lit target
	else if ((standupLightsLit & target) && !timed_mode_running_p(&standupFrenzy_mode)) standupHandler1 (target, lamp);
	//else mode NOT running and target NOT lit ---check for prison break or huxley
	else {
		if (flag_test(FLAG_IS_HUXLEY_RUNNING) )	huxley_mode_shot_made();
		else if (flag_test (FLAG_IS_PBREAK_RUNNING) ) { prison_break_made(); }
	}//otherwise - do nothing
}//end of standupHandler



void update_exploded (U8 target){
	switch (target) {
			case 0x1:  exploded[0]++; break;
			case 0x2:  exploded[1]++; break;
			case 0x4:  exploded[2]++; break;
			case 0x8:  exploded[3]++; break;
			case 0x10: exploded[4]++; break;
	}//end of switch
}//end of function


void frenzyHandler (U8 target, U8 lamp) {
		++standupFrenzyNumHits;
		sound_send (EXPLOSION1_LONG);
		update_exploded(target); //this triggers explosion in display
		standupFrenzyLightsLit &= ~target;  // flag that target as hit
		standup_lamp_update1 (target, lamp);//turn that lamp off
		standupFrenzy_sounds();

		//score 5 million plus 1 million times number of hits
		score_zero (standupFrenzy_temp_score);//zero out temp score
		score_add (standupFrenzy_temp_score, score_table[FRENZY_ADD_SCORE]);//multiply 1M by num hits
		score_mul (standupFrenzy_temp_score, standupFrenzyNumHits);
		score (FRENZY_HIT_SCORE);
		score_long_unmultiplied (standupFrenzy_temp_score); //add temp score to player's score

		//do same for mode score
		score_add (standupFrenzyTotalScore, score_table[FRENZY_HIT_SCORE]);
		score_add (standupFrenzyTotalScore, standupFrenzy_temp_score);

		//do same for last score-this is the one we display
			score_zero (standupFrenzyLastScore);
			score_add (standupFrenzyLastScore, score_table[FRENZY_HIT_SCORE]);
			score_add (standupFrenzyLastScore, standupFrenzy_temp_score);

		//do same for next score-this is the one we display
		score_zero (standupFrenzyNextScore);
		score_add (standupFrenzyNextScore, score_table[FRENZY_HIT_SCORE]);
		score_add (standupFrenzyNextScore, standupFrenzy_temp_score);
		score_add (standupFrenzyNextScore, score_table[FRENZY_ADD_SCORE]);//next score will be 1 million more
		//if 5th light out then reset all lights back on
		if (standupFrenzyNumHits % 5 == 0) 	{
			standupFrenzyLightsLit = ALL_TARGETS;
			callset_invoke (lamp_update);//turn all lamps on
			U8 i;
			for (i = 0; i < 5; i++) exploded[i] = 0; //RESET DEFF DISPLAY
		}
}//end of function



void standupHandler1 (U8 target, U8 lamp) {
		++standup_num_of_hits;
		standup_effect();
		standupLightsLit &= ~target;  /* flag that target as hit */
		standup_lamp_update1 (target, lamp);
		if (flag_test (FLAG_IS_PBREAK_RUNNING) ) 		prison_break_made();
		else 											standup_sounds();
		lamp_tristate_flash (lamp);
		score (STANDUP_SCORE);
		//if we reached our quick freeze goal and quick freeze is not already set
		if (standup_num_of_hits >= standup_goal  && !flag_test(FLAG_IS_LRAMP_QUICKFREEZE_ACTIVATED)) {
				sound_start (ST_SPEECH, SPCH_QUICK_FREEZE_ACTIVATED, SL_4S, PRI_GAME_QUICK5);
				light_quick_freeze_light_on();//sent to inlanes.c
				if (standup_goal < STANDUP_GOAL_MAX ) standup_goal += STANDUP_GOAL_INCREMENT;
			}
		//if 5th light out then reset all lights back on
		if (standup_num_of_hits % 5 == 0) 	{
			standupLightsLit = ALL_TARGETS;
			callset_invoke (lamp_update);//turn all lamps on
		}
}//end of function



U8 collect_standups_counter;
void collect_standups(void ) {
	for (collect_standups_counter = 0; collect_standups_counter <= standup_num_of_hits; collect_standups_counter++) {
			score (STANDUP_SCORE);
			deff_start (DEFF_COLLECT_STANDUPS_EFFECT);
	}//end of loop
}//end of function


/****************************************************************************
 * stand up lamps update
 ****************************************************************************/
void standup_lamp_update1 (U8 mask, U8 lamp) {
	if (timed_mode_running_p (&standupFrenzy_mode))	{
			//target was not hit yet, flash it
			if (standupFrenzyLightsLit & mask)	lamp_tristate_flash (lamp);
			else								lamp_tristate_off (lamp);
	}//end of if
	else { //mode not running
		//target was not hit yet so it is on
		if (standupLightsLit & mask) lamp_tristate_on (lamp);
		else	lamp_tristate_off (lamp);
	}//end of else
}//end of standup_lamp_update1



CALLSET_ENTRY (standupFrenzy, lamp_update) {
	standup_lamp_update1 (0x1, LM_STANDUP_1);
	standup_lamp_update1 (0x2, LM_STANDUP_2);
	standup_lamp_update1 (0x4, LM_STANDUP_3);
	standup_lamp_update1 (0x8, LM_STANDUP_4);
	standup_lamp_update1 (0x10, LM_STANDUP_5);
}//end of lamp_update 


//claw freeze starts standup frenzy instead of locking a ball
CALLSET_ENTRY (standupFrenzy, sw_claw_freeze) {
	demotime_increment();
	timed_mode_begin (&standupFrenzy_mode);
}



/*  not sure about this
void standupFrenzy_mode_expire (void) {
	if (score_compare (score_table[SC_10M], standupFrenzyTotalScore) == 1)
		callset_invoke (start_hurryup);
}// end of standupFrenzy_mode_expire 
*/


/****************************************************************************
 * sound effects
 ****************************************************************************/
void standup_sounds (void) {
	standup_SoundCounter = random_scaled(3);//from kernal/random.c
	if ( standup_SoundCounter  == 0 )
			sound_start (ST_EFFECT, ZAPP_3_SHORT, SL_2S, PRI_GAME_QUICK5);
	else if ( standup_SoundCounter  == 1 )
			sound_start (ST_EFFECT, ZAPP_3_MED, SL_2S, PRI_GAME_QUICK5);
	else if ( standup_SoundCounter  == 2 )
			sound_start (ST_EFFECT, ZAPP_3_LONG, SL_2S, PRI_GAME_QUICK5);
}//end of function



void standupFrenzy_sounds (void) {
	standupFrenzy_SoundCounter = random_scaled(3);//from kernal/random.c
	if ( standupFrenzy_SoundCounter  == 0 )
			sound_start (ST_EFFECT, CHORD1, SL_2S, PRI_GAME_QUICK5);
	else if ( standupFrenzy_SoundCounter  == 1 )
			sound_start (ST_EFFECT, CHORD2, SL_2S, PRI_GAME_QUICK5);
	else if ( standupFrenzy_SoundCounter  == 2 )
			sound_start (ST_EFFECT, CHORD3, SL_2S, PRI_GAME_QUICK5);
}//end of function


/****************************************************************************
 * DMD display - non-frenzy
 ****************************************************************************/
//flash the page and show the number - counting down
void collect_standups_effect_deff(void) {
	dmd_alloc_pair ();
	dmd_clean_page_low ();
	sprintf ("%d", standup_num_of_hits - collect_standups_counter);
	font_render_string_center(&font_fireball, DMD_MIDDLE_X, DMD_MIDDLE_Y, sprintf_buffer);
	dmd_copy_low_to_high ();
	dmd_show_low ();
	dmd_invert_page (dmd_low_buffer);
	deff_swap_low_high (1, TIME_100MS);

	if (collect_standups_counter < 3 )		{
		sound_start (ST_EFFECT, ZAPP_3_LONG, SL_2S, PRI_GAME_QUICK2);
		task_sleep (TIME_500MS);
		task_sleep (TIME_250MS);
	}
	else if (collect_standups_counter < 6) {
		sound_start (ST_EFFECT, ZAPP_3_MED, SL_2S, PRI_GAME_QUICK3);
		task_sleep (TIME_500MS);
	}
	else 			{
		sound_start (ST_EFFECT, ZAPP_3_SHORT, SL_2S, PRI_GAME_QUICK5);
		task_sleep (TIME_300MS);
	}
	deff_exit ();
}//end of FUNCTION



void standup_effect(void) {
	deff_start (DEFF_STANDUP_EFFECT);
}//end of FUNCTION



void standup_effect_deff (void) {
	dmd_alloc_low_clean ();
	dmd_sched_transition (&trans_scroll_up);
	font_render_string_center (&font_fireball, DMD_MIDDLE_X, DMD_BIG_CY_Top, "STAND");
	dmd_show_low ();

	dmd_alloc_low_clean ();
	dmd_sched_transition (&trans_scroll_up);
	font_render_string_center (&font_fireball, DMD_MIDDLE_X, DMD_BIG_CY_Top, "UP");
	dmd_show_low ();

	dmd_clean_page_low ();
	dmd_sched_transition (&trans_scroll_up);
	switch (++standup_MessageCounter % 3) {
		default:
		case 0:
			if (standup_num_of_hits == 0) 			sprintf ("0");//for testing in development
			else 									sprintf ("%d", standup_num_of_hits);
			font_render_string_center(&font_fireball, DMD_MIDDLE_X, DMD_BIG_CY_Top - 2, sprintf_buffer);
			sprintf ("STANDUPS");
			font_render_string_center(&font_var5, DMD_MIDDLE_X, DMD_SMALL_CY_3, sprintf_buffer);
			sprintf ("HIT");
			font_render_string_center(&font_var5, DMD_MIDDLE_X, DMD_SMALL_CY_4, sprintf_buffer);
			break;
		case 1:
			if (!flag_test(FLAG_IS_LRAMP_QUICKFREEZE_ACTIVATED)) {
				sprintf ("HIT %d MORE", standup_goal - standup_num_of_hits);
				font_render_string_center (&font_var5, DMD_MIDDLE_X, DMD_SMALL_CY_1, sprintf_buffer);
				font_render_string_center (&font_var5, DMD_MIDDLE_X, DMD_SMALL_CY_3, "TO");
				font_render_string_center (&font_var5, DMD_MIDDLE_X, DMD_SMALL_CY_4, "LIGHT QUICK FREEZE");
			}
			else {
				font_render_string_center (&font_lithograph, DMD_MIDDLE_X, DMD_SMALL_CY_1, "FREEZE");
				font_render_string_center (&font_var5, DMD_MIDDLE_X, DMD_SMALL_CY_3, "IS");
				font_render_string_center (&font_var5, DMD_MIDDLE_X, DMD_SMALL_CY_4, "ACTIVATED");
			}
			break;
		case 2:
			font_render_string_center (&font_var5, DMD_MIDDLE_X, DMD_SMALL_CY_1, "CRYO CLAW");
			font_render_string_center (&font_var5, DMD_MIDDLE_X, DMD_SMALL_CY_2, "LOCK FREEZE");
			font_render_string_center (&font_var5, DMD_MIDDLE_X, DMD_SMALL_CY_3, "STARTS");
			font_render_string_center (&font_var5, DMD_MIDDLE_X, DMD_SMALL_CY_4, "STANDUP FRENZY");
			break;
		}//end of switch
	dmd_show_low ();
	task_sleep_sec (1);
	task_sleep (TIME_500MS);
	deff_exit ();
}//end of standupFrenzy_mode_deff


/****************************************************************************
 * DMD display - frenzy
 ****************************************************************************/
void standupfrenzy_start_effect_deff (void) {
	U8 x;
	U8 y;
	U8 i = 0;
	sound_send (EXPLOSION1_SHORT);
	do { // Shake the text
			dmd_alloc_pair_clean ();
			dmd_draw_thin_border (dmd_low_buffer);
			if 		(i < 5) { 	x = random_scaled (1);	y = random_scaled (2); }
			else if (i < 10) {  x = random_scaled (2);  y = random_scaled (3); }
			else if (i < 15) {  x = random_scaled (4);  y = random_scaled (5); }
			else if (i < 22) {  x = random_scaled (8);  y = random_scaled (4); }
			else 			 {  x = random_scaled (10); y = random_scaled (5); }
			if (i % 2 == 0)  {
				font_render_string_center (&font_fireball, DMD_MIDDLE_X + x, y + DMD_BIG_CY_Cent, "FRENZY");
				font_render_string_center (&font_fireball, DMD_MIDDLE_X - x, DMD_BIG_CY_Cent - y, "FRENZY");
			}
			else {
				font_render_string_center (&font_fireball, DMD_MIDDLE_X + x, y - DMD_BIG_CY_Cent, "FRENZY");
				font_render_string_center (&font_fireball, DMD_MIDDLE_X - x, DMD_BIG_CY_Cent + y, "FRENZY");
			}
				//			dmd_show_high ();
//			dmd_show2 ();//shows a 4 color image
			dmd_show_low ();
			if (i == 0) sound_send (EXPLOSION1_SHORT);
			if (i == 10) sound_send (EXPLOSION1_SHORT);
			if (i == 18) sound_send (EXPLOSION1_SHORT);
			if (i == 24) sound_send (EXPLOSION1_SHORT);
			if (i == 28) sound_send (EXPLOSION1_MED);
			task_sleep (TIME_66MS);
	} while (i++ < 30); //APPROX 2 SEC
	//pulsate words in middle
	sound_send (EXPLOSION1_LONG);
	i = 4;
	dmd_alloc_pair_clean ();
	dmd_draw_thin_border (dmd_low_buffer);
	font_render_string_center (&font_fireball, DMD_MIDDLE_X, DMD_BIG_CY_Cent, "FRENZY");
	// low = text, high = blank
	while (--i > 0){
		dmd_show2 ();
		task_sleep (TIME_66MS);

		dmd_flip_low_high ();
		dmd_show2 ();
		task_sleep (TIME_66MS);

		dmd_show_high ();
		task_sleep (TIME_66MS);

		dmd_show2 ();
		task_sleep (TIME_66MS);
		dmd_flip_low_high ();
	}//end of loop - APPROX 1 SEC
	deff_exit (); //TOTAL OF 3 SEC
} // standupFrenzyTotalScore_deff



void standupfrenzy_show_circle_1_rotate_cw (U8 step, U8 x, U8 y) {
	switch (step) {
	case 0:	bitmap_blit (circle1_1_low_bits, x, y);	break;
	case 1:	bitmap_blit (circle1_2_low_bits, x, y);	break;
	case 2:	bitmap_blit (circle1_3_low_bits, x, y);	break;
	case 3:	bitmap_blit (circle1_4_low_bits, x, y);	break;
	case 4:	bitmap_blit (circle1_5_low_bits, x, y);	break;
	case 5:	bitmap_blit (circle1_6_low_bits, x, y);	break;
	case 6:	bitmap_blit (circle1_7_low_bits, x, y);	break;
	case 7:	bitmap_blit (circle1_8_low_bits, x, y);	break;
	}//end of switch
}//end of function

void standupfrenzy_show_circle_1_rotate_ccw (U8 step, U8 x, U8 y) {
	switch (step) {
	case 0:	bitmap_blit (circle1_8_low_bits, x, y);	break;
	case 1:	bitmap_blit (circle1_7_low_bits, x, y);	break;
	case 2:	bitmap_blit (circle1_6_low_bits, x, y);	break;
	case 3:	bitmap_blit (circle1_5_low_bits, x, y);	break;
	case 4:	bitmap_blit (circle1_4_low_bits, x, y);	break;
	case 5:	bitmap_blit (circle1_3_low_bits, x, y);	break;
	case 6:	bitmap_blit (circle1_2_low_bits, x, y);	break;
	case 7:	bitmap_blit (circle1_1_low_bits, x, y);	break;
	}//end of switch
}//end of function

void standupfrenzy_show_circle_2_rotate_cw (U8 step, U8 x, U8 y) {
	switch (step) {
	case 0:	bitmap_blit (circle2_1_low_bits, x, y);	break;
	case 1:	bitmap_blit (circle2_2_low_bits, x, y);	break;
	case 2:	bitmap_blit (circle2_3_low_bits, x, y);	break;
	case 3:	bitmap_blit (circle2_4_low_bits, x, y);	break;
	case 4:	bitmap_blit (circle2_5_low_bits, x, y);	break;
	case 5:	bitmap_blit (circle2_6_low_bits, x, y);	break;
	case 6:	bitmap_blit (circle2_7_low_bits, x, y);	break;
	case 7:	bitmap_blit (circle2_8_low_bits, x, y);	break;
	}//end of switch
}//end of function

void standupfrenzy_show_circle_2_rotate_ccw (U8 step, U8 x, U8 y) {
	switch (step) {
	case 0:	bitmap_blit (circle2_8_low_bits, x, y);	break;
	case 1:	bitmap_blit (circle2_7_low_bits, x, y);	break;
	case 2:	bitmap_blit (circle2_6_low_bits, x, y);	break;
	case 3:	bitmap_blit (circle2_5_low_bits, x, y);	break;
	case 4:	bitmap_blit (circle2_4_low_bits, x, y);	break;
	case 5:	bitmap_blit (circle2_3_low_bits, x, y);	break;
	case 6:	bitmap_blit (circle2_2_low_bits, x, y);	break;
	case 7:	bitmap_blit (circle2_1_low_bits, x, y);	break;
	}//end of switch
}//end of function

void standupfrenzy_show_rotate_ccw_exploded (U8 step, U8 x, U8 y) {
	switch (step) {
	case 0:	bitmap_blit (explos1_1_low_bits, x, y);	break;
	case 1:	bitmap_blit (explos1_2_low_bits, x, y);	break;
	case 2:	bitmap_blit (explos1_3_low_bits, x, y);	break;
	case 3:	bitmap_blit (explos1_4_low_bits, x, y);	break;
	case 4:	bitmap_blit (explos1_5_low_bits, x, y);	break;
	case 5:	bitmap_blit (explos1_6_low_bits, x, y);	break;
	case 6:	bitmap_blit (explos1_7_low_bits, x, y);	break;
	case 7:	break;
	}//end of switch
}//end of function

void standupfrenzy_show_rotate_cw_exploded (U8 step, U8 x, U8 y) {
	switch (step) {
	case 0:	bitmap_blit (explos2_1_low_bits, x, y);	break;
	case 1:	bitmap_blit (explos2_2_low_bits, x, y);	break;
	case 2:	bitmap_blit (explos2_3_low_bits, x, y);	break;
	case 3:	bitmap_blit (explos2_4_low_bits, x, y);	break;
	case 4:	bitmap_blit (explos2_5_low_bits, x, y);	break;
	case 5:	bitmap_blit (explos2_6_low_bits, x, y);	break;
	case 6:	bitmap_blit (explos2_7_low_bits, x, y);	break;
	case 7:	bitmap_blit (explos2_8_low_bits, x, y);	break;
	case 8:	bitmap_blit (explos2_9_low_bits, x, y);	break;
	case 9:	bitmap_blit (explos2_10_low_bits, x, y);	break;
	}//end of switch
}//end of function



void standupfrenzy_effect_deff (void) {
	U8 i = 0;
	for (;;) {
		i++;
		//for testing only
/*
		if (i == 30) {exploded[0]++;}
		if (i == 50) {exploded[1]++;}
		if (i == 70) {exploded[2]++;}
		if (i == 90) {exploded[3]++;}
		if (i == 110) {exploded[4]++;}
*/
		dmd_alloc_low_clean ();
		dmd_draw_thin_border (dmd_low_buffer);

		font_render_string_center (&font_var5, DMD_MIDDLE_X, DMD_SMALL_CY_1, "SHOOT STANDUP TARGETS");

		if (exploded[0]) 	standupfrenzy_show_rotate_ccw_exploded( exploded[0]++, 2, 8);
		else				standupfrenzy_show_circle_2_rotate_ccw 	( (i)%8, 	2, 8);

		if (exploded[1]) 	standupfrenzy_show_rotate_cw_exploded( exploded[1]++, 27, 8);
		else				standupfrenzy_show_circle_1_rotate_cw 	( (i+3)%8, 27, 8);

		if (exploded[2]) 	standupfrenzy_show_rotate_ccw_exploded( exploded[2]++, 52, 8);
		else				standupfrenzy_show_circle_1_rotate_ccw 	( (i+1)%8, 52, 8);

		if (exploded[3]) 	standupfrenzy_show_rotate_cw_exploded( exploded[3]++, 77, 8);
		else				standupfrenzy_show_circle_2_rotate_cw 	( (i+5)%8, 77, 8);

		if (exploded[4]) 	standupfrenzy_show_rotate_ccw_exploded( exploded[4]++, 102, 8);
		else				standupfrenzy_show_circle_1_rotate_ccw 	( (i+1)%8,102, 8);
		dmd_show_low ();
		task_sleep (TIME_100MS);
	}//END OF ENDLESS LOOP
}//end of standupFrenzy_mode_deff


void standupfrenzy_end_effect_deff (void) {
	dmd_alloc_low_clean ();
	font_render_string_center (&font_fireball, DMD_MIDDLE_X, DMD_BIG_CY_Top, "FRENZY");
	sprintf_score (standupFrenzyTotalScore);
	font_render_string_center (&font_var5, DMD_MIDDLE_X, DMD_BIG_CY_Bot, sprintf_buffer);
	dmd_show_low ();
	task_sleep_sec (2);
	deff_exit ();
} // standupFrenzyTotalScore_deff
