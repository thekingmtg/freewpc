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
const U8 		computerAwardsNumOfSounds = 7; //num between 0 and 6
const sound_code_t computerAwardsSoundsArray[] = {	COMPUTER1, 				COMPUTER_ADDING,   COMPUTER_AWARD_SMALL,
													COMPUTER_AWARD_LONG,	COMPUTER2, 			SPCH_ACCESSING,
													SPCH_COMPUTER_AWARD};

//local variables
U8 			undergroundSwitchDebouncer;
U8 			underground_shots_made;
U8 			underground_goal;
U8			computerAwards;
__boolean 	underground_inBall_search;
//external variables

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
	computerAwards = 0;
}//end of function



void player_underground_reset(void) {
	underground_reset();
}//end of function



CALLSET_ENTRY (underground, start_player) { player_underground_reset(); }
CALLSET_ENTRY (underground, start_ball) { underground_reset(); }


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
}//end of function



/****************************************************************************
 * body
 *
 ****************************************************************************/
void underground_task (void) {
	task_sleep_sec(3);
	undergroundSwitchDebouncer = 0;
	task_exit();
}//end of function



CALLSET_ENTRY (underground, sw_bottom_popper) {
//	if (!underground_inBall_search) {
			if (++undergroundSwitchDebouncer == 1) {
					++underground_shots_made;
					score (BOT_POP_SCORE);//located in kernal/score.c

					//LIGHTING EFFECTS*****************************
						//	leff_start (LEFF_UNDERGROUND_KICKOUT);


					//CALLS			*****************************
					if 		(flag_test(FLAG_IS_COMPUTER_ACTIVATED) ) 			deff_start_sync (DEFF_COMPUTER_AWARD);
					else if (flag_test(FLAG_IS_DEMOTIME_ENABLED) ) 				demotime_start();
					else if (flag_test(FLAG_IS_HUXLEY_RUNNING) )				huxley_mode_shot_made();
					else if (flag_test(FLAG_IS_UGROUND_JACKPOT_ACTIVATED) ) 	score_jackpot();
					else if (flag_test (FLAG_IS_CAPSIM_UNDER_ACTIVATED) )		capture_simon_made();
					else if (flag_test (FLAG_IS_PBREAK_RUNNING) )   			prison_break_made();
					else if (flag_test(FLAG_IS_COMBOS_KILLED) ) 				combo_init();
					else if ( flag_test(FLAG_IS_COMBO_UNDER_ACTIVATED)) 		combo_hit();

			//if nothing special, do normal effects
					if(		!flag_test (FLAG_IS_PBREAK_RUNNING)
						&& 	!flag_test (FLAG_IS_CAPSIM_UNDER_ACTIVATED)
						&& 	!flag_test (FLAG_IS_COMPUTER_ACTIVATED)
						&& 	!flag_test (FLAG_IS_UGROUND_JACKPOT_ACTIVATED) ) {
										deff_start (DEFF_UNDERGROUND_EFFECT);
										U8	underground_SoundCounter;
										underground_SoundCounter = random_scaled(2);//from kernal/random.c
										if (underground_SoundCounter == 0 ) sound_start (ST_EFFECT, SUBWAY, SL_2S, SP_NORMAL);
										else if (underground_SoundCounter == 1 ) sound_start (ST_EFFECT, SUBWAY2, SL_2S, SP_NORMAL);
					}
			//fire sol
					sol_request_async(SOL_BOTTOM_POPPER);
			}//end of if DEBOUNCER
			task_create_gid1 (GID_UG_DEBOUNCE, underground_task);
//	}//end of !underground_inBall_search
}//end of function



CALLSET_ENTRY (underground, ball_search){
	underground_inBall_search = TRUE;
}



/****************************************************************************
 * display effects
 ****************************************************************************/
void underground_effect_deff(void) {
	U16 fno;
	U8 	MessageCounter;
	MessageCounter = random_scaled(3);

	dmd_alloc_pair_clean ();// Clean both pages

	switch (MessageCounter) {
		default:
		case 0:
			for (fno = IMG_PHOENIX_A2_START; fno <= IMG_PHOENIX_A2_END; fno += 2) {
						dmd_alloc_pair ();
						frame_draw(fno);
						dmd_show2 ();
						task_sleep (TIME_100MS);
			}//end of for loop

			for (fno = IMG_PHOENIX_A2_END; fno >= IMG_PHOENIX_A2_START; fno -= 2) {
				sprintf ("SUBWAY");
					dmd_map_overlay ();
					dmd_clean_page_low ();
					font_render_string_center (&font_fireball, DMD_MIDDLE_X - 25, DMD_BIG_CY_Top, sprintf_buffer);
						dmd_text_outline ();
						dmd_alloc_pair ();
						frame_draw(fno);
						dmd_overlay_outline ();
						dmd_show2 ();
						task_sleep (TIME_100MS);
			}//end of for loop
			break;
		case 1:
			for (fno = IMG_JOHN_SPARTAN_A_START; fno <= IMG_JOHN_SPARTAN_A_END; fno += 2) {
						dmd_alloc_pair ();
						frame_draw(fno);
						dmd_show2 ();
						task_sleep (TIME_100MS);
			}//end of for loop

			for (fno = IMG_JOHN_SPARTAN_A_END; fno >= IMG_JOHN_SPARTAN_A_START; fno -= 2) {
				sprintf ("SUBWAY");
					dmd_map_overlay ();
					dmd_clean_page_low ();
					font_render_string_center (&font_fireball, DMD_MIDDLE_X - 25, DMD_BIG_CY_Top, sprintf_buffer);
						dmd_text_outline ();
						dmd_alloc_pair ();
						frame_draw(fno);
						dmd_overlay_outline ();
						dmd_show2 ();
						task_sleep (TIME_100MS);
			}//end of for loop
			break;
		case 2:
			for (fno = IMG_SIMON_FIRE1_START; fno <= IMG_SIMON_FIRE1_END; fno += 2) {
						dmd_alloc_pair ();
						frame_draw(fno);
						dmd_show2 ();
						task_sleep (TIME_100MS);
			}//end of for loop

			for (fno = IMG_SIMON_FIRE1_END; fno >= IMG_SIMON_FIRE1_START; fno -= 2) {
				sprintf ("SUBWAY");
					dmd_map_overlay ();
					dmd_clean_page_low ();
					font_render_string_center (&font_fireball, DMD_MIDDLE_X - 25, DMD_BIG_CY_Top, sprintf_buffer);
						dmd_text_outline ();
						dmd_alloc_pair ();
						frame_draw(fno);
						dmd_overlay_outline ();
						dmd_show2 ();
						task_sleep (TIME_100MS);
			}//end of for loop
			break;
	}//END OF SWITCH
		task_sleep (TIME_500MS);
		deff_exit ();
}//end of mode_effect_deff






void computer_award_deff(void) {
	ball_search_monitor_stop ();

	sound_start (ST_ANY, SPCH_ACCESSING, SL_3S, PRI_GAME_QUICK6);

	dmd_alloc_low_clean ();
	sprintf ("COMPUTER AWARD");
	font_render_string_center (&font_var5, DMD_MIDDLE_X, DMD_SMALL_CY_1, sprintf_buffer);
	dmd_show_low ();
	task_sleep_sec (1);
	dmd_map_overlay ();

	dmd_clean_page_high ();
	dmd_clean_page_low ();
	sprintf ("COMPUTER AWARD");
	font_render_string_center (&font_var5, DMD_MIDDLE_X, DMD_SMALL_CY_1, sprintf_buffer);

	sound_start (ST_EFFECT, COMPUTER_ADDING, SL_3S, PRI_GAME_QUICK6);

	show_text_on_squares (10);//5 times per second

	sound_start (ST_EFFECT, COMPUTER_AWARD_LONG, SL_3S, PRI_GAME_QUICK6);
	task_sleep(TIME_2S);

	dmd_alloc_low_clean ();

	computerAwards = random_scaled(7);
		switch (computerAwards) {
	case 0 :
			sound_start(ST_SPEECH, SPCH_COLLECT_BONUS, SL_4S, PRI_GAME_QUICK5);
			sprintf ("COLLECT");
			font_render_string_center (&font_steel, DMD_MIDDLE_X, DMD_BIG_CY_Top, sprintf_buffer);
			sprintf ("BONUS");
			font_render_string_center (&font_steel, DMD_MIDDLE_X, DMD_BIG_CY_Bot, sprintf_buffer);
			dmd_show_low ();
			task_sleep_sec (1);
			deff_start (DEFF_BONUS_CA);
			break;
	case 1 :
			sound_start(ST_SPEECH, SPCH_TRIPLE_CAR_CRASH, SL_4S, PRI_GAME_QUICK5);
			sprintf ("TRIPLE");
			font_render_string_center (&font_steel, DMD_MIDDLE_X, DMD_BIG_CY_Top, sprintf_buffer);
			sprintf ("CAR CRASH");
			font_render_string_center (&font_steel, DMD_MIDDLE_X, DMD_BIG_CY_Bot, sprintf_buffer);
			dmd_show_low ();
			comp_award_trip_car_crash();
			break;
	case 2 :
			sound_start(ST_SPEECH, SPCH_COLLECT_STANDUPS, SL_4S, PRI_GAME_QUICK5);
			sprintf ("COLLECT");
			font_render_string_center (&font_steel, DMD_MIDDLE_X, DMD_BIG_CY_Top, sprintf_buffer);
			sprintf ("STANDUPS");
			font_render_string_center (&font_steel, DMD_MIDDLE_X, DMD_BIG_CY_Bot, sprintf_buffer);
			dmd_show_low ();
			collect_standups();
			break;
	case 3 :
			sound_start(ST_SPEECH, SPCH_LIGHT_ARROWS, SL_4S, PRI_GAME_QUICK5);
			sprintf ("LIGHT");
			font_render_string_center (&font_steel, DMD_MIDDLE_X, DMD_BIG_CY_Top, sprintf_buffer);
			sprintf ("ARROWS");
			font_render_string_center (&font_steel, DMD_MIDDLE_X, DMD_BIG_CY_Bot, sprintf_buffer);
			dmd_show_low ();
			comp_award_light_arrows(); // combos.c
			break;
	case 4 :
			sound_start(ST_SPEECH, SPCH_LIGHT_EXTRA_BALL, SL_4S, PRI_GAME_QUICK5);
			sprintf ("LIGHT");
			font_render_string_center (&font_steel, DMD_MIDDLE_X, DMD_BIG_CY_Top, sprintf_buffer);
			sprintf ("EXTRA BALL");
			font_render_string_center (&font_steel, DMD_MIDDLE_X, DMD_BIG_CY_Bot, sprintf_buffer);
			dmd_show_low ();
			extraball_light_on();
			break;
	case 5 :
			sound_start(ST_SPEECH, SPCH_MAXIMIZE_FREEZES, SL_4S, PRI_GAME_QUICK5);
			sprintf ("MAXIMIZE");
			font_render_string_center (&font_steel, DMD_MIDDLE_X, DMD_BIG_CY_Top, sprintf_buffer);
			sprintf ("FREEZES");
			font_render_string_center (&font_steel, DMD_MIDDLE_X, DMD_BIG_CY_Bot, sprintf_buffer);
			dmd_show_low ();
			maximize_freeze();
			break;
	case 6 :
			sound_start(ST_SPEECH, SPCH_DOUBLE_RETINA_SCAN, SL_4S, PRI_GAME_QUICK5);
			sprintf ("DOUBLE");
			font_render_string_center (&font_steel, DMD_MIDDLE_X, DMD_BIG_CY_Top, sprintf_buffer);
			sprintf ("RETINA SCAN");
			font_render_string_center (&font_steel, DMD_MIDDLE_X, DMD_BIG_CY_Bot, sprintf_buffer);
			dmd_show_low ();
			comp_award_doub_retina();
			break;
	}//end of switch
	task_sleep(TIME_4S);

	computer_light_off();
	ball_search_monitor_start ();
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





/****************************************************************************
 * lighting effects
 ****************************************************************************/
/*
static void under_kickout_subtask (void) {
	U8 i;
	for (i = 0; i < 5; i++) {
		leff_toggle (LAMPLIST_UNDERGROUND_SCOOP);
		task_sleep (TIME_100MS);
	}//end of loop
	task_exit ();
}//end of function

void underground_kickout_leff (void) {
	gi_leff_enable (PINIO_GI_STRINGS);
	leff_create_peer (under_kickout_subtask);
	task_sleep (TIME_500MS);
	gi_leff_enable (PINIO_GI_STRINGS);
	leff_exit ();
}//end of function

*/
