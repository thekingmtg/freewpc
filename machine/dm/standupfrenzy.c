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

/*ALLSET_SECTION (standupfrenzy, __machine2__) */


#include <freewpc.h>
#include "dm/global_constants.h"

//constants
/** Bitmask referring to all 5 standup targets */
#define 	NO_TARGETS 	0x0
#define 	ALL_TARGETS 0x1f
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

__local__ U8		standupFrenzy_modes_achieved;
__local__ U8 		standupFrenzyNumHits;
U8 					standupFrenzyTimer;
__local__ score_t 	standupFrenzyTotalScore;
score_t 			standupFrenzyLastScore;
score_t 			standupFrenzyNextScore;
U8 					standupFrenzyLightsLit; //tracks which LIGHTS ARE LIT
U8 					standupLightsLit; 		//tracks which LIGHTS ARE LIT
__local__ U8 		standup_num_of_hits;
__local__ U8 		standup_goal;
__boolean 		isStandupFrenzyActivated;

//external variables
extern __local__ __boolean 	left_Ramp_QuickFreeze_activated;

//prototypes
void standupFrenzy_mode_init (void);
void standupFrenzy_mode_exit (void);
void standupHandler (U8 target);
void standupHandler1 (U8 target, U8 lamp);
void frenzyHandler (U8 target, U8 lamp);
void standup_lamp_update1 (U8 mask, U8 lamp);
void standupFrenzyTotalScore_effect_deff (void);
void standupFrenzy_mode_effect_deff (void);
void standupFrenzy_sounds (void);
void standup_sounds (void);



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
	.prio = PRI_GAME_MODE2,//lower priority than claw triggered modes
	.init_timer = 23, //time displayed plus length of starting deff
	.timer = &standupFrenzyTimer,
	.grace_timer = 3, //default is 2
	//.pause = , // default is null_false_function - other is system_timer_pause,
};


/****************************************************************************
 * initialize  and exit
 ***************************************************************************/
void standupFrenzy_mode_init (void) {
	isStandupFrenzyActivated = TRUE;
	++standupFrenzy_modes_achieved;
	standupFrenzyNumHits = 0;
	standupFrenzyLightsLit = ALL_TARGETS;
	callset_invoke (lamp_update);
	score_zero (standupFrenzyTotalScore);
	score_zero (standupFrenzyLastScore);
	score_zero (standupFrenzyNextScore);
	score_add (standupFrenzyNextScore, score_table[SC_5M]);
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
standupFrenzy_modes_achieved = 0;
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
}//end of standupHandler



void frenzyHandler (U8 target, U8 lamp) {
		++standupFrenzyNumHits;
		deff_start (DEFF_STANDUPFRENZY_HIT_EFFECT);
		standupFrenzyLightsLit &= ~target;  // flag that target as hit
		standup_lamp_update1 (target, lamp);//turn that lamp off
		standupFrenzy_sounds();
		//score 5 million plus 1 million times number of hits
		score_zero (standupFrenzy_temp_score);//zero out temp score
		score_add (standupFrenzy_temp_score, score_table[SC_1M]);//multiply 1M by num hits
		score_mul (standupFrenzy_temp_score, standupFrenzyNumHits);
		score (SC_5M);
		score_long_unmultiplied (standupFrenzy_temp_score); //add temp score to player's score
		//do same for mode score
		score_add (standupFrenzyTotalScore, score_table[SC_5M]);
		score_add (standupFrenzyTotalScore, standupFrenzy_temp_score);
			score_zero (standupFrenzyLastScore);
			score_add (standupFrenzyLastScore, score_table[SC_5M]);
			score_add (standupFrenzyLastScore, standupFrenzy_temp_score);
		score_zero (standupFrenzyNextScore);
		score_add (standupFrenzyNextScore, score_table[SC_5M]);
		score_add (standupFrenzyNextScore, standupFrenzy_temp_score);
		score_add (standupFrenzyNextScore, score_table[SC_1M]);//next score will be 1 million more
		//if 5th light out then reset all lights back on
		if (standupFrenzyNumHits % 5 == 0) 	{
			standupFrenzyLightsLit = ALL_TARGETS;
			callset_invoke (lamp_update);//turn all lamps on
		}
}//end of function



void standupHandler1 (U8 target, U8 lamp) {
		++standup_num_of_hits;
		deff_start (DEFF_STANDUP_EFFECT);
		standupLightsLit &= ~target;  /* flag that target as hit */
		standup_lamp_update1 (target, lamp);
		standup_sounds();
		lamp_tristate_flash (lamp);
		score (SC_500K);
		//if we reached our quick freeze goal and quick freeze is not already set
		if (standup_num_of_hits >= standup_goal  && !left_Ramp_QuickFreeze_activated) {
				sound_start (ST_SPEECH, SPCH_QUICK_FREEZE_ACTIVATED, SL_4S, PRI_GAME_QUICK5);
				callset_invoke(activate_left_ramp_quickfreeze);//sent to ramps.c  --left ramp
				if (standup_goal < STANDUP_GOAL_MAX ) standup_goal += standup_goal_increment;
			}
		//if 5th light out then reset all lights back on
		if (standup_num_of_hits % 5 == 0) 	{
			standupLightsLit = ALL_TARGETS;
			callset_invoke (lamp_update);//turn all lamps on
		}
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
CALLSET_ENTRY (standupFrenzy, sw_claw_freeze) { timed_mode_begin (&standupFrenzy_mode); }



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
void standup_effect_deff (void) {
	dmd_alloc_low_clean ();
	font_render_string_center (&font_fixed10, DMD_BIG_CX_Top, DMD_BIG_CY_Top, "STAND UP");
	dmd_show_low ();
	task_sleep_sec (1);
	dmd_sched_transition (&trans_vstripe_left2right);
	dmd_clean_page_low ();
	switch (++standup_MessageCounter % 3) {
		case 0:
			sprintf ("%d STANDUPS HIT", standup_num_of_hits);
			font_render_string_center(&font_mono5, DMD_BIG_CX_Bot, DMD_BIG_CY_Bot, sprintf_buffer);
			break;
		case 1:
			if (!left_Ramp_QuickFreeze_activated) {
				sprintf ("HIT %d MORE", standup_goal - standup_num_of_hits);
				font_render_string_center (&font_mono5, DMD_SMALL_CX_1, DMD_SMALL_CY_1, sprintf_buffer);
				font_render_string_center (&font_mono5, DMD_SMALL_CX_3, DMD_SMALL_CY_3, "TO");
				font_render_string_center (&font_mono5, DMD_SMALL_CX_4, DMD_SMALL_CY_4, "LIGHT QUICK FREEZE");
			}
			else {
				font_render_string_center (&font_mono5, DMD_SMALL_CX_1, DMD_SMALL_CY_1, "QUICK FREEZE");
				font_render_string_center (&font_mono5, DMD_SMALL_CX_3, DMD_SMALL_CY_3, "IS");
				font_render_string_center (&font_mono5, DMD_SMALL_CX_4, DMD_SMALL_CY_4, "ACTIVATED");
			}
			break;
		case 2:
			font_render_string_center (&font_mono5, DMD_SMALL_CX_1, DMD_SMALL_CY_1, "CRYO CLAW");
			font_render_string_center (&font_mono5, DMD_SMALL_CX_2, DMD_SMALL_CY_2, "LOCK FREEZE");
			font_render_string_center (&font_mono5, DMD_SMALL_CX_3, DMD_SMALL_CY_3, "STARTS");
			font_render_string_center (&font_mono5, DMD_SMALL_CX_4, DMD_SMALL_CY_4, "STANDUP FRENZY");
			break;
	}//END OF SWITCH
	dmd_show_low ();
	task_sleep_sec (2);
	deff_exit ();
}//end of standupFrenzy_mode_deff



/****************************************************************************
 * DMD display - frenzy
 ****************************************************************************/
/*mode is starting*/
void standupfrenzy_start_effect_deff (void) {
	U8 x;
	U8 y;
	U8 i = 0;
	dmd_alloc_low_clean ();
	font_render_string_center (&font_term6, DMD_BIG_CX_Top, DMD_BIG_CY_Top, "STANDUP");
	dmd_show_low ();
	task_sleep_sec (2);
	sound_send (EXPLOSION1_SHORT);
	do { // Shake the text
			dmd_alloc_low_clean ();
			if 		(i < 5) { 	x = random_scaled (1);	y = random_scaled (2); }
			else if (i < 10) {  x = random_scaled (2);  y = random_scaled (3); }
			else if (i < 15) {  x = random_scaled (4);  y = random_scaled (5); }
			else if (i < 20) {  x = random_scaled (8);  y = random_scaled (4); }
			else 			 {  x = random_scaled (10); y = random_scaled (5); }
			font_render_string_center (&font_fixed10, DMD_BIG_CX_Cent + x, 22 + DMD_BIG_CY_Cent, "FRENZY");
			dmd_show_low ();
			if (i % 10 == 0) sound_send (EXPLOSION1_SHORT);
			task_sleep (TIME_66MS);
	} while (i++ < 25);
	sound_send (EXPLOSION1_LONG);
	dmd_alloc_low_clean ();
	font_render_string_center (&font_fixed10, DMD_BIG_CX_Cent, DMD_BIG_CY_Cent, "FRENZY");
	dmd_show_low ();
	task_sleep_sec (1);
	deff_exit ();
} // standupFrenzyTotalScore_deff



/*HIT mode*/
void standupfrenzy_hit_effect_deff (void) {
	dmd_alloc_low_clean ();
	sprintf_score (standupFrenzyLastScore);
	font_render_string_center (&font_fixed10, DMD_BIG_CX_Cent, DMD_BIG_CY_Cent, sprintf_buffer);
	dmd_show_low ();
	task_sleep_sec (1);
	deff_exit ();
} // standupFrenzyTotalScore_deff



/*during mode*/
void standupfrenzy_effect_deff (void) {
	for (;;) {
		dmd_alloc_low_clean ();
		font_render_string_center (&font_fixed6, DMD_SMALL_CX_1, DMD_SMALL_CY_1, "FRENZY");
		sprintf ("%d SEC LEFT,  %d HIT", standupFrenzyTimer, standupFrenzyNumHits);
		font_render_string_center (&font_mono5, DMD_SMALL_CX_3, DMD_SMALL_CY_3, sprintf_buffer);
		sprintf_score (standupFrenzyNextScore);
		font_render_string_center (&font_mono5, DMD_SMALL_CX_4, DMD_SMALL_CY_4, sprintf_buffer);
		dmd_show_low ();
		task_sleep (TIME_200MS);
	}//END OF ENDLESS LOOP
}//end of standupFrenzy_mode_deff



/*mode is over*/
void standupfrenzy_end_effect_deff (void) {
	dmd_alloc_low_clean ();
	font_render_string_center (&font_mono5, DMD_BIG_CX_Top, DMD_BIG_CY_Top, "STANDUP FRENZY");
	sprintf_score (standupFrenzyTotalScore);
	font_render_string_center (&font_mono5, DMD_BIG_CX_Bot, DMD_BIG_CY_Bot, sprintf_buffer);
	dmd_show_low ();
	task_sleep_sec (2);
	deff_exit ();
} // standupFrenzyTotalScore_deff
