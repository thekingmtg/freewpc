/*
 * demolition man
 * dm_amode.c
 *
 * written by James Cardona
 * - some code liberally borrowed from tz
 *
 * this is a custom attract mode
 *
 * 	dmd_sched_transition (&transition type);
 *
 * 	transition types are as follows:
 * 	trans_scroll_up --fastest
 * 	trans_scroll_up_avg
 * 	trans_scroll_up_slow
 *
 * 	trans_scroll_down
 *  trans_scroll_down_avg
 *  trans_scroll_down_slow
 *
 *  trans_scroll_left  --slowest
 *  trans_scroll_left_fast
 *  trans_scroll_right
 *  trans_scroll_right_fast
 *
 *  trans_sequential_boxfade
 *  trans_random_boxfade
 *
 *  trans_vstripe_left2right
 *  trans_vstripe_right2left
 *
 *  trans_bitfade_slow
 *  trans_bitfade_fast
 *  trans_bitfade_fastest
 *
 *  trans_unroll_vertical
 *
 */

/* CALLSET_SECTION (dm_amode, __machine3__) */
#include <freewpc.h>
U8 alternator;

/****************************************************************************
 * lighting effects
 ***************************************************************************/
U8 lamplist;

static void amode_leff1 (void)
{
	register U8 my_lamplist = lamplist;
	lamplist_set_apply_delay (TIME_133MS);
	for (;;)
		lamplist_apply (my_lamplist, leff_toggle);
}//end of function



void amode_leff (void) {
	gi_leff_enable (PINIO_GI_STRINGS);
	for (lamplist = LAMPLIST_RIGHT_RAMP_AWARDS; lamplist <= LAMPLIST_CAR_CRASH; lamplist++) {
		leff_create_peer (amode_leff1);
		task_sleep (TIME_133MS);
	}//END OF LOOP
	task_exit ();
}//end of function


/****************************************************************************
 * display effects
 ***************************************************************************/
void show_driver_animation (void) {
	U16 fno;
	U8 i;
	dmd_clean_page_high ();//
	dmd_clean_page_low ();//
	for (i = 0; i < 5; i++) {
		for (fno = IMG_DRIVER_START; fno <= IMG_DRIVER_END; fno += 2) {
			dmd_alloc_pair ();
			frame_draw_plane (fno++);
			dmd_flip_low_high ();
			frame_draw_plane (fno);
			dmd_flip_low_high ();
			dmd_show2 ();
			task_sleep (TIME_66MS);
		}//end of inner loop
	}//end of outer loop
	dmd_alloc_pair_clean ();/* Clean both pages */
}//end of function



void show_rocket_animation (void) {
	U16 fno;
		for (fno = IMG_ROCKET_LOAD_START; fno <= IMG_ROCKET_LOAD_END; fno ++) {
			/* We are drawing a full frame, so a clean isn't needed */
			dmd_alloc_pair ();
			frame_draw (fno);
			dmd_show2 ();
			task_sleep (TIME_133MS);
		}//end of loop
		task_sleep (TIME_1S);
			for (fno = IMG_ROCKET_LOAD_END; fno >= IMG_ROCKET_LOAD_START; fno --) {
				/* We are drawing a full frame, so a clean isn't needed */
				dmd_alloc_pair ();
				frame_draw (fno);
				dmd_show2 ();
				task_sleep (TIME_133MS);
		}//end of loop
	/* Clean both pages */
	dmd_alloc_pair_clean ();
	task_sleep (TIME_1S);
}//end of function




CALLSET_ENTRY (dm_amode, amode_page) {
	alternator++;
	dmd_clean_page_low ();
	font_render_string_center (&font_steel, 64, 7, "DEMOLITION");
	font_render_string_center (&font_steel, 64, 22, "TIME");
	dmd_show_low ();
	task_sleep(TIME_4S);

	if (alternator % 2 == 0) {
		dmd_sched_transition (&trans_scroll_right);
		show_rocket_animation ();
	} else {
		dmd_sched_transition (&trans_scroll_left);
		show_driver_animation ();
		}

	dmd_sched_transition (&trans_bitfade_slow);
	dmd_clean_page_low ();
	font_render_string_center (&font_term6, 64, 7, "GAME RULES BY");
	font_render_string_center (&font_lithograph, 64, 20, "CARDONA");
	dmd_show_low ();
	task_sleep(TIME_2S);

	dmd_sched_transition (&trans_scroll_up_slow);
	dmd_clean_page_low ();
	font_render_string_center (&font_lithograph, 64, 7, "GRAPHICS BY");
	font_render_string_center (&font_term6, 64, 20, "CARDONA");
	dmd_show_low ();
	task_sleep(TIME_2S);

	//	dmd_clean_page_high ();
//	dmd_text_blur ();
//	dmd_map_overlay ();
	//	frame_draw (IMG_TEST);
	//show_random_factoid ();
	/* Clean the low screen for the transition */
//	dmd_alloc_low_clean ();
//	dmd_show_low ();
	dmd_sched_transition (&trans_bitfade_slow);
	dmd_clean_page_low ();
	font_render_string_center (&font_steel, 64, 7, "DEMOLITION");
	font_render_string_center (&font_steel, 64, 22, "TIME");
	dmd_show_low ();
	task_sleep(TIME_4S);
}//end of function

