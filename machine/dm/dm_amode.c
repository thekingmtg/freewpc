/*
 * demolition man
 * dm_amode.c
 *
 * written by James Cardona
 * - some code liberally borrowed from tz
 *
 * this is a custom attract mode
 *
 */
#include <freewpc.h>
U8 alternator;

void show_driver_animation (void) {
	/* Show driver animation */	
	U16 fno;
	U8 i;
	for (i = 0; i < 5; i++) {
		for (fno = IMG_DRIVER_START; fno <= IMG_DRIVER_END; fno += 2) {
			/* We are drawing a full frame, so a clean isn't needed */
			dmd_alloc_pair ();
			frame_draw (fno);
			dmd_show2 ();
			task_sleep (TIME_66MS);
		}//end of inner loop
	}//end of outer loop
	/* Clean both pages */
	dmd_alloc_pair_clean ();
}//end of function



void show_rocket_animation (void) {
	/* Show driver animation */
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
		show_driver_animation ();
		}
	else {
		dmd_sched_transition (&trans_scroll_left);
		show_rocket_animation ();
		}

	dmd_sched_transition (&trans_bitfade_slow);
	dmd_clean_page_low ();
	font_render_string_center (&font_term6, 64, 7, "SOFTWARE BY");
	font_render_string_center (&font_lithograph, 64, 20, "CARDONA");
	dmd_show_low ();
	task_sleep(TIME_3S);

	dmd_sched_transition (&trans_scroll_up_slow);
	dmd_clean_page_low ();
	font_render_string_center (&font_lithograph, 64, 7, "GRAPHICS BY");
	font_render_string_center (&font_term6, 64, 20, "CARDONA");
	dmd_show_low ();
	task_sleep(TIME_3S);

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

