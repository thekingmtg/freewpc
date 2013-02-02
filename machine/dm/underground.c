/*
 * demolition man
 * underground.c
 *
 * written by James Cardona
 *
 * Location Description:
 * subway / underground is a scoop across from the upper flipper and to the left of the jets
 * and to the right of the center ramp, with the opening on the left side. It is an easy
 * shot from the upper left flipper, and it can also be made with a little luck from either
 * of the lower flippers. The Computer and Demolition Time are awarded here. The
 * Underground has a Jackpot lamp and a Combo arrow. The ball is returned, via a VUK
 * to the right ramp habitrail. The Computer and Demolition time are indicated by
 * yellow and blue lights mounted over the Underground entrance.
 *
 * The Computer is accessed via the UnderGround shot and is lit by a yellow lamp
 * immediately above. Lit every 10 combos, the Computer is a random award like
 * the Camera on Twilight Zone. Awards seen so far :
 * Light Extra Ball
 * Light Explodes (lights all four explode 10M lamps)
 * 3x Car Crash
 * 2x Retina Scan
 * Light Arrows (lights Laser millions on all combo shots)
 * Maximize Freezes (freezes four balls and lights multiball)
 * Collect Bonus (complete with bonus music!)
 * Collect Standups (awards all the lit yellow targets)
 *
 * Scoring Description: (original game)
 *
 *
 * Scoring Description: (my rules)
 * same as above except
 *
 */
/*CALLSET_SECTION (underground, __machine2__)*/
#include <freewpc.h>
#include "dm/global_constants.h"
#include "search.h"

//constants
const U8 		computerAwardsChooserNumOfSounds = 7; //num between 0 and 6
const sound_code_t computerAwardsChooserSoundsArray[] = {	COMPUTER1, 				COMPUTER_ADDING,   COMPUTER_AWARD_SMALL,
													COMPUTER_AWARD_LONG,	COMPUTER2, 			SPCH_ACCESSING,
													SPCH_COMPUTER_AWARD};

//local variables
U8 			undergroundSwitchDebouncer;
U8 			underground_goal;
U8 			MessageCounter;
U8			underground_SoundCounter;
__boolean 	underground_inBall_search;
__boolean 	computer_award_just_awarded;
score_t		ca_bonus_scored;
score_t		ca_total_bonus;
__local__ U8 	computerAwardsChooser;
__local__ U8 	nextcomputerAwardsChooser;
__local__ U8 	underground_shots_made;



//external variables
extern __local__ U8 special_shot_selector; //from special_shots.c

extern __local__ U8 standup_num_of_hits;

extern U8 		car_crash_shots_made; 				//from car_crash.c

extern U8 		car_chase_mode_shots_made;			//from car_chase.c
extern U8 		car_chase_modes_achieved;
extern U8 		car_chase_modes_completed;

extern U8 		capture_simon_mode_shots_made;		//from capture_simon.c
extern U8 		capture_simon_modes_achieved;
extern U8 		capture_simon_modes_completed;

extern U8 		prison_break_mode_shots_made;		//from prison_break.c
extern U8 		prison_break_modes_achieved;

extern U8 		acmag_mode_shots_made;				//from acmag.c
extern U8 		acmag_modes_achieved;

extern U8 		explode_mode_shots_made;			//from EXPLODE.c
extern score_t 	explode_mode_score;

extern U8 		superjets_modes_achieved;			//from superjets.c
extern U8 		superjets_mode_shots_made;

extern U8		standupFrenzy_modes_achieved;		//from standupfrenzy.c
extern U8 		standupFrenzyNumHits;
//

extern U8 		combo_counter;						//from combos.c
extern U8		fortress_jackpot_shots_made;

extern U8 		rollover_bonus_multiplier; 			//from rollovers.c

extern U8		fortress_jackpot_shots_made;
extern U8		demotime_jackpot_shots_made;
extern U8		wasteland_jackpot_shots_made;
extern U8		cryoprison_jackpot_shots_made;

extern U8		demotime_jackpot_shots_made;

extern U8 		huxley_mode_shots_made;

extern U8		back_in_the_fridge_shots_made;





//internally called function prototypes  --external found at protos.h
void underground_reset (void);
void underground_task (void);
void square_draw (U8 sq);
void show_text_on_squares (U8 times);
void square_init (void);
void computer_award(void);
void computer_light_off (void);

/****************************************************************************
 * initialize  and exit
 ***************************************************************************/
void underground_reset (void) {
	undergroundSwitchDebouncer = 0;
	underground_shots_made = 0;
	underground_goal = 0;
	underground_inBall_search = FALSE;
	computer_award_just_awarded = FALSE;
}//end of function



void player_underground_reset(void) {
	if (IN_TEST) {
		computerAwardsChooser = 0;
		nextcomputerAwardsChooser = 1;
	}
	else {
		computerAwardsChooser = 	random_scaled(10);
		nextcomputerAwardsChooser = random_scaled(10);
	}
	underground_reset();
	MessageCounter = 0;
	underground_SoundCounter = 0;
}//end of function



CALLSET_ENTRY (underground, start_player) { player_underground_reset(); }
CALLSET_ENTRY (underground, start_ball) { underground_reset(); }
CALLSET_ENTRY (underground, ball_search) { 		underground_inBall_search = TRUE; }
CALLSET_ENTRY (underground, ball_search_end) { 	underground_inBall_search = FALSE; }
CALLSET_ENTRY (underground, end_game) { 		computer_light_off(); }



/****************************************************************************
 * playfield lights and flags
 ***************************************************************************/
void underground_jackpot_light_on(void) {
	flag_on(FLAG_IS_UGROUND_JACKPOT_ACTIVATED);
	lamp_tristate_on (LM_UNDERGROUND_JACKPOT);
}//end of function

void underground_jackpot_light_off(void) {
	flag_off(FLAG_IS_UGROUND_JACKPOT_ACTIVATED);
	lamp_tristate_off (LM_UNDERGROUND_JACKPOT);
}//end of function

void underground_arrow_light_on(void) {
	flag_on (FLAG_IS_UGROUND_ARROW_ACTIVATED);
	lamp_tristate_on (LM_UNDERGROUND_ARROW);
}//end of function

void underground_arrow_light_off(void) {
	flag_off (FLAG_IS_UGROUND_ARROW_ACTIVATED);
	lamp_tristate_off (LM_UNDERGROUND_ARROW);
}//end of function

void computer_light_on(void) {
	flag_on (FLAG_IS_COMPUTER_ACTIVATED);
	lamp_tristate_flash (LM_COMPUTER);
}//end of function

void computer_light_off(void) {
	flag_off (FLAG_IS_COMPUTER_ACTIVATED);
	lamp_tristate_off (LM_COMPUTER);
	lamp_tristate_off (LM_DEMO_TIME);
}//end of function



/****************************************************************************
 *
 * body
 *
 ****************************************************************************/
//sw_bottom_popper
CALLSET_ENTRY (underground, dev_subway_vuk_enter) {
	if (!underground_inBall_search) {
		if (!flag_test (FLAG_CAPTURE_SIMON_INITIALSTART) ){
					ball_search_monitor_stop ();
					computer_award_just_awarded = FALSE;
					++underground_shots_made;
					score (BOT_POP_SCORE);//located in kernal/score.c

					//CALLS			*****************************
					if 		(flag_test(FLAG_SKILLSHOT_ENABLED) ) 				award_skill_shot (1);
					else if (flag_test(FLAG_BACK_IN_THE_FRIDGE_RUNNING) ) 		back_in_the_fridge_shot_made();
					else if (flag_test(FLAG_IS_DEMOTIME_ENABLED) ) 				demotime_start();
					else if (flag_test(FLAG_IS_HUXLEY_RUNNING) )				huxley_mode_shot_made();
					else if (flag_test(FLAG_IS_UGROUND_JACKPOT_ACTIVATED) ) 	score_jackpot();
					else if (flag_test (FLAG_IS_CAPSIM_UNDER_ACTIVATED) )		capture_simon_made();
					else if (flag_test(FLAG_IS_COMPUTER_ACTIVATED) ) {			leff_start (LEFF_COMPUTER_AWARD);
																				deff_start_sync (DEFF_COMPUTER_AWARD); }
					else if (flag_test (FLAG_IS_PBREAK_RUNNING) )   			prison_break_made();

					else if ( flag_test(FLAG_IS_COMBO_UNDER_ACTIVATED)) 		combo_hit();
					else if (flag_test(FLAG_IS_COMBOS_KILLED)
						&&	 flag_test(FLAG_IS_COMBOS_RESTARTABLE) ) 			combo_init();

			//if nothing special, do normal effects
					if(		!flag_test (FLAG_IS_PBREAK_RUNNING)
						&& 	!flag_test (FLAG_IS_CAPSIM_UNDER_ACTIVATED)
						&& 	!computer_award_just_awarded
						&& 	!flag_test (FLAG_IS_UGROUND_JACKPOT_ACTIVATED)
						&&	!flag_test(FLAG_IS_DEMOTIME_IS_STARTING)			) {

										if (	underground_shots_made > 5
											&& 	special_shot_selector == 0
											&& 	!flag_test(FLAG_LASER_SHOT_ENABLED) ) {

														underground_shots_made = 0;
														//special_shot_selector++;
														flag_on (FLAG_LASER_SHOT_ENABLED);
										}

										deff_start (DEFF_UNDERGROUND_EFFECT);

					}//end of if nothing special
					computer_award_just_awarded = FALSE;

					if (	!flag_test(FLAG_IS_DEMOTIME_IS_STARTING)
						&&  !flag_test(FLAG_VIDEO_MODE_RUNNING) ) {
						leff_start (LEFF_UNDERGROUND);
						sol_request_async(SOL_BOTTOM_POPPER);
					}
					ball_search_monitor_start ();
		}//end of !flag_test FLAG_CAPTURE_SIMON_INITIALSTART
	}//end of !underground_inBall_search
}//end of function






/****************************************************************************
 *
 * lighting effects
 *
 ****************************************************************************/
void underground_leff1 (void) {
	U8 i;
	lamplist_set_apply_delay (TIME_66MS);
	for (i = 0; i < 8; i++)
		lamplist_apply (LAMPLIST_UNDERGROUND_SCOOP, leff_toggle);
	leff_exit ();
}//end of function


void underground_leff (void) {
	leff_create_peer (underground_leff1);

	flasher_pulse (FLASH_DIVERTER_FLASHER);
	flasher_pulse (FLASH_LEFT_RAMP_UP_FLASHER);
	task_sleep (TIME_100MS);

	flasher_pulse (FLASH_RIGHT_RAMP_UP_FLASHER);
	flasher_pulse (FLASH_DIVERTER_FLASHER);
	task_sleep (TIME_100MS);

	flasher_pulse (FLASH_LEFT_RAMP_UP_FLASHER);
	flasher_pulse (FLASH_RIGHT_RAMP_UP_FLASHER);
	task_sleep (TIME_100MS);

	flasher_pulse (FLASH_DIVERTER_FLASHER);
	flasher_pulse (FLASH_LEFT_RAMP_UP_FLASHER);
	task_sleep (TIME_100MS);

	flasher_pulse (FLASH_RIGHT_RAMP_UP_FLASHER);
	flasher_pulse (FLASH_DIVERTER_FLASHER);
	task_sleep (TIME_100MS);

	flasher_pulse (FLASH_LEFT_RAMP_UP_FLASHER);
	flasher_pulse (FLASH_RIGHT_RAMP_UP_FLASHER);
	task_sleep (TIME_100MS);
	leff_exit ();
}//end of function




void computer_award_leff(void) {
	gi_leff_disable (PINIO_GI_STRINGS); //turn off gi
	leff_start(LEFF_FLASH_ALL);
	gi_leff_enable (PINIO_GI_STRINGS); //turn on gi
	task_sleep_sec (TIME_1S);
	leff_exit();
}//end of function






/****************************************************************************
 *
 * display effects
 *
 ****************************************************************************/
void underground_effect_deff(void) {
	U16 fno;

	if (IN_TEST) {
		if (++MessageCounter > 5) 			MessageCounter = 0;
		if (++underground_SoundCounter > 3) underground_SoundCounter = 0;
	}
	else		{
//		MessageCounter = random_scaled(3);
		underground_SoundCounter = random_scaled(2);//from kernal/random.c
	}

	dmd_alloc_pair_clean ();// Clean both pages

	//one shot away from HUXLEY so tell the player so
	if (flag_test(FLAG_IS_HUXLEY_ENABLED) || (IN_TEST && MessageCounter == 0) ) {
		sound_start (ST_EFFECT, SUBWAY2, SL_2S, SP_NORMAL);
		for (fno = IMG_JOHN_SPARTAN_A_START; fno <= IMG_JOHN_SPARTAN_A_END; fno += 2) {
					dmd_alloc_pair ();
					frame_draw(fno);
					dmd_show2 ();
					task_sleep (TIME_100MS);
		}//end of for loop
		sound_start (ST_EFFECT, SUBWAY2, SL_2S, SP_NORMAL);
		for (fno = IMG_JOHN_SPARTAN_A_END; fno >= IMG_JOHN_SPARTAN_A_START; fno -= 2) {
				dmd_map_overlay ();
				dmd_clean_page_low ();
				font_render_string_center (&font_fireball, DMD_MIDDLE_X, DMD_BIG_CY_Top, "HUXLEY");
				font_render_string_center (&font_fireball, DMD_MIDDLE_X, DMD_BIG_CY_Bot, "READY");
					dmd_text_outline ();
					dmd_alloc_pair ();
					frame_draw(fno);
					dmd_overlay_outline ();
					dmd_show2 ();
					task_sleep (TIME_100MS);
		}//end of for loop
	}//END OF - one shot away from HUXLEY so tell the player so
	else

	//one shot away from car chase so tell the player so
	if (flag_test(FLAG_IS_CARCHASE_MODE_ENABLED)  || (IN_TEST && MessageCounter == 1) ) {
		sound_start (ST_EFFECT, SUBWAY2, SL_2S, SP_NORMAL);
		for (fno = IMG_PHOENIX_A2_START; fno <= IMG_PHOENIX_A2_END; fno += 2) {
					dmd_alloc_pair ();
					frame_draw(fno);
					dmd_show2 ();
					task_sleep (TIME_100MS);
		}//end of for loop
		sound_start (ST_EFFECT, SUBWAY2, SL_2S, SP_NORMAL);
		for (fno = IMG_PHOENIX_A2_END; fno >= IMG_PHOENIX_A2_START; fno -= 2) {
				dmd_map_overlay ();
				dmd_clean_page_low ();
				font_render_string_center (&font_fireball, DMD_MIDDLE_X, DMD_BIG_CY_Top, "CAR CHASE");
				font_render_string_center (&font_fireball, DMD_MIDDLE_X, DMD_BIG_CY_Bot, "READY");
					dmd_text_outline ();
					dmd_alloc_pair ();
					frame_draw(fno);
					dmd_overlay_outline ();
					dmd_show2 ();
					task_sleep (TIME_100MS);
		}//end of for loop
	}//END OF - one shot away from car chase so tell the player so
	else

	//one shot away from explode so tell the player so
	if (flag_test(FLAG_IS_EXPLODE_MODE_ENABLED)  || (IN_TEST && MessageCounter == 2) ) {
		sound_start (ST_EFFECT, SUBWAY2, SL_2S, SP_NORMAL);
		for (fno = IMG_JOHN_SPARTAN_A_START; fno <= IMG_JOHN_SPARTAN_A_END; fno += 2) {
					dmd_alloc_pair ();
					frame_draw(fno);
					dmd_show2 ();
					task_sleep (TIME_100MS);
		}//end of for loop
		sound_start (ST_EFFECT, SUBWAY2, SL_2S, SP_NORMAL);
		for (fno = IMG_JOHN_SPARTAN_A_END; fno >= IMG_JOHN_SPARTAN_A_START; fno -= 2) {
				dmd_map_overlay ();
				dmd_clean_page_low ();
				font_render_string_center (&font_fireball, DMD_MIDDLE_X, DMD_BIG_CY_Top, "EXPLODE");
				font_render_string_center (&font_fireball, DMD_MIDDLE_X, DMD_BIG_CY_Bot, "READY");
					dmd_text_outline ();
					dmd_alloc_pair ();
					frame_draw(fno);
					dmd_overlay_outline ();
					dmd_show2 ();
					task_sleep (TIME_100MS);
		}//end of for loop
	}//END OF - one shot away from explode so tell the player so
	else

		//one shot away from video mode so tell the player so
		if (flag_test(FLAG_VIDEO_MODE_ENABLED) || (IN_TEST && MessageCounter == 3) ) {
			sound_start (ST_EFFECT, SUBWAY2, SL_2S, SP_NORMAL);
			for (fno = IMG_PHOENIX_A2_START; fno <= IMG_PHOENIX_A2_END; fno += 2) {
						dmd_alloc_pair ();
						frame_draw(fno);
						dmd_show2 ();
						task_sleep (TIME_100MS);
			}//end of for loop
			sound_start (ST_EFFECT, SUBWAY2, SL_2S, SP_NORMAL);
			for (fno = IMG_PHOENIX_A2_END; fno >= IMG_PHOENIX_A2_START; fno -= 2) {
					dmd_map_overlay ();
					dmd_clean_page_low ();
					font_render_string_center (&font_fireball, DMD_MIDDLE_X, DMD_BIG_CY_Top, "VIDEO MODE");
					font_render_string_center (&font_fireball, DMD_MIDDLE_X, DMD_BIG_CY_Bot, "READY");
						dmd_text_outline ();
						dmd_alloc_pair ();
						frame_draw(fno);
						dmd_overlay_outline ();
						dmd_show2 ();
						task_sleep (TIME_100MS);
			}//end of for loop
		}//END OF - one shot away from video mode so tell the player so

		//set up for laser shot
		else if (flag_test(FLAG_LASER_SHOT_ENABLED)  || (IN_TEST && MessageCounter == 4) ) {
			sound_start (ST_EFFECT, SUBWAY2, SL_2S, SP_NORMAL);
			for (fno = IMG_SIMON_FIRE1_START; fno <= IMG_SIMON_FIRE1_END; fno += 2) {
						dmd_alloc_pair ();
						frame_draw(fno);
						dmd_show2 ();
						task_sleep (TIME_100MS);
			}//end of for loop
			sound_start (ST_EFFECT, SUBWAY2, SL_2S, SP_NORMAL);
			for (fno = IMG_SIMON_FIRE1_END; fno >= IMG_SIMON_FIRE1_START; fno -= 2) {
					dmd_map_overlay ();
					dmd_clean_page_low ();
					font_render_string_center (&font_fireball, DMD_MIDDLE_X, DMD_BIG_CY_Top, "CENTER");
					font_render_string_center (&font_fireball, DMD_MIDDLE_X, DMD_BIG_CY_Bot, "COW SHOT");
						dmd_text_outline ();
						dmd_alloc_pair ();
						frame_draw(fno);
						dmd_overlay_outline ();
						dmd_show2 ();
						task_sleep (TIME_100MS);
			}//end of for loop
		}//END OF - set up for laser shot

	else { // STANDARD DISPLAY OR (IN_TEST && MessageCounter == 5)
		sound_start (ST_EFFECT, SUBWAY2, SL_2S, SP_NORMAL);
		for (fno = IMG_SIMON_FIRE1_START; fno <= IMG_SIMON_FIRE1_END; fno += 2) {
					dmd_alloc_pair ();
					frame_draw(fno);
					dmd_show2 ();
					task_sleep (TIME_100MS);
		}//end of for loop
		sound_start (ST_EFFECT, SUBWAY2, SL_2S, SP_NORMAL);
		for (fno = IMG_SIMON_FIRE1_END; fno >= IMG_SIMON_FIRE1_START; fno -= 2) {
			sprintf ("SUBWAY");
				dmd_map_overlay ();
				dmd_clean_page_low ();
				font_render_string_center (&font_fireball, DMD_MIDDLE_X - 25, DMD_BIG_CY_Bot, sprintf_buffer);
					dmd_text_outline ();
					dmd_alloc_pair ();
					frame_draw(fno);
					dmd_overlay_outline ();
					dmd_show2 ();
					task_sleep (TIME_100MS);
		}//end of for loop
	}//end of else

	task_sleep (TIME_500MS);
		deff_exit ();
}//end of mode_effect_deff






void computer_award_deff(void) {
	U8 i;
	ball_search_monitor_stop ();

	sound_start (ST_ANY, SPCH_ACCESSING, SL_3S, PRI_GAME_QUICK6);

	dmd_alloc_low_clean ();
	dmd_draw_thin_border (dmd_low_buffer);
	sprintf ("COMPUTER AWARD");
	font_render_string_center (&font_var5, DMD_MIDDLE_X, DMD_SMALL_CY_1, sprintf_buffer);
	dmd_show_low ();
	task_sleep_sec (1);

	dmd_map_overlay ();
	dmd_clean_page_high ();
	dmd_clean_page_low ();
	dmd_draw_thin_border (dmd_low_buffer);
	sprintf ("COMPUTER AWARD");
	font_render_string_center (&font_var5, DMD_MIDDLE_X, DMD_SMALL_CY_1, sprintf_buffer);

	sound_start (ST_EFFECT, COMPUTER_ADDING, SL_3S, PRI_GAME_QUICK6);

	show_text_on_squares (10);//5 times per second

	sound_start (ST_EFFECT, COMPUTER_AWARD_LONG, SL_3S, PRI_GAME_QUICK6);
	task_sleep(TIME_2S);

	dmd_alloc_low_clean ();

	if (IN_TEST) {	if (++computerAwardsChooser > 9) {
								computerAwardsChooser = 0;
								nextcomputerAwardsChooser = 1;
								}
					else nextcomputerAwardsChooser++;
				}//end of if in test
	else		{
				computerAwardsChooser = nextcomputerAwardsChooser;
				nextcomputerAwardsChooser = random_scaled(10);
				}//end of else - not in test

		switch (computerAwardsChooser) {
//COLLECT BONUS
	case 0 :
			dmd_draw_thin_border (dmd_low_buffer);
			sound_start(ST_SPEECH, SPCH_COLLECT_BONUS, SL_4S, PRI_GAME_QUICK5);
			sprintf ("COLLECT");
			font_render_string_center (&font_steel, DMD_MIDDLE_X, DMD_BIG_CY_Top, sprintf_buffer);
			sprintf ("BONUS");
			font_render_string_center (&font_steel, DMD_MIDDLE_X, DMD_BIG_CY_Bot, sprintf_buffer);
			dmd_show_low ();
			task_sleep_sec (1);

			in_bonus = TRUE;
			task_sleep (TIME_100MS);	/* Wait a bit so the previous music_stop doesn't kill the sounds */
			sound_start1 (ST_ANY, BONUS_SHORT);
			score_zero (ca_total_bonus);	/* Clear the bonus score */
			/* Show Initial bonus screen */
			dmd_alloc_low_clean ();
			font_render_string_center (&font_fixed10, DMD_MIDDLE_X, DMD_BIG_CY_Cent, "BONUS");
			dmd_sched_transition (&trans_scroll_down);
			dmd_show_low ();
			task_sleep_sec (1);

			/**
			 * *car crash scoring and display *
			 * **/
			if (car_crash_shots_made > 0) {
						sound_start1 (ST_ANY, BONUS_SHORT);
					dmd_alloc_low_clean ();
					score_zero (ca_bonus_scored);
					score_add (ca_bonus_scored, score_table[SC_100K]);
					score_mul (ca_bonus_scored, car_crash_shots_made);
					score_add (ca_total_bonus, ca_bonus_scored);

					sprintf_score (ca_bonus_scored);
					font_render_string_center (&font_fixed10, DMD_MIDDLE_X, DMD_BIG_CY_Cent, sprintf_buffer);
					sprintf ("CAR CRASH");
					font_render_string_center (&font_var5, DMD_MIDDLE_X, DMD_SMALL_CY_1, sprintf_buffer);
							dmd_sched_transition (&trans_scroll_down);
					dmd_show_low ();
					task_sleep_sec (1);
			}/***end of car crash scoring and display ***/

			/**
			 * *car chase scoring and display *
			 * **/
			if (car_chase_modes_achieved > 0) {
						sound_start1 (ST_ANY, BONUS_SHORT);
					dmd_alloc_low_clean ();
					score_zero (ca_bonus_scored);
					score_add (ca_bonus_scored, score_table[SC_100K]);
					score_mul (ca_bonus_scored, car_chase_mode_shots_made);
					score_add (ca_total_bonus, ca_bonus_scored);

					score_zero (ca_bonus_scored);
					score_add (ca_bonus_scored, score_table[SC_1M]);
					score_mul (ca_bonus_scored, car_chase_modes_achieved);
					score_add (ca_total_bonus, ca_bonus_scored);

					sprintf_score (ca_bonus_scored);
					font_render_string_center (&font_fixed10, DMD_MIDDLE_X, DMD_BIG_CY_Cent, sprintf_buffer);
					sprintf ("CAR CHASE");
					font_render_string_center (&font_var5, DMD_MIDDLE_X, DMD_SMALL_CY_1, sprintf_buffer);
							dmd_sched_transition (&trans_scroll_down);
					dmd_show_low ();
					task_sleep_sec (1);
			}/***end of car chase scoring and display ***/

			/**
			 * *capture_simon scoring and display *
			 * **/
			if (capture_simon_modes_achieved > 0) {
						sound_start1 (ST_ANY, BONUS_SHORT);
					dmd_alloc_low_clean ();
					score_zero (ca_bonus_scored);
					score_add (ca_bonus_scored, score_table[SC_100K]);
					score_mul (ca_bonus_scored, capture_simon_mode_shots_made);
					score_add (ca_total_bonus, ca_bonus_scored);

					score_zero (ca_bonus_scored);
					score_add (ca_bonus_scored, score_table[SC_1M]);
					score_mul (ca_bonus_scored, capture_simon_modes_achieved);
					score_add (ca_total_bonus, ca_bonus_scored);

					score_zero (ca_bonus_scored);
					score_add (ca_bonus_scored, score_table[SC_5M]);
					score_mul (ca_bonus_scored, capture_simon_modes_completed);
					score_add (ca_total_bonus, ca_bonus_scored);

					sprintf_score (ca_bonus_scored);
					font_render_string_center (&font_fixed10, DMD_MIDDLE_X, DMD_BIG_CY_Cent, sprintf_buffer);
					sprintf ("CAPTURE SIMON");
					font_render_string_center (&font_var5, DMD_MIDDLE_X, DMD_SMALL_CY_1, sprintf_buffer);
							dmd_sched_transition (&trans_scroll_down);
					dmd_show_low ();
					task_sleep_sec (1);
			}/***end of capture_simon scoring and display ***/

			/**
			 * *prison_break scoring and display *
			 * **/
			if (prison_break_modes_achieved > 0) {
						sound_start1 (ST_ANY, BONUS_SHORT);
					dmd_alloc_low_clean ();
					score_zero (ca_bonus_scored);
					score_add (ca_bonus_scored, score_table[SC_100K]);
					score_mul (ca_bonus_scored, prison_break_mode_shots_made);
					score_add (ca_total_bonus, ca_bonus_scored);

					score_zero (ca_bonus_scored);
					score_add (ca_bonus_scored, score_table[SC_1M]);
					score_mul (ca_bonus_scored, prison_break_modes_achieved);
					score_add (ca_total_bonus, ca_bonus_scored);

					sprintf_score (ca_bonus_scored);
					font_render_string_center (&font_fixed10, DMD_MIDDLE_X, DMD_BIG_CY_Cent, sprintf_buffer);
					sprintf ("BREAKOUT");
					font_render_string_center (&font_var5, DMD_MIDDLE_X, DMD_SMALL_CY_1, sprintf_buffer);
							dmd_sched_transition (&trans_scroll_down);
					dmd_show_low ();
					task_sleep_sec (1);
			}/***end of prison_break scoring and display ***/

			/**
			 * *acmag scoring and display *
			 * **/
			if (acmag_modes_achieved > 0) {
						sound_start1 (ST_ANY, BONUS_SHORT);
					dmd_alloc_low_clean ();
					score_zero (ca_bonus_scored);
					score_add (ca_bonus_scored, score_table[SC_100K]);
					score_mul (ca_bonus_scored, acmag_mode_shots_made);
					score_add (ca_total_bonus, ca_bonus_scored);

					score_zero (ca_bonus_scored);
					score_add (ca_bonus_scored, score_table[SC_1M]);
					score_mul (ca_bonus_scored, acmag_modes_achieved);
					score_add (ca_total_bonus, ca_bonus_scored);

					sprintf_score (ca_bonus_scored);
					font_render_string_center (&font_fixed10, DMD_MIDDLE_X, DMD_BIG_CY_Cent, sprintf_buffer);
					sprintf ("ACMAG");
					font_render_string_center (&font_var5, DMD_MIDDLE_X, DMD_SMALL_CY_1, sprintf_buffer);
							dmd_sched_transition (&trans_scroll_down);
					dmd_show_low ();
					task_sleep_sec (1);
			}/***end of acmag scoring and display ***/

			/**
			 * *explode scoring and display *
			 * **/
			if (explode_mode_shots_made > 0) {
						sound_start1 (ST_ANY, BONUS_SHORT);
					dmd_alloc_low_clean ();
					score_zero (ca_bonus_scored);
					score_add (ca_bonus_scored, score_table[SC_100K]);
					score_mul (ca_bonus_scored, explode_mode_shots_made);
					score_add (ca_bonus_scored, score_table[SC_1M]);
					score_add (ca_total_bonus, ca_bonus_scored);

					sprintf_score (ca_bonus_scored);
					font_render_string_center (&font_fixed10, DMD_MIDDLE_X, DMD_BIG_CY_Cent, sprintf_buffer);
					sprintf ("EXPLODE");
					font_render_string_center (&font_var5, DMD_MIDDLE_X, DMD_SMALL_CY_1, sprintf_buffer);
							dmd_sched_transition (&trans_scroll_down);
					dmd_show_low ();
					task_sleep_sec (1);
			}/***end of explode scoring and display ***/

			/**
			 * *superjets scoring and display *
			 * **/
			if (superjets_modes_achieved > 0) {
					sound_start1 (ST_ANY, BONUS_SHORT);
					dmd_alloc_low_clean ();
					score_zero (ca_bonus_scored);
					score_add (ca_bonus_scored, score_table[SC_100K]);
					score_mul (ca_bonus_scored, superjets_mode_shots_made);
					score_add (ca_total_bonus, ca_bonus_scored);

					score_zero (ca_bonus_scored);
					score_add (ca_bonus_scored, score_table[SC_1M]);
					score_mul (ca_bonus_scored, superjets_modes_achieved);
					score_add (ca_total_bonus, ca_bonus_scored);

					sprintf_score (ca_bonus_scored);
					font_render_string_center (&font_fixed10, DMD_MIDDLE_X, DMD_BIG_CY_Cent, sprintf_buffer);
					sprintf ("SUPERJETS");
					font_render_string_center (&font_var5, DMD_MIDDLE_X, DMD_SMALL_CY_1, sprintf_buffer);
							dmd_sched_transition (&trans_scroll_down);
					dmd_show_low ();
					task_sleep_sec (1);
			}/***end of superjets scoring and display ***/

			/**
			 * *standupFrenzy scoring and display *
			 * **/
			if (standupFrenzy_modes_achieved > 0) {
					sound_start1 (ST_ANY, BONUS_SHORT);
					dmd_alloc_low_clean ();
					score_zero (ca_bonus_scored);
					score_add (ca_bonus_scored, score_table[SC_100K]);
					score_mul (ca_bonus_scored, standupFrenzyNumHits);
					score_add (ca_total_bonus, ca_bonus_scored);

					score_zero (ca_bonus_scored);
					score_add (ca_bonus_scored, score_table[SC_1M]);
					score_mul (ca_bonus_scored, standupFrenzy_modes_achieved);
					score_add (ca_total_bonus, ca_bonus_scored);

					sprintf_score (ca_bonus_scored);
					font_render_string_center (&font_fixed10, DMD_MIDDLE_X, DMD_BIG_CY_Cent, sprintf_buffer);
					sprintf ("FRENZY");
					font_render_string_center (&font_var5, DMD_MIDDLE_X, DMD_SMALL_CY_1, sprintf_buffer);
							dmd_sched_transition (&trans_scroll_down);
					dmd_show_low ();
					task_sleep_sec (1);
			}/***end of standupFrenzy scoring and display ***/

			/**
			 * *fortress scoring and display *
			 * **/
			if (fortress_jackpot_shots_made > 0) {
					sound_start1 (ST_ANY, BONUS_SHORT);
					dmd_alloc_low_clean ();
					score_zero (ca_bonus_scored);
					score_add (ca_bonus_scored, score_table[SC_100K]);
					score_mul (ca_bonus_scored, fortress_jackpot_shots_made);
					score_add (ca_total_bonus, ca_bonus_scored);

					sprintf_score (ca_bonus_scored);
					font_render_string_center (&font_fixed10, DMD_MIDDLE_X, DMD_BIG_CY_Cent, sprintf_buffer);
					sprintf ("FORTRESS");
					font_render_string_center (&font_var5, DMD_MIDDLE_X, DMD_SMALL_CY_1, sprintf_buffer);
					dmd_sched_transition (&trans_scroll_down);
					dmd_show_low ();
					task_sleep_sec (1);
			}/***end of fortress scoring and display ***/

			/**
			 * *wasteland scoring and display *
			 * **/
			if (wasteland_jackpot_shots_made > 0) {
					sound_start1 (ST_ANY, BONUS_SHORT);
					dmd_alloc_low_clean ();
					score_zero (ca_bonus_scored);
					score_add (ca_bonus_scored, score_table[SC_100K]);
					score_mul (ca_bonus_scored, wasteland_jackpot_shots_made);
					score_add (ca_total_bonus, ca_bonus_scored);

					sprintf_score (ca_bonus_scored);
					font_render_string_center (&font_fixed10, DMD_MIDDLE_X, DMD_BIG_CY_Cent, sprintf_buffer);
					sprintf ("WASTELAND");
					font_render_string_center (&font_var5, DMD_MIDDLE_X, DMD_SMALL_CY_1, sprintf_buffer);
					dmd_sched_transition (&trans_scroll_down);
					dmd_show_low ();
					task_sleep_sec (1);
			}/***end of wasteland_ scoring and display ***/

			/**
			 * *cryoprison scoring and display *
			 * **/
			if (cryoprison_jackpot_shots_made > 0) {
					sound_start1 (ST_ANY, BONUS_SHORT);
					dmd_alloc_low_clean ();
					score_zero (ca_bonus_scored);
					score_add (ca_bonus_scored, score_table[SC_100K]);
					score_mul (ca_bonus_scored, cryoprison_jackpot_shots_made);
					score_add (ca_total_bonus, ca_bonus_scored);

					sprintf_score (ca_bonus_scored);
					font_render_string_center (&font_fixed10, DMD_MIDDLE_X, DMD_BIG_CY_Cent, sprintf_buffer);
					sprintf ("CRYOPRISON");
					font_render_string_center (&font_var5, DMD_MIDDLE_X, DMD_SMALL_CY_1, sprintf_buffer);
					dmd_sched_transition (&trans_scroll_down);
					dmd_show_low ();
					task_sleep_sec (1);
			}/***end of cryoprison_ scoring and display ***/

			/**
			 * *demotime scoring and display *
			 * **/
			if (demotime_jackpot_shots_made > 0) {
					sound_start1 (ST_ANY, BONUS_SHORT);
					dmd_alloc_low_clean ();
					score_zero (ca_bonus_scored);
					score_add (ca_bonus_scored, score_table[SC_100K]);
					score_mul (ca_bonus_scored, demotime_jackpot_shots_made);
					score_add (ca_total_bonus, ca_bonus_scored);

					sprintf_score (ca_bonus_scored);
					font_render_string_center (&font_fixed10, DMD_MIDDLE_X, DMD_BIG_CY_Cent, sprintf_buffer);
					sprintf ("DEMOLITION TIME");
					font_render_string_center (&font_var5, DMD_MIDDLE_X, DMD_SMALL_CY_1, sprintf_buffer);
					dmd_sched_transition (&trans_scroll_down);
					dmd_show_low ();
					task_sleep_sec (1);
			}/***end of demotime_ scoring and display ***/

			/**
			 * *huxley scoring and display *
			 * **/
			if (huxley_mode_shots_made > 0) {
					sound_start1 (ST_ANY, BONUS_SHORT);
					dmd_alloc_low_clean ();
					score_zero (ca_bonus_scored);
					score_add (ca_bonus_scored, score_table[SC_100K]);
					score_mul (ca_bonus_scored, huxley_mode_shots_made);
					score_add (ca_total_bonus, ca_bonus_scored);

					sprintf_score (ca_bonus_scored);
					font_render_string_center (&font_fixed10, DMD_MIDDLE_X, DMD_BIG_CY_Cent, sprintf_buffer);
					sprintf ("HUXLEY");
					font_render_string_center (&font_var5, DMD_MIDDLE_X, DMD_SMALL_CY_1, sprintf_buffer);
					dmd_sched_transition (&trans_scroll_down);
					dmd_show_low ();
					task_sleep_sec (1);
			}/***end of huxley scoring and display ***/

			/**
			 * *back_in_the_fridge scoring and display *
			 * **/
			if (back_in_the_fridge_shots_made > 0) {
					sound_start1 (ST_ANY, BONUS_SHORT);
					dmd_alloc_low_clean ();
					score_zero (ca_bonus_scored);
					score_add (ca_bonus_scored, score_table[SC_100K]);
					score_mul (ca_bonus_scored, back_in_the_fridge_shots_made);
					score_add (ca_total_bonus, ca_bonus_scored);

					sprintf_score (ca_bonus_scored);
					font_render_string_center (&font_fixed10, DMD_MIDDLE_X, DMD_BIG_CY_Cent, sprintf_buffer);
					sprintf ("FRIDGE");
					font_render_string_center (&font_var5, DMD_MIDDLE_X, DMD_SMALL_CY_1, sprintf_buffer);
					dmd_sched_transition (&trans_scroll_down);
					dmd_show_low ();
					task_sleep_sec (1);
			}/***end of back_in_the_fridge scoring and display ***/

			/* Show final score */
			sound_start1 (ST_ANY, BONUS_LONG);

			score_long (ca_total_bonus); // Add to total bonus to player score
			dmd_alloc_low_clean ();
			scores_draw ();
			dmd_sched_transition (&trans_scroll_up);
			dmd_show_low ();
			task_sleep_sec (1);
			in_bonus = FALSE;
			break;

//TRIPLE CAR CRASH
	case 1 :
			dmd_draw_thin_border (dmd_low_buffer);
			sound_start(ST_SPEECH, SPCH_TRIPLE_CAR_CRASH, SL_4S, PRI_GAME_QUICK5);
			sprintf ("TRIPLE");
			font_render_string_center (&font_steel, DMD_MIDDLE_X, DMD_BIG_CY_Top, sprintf_buffer);
			sprintf ("CAR CRASH");
			font_render_string_center (&font_steel, DMD_MIDDLE_X, DMD_BIG_CY_Bot, sprintf_buffer);
			dmd_show_low ();
			comp_award_trip_car_crash();
			task_sleep_sec (1);
						lamp_tristate_flash (LM_CAR_CRASH_TOP);
						lamp_tristate_flash (LM_CAR_CRASH_CENTER);
						lamp_tristate_flash (LM_CAR_CRASH_BOTTOM);
						for (i = 0; i < 3; i++) {
							sound_start (ST_EFFECT, ZAPP_3_SHORT, SL_2S, PRI_GAME_QUICK2);
							flasher_pulse (FLASH_CAR_CHASE_LOWER_FLASHER);
							flasher_pulse (FLASH_CAR_CHASE_CENTER_FLASHER);
							flasher_pulse (FLASH_CAR_CHASE_UPPER_FLASHER);
							task_sleep (TIME_300MS);
						}
						for (i = 0; i < 3; i++) {
							sound_start (ST_EFFECT, ZAPP_3_MED, SL_2S, PRI_GAME_QUICK2);
							flasher_pulse (FLASH_CAR_CHASE_LOWER_FLASHER);
							flasher_pulse (FLASH_CAR_CHASE_CENTER_FLASHER);
							flasher_pulse (FLASH_CAR_CHASE_UPPER_FLASHER);
							task_sleep (TIME_500MS);
						}
						task_sleep_sec (1);
						sound_start (ST_EFFECT, ZAPP_3_LONG, SL_2S, PRI_GAME_QUICK2);
						restore_carcrash_lights_state();
			task_sleep_sec (1);
			break;
//COLLECT STANDUPS
	case 2 :
			dmd_draw_thin_border (dmd_low_buffer);
			sound_start(ST_SPEECH, SPCH_COLLECT_STANDUPS, SL_4S, PRI_GAME_QUICK5);
			sprintf ("COLLECT");
			font_render_string_center (&font_steel, DMD_MIDDLE_X, DMD_BIG_CY_Top, sprintf_buffer);
			sprintf ("STANDUPS");
			font_render_string_center (&font_steel, DMD_MIDDLE_X, DMD_BIG_CY_Bot, sprintf_buffer);
			dmd_show_low ();
			task_sleep_sec (2);
			/////////////////////collectstandups deff
				U8 collect_standups_counter = 0;
				U8 digit_counter = 0;
					for (collect_standups_counter = 0; collect_standups_counter <= standup_num_of_hits; collect_standups_counter++) {
							score (SC_500K);

							dmd_alloc_pair ();
							dmd_clean_page_low ();

//							sprintf ("%d", standup_num_of_hits - collect_standups_counter);
							if (collect_standups_counter == 0) 		sprintf ("500 THOUSAND");
							else if (collect_standups_counter %2 == 1) { digit_counter++; 	sprintf ("%d MILLION", digit_counter); }
							else if (collect_standups_counter %2 == 0) { 					sprintf ("%d.5 MILLION", digit_counter); }

							font_render_string_center(&font_fireball, DMD_MIDDLE_X, DMD_MIDDLE_Y, sprintf_buffer);
							dmd_copy_low_to_high ();
							dmd_show_low ();
							dmd_invert_page (dmd_low_buffer);
							deff_swap_low_high (1, TIME_100MS);


							if (collect_standups_counter < 3 )		{
								sound_start (ST_EFFECT, ZAPP_3_LONG, SL_2S, PRI_GAME_QUICK2);
								task_sleep (TIME_500MS);
								task_sleep (TIME_250MS);
							} else if (collect_standups_counter < 6) {
								sound_start (ST_EFFECT, ZAPP_3_MED, SL_2S, PRI_GAME_QUICK3);
								task_sleep (TIME_500MS);
							} else 	{
								sound_start (ST_EFFECT, ZAPP_3_SHORT, SL_2S, PRI_GAME_QUICK5);
								task_sleep (TIME_300MS);
							}
					}//end of loop
			task_sleep_sec (1);
			break;
//LIGHT ARROWS
	case 3 :
			dmd_draw_thin_border (dmd_low_buffer);
			sound_start(ST_SPEECH, SPCH_LIGHT_ARROWS, SL_4S, PRI_GAME_QUICK5);
			sprintf ("LIGHT");
			font_render_string_center (&font_steel, DMD_MIDDLE_X, DMD_BIG_CY_Top, sprintf_buffer);
			sprintf ("ARROWS");
			font_render_string_center (&font_steel, DMD_MIDDLE_X, DMD_BIG_CY_Bot, sprintf_buffer);
			dmd_show_low ();
			task_sleep_sec (1);

						lamp_tristate_flash (LM_SIDE_RAMP_ARROW);
						lamp_tristate_flash (LM_LEFT_RAMP_ARROW);
						lamp_tristate_flash (LM_RIGHT_RAMP_ARROW);
						lamp_tristate_flash (LM_UNDERGROUND_ARROW);
						lamp_tristate_flash (LM_CENTER_RAMP_ARROW);
						lamp_tristate_flash (LM_LEFT_LOOP_ARROW);
						lamp_tristate_flash (LM_RIGHT_LOOP_ARROW);

						for (i = 0; i < 3; i++) {
							sound_start (ST_EFFECT, ZAPP_3_SHORT, SL_2S, PRI_GAME_QUICK2);
							task_sleep (TIME_300MS);
						}
						for (i = 0; i < 2; i++) {
						sound_start (ST_EFFECT, ZAPP_3_MED, SL_2S, PRI_GAME_QUICK2);
						task_sleep (TIME_500MS);
						}
						for (i = 0; i < 2; i++) {
						sound_start (ST_EFFECT, ZAPP_3_LONG, SL_2S, PRI_GAME_QUICK2);
						task_sleep (TIME_500MS);
						}

			comp_award_light_arrows(); // combos.c
			task_sleep_sec (1);
			break;
//LIGHT EXTRA BALL
	case 4 :
			dmd_draw_thin_border (dmd_low_buffer);
			sound_start(ST_SPEECH, SPCH_LIGHT_EXTRA_BALL, SL_4S, PRI_GAME_QUICK5);
			sprintf ("LIGHT");
			font_render_string_center (&font_steel, DMD_MIDDLE_X, DMD_BIG_CY_Top, sprintf_buffer);
			sprintf ("EXTRA BALL");
			font_render_string_center (&font_steel, DMD_MIDDLE_X, DMD_BIG_CY_Bot, sprintf_buffer);
			dmd_show_low ();

			flasher_pulse (FLASH_ELEVATOR_1_FLASHER);
			task_sleep (TIME_33MS);
			flasher_pulse (FLASH_ELEVATOR_2_FLASHER);
			task_sleep (TIME_33MS);
			flasher_pulse (FLASH_CLAW_FLASHER);
			sound_start (ST_EFFECT, ZAPP_3_SHORT, SL_2S, PRI_GAME_QUICK2);
			task_sleep (TIME_100MS);

			flasher_pulse (FLASH_CLAW_FLASHER);
			task_sleep (TIME_33MS);
			flasher_pulse (FLASH_ELEVATOR_2_FLASHER);
			task_sleep (TIME_33MS);
			flasher_pulse (FLASH_ELEVATOR_1_FLASHER);
			sound_start (ST_EFFECT, ZAPP_3_SHORT, SL_2S, PRI_GAME_QUICK2);
			task_sleep (TIME_100MS);

			flasher_pulse (FLASH_ELEVATOR_1_FLASHER);
			task_sleep (TIME_33MS);
			flasher_pulse (FLASH_CLAW_FLASHER);
			task_sleep (TIME_33MS);
			flasher_pulse (FLASH_ELEVATOR_2_FLASHER);
			sound_start (ST_EFFECT, ZAPP_3_MED, SL_2S, PRI_GAME_QUICK2);
			task_sleep (TIME_100MS);

			for (i = 0; i < 6; i++) {
				flasher_pulse (FLASH_LEFT_RAMP_UP_FLASHER);
				task_sleep (TIME_33MS);
				flasher_pulse (FLASH_DIVERTER_FLASHER);
				task_sleep (TIME_33MS);
				flasher_pulse (FLASH_RIGHT_RAMP_UP_FLASHER);
				sound_start (ST_EFFECT, ZAPP_3_MED, SL_2S, PRI_GAME_QUICK2);
				task_sleep (TIME_100MS);
			}

			start_extraball();
			task_sleep_sec (1);
			break;
//MAXIMIZE FREEZES
	case 5 :
			dmd_draw_thin_border (dmd_low_buffer);
			sound_start(ST_SPEECH, SPCH_MAXIMIZE_FREEZES, SL_4S, PRI_GAME_QUICK5);
			sprintf ("MAXIMIZE");
			font_render_string_center (&font_steel, DMD_MIDDLE_X, DMD_BIG_CY_Top, sprintf_buffer);
			sprintf ("FREEZES");
			font_render_string_center (&font_steel, DMD_MIDDLE_X, DMD_BIG_CY_Bot, sprintf_buffer);
			dmd_show_low ();

			lamp_tristate_flash (LM_FREEZE_1);
			lamp_tristate_flash (LM_FREEZE_2);
			lamp_tristate_flash (LM_FREEZE_3);
			lamp_tristate_flash (LM_FREEZE_4);

			for (i = 0; i < 3; i++) {
				sound_start (ST_EFFECT, ZAPP_3_SHORT, SL_2S, PRI_GAME_QUICK2);
				task_sleep (TIME_300MS);
			}
			sound_start (ST_EFFECT, ZAPP_3_MED, SL_2S, PRI_GAME_QUICK2);
			task_sleep (TIME_500MS);
			sound_start (ST_EFFECT, ZAPP_3_MED, SL_2S, PRI_GAME_QUICK2);
			task_sleep (TIME_500MS);

			sound_start (ST_EFFECT, ZAPP_3_LONG, SL_2S, PRI_GAME_QUICK2);
			task_sleep (TIME_500MS);
			sound_start (ST_EFFECT, ZAPP_3_LONG, SL_2S, PRI_GAME_QUICK2);
			task_sleep (TIME_500MS);

			maximize_freeze();
			task_sleep_sec (1);
			break;
//DOUBLE RETINA SCAN
		case 6 :
			dmd_draw_thin_border (dmd_low_buffer);
			sound_start(ST_SPEECH, SPCH_DOUBLE_RETINA_SCAN, SL_4S, PRI_GAME_QUICK5);
			sprintf ("DOUBLE");
			font_render_string_center (&font_steel, DMD_MIDDLE_X, DMD_BIG_CY_Top, sprintf_buffer);
			sprintf ("RETINA SCAN");
			font_render_string_center (&font_steel, DMD_MIDDLE_X, DMD_BIG_CY_Bot, sprintf_buffer);
			dmd_show_low ();

			lamp_tristate_flash(LM_RETINA_SCAN);
			for (i = 0; i < 3; i++) {
				flasher_pulse (FLASH_EJECT_FLASHER);
				flasher_pulse (FLASH_EYEBALL_FLASHER);
				flasher_pulse (FLASH_EJECT_FLASHER);
				flasher_pulse (FLASH_EYEBALL_FLASHER);
				sound_start (ST_EFFECT, ZAPP_3_SHORT, SL_2S, PRI_GAME_QUICK2);
				task_sleep (TIME_300MS);
			}
			for (i = 0; i < 4; i++) {
				flasher_pulse (FLASH_EJECT_FLASHER);
				flasher_pulse (FLASH_EYEBALL_FLASHER);
				flasher_pulse (FLASH_EJECT_FLASHER);
				flasher_pulse (FLASH_EYEBALL_FLASHER);
				sound_start (ST_EFFECT, ZAPP_3_MED, SL_2S, PRI_GAME_QUICK2);
				task_sleep (TIME_500MS);
			}
			comp_award_doub_retina();
			lamp_tristate_off(LM_RETINA_SCAN);
			task_sleep_sec (1);
			break;
	//BIG POINTS
		case 7 :
			score (SC_10M);
			dmd_draw_thin_border (dmd_low_buffer);
			sprintf ("BIG POINTS");
			font_render_string_center (&font_steel, DMD_MIDDLE_X, DMD_BIG_CY_Top, sprintf_buffer);
			sprintf ("10 MILLION");
			font_render_string_center (&font_steel, DMD_MIDDLE_X, DMD_BIG_CY_Bot, sprintf_buffer);
			dmd_show_low ();

			for (i = 0; i < 5; i++) {
				flasher_pulse (FLASH_LEFT_RAMP_UP_FLASHER);
				task_sleep (TIME_33MS);
				flasher_pulse (FLASH_DIVERTER_FLASHER);
				task_sleep (TIME_33MS);
				flasher_pulse (FLASH_RIGHT_RAMP_UP_FLASHER);
				sound_start (ST_EFFECT, ZAPP_3_MED, SL_2S, PRI_GAME_QUICK2);
				task_sleep (TIME_100MS);
			}

			task_sleep (TIME_500MS);
			break;
	//CAR CHASE
		case 8 :
			dmd_draw_thin_border (dmd_low_buffer);
			sprintf ("START");
			font_render_string_center (&font_steel, DMD_MIDDLE_X, DMD_BIG_CY_Top, sprintf_buffer);
			sprintf ("CAR CHASE");
			font_render_string_center (&font_steel, DMD_MIDDLE_X, DMD_BIG_CY_Bot, sprintf_buffer);
			dmd_show_low ();
			task_sleep_sec (1);
			start_car_chase(); //at car_chase_mode.c
			break;
	//EXPLODE
		case 9 :
			dmd_draw_thin_border (dmd_low_buffer);
			sprintf ("START");
			font_render_string_center (&font_steel, DMD_MIDDLE_X, DMD_BIG_CY_Top, sprintf_buffer);
			sprintf ("EXPLODE");
			font_render_string_center (&font_steel, DMD_MIDDLE_X, DMD_BIG_CY_Bot, sprintf_buffer);
			dmd_show_low ();
			task_sleep_sec (1);
			start_explode();//start explode mode
			break;
	}//end of switch
	task_sleep(TIME_500MS);

	//////////////////////////////////////////////////////////////////////
	dmd_alloc_pair_clean ();// Clean both pages
	dmd_draw_thin_border (dmd_low_buffer);
	sprintf ("NEXT AWARD");
	font_render_string_center (&font_steel, DMD_MIDDLE_X, DMD_BIG_CY_Top, sprintf_buffer);

	switch (nextcomputerAwardsChooser) {
	case 0 : font_render_string_center (&font_steel, DMD_MIDDLE_X, DMD_BIG_CY_Bot, "BONUS"); break;
	case 1 : font_render_string_center (&font_steel, DMD_MIDDLE_X, DMD_BIG_CY_Bot, "3X CRASH"); break;
	case 2 : font_render_string_center (&font_steel, DMD_MIDDLE_X, DMD_BIG_CY_Bot, "STANDUPS"); break;
	case 3 : font_render_string_center (&font_steel, DMD_MIDDLE_X, DMD_BIG_CY_Bot, "ARROWS"); break;
	case 4 : font_render_string_center (&font_steel, DMD_MIDDLE_X, DMD_BIG_CY_Bot, "EXTRA BALL"); break;
	case 5 : font_render_string_center (&font_steel, DMD_MIDDLE_X, DMD_BIG_CY_Bot, "MAX FREEZES"); break;
	case 6 : font_render_string_center (&font_steel, DMD_MIDDLE_X, DMD_BIG_CY_Bot, "2X RETINA SCAN"); break;
	case 7 : font_render_string_center (&font_steel, DMD_MIDDLE_X, DMD_BIG_CY_Bot, "BIG POINTS"); break;
	case 8 : font_render_string_center (&font_steel, DMD_MIDDLE_X, DMD_BIG_CY_Bot, "CAR CHASE"); break;
	case 9 : font_render_string_center (&font_steel, DMD_MIDDLE_X, DMD_BIG_CY_Bot, "EXPLODE"); break;
	}//end of switch
	dmd_show_low ();
	task_sleep(TIME_1S);

	computer_light_off(); //turns off flag here also
	combo_rehit_check (); //check to see if enough combos for next computer award
	ball_search_monitor_start ();
	computer_award_just_awarded = TRUE;
	deff_exit ();
}//end of function





#define MAX_SQUARE 24
#define MAX_STATE_SQUARE 5

struct square_state {
	U8 state;
	U8 x;
	U8 y;
} square_states[MAX_SQUARE];



/*
 * The format of the image data is the same as for a font glyph:
the first byte is its bit-width, the second byte is its
bit-height, and the remaining bytes are the image data, going
from left to right, top to bottom.
 *font_byte_width = (font_width + 7) >> 3
 */
static const U8 open_square_bitmaps1[] = {//open, big hole in middle
		8, 8,
		0xFF, 0X81, 0X81, 0X81,
		0X81, 0X81, 0X81, 0xFF,

};

static const U8 open_square_bitmaps2[] = {//open, small hole in middle
		8, 8,
		0xFF, 0XFF, 0XC3, 0XC3,
		0XC3, 0XC3, 0XFF, 0xFF,
};

static const U8 closed_square_bitmaps[] = {//solid square
		8, 8,
		0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF,
};

static const U8 closed_square_bitmaps2[] = {//small solid square
		8, 8,
		0x00, 0x00, 0x3C, 0x3C,
		0x3C, 0x3C, 0x00, 0x00,
};

static const U8 closed_square_bitmaps3[] = {//tiny solid square
		8, 8,
		0x00, 0x00, 0x18, 0x18,
		0x18, 0x18, 0x00, 0x00,
};

void square_init (void) {
	U8 TEMPX = 6;
	U8 TEMPY = 0;
	U8 n;
	for (n = 0; n < 24; n++) {
		struct square_state *s = &square_states[n];
		if (n % 12 == 0) {
			TEMPX = 6;
			TEMPY += 9;
		}
		s->x = TEMPX; // from 14 to 114 --total size is 128
		s->y = TEMPY;
		TEMPX += 10;
		s->state = random_scaled (12); // 0 to n-1
	}
}//end of function



void square_draw (U8 sq) {
	struct square_state *s = &square_states[sq];
//	s->state = 1; // 0 to n-1
	s->state = random_scaled (12); // 0 to n-1
	switch (s->state) {
	case 0:
		bitmap_blit (open_square_bitmaps1, s->x, s->y); //open square - light
		break;
	case 1:
		bitmap_blit (open_square_bitmaps1, s->x, s->y); //open square - dark
		dmd_flip_low_high();
		bitmap_blit (open_square_bitmaps1, s->x, s->y);
		dmd_flip_low_high();
		break;
	case 2:
		bitmap_blit (open_square_bitmaps2, s->x, s->y); //open small square - light
		break;
	case 3:
		bitmap_blit (open_square_bitmaps2, s->x, s->y); //open small square - dark
		dmd_flip_low_high();
		bitmap_blit (open_square_bitmaps2, s->x, s->y);
		dmd_flip_low_high();
		break;
	case 4:
		bitmap_blit (closed_square_bitmaps, s->x, s->y); //closed square, light
		break;
	case 5:
		bitmap_blit (closed_square_bitmaps, s->x, s->y); //closed square, dark
		dmd_flip_low_high();
		bitmap_blit (closed_square_bitmaps, s->x, s->y);
		dmd_flip_low_high();
		break;
	case 6:
		bitmap_blit (closed_square_bitmaps2, s->x, s->y); //closed small square, light
		break;
	case 7:
		bitmap_blit (closed_square_bitmaps2, s->x, s->y); //closed small square, dark
		dmd_flip_low_high();
		bitmap_blit (closed_square_bitmaps2, s->x, s->y);
		dmd_flip_low_high();
		break;
	case 8:
		bitmap_blit (open_square_bitmaps1, s->x, s->y); //dark in middle, light outside
		dmd_flip_low_high();							//slightly tilted to left
		bitmap_blit (closed_square_bitmaps2, s->x, s->y);
		dmd_flip_low_high();
		break;
	case 9:
		bitmap_blit (closed_square_bitmaps2, s->x, s->y); //light in middle, dark outside
		dmd_flip_low_high();							//slightly tilted to left
		bitmap_blit (open_square_bitmaps1, s->x, s->y);
		dmd_flip_low_high();
		break;
	case 10:
		bitmap_blit (open_square_bitmaps1, s->x, s->y); //dark in middle, light outside
		dmd_flip_low_high();							//slightly tilted to left
		bitmap_blit (closed_square_bitmaps3, s->x, s->y);
		dmd_flip_low_high();
		break;
	case 11:
		bitmap_blit (closed_square_bitmaps3, s->x, s->y); //light in middle, dark outside
		dmd_flip_low_high();							//slightly tilted to left
		bitmap_blit (open_square_bitmaps1, s->x, s->y);
		dmd_flip_low_high();
		break;
	default:
		bitmap_blit (closed_square_bitmaps2, s->x, s->y);
		break;
	}
//	bitmap_blit2 (square_bitmaps + s->state * 5, s->x, s->y);//bitmap_erase_asm (...);
}//end of function



void show_text_on_squares (U8 times) {
	square_init();
	U8 n;
	U8 z;
	for (z = 0; z < times; z++) {
			dmd_dup_mapped ();//allocate new space but make it a copy of what is on DMD now
			dmd_overlay_onto_color ();//mono overlay onto current color page
			for (n = 0; n < 24; n++) {
				square_draw (n);
			}
		//	dmd_show_low();//shows a 2 color image
			dmd_show2 ();//shows a 4 color image
			task_sleep (TIME_200MS);
		//	dmd_map_overlay ();/** Map a consecutive display page pair into windows 0 & 1 */
			dmd_alloc_pair_clean ();
	}
}//END OF FUNCTION
