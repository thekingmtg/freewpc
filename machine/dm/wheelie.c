/*
 * Copyright 2011 by Ewan Meadows <sonny_jim@hotmail.com>
 *
 * This file is part of FreeWPC.
 *
 * FreeWPC is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * FreeWPC is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with FreeWPC; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

/* CALLSET_SECTION (wheelie, __machine4__) */

#include <freewpc.h>
#include "wheelie.h"

//constants
#define JUMP_HEIGHT 6
#define BIKE_YPOS 32 - wheelie1_low_height

//local variables
U8 wheelie_ypos;
U8 wheelie_xpos;
U8 wheelie_speed;
U8 wheelie_speed_calc;
U8 wheelie_buttons_pressed;
U8 wheelie_arrow_offset;

U8 wheelie_max_level = 88;
__boolean wheelie_running;

enum wheelie_states {
	WHEELIE_FLAT,
	WHEELIE_FLAT2,
	WHEELIE_UP1,
	WHEELIE_UP2,
	WHEELIE_UP3,
	WHEELIE_JUMP,
	WHEELIE_DOWN3,
	WHEELIE_DOWN2,
	WHEELIE_DOWN1,
} wheelie_state;

enum wheelie_game_states {
	WHEELIE_GAME_INTRO,
	WHEELIE_GAME_SPEED,
	WHEELIE_GAME_PREJUMP,
	WHEELIE_GAME_ANGLE,
	WHEELIE_GAME_JUMP,
	WHEELIE_GAME_JUMPING,
	WHEELIE_GAME_END,
} wheelie_game_state;

//external variables

//prototypes
static void draw_wheelie1 (U8 x, U8 y);
static void draw_wheelie2 (U8 x, U8 y);
static void draw_wheelie3 (U8 x, U8 y);
static void draw_wheelie4 (U8 x, U8 y);
static void draw_wheelie5 (U8 x, U8 y);
static void draw_wheelie6 (U8 x, U8 y);

void start_wheelie (void);
static void wheelie_start (void);
static void wheelie_button_press (void);
static void wheelie_sleep (void);
static void draw_wheelie (U8 xpos);
static void start_jump (void);
static bool wheelie_going_up (void);
static void calculate_jump (void);


/****************************************************************************
 *
 * init
 *
 ****************************************************************************/
struct progress_bar_ops wheelie_progress_bar = {
	.x = 39,
	.y = 1,
	.fill_level = &wheelie_speed_calc,
	.max_level = &wheelie_max_level,
	.bar_width = 88,
};



void start_wheelie (void) {
	wheelie_running = TRUE;
	deff_start (DEFF_WHEELIE);
}//end of function





static void wheelie_start (void) {
	wheelie_game_state = WHEELIE_GAME_INTRO;
	wheelie_state = WHEELIE_FLAT;
	wheelie_xpos = 2;
	wheelie_ypos = 0;
	wheelie_speed = 0;
	wheelie_speed_calc = 1;
	wheelie_buttons_pressed = 1;
	wheelie_arrow_offset = 0;
}

/****************************************************************************
 * external event listeners
 ****************************************************************************/
CALLSET_ENTRY (wheelie, music_refresh) { if (wheelie_running)			music_request (MUS_MB_A, PRI_GAME_VMODE); }
CALLSET_ENTRY (wheelie, start_player) 	{ wheelie_running = FALSE; }

CALLSET_ENTRY (wheelie, sw_right_button, sw_left_button, sw_upper_right_button, sw_upper_left_button) {
	if (deff_get_active () == DEFF_WHEELIE && wheelie_game_state == WHEELIE_GAME_SPEED)
		wheelie_button_press ();
}//end of function

/****************************************************************************
 *
 * body
 *
 ****************************************************************************/
static void wheelie_button_press (void) {
	if (wheelie_speed < 8)
		bounded_increment (wheelie_buttons_pressed, 10);
	else
		bounded_increment (wheelie_buttons_pressed, 8);

	if (wheelie_buttons_pressed >= 10 && wheelie_speed < 8)
	{
		wheelie_buttons_pressed = 0;
		sound_start (ST_EFFECT, ZAPP, SL_2S, PRI_GAME_QUICK5);
		bounded_increment (wheelie_speed, 8);
	}
	wheelie_speed_calc = (wheelie_speed * 10) + wheelie_buttons_pressed;
	if (wheelie_speed_calc == 88)
	{
		//88 MPH!
	}
}//end of function



static bool wheelie_going_up (void) {
	if (wheelie_state >= WHEELIE_UP1 && wheelie_state <= WHEELIE_JUMP)		return TRUE;
	else																	return FALSE;
}//end of function



static void calculate_jump (void) {
	if (wheelie_state > WHEELIE_UP1 && wheelie_state < WHEELIE_DOWN3)
		wheelie_xpos += 2;
	if (wheelie_going_up ())
	{	
		if (wheelie_ypos < JUMP_HEIGHT)
			wheelie_ypos++;
		else if (wheelie_ypos == JUMP_HEIGHT)
			wheelie_state = WHEELIE_DOWN3;
	}
	else if (wheelie_state > WHEELIE_JUMP)
	{
		bounded_decrement (wheelie_speed, 0);
		if (wheelie_ypos > 0 && wheelie_speed == 0)
		wheelie_ypos--;
	}
}//end of function



static void start_jump (void) {
	if (wheelie_ypos <= 1) {
		sound_start (ST_EFFECT, ZAPP, SL_2S, PRI_GAME_QUICK5);
		wheelie_state = WHEELIE_UP1;
	}
}//end of function



static void draw_wheelie (U8 xpos) {
	if (wheelie_game_state == WHEELIE_GAME_INTRO) {
		draw_wheelie1 (xpos, BIKE_YPOS - wheelie_ypos);
		return;
	}
	switch (wheelie_state) {
		case WHEELIE_FLAT:
			draw_wheelie1 (xpos, BIKE_YPOS - wheelie_ypos);
			wheelie_state = WHEELIE_FLAT2;
			break;
		case WHEELIE_FLAT2:
			draw_wheelie2 (xpos, BIKE_YPOS - wheelie_ypos);
			wheelie_state = WHEELIE_FLAT;
			break;
		case WHEELIE_UP1:
			draw_wheelie3 (xpos, BIKE_YPOS - wheelie_ypos);
			wheelie_state = WHEELIE_UP2;
			break;
		case WHEELIE_UP2:
			draw_wheelie4 (xpos, BIKE_YPOS - wheelie_ypos);
			wheelie_state = WHEELIE_UP3;
			break;
		case WHEELIE_UP3:
			draw_wheelie5 (xpos, BIKE_YPOS - wheelie_ypos);
			wheelie_state = WHEELIE_JUMP;
			break;
		case WHEELIE_JUMP:

			draw_wheelie6 (xpos, BIKE_YPOS - wheelie_ypos);
			break;
		case WHEELIE_DOWN3:
			draw_wheelie5 (xpos, BIKE_YPOS - wheelie_ypos);
			wheelie_state = WHEELIE_DOWN2;
			break;
		case WHEELIE_DOWN2:
			draw_wheelie4 (xpos, BIKE_YPOS - wheelie_ypos);
			wheelie_state = WHEELIE_DOWN1;
			break;
		case WHEELIE_DOWN1:
			draw_wheelie3 (xpos, BIKE_YPOS - wheelie_ypos);
			wheelie_state = WHEELIE_FLAT;
			break;
	}// end of switch
}//end of function



static void wheelie_sleep (void) {
	switch (wheelie_speed) {
		default:
		case 1:		task_sleep (TIME_300MS);		break;
		case 2:		task_sleep (TIME_200MS);		break;
		case 3:		task_sleep (TIME_133MS);		break;
		case 4:		task_sleep (TIME_100MS);		break;
		case 5:		task_sleep (TIME_66MS);			break;
		case 6:
		case 7:
		case 8:		task_sleep (TIME_33MS);			break;
	}// end of switch
}//end of function


/****************************************************************************
 * lighting effects
 ****************************************************************************/


/****************************************************************************
 * display effects
 ****************************************************************************/
static void wheelie_draw_speed (void) {
		sprintf ("%d MPH", wheelie_speed_calc);
	font_render_string_center (&font_var5, 64, 16, sprintf_buffer);
}//end of function



static void show_intro_screen (void) {
	dmd_alloc_pair_clean ();
	font_render_string_center (&font_var5, 64, 0, "WHEELIE!");
	//font_render_string_center_ytop (&font_var5, 64, 0, "WHEELIE!");
//	dmd_copy_low_to_high ();
	//draw_arrows ();
	font_render_string_center (&font_var5, 64, 12, "BASH FLIPPER BUTTONS");
	font_render_string_center (&font_var5, 64, 20, "TO BUILD UP SPEED");
	dmd_show_low ();
	//sound_send (SND_WELCOME_RACE_FANS);
	task_sleep_sec (4);
	
	dmd_alloc_pair_clean ();
	font_render_string_center (&font_var5, 64, 0, "WHEELIE!");
//	font_render_string_center_ytop (&font_var5, 64, 0, "WHEELIE!");
//	dmd_copy_low_to_high ();
	draw_wheelie (wheelie_xpos);
	//draw_arrows ();
	font_render_string_center (&font_var5, 64, 16, "3");
	//sound_send (SND_THREE);
//	dmd_show2 ();
	dmd_show_low ();
	task_sleep_sec (1);

	dmd_alloc_pair_clean ();
	font_render_string_center (&font_var5, 64, 0, "WHEELIE!");
	//font_render_string_center_ytop (&font_var5, 64, 0, "WHEELIE!");
//	dmd_copy_low_to_high ();
	draw_wheelie (wheelie_xpos);
	//draw_arrows ();
	font_render_string_center (&font_var5, 64, 16, "2");
	//sound_send (SND_TWO);
//	dmd_show2 ();
	dmd_show_low ();
	task_sleep_sec (1);

	dmd_alloc_pair_clean ();
	font_render_string_center (&font_var5, 64, 0, "WHEELIE!");
	//font_render_string_center_ytop (&font_var5, 64, 0, "WHEELIE!");
//	dmd_copy_low_to_high ();
	draw_wheelie (wheelie_xpos);
	//draw_arrows ();
	font_render_string_center (&font_var5, 64, 16, "1");
	//sound_send (SND_ONE);
//	dmd_show2 ();
	dmd_show_low ();
	task_sleep_sec (1);
	//sound_send (SND_CLOCK_GONG);
}



void wheelie_deff (void)
{
	U8 i;
	wheelie_start ();
	show_intro_screen ();
	wheelie_game_state = WHEELIE_GAME_SPEED;
	//for (wheelie_xpos = 2; wheelie_xpos < 128 - wheelie1_low_width; wheelie_xpos += wheelie_speed)
	timer_restart_free (GID_WHEELIE_GAME_TIMER, TIME_10S);
	while (task_find_gid (GID_WHEELIE_GAME_TIMER))
	{
//		dmd_map_overlay ();
	//	frame_draw (IMG_CITY);
		dmd_alloc_pair ();
		dmd_clean_page_low ();
		dmd_clean_page_high ();
		wheelie_draw_speed ();
		draw_wheelie (wheelie_xpos);
//		if (!check_for_marty ())
			draw_progress_bar (&wheelie_progress_bar);
//		dmd_overlay_color ();
//		sound_send (0x54);
		dmd_show2 ();
		wheelie_sleep ();
	}
	wheelie_game_state = WHEELIE_GAME_JUMP;
	//Move the bike slowly to the center
	for (i = 0; i < 20; i++)
	{
//		dmd_map_overlay ();
		dmd_alloc_pair ();
		dmd_clean_page_low ();
		dmd_clean_page_high ();
		wheelie_draw_speed ();
		wheelie_xpos++;
		draw_wheelie (wheelie_xpos);
		//dmd_text_outline ();
//		dmd_alloc_pair ();
//		frame_draw (IMG_CITY);
//		dmd_overlay_color ();
//		sound_send (0x54);
		dmd_show2 ();
		wheelie_sleep ();
	}
	start_jump ();
	wheelie_game_state = WHEELIE_GAME_JUMPING;
	timer_restart_free (GID_WHEELIE_GAME_TIMER, TIME_10S);
	while (task_find_gid (GID_WHEELIE_GAME_TIMER))
	{
//		dmd_map_overlay ();
		dmd_alloc_pair ();
		dmd_clean_page_low ();
		dmd_clean_page_high ();
		calculate_jump ();
		draw_wheelie (wheelie_xpos);
		//dmd_text_outline ();
//		dmd_alloc_pair ();
//		frame_draw (IMG_CITY);
//		dmd_overlay_color ();
		sound_start (ST_EFFECT, ZAPP, SL_2S, PRI_GAME_QUICK5);
		dmd_show2 ();
		wheelie_sleep ();
	}
	wheelie_game_state = WHEELIE_GAME_END;
	deff_exit ();
	wheelie_running = FALSE;
	video_mode_finished ();
}






















/****************************************************************************
 *
 * draw wheelies
 *
 ****************************************************************************/
static void draw_wheelie1 (U8 x, U8 y)
{
	bitmap_blit (wheelie1_low_bits, x, y);
	dmd_flip_low_high ();
	bitmap_blit (wheelie1_high_bits, x, y);
	dmd_flip_low_high ();
}


static void draw_wheelie2 (U8 x, U8 y)
{
	bitmap_blit (wheelie2_low_bits, x, y);
	dmd_flip_low_high ();
	bitmap_blit (wheelie2_high_bits, x, y);
	dmd_flip_low_high ();
}


static void draw_wheelie3 (U8 x, U8 y)
{
	bitmap_blit (wheelie3_low_bits, x, y);
	dmd_flip_low_high ();
	bitmap_blit (wheelie3_high_bits, x, y);
	dmd_flip_low_high ();
}


static void draw_wheelie4 (U8 x, U8 y)
{
	bitmap_blit (wheelie4_low_bits, x, y);
	dmd_flip_low_high ();
	bitmap_blit (wheelie4_high_bits, x, y);
	dmd_flip_low_high ();
}


static void draw_wheelie5 (U8 x, U8 y)
{
	bitmap_blit (wheelie5_low_bits, x, y);
	dmd_flip_low_high ();
	bitmap_blit (wheelie5_high_bits, x, y);
	dmd_flip_low_high ();
}


static void draw_wheelie6 (U8 x, U8 y)
{
	bitmap_blit (wheelie6_low_bits, x, y);
	dmd_flip_low_high ();
	bitmap_blit (wheelie6_high_bits, x, y);
	dmd_flip_low_high ();
}




/****************************************************************************
 *
 * future
 *
 ****************************************************************************/
#define BITMAP_SIZE 7
#define BITMAP_WIDTH 6
/*
static void draw_arrows (void)
{
	U8 i;
	for (i=0; i <= 128 - BITMAP_WIDTH - 4; i += BITMAP_WIDTH)
	{
		bitmap_blit (wheelie_arrow_bits + wheelie_arrow_offset * BITMAP_SIZE, 4 + i, 26);
	}
	if (wheelie_game_state == WHEELIE_GAME_INTRO)
		return;
	if (++wheelie_arrow_offset >= 6)
		wheelie_arrow_offset = 0;
}

*/

