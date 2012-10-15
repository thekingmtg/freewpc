/*
 * demolition man
 * dm_millions.c
 *
 * written by James Cardona
 *
 *
 */
/* CALLSET_SECTION (dm_millions, __machine4__) */

#include <freewpc.h>
#include "dm/global_constants.h"
#include "dm_millions.h"

void display_0_top(void) {
	bitmap_blit (dm_millions_0_high_bits, DMD_MIDDLE_X - 16, 1);
	dmd_flip_low_high ();
	bitmap_blit (dm_millions_0_low_bits, DMD_MIDDLE_X - 16, 1);
	dmd_flip_low_high ();
}

void display_1_top(void) {
	bitmap_blit (dm_millions_1_high_bits, DMD_MIDDLE_X - 8, 1);
	dmd_flip_low_high ();
	bitmap_blit (dm_millions_1_low_bits, DMD_MIDDLE_X - 8, 1);
	dmd_flip_low_high ();
}

void display_2_top(void) {
	bitmap_blit (dm_millions_2_high_bits, DMD_MIDDLE_X - 12, 1);
	dmd_flip_low_high ();
	bitmap_blit (dm_millions_2_low_bits, DMD_MIDDLE_X - 12, 1);
	dmd_flip_low_high ();
}

void display_3_top(void) {
	bitmap_blit (dm_millions_3_high_bits, DMD_MIDDLE_X - 12, 1);
	dmd_flip_low_high ();
	bitmap_blit (dm_millions_3_low_bits, DMD_MIDDLE_X - 12, 1);
	dmd_flip_low_high ();
}

void display_4_top(void) {
	bitmap_blit (dm_millions_4_high_bits, DMD_MIDDLE_X - 12, 1);
	dmd_flip_low_high ();
	bitmap_blit (dm_millions_4_low_bits, DMD_MIDDLE_X - 12, 1);
	dmd_flip_low_high ();
}



void display_5_top(void) {
	bitmap_blit (dm_millions_5_high_bits, DMD_MIDDLE_X - 12, 1);
	dmd_flip_low_high ();
	bitmap_blit (dm_millions_5_low_bits, DMD_MIDDLE_X - 12, 1);
	dmd_flip_low_high ();
}



void display_6_top(void) {
	bitmap_blit (dm_millions_6_high_bits, DMD_MIDDLE_X - 12, 1);
	dmd_flip_low_high ();
	bitmap_blit (dm_millions_6_low_bits, DMD_MIDDLE_X - 12, 1);
	dmd_flip_low_high ();
}



void display_7_top(void) {
	bitmap_blit (dm_millions_7_high_bits, DMD_MIDDLE_X - 12, 1);
	dmd_flip_low_high ();
	bitmap_blit (dm_millions_7_low_bits, DMD_MIDDLE_X - 12, 1);
	dmd_flip_low_high ();
}



void display_8_top(void) {
	bitmap_blit (dm_millions_8_high_bits, DMD_MIDDLE_X - 12, 1);
	dmd_flip_low_high ();
	bitmap_blit (dm_millions_8_low_bits, DMD_MIDDLE_X - 12, 1);
	dmd_flip_low_high ();
}



void display_9_top(void) {
	bitmap_blit (dm_millions_9_high_bits, DMD_MIDDLE_X - 12, 1);
	dmd_flip_low_high ();
	bitmap_blit (dm_millions_9_low_bits, DMD_MIDDLE_X - 12, 1);
	dmd_flip_low_high ();
}


void show_num_dm_millions(U8 num) {
	switch (num) {
		default: display_0_top(); break;
		case 1:  display_1_top(); break;
		case 2:  display_2_top(); break;
		case 3:  display_3_top(); break;
		case 4:  display_4_top(); break;
		case 5:  display_5_top(); break;
		case 6:  display_6_top(); break;
		case 7:  display_7_top(); break;
		case 8:  display_8_top(); break;
		case 9:  display_9_top(); break;
		}//end of switch
}//end of function



void display_dm_millions(U8 num) {
	dmd_alloc_pair ();
	dmd_clean_page_low ();
	dmd_clean_page_high ();
	show_num_dm_millions(num);
	dmd_show2 ();
	task_sleep_sec (1);

	dmd_sched_transition (&trans_bitfade_slow);
	//	dmd_clean_page_low ();
	//	dmd_clean_page_high ();
	dmd_map_overlay ();
	sprintf ("MILLION");
	font_render_string_center (&font_amiga4ever, DMD_MIDDLE_X, DMD_BIG_CY_Cent, sprintf_buffer);
	dmd_flip_low_high ();
	sprintf ("MILLION");
	font_render_string_center (&font_amiga4ever, DMD_MIDDLE_X, DMD_BIG_CY_Cent, sprintf_buffer);
	font_render_string_center (&font_amiga4ever, DMD_MIDDLE_X+2, DMD_BIG_CY_Cent + 3, sprintf_buffer);
	dmd_flip_low_high ();

	dmd_show2 ();
	task_sleep_sec (1);
	task_sleep (TIME_500MS);
}//end of function



void display_and_shake_dm_millions(U8 num) {
	dmd_alloc_pair ();
	dmd_clean_page_low ();
	dmd_clean_page_high ();
	show_num_dm_millions(num);
	dmd_show2 ();
	task_sleep_sec (1);

//	dmd_sched_transition (&trans_bitfade_slow);

	U8 i = 0;
	sprintf ("MILLION");
	do {
		U8 x = random_scaled (4);
		U8 y = random_scaled (2);
		if (i%4 == 0)		font_render_string_center (&font_amiga4ever, DMD_MIDDLE_X + x, DMD_BIG_CY_Cent - y, sprintf_buffer);
		else if (i%4 == 1)	font_render_string_center (&font_amiga4ever, DMD_MIDDLE_X + x, DMD_BIG_CY_Cent + y, sprintf_buffer);
		else if (i%4 == 2)	font_render_string_center (&font_amiga4ever, DMD_MIDDLE_X - x, DMD_BIG_CY_Cent - y, sprintf_buffer);
		else 				font_render_string_center (&font_amiga4ever, DMD_MIDDLE_X - x, DMD_BIG_CY_Cent + y, sprintf_buffer);
		dmd_flip_low_high ();
		if (i%4 == 0)		font_render_string_center (&font_amiga4ever, DMD_MIDDLE_X + x, DMD_BIG_CY_Cent - y, sprintf_buffer);
		else if (i%4 == 1)	font_render_string_center (&font_amiga4ever, DMD_MIDDLE_X + x, DMD_BIG_CY_Cent + y, sprintf_buffer);
		else if (i%4 == 2)	font_render_string_center (&font_amiga4ever, DMD_MIDDLE_X - x, DMD_BIG_CY_Cent - y, sprintf_buffer);
		else 				font_render_string_center (&font_amiga4ever, DMD_MIDDLE_X - x, DMD_BIG_CY_Cent + y, sprintf_buffer);

		font_render_string_center (&font_amiga4ever, DMD_MIDDLE_X+2, DMD_BIG_CY_Cent + 3, sprintf_buffer);
		dmd_flip_low_high ();
		dmd_show2 ();
		task_sleep (TIME_66MS);
		dmd_clean_page_low ();
		dmd_clean_page_high ();
	} while (i++ < 16);
}//end of function
