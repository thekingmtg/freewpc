/*
 * demolition man
 * standupfrenzy.c
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
 *
 * */

#include <freewpc.h>
//constants
/** Bitmask referring to all 5 standup targets */
#define 	NO_TARGETS 	0x0
#define 	ALL_TARGETS 0x1f
const U8 			standup_goal_increment = 2;
const U8 			standup_goal_max = 8;

//local variables
U8				standupFrenzy_SoundCounter = 0;
U8 				standupFrenzyNumHits;
U8 				standupFrenzyTimer;
score_t 		standupFrenzyTotalScore;
U8 				standupFrenzyDefaultLightsLit;
U8 				standupFrenzyLightsLit; //means target was hit
U8			standup_SoundCounter = 0;
U8 			standupLightsLit;
U8 			standup_counter;
U8 			standup_goal;

//prototypes
void standupFrenzy_mode_init (void);
void standupFrenzy_mode_exit (void);
void standupHandler (U8 target);
void standup_lamp_update1 (U8 mask, U8 lamp);
void standupFrenzyTotalScore_deff (void);
void standupFrenzy_mode_deff (void);
void standupFrenzy_sounds (void);
void standup_sounds (void);

struct timed_mode_ops standupFrenzy_mode = {
	DEFAULT_MODE,
	.init = standupFrenzy_mode_init,
	.exit = standupFrenzy_mode_exit,
	.gid = GID_STANDUPFRENZY_MODE_RUNNING,
	.music = MUS_MD_1,
//	.deff_running = DEFF_STANDUPFRENZY_MODE,
//	.deff_ending = DEFF_STANDUPFRENZYTOTALSCORE,
	.prio = PRI_GAME_MODE2,
	.init_timer = 20,
	.timer = &standupFrenzyTimer,
	.grace_timer = 3,
	.pause = system_timer_pause,
};


/****************************************************************************
 * initialize  and exit
 ***************************************************************************/
void standupFrenzy_mode_init (void) {
	standupFrenzyNumHits = 0;
	standupFrenzyDefaultLightsLit = ALL_TARGETS;
	callset_invoke (lamp_update);
	//deff_start (DEFF_STANDUPFRENZY_MODE);
	score_zero (standupFrenzyTotalScore);
	lamp_tristate_on(LM_CLAW_FREEZE);
}//end of standupFrenzy_mode_init 

void standupFrenzy_mode_exit (void) {
	/* Light all 'default' lamps */
	standupFrenzyDefaultLightsLit = ALL_TARGETS;
	standupFrenzyLightsLit = NO_TARGETS;
	standupLightsLit  = ALL_TARGETS;
	callset_invoke (lamp_update);
	standup_counter = 0;
}//end of standupFrenzy_mode_exit 

/*light the default standup lights*/
CALLSET_ENTRY (standupFrenzy, start_player, start_ball) {
	/* Light all 'default' lamps */
	standupFrenzyDefaultLightsLit = ALL_TARGETS;
	standupFrenzyLightsLit = NO_TARGETS;
	standupLightsLit  = ALL_TARGETS;
	callset_invoke (lamp_update);
	standup_counter = 0;
	standup_goal = 1;
}//end of start_player

/****************************************************************************
 * playfield lights and flags
 *
 *
 ***************************************************************************/




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

	//verify target hit was a lit target and mode is still running
	if ( (standupFrenzyLightsLit & target) && timed_mode_running_p(&standupFrenzy_mode) ) {
		standupFrenzyLightsLit &= ~target;  /* flag that target as hit */
		standupFrenzy_sounds();
		score (SC_1M);
		score (standupFrenzyNumHits * SC_1M);
		score_add (standupFrenzyTotalScore, score_table[SC_1M]);
		score_add (standupFrenzyTotalScore, (standupFrenzyNumHits * 1000000000) );
		//sound_send (SND_STANDUPFRENZY_MODE_BOOM);
		standup_lamp_update1 (target, lamp);
		}

	//else mode NOT running ---verify target hit was a lit target
	else if (standupLightsLit & target) {
			standupLightsLit &= ~target;  /* flag that target as hit */
			standup_sounds();
			lamp_tristate_flash (lamp);
			score (SC_50K);
			++standup_counter;
			if (standup_counter >= standup_goal) {
				callset_invoke(light_quick_freeze_light_on);//sent to inlanes.c
				if (standup_goal < standup_goal_max ) standup_goal += standup_goal_increment;
				}
			}//end of else if
}//end of standupHandler 


/****************************************************************************
 * stand up lamps update
 ****************************************************************************/
void standup_lamp_update1 (U8 mask, U8 lamp) {
	if (timed_mode_running_p (&standupFrenzy_mode))	{
			//target was hit so flash it
			if (standupFrenzyLightsLit & mask)	{
					lamp_tristate_flash (lamp);
					}
			else	lamp_tristate_off (lamp);
	}//end of if
	else { //mode not running
		//target was hit so flash it
		if (standupLightsLit & mask)	{
				lamp_tristate_flash (lamp);
				}
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


/****************************************************************************
 * external event listeners
 ****************************************************************************/
CALLSET_ENTRY (standupFrenzy, display_update) { timed_mode_display_update (&standupFrenzy_mode); }
CALLSET_ENTRY (standupFrenzy, music_refresh)  { timed_mode_music_refresh (&standupFrenzy_mode); }
CALLSET_ENTRY (standupFrenzy, end_ball)		{ timed_mode_end (&standupFrenzy_mode); }

//claw freeze starts standup frenzy instead of locking a ball
CALLSET_ENTRY (standupFrenzy, sw_claw_freeze) { timed_mode_begin (&standupFrenzy_mode); }



/*  not sure about this
void standupFrenzy_mode_expire (void) {
	if (score_compare (score_table[SC_10M], standupFrenzyTotalScore) == 1)
		callset_invoke (start_hurryup);
}// end of standupFrenzy_mode_expire 
*/


/****************************************************************************
 * DMD display and sound effects
 ****************************************************************************/

void standup_sounds (void) {
	standup_SoundCounter = random_scaled(3);//from kernal/random.c
	if ( standup_SoundCounter  == 0 )
	sound_start (ST_EFFECT, ZAPP_3_SHORT, SL_1S, PRI_GAME_QUICK5);
else if ( standup_SoundCounter  == 1 )
	sound_start (ST_EFFECT, ZAPP_3_MED, SL_1S, PRI_GAME_QUICK5);
else if ( standup_SoundCounter  == 2 )
	sound_start (ST_EFFECT, ZAPP_3_LONG, SL_1S, PRI_GAME_QUICK5);
}


void standupFrenzy_sounds (void) {
	standupFrenzy_SoundCounter = random_scaled(3);//from kernal/random.c
	if ( standupFrenzy_SoundCounter  == 0 )
	sound_start (ST_EFFECT, CHORD1, SL_1S, PRI_GAME_QUICK5);
else if ( standupFrenzy_SoundCounter  == 1 )
	sound_start (ST_EFFECT, CHORD2, SL_1S, PRI_GAME_QUICK5);
else if ( standupFrenzy_SoundCounter  == 2 )
	sound_start (ST_EFFECT, CHORD3, SL_1S, PRI_GAME_QUICK5);
}


/*mode is over*/
void standupFrenzyTotalScore_deff (void) {
	//sound_send (SND_SEE_WHAT_GREED);
	dmd_alloc_low_clean ();
	font_render_string_center (&font_fixed6, 64, 5, "Standup Frenzy");
	sprintf_score (standupFrenzyTotalScore);
	font_render_string_center (&font_fixed6, 64, 16, sprintf_buffer);
	font_render_string_center (&font_var5, 64, 27, "POINTS EARNED");
	dmd_show_low ();
	task_sleep_sec (4);
	deff_exit ();
} // standupFrenzyTotalScore_deff 

/*during mode*/
void standupFrenzy_mode_deff (void) {
/*	dmd_alloc_pair_clean ();
	U16 imageFrameNumber;
	for (;;) {
		U8 i = 0;
		for (imageFrameNumber = IMG_GREED_START; 
       imageFrameNumber <= IMG_GREED_END; imageFrameNumber += 2) {
			dmd_map_overlay ();
			dmd_clean_page_low ();
			font_render_string_center (&font_fixed6, 64, 5, "Standup Frenzy");
			i++;
			if (i > 1) {
				sprintf_score (standupFrenzyTotalScore);
				font_render_string_center (&font_fixed6, 64, 16, sprintf_buffer);
				}
			if (i > 2)	{i = 0;}
			
			font_render_string_center (&font_var5, 64, 27, "SHOOT FLASHING STANDUPS");
			sprintf ("%d", standupFrenzyTimer);
			font_render_string (&font_var5, 2, 2, sprintf_buffer);
			font_render_string_right (&font_var5, 126, 2, sprintf_buffer);
			dmd_text_outline ();
			dmd_alloc_pair ();
			frame_draw (imageFrameNumber);
			callset_invoke (score_overlay);
			dmd_overlay_outline ();
			dmd_show2 ();
			task_sleep (TIME_66MS);
		}//end of for (imageFrameNumber…
	}//end of for (;;)
*/
}//end of standupFrenzy_mode_deff 

