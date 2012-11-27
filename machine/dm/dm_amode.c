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

/* CALLSET_SECTION (dm_amode, __machine5__) */

#include <freewpc.h>
#include "dm/global_constants.h"


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
void dm_amode_animation_display_effect (U16 start_frame, U16 end_frame){
	U16 fno;
	for (fno = start_frame; fno <= end_frame; fno += 2) {
		dmd_alloc_pair ();
		frame_draw(fno);
		dmd_show2 ();
		task_sleep (TIME_100MS);
	}//end of inner loop
}

void dm_amode_frame_bitfade_fast (U16 frame){
	dmd_sched_transition (&trans_bitfade_fast);
	dmd_alloc_pair ();
	frame_draw(frame);
	dmd_show2 ();
	task_sleep (TIME_100MS);
}




void show_john_spartan_animation(void) {
	dmd_clean_page_high ();//
	dmd_clean_page_low ();//
	dm_amode_animation_display_effect (IMG_FORTRESS_D1_START, IMG_FORTRESS_D1_END);
	dm_amode_frame_bitfade_fast(IMG_FORTRESS_D2_START);
	dm_amode_animation_display_effect (IMG_FORTRESS_D2_START, IMG_FORTRESS_D2_END);
	dm_amode_frame_bitfade_fast(IMG_FORTRESS_D3_START);
	dm_amode_animation_display_effect (IMG_FORTRESS_D3_START, IMG_FORTRESS_D4_END);

	dmd_alloc_pair_clean ();// Clean both pages
		dmd_map_overlay ();
		dmd_clean_page_low ();
		font_render_string_center (&font_steel, DMD_MIDDLE_X - 20, DMD_BIG_CY_Top, "JOHN");
		font_render_string_center (&font_steel, DMD_MIDDLE_X - 20, DMD_BIG_CY_Bot, "SPARTAN");
		dmd_text_outline ();
		dmd_alloc_pair ();
		frame_draw(IMG_FORTRESS_D4_END);
		dmd_overlay_outline ();
		dmd_show2 ();
	task_sleep(TIME_2S);
}//end of mode_effect_deff


void show_john_spartan2_animation(void) {
	U16 fno;
	dmd_alloc_pair_clean ();// Clean both pages

	for (fno = IMG_JOHN_SPARTAN_A_END; fno >= IMG_JOHN_SPARTAN_A_START; fno -= 2) {
		dmd_alloc_pair ();
		frame_draw(fno);
		dmd_show2 ();
		task_sleep (TIME_100MS);
	} //end of loop

	dm_amode_animation_display_effect (IMG_JOHN_SPARTAN_A_START, IMG_JOHN_SPARTAN_A_END);
		dmd_map_overlay ();
		dmd_clean_page_low ();
		font_render_string_center (&font_steel, DMD_MIDDLE_X + 25, DMD_BIG_CY_Top, "JOHN");
		font_render_string_center (&font_steel, DMD_MIDDLE_X + 25, DMD_BIG_CY_Bot, "SPARTAN");
		dmd_text_outline ();
		dmd_alloc_pair ();
		frame_draw(IMG_JOHN_SPARTAN_A_END);
		dmd_overlay_outline ();
		dmd_show2 ();
	task_sleep(TIME_2S);
}//end of mode_effect_deff



void show_simon_phoenix_animation(void) {
	dmd_clean_page_high ();//
	dmd_clean_page_low ();//
	dm_amode_animation_display_effect (IMG_SIMON_A_START, IMG_SIMON_A_END);
	dm_amode_frame_bitfade_fast(IMG_SIMON_B_START);
	dm_amode_animation_display_effect (IMG_SIMON_B_START, IMG_SIMON_B_END);
	dm_amode_animation_display_effect (IMG_SIMON_C_START, IMG_SIMON_C_END);

	dmd_alloc_pair_clean ();// Clean both pages
		dmd_map_overlay ();
		dmd_clean_page_low ();
		font_render_string_center (&font_steel, DMD_MIDDLE_X - 20, DMD_BIG_CY_Top, "SIMON");
		font_render_string_center (&font_steel, DMD_MIDDLE_X - 30, DMD_BIG_CY_Bot, "PHOENIX");
		dmd_text_outline ();
		dmd_alloc_pair ();
		frame_draw(IMG_SIMON_C_END);
		dmd_overlay_outline ();
		dmd_show2 ();
		task_sleep(TIME_2S);
}//end of mode_effect_deff



void show_simon_phoenix2_animation(void) {
	dmd_clean_page_high ();//
	dmd_clean_page_low ();//
	dm_amode_animation_display_effect (IMG_MUSEUM_D1_START, IMG_MUSEUM_D1_END);
	dm_amode_frame_bitfade_fast(IMG_MUSEUM_C2_START);
	dm_amode_animation_display_effect (IMG_MUSEUM_C2_START, IMG_MUSEUM_C2_END);
	dm_amode_frame_bitfade_fast(IMG_MUSEUM_D3_START);
	dm_amode_animation_display_effect (IMG_MUSEUM_D3_START, IMG_MUSEUM_D3_END);
	dm_amode_animation_display_effect (IMG_MUSEUM_D3_START, IMG_MUSEUM_D3_END);
	dm_amode_animation_display_effect (IMG_MUSEUM_D3_START, IMG_MUSEUM_D3_END);
	dm_amode_animation_display_effect (IMG_MUSEUM_D3_START, IMG_MUSEUM_D3_END);

	dmd_alloc_pair_clean ();// Clean both pages
		dmd_map_overlay ();
		dmd_clean_page_low ();
		font_render_string_center (&font_steel, DMD_MIDDLE_X - 20, DMD_BIG_CY_Top, "SIMON");
		font_render_string_center (&font_steel, DMD_MIDDLE_X - 30, DMD_BIG_CY_Bot, "PHOENIX");
		dmd_text_outline ();
		dmd_alloc_pair ();
		frame_draw(IMG_MUSEUM_D3_END);
		dmd_overlay_outline ();
		dmd_show2 ();
		task_sleep(TIME_2S);
}//end of mode_effect_deff



void show_assoc_bob_animation(void) {
	U16 fno;

	dmd_clean_page_high ();//
	dmd_clean_page_low ();//
	dm_amode_animation_display_effect (IMG_ASSOC_BOB_START, IMG_ASSOC_BOB_END);

	for (fno = IMG_ASSOC_BOB_END; fno >= IMG_ASSOC_BOB_START; fno -= 2) {
		dmd_alloc_pair ();
		frame_draw(fno);
		dmd_show2 ();
		task_sleep (TIME_100MS);
	} //end of loop

	dm_amode_animation_display_effect (IMG_ASSOC_BOB_START, IMG_ASSOC_BOB_END);

	dmd_alloc_pair_clean ();// Clean both pages
		dmd_map_overlay ();
		dmd_clean_page_low ();
		font_render_string_center (&font_steel, DMD_MIDDLE_X - 20, DMD_BIG_CY_Top, "ASSOCIATE");
		font_render_string_center (&font_steel, DMD_MIDDLE_X - 30, DMD_BIG_CY_Bot, "BOB");
		dmd_text_outline ();
		dmd_alloc_pair ();
		frame_draw(IMG_ASSOC_BOB_END);
		dmd_overlay_outline ();
		dmd_show2 ();
		task_sleep(TIME_2S);
}//end of mode_effect_deff




void show_chief_earle_animation(void) {
	U16 fno;
	dmd_clean_page_high ();//
	dmd_clean_page_low ();//
	dm_amode_animation_display_effect (IMG_CHIEF_EARLE2_START, IMG_CHIEF_EARLE2_END);

	for (fno = IMG_CHIEF_EARLE2_END; fno >= IMG_CHIEF_EARLE2_START; fno -= 2) {
		dmd_alloc_pair ();
		frame_draw(fno);
		dmd_show2 ();
		task_sleep (TIME_100MS);
	} //end of loop

	dm_amode_animation_display_effect (IMG_CHIEF_EARLE2_START, IMG_CHIEF_EARLE2_END);

	dmd_alloc_pair_clean ();// Clean both pages
		dmd_map_overlay ();
		dmd_clean_page_low ();
		font_render_string_center (&font_steel, DMD_MIDDLE_X - 20, DMD_BIG_CY_Top, "CHIEF");
		font_render_string_center (&font_steel, DMD_MIDDLE_X - 30, DMD_BIG_CY_Bot, "EARLE");
		dmd_text_outline ();
		dmd_alloc_pair ();
		frame_draw(IMG_CHIEF_EARLE2_END);
		dmd_overlay_outline ();
		dmd_show2 ();
		task_sleep(TIME_2S);
}//end of mode_effect_deff




void show_cocteau_animation(void) {
	U16 fno;
	dmd_clean_page_high ();//
	dmd_clean_page_low ();//
	dm_amode_animation_display_effect (IMG_DR_COCTEAU_B_START, IMG_DR_COCTEAU_B_END);

	for (fno = IMG_DR_COCTEAU_B_END; fno >= IMG_DR_COCTEAU_B_START; fno -= 2) {
		dmd_alloc_pair ();
		frame_draw(fno);
		dmd_show2 ();
		task_sleep (TIME_100MS);
	} //end of loop

	dm_amode_animation_display_effect (IMG_DR_COCTEAU_B_START, IMG_DR_COCTEAU_B_END);

	dmd_alloc_pair_clean ();// Clean both pages
		dmd_map_overlay ();
		dmd_clean_page_low ();
		font_render_string_center (&font_steel, DMD_MIDDLE_X - 20, DMD_BIG_CY_Top, "DR");
		font_render_string_center (&font_steel, DMD_MIDDLE_X - 30, DMD_BIG_CY_Bot, "COCTEAU");
		dmd_text_outline ();
		dmd_alloc_pair ();
		frame_draw(IMG_DR_COCTEAU_B_END);
		dmd_overlay_outline ();
		dmd_show2 ();
		task_sleep(TIME_2S);
}//end of mode_effect_deff





void show_lenina1_animation(void) {
	dmd_clean_page_high ();//
	dmd_clean_page_low ();//
	dm_amode_animation_display_effect (IMG_HUXLEY_A_START, IMG_HUXLEY_A_END);
	dm_amode_frame_bitfade_fast(IMG_HUXLEY_B1_START);
	dm_amode_animation_display_effect (IMG_HUXLEY_B1_START, IMG_HUXLEY_B1_END);
	dm_amode_frame_bitfade_fast(IMG_HUXLEY_B2_START);
	dm_amode_animation_display_effect (IMG_HUXLEY_B2_START, IMG_HUXLEY_B2_END);

	dmd_alloc_pair_clean ();// Clean both pages
		dmd_map_overlay ();
		dmd_clean_page_low ();
		font_render_string_center (&font_steel, DMD_MIDDLE_X - 30, DMD_BIG_CY_Top, "LENINA");
		font_render_string_center (&font_steel, DMD_MIDDLE_X - 30, DMD_BIG_CY_Bot, "HUXLEY");
		dmd_text_outline ();
		dmd_alloc_pair ();
		frame_draw(IMG_HUXLEY_B2_END);
		dmd_overlay_outline ();
		dmd_show2 ();
		task_sleep(TIME_2S);
}//end of mode_effect_deff



void show_lenina2_animation(void) {
	dmd_clean_page_high ();//
	dmd_clean_page_low ();//
	dm_amode_animation_display_effect (IMG_HUXLEY_D_START, IMG_HUXLEY_D_END);
	dm_amode_frame_bitfade_fast(IMG_HUXLEY_E_START);
	dm_amode_animation_display_effect (IMG_HUXLEY_E_START, IMG_HUXLEY_E_END);

	dmd_alloc_pair_clean ();// Clean both pages
		dmd_map_overlay ();
		dmd_clean_page_low ();
		font_render_string_center (&font_steel, DMD_MIDDLE_X - 30, DMD_BIG_CY_Top, "LENINA");
		font_render_string_center (&font_steel, DMD_MIDDLE_X - 30, DMD_BIG_CY_Bot, "HUXLEY");
		dmd_text_outline ();
		dmd_alloc_pair ();
		frame_draw(IMG_HUXLEY_E_END);
		dmd_overlay_outline ();
		dmd_show2 ();
		task_sleep(TIME_2S);
}//end of mode_effect_deff



void show_huxley_info (void) {
	dmd_clean_page_high ();//
	dmd_clean_page_low ();//
	dm_amode_animation_display_effect (IMG_HUXLEY_B1_START, IMG_HUXLEY_B1_END);
	dm_amode_frame_bitfade_fast(IMG_HUXLEY_B2_START);
	dm_amode_animation_display_effect (IMG_HUXLEY_B2_START, IMG_HUXLEY_B2_END);
	task_sleep_sec (1);
			dmd_sched_transition (&trans_scroll_down);
			dmd_clean_page_low ();
			font_render_string_center (&font_var5, DMD_MIDDLE_X, DMD_SMALL_CY_1, "START 4 NON-CLAW");
			font_render_string_center (&font_var5, DMD_MIDDLE_X, DMD_SMALL_CY_2, "MODES FOR");
			font_render_string_center (&font_fipps, DMD_MIDDLE_X, DMD_BIG_CY_Bot, "HUXLEY");
			dmd_show_low();
			task_sleep_sec (3);
}//end of mode_effect_deff





void show_rollovers1_info (void) {
	dmd_clean_page_high ();//
	dmd_clean_page_low ();//
	dm_amode_animation_display_effect (IMG_CAPSIMON_C_START, IMG_CAPSIMON_C_END);
	task_sleep_sec (1);
			dmd_sched_transition (&trans_scroll_down);
			dmd_clean_page_low ();
			font_render_string_center (&font_var5, DMD_MIDDLE_X, DMD_SMALL_CY_1, "LIGHT ALL M T L");
			font_render_string_center (&font_var5, DMD_MIDDLE_X, DMD_SMALL_CY_2, "ROLLOVERS");
			font_render_string_center (&font_var5, DMD_MIDDLE_X, DMD_SMALL_CY_3, "TO");
			font_render_string_center (&font_bitcube10, DMD_MIDDLE_X, DMD_SMALL_CY_4, "LIGHT CRYOCLAW");
			dmd_show_low();
			task_sleep_sec (3);
}//end of mode_effect_deff




void show_standups_info (void) {
	dmd_clean_page_high ();//
	dmd_clean_page_low ();//
	dm_amode_animation_display_effect (IMG_WASTELAND_A1_START, IMG_WASTELAND_A1_END);
	dm_amode_frame_bitfade_fast(IMG_WASTELAND_A2_START);
	dm_amode_animation_display_effect (IMG_WASTELAND_A2_START, IMG_WASTELAND_A2_END);
	task_sleep_sec (1);
			dmd_sched_transition (&trans_scroll_down);
			dmd_clean_page_low ();
			font_render_string_center (&font_var5, DMD_MIDDLE_X, DMD_SMALL_CY_1, "HIT STANDUPS");
			font_render_string_center (&font_var5, DMD_MIDDLE_X, DMD_SMALL_CY_3, "TO");
			font_render_string_center (&font_bitcube10, DMD_MIDDLE_X, DMD_SMALL_CY_4, "LIGHT QUICK FREEZE");
			dmd_show_low();
			task_sleep_sec (3);
}//end of mode_effect_deff



void show_demotime_info (void) {
	dmd_clean_page_high ();//
	dmd_clean_page_low ();//
	dm_amode_animation_display_effect (IMG_SIMON_FIRE1_START, IMG_SIMON_SHOOT_END);
	dm_amode_frame_bitfade_fast(IMG_FIGHT_A_START);
	dm_amode_animation_display_effect (IMG_FIGHT_A_START, IMG_JOHN_SPARTAN_A_END);
	task_sleep_sec (1);

			dmd_sched_transition (&trans_scroll_down);
			dmd_clean_page_low ();
			font_render_string_center (&font_var5, DMD_MIDDLE_X, DMD_SMALL_CY_1, "START 5");
			font_render_string_center (&font_var5, DMD_MIDDLE_X, DMD_SMALL_CY_2, "CLAW MODES");
			font_render_string_center (&font_var5, DMD_MIDDLE_X, DMD_SMALL_CY_3, "FOR");
			font_render_string_center (&font_bitcube10, DMD_MIDDLE_X, DMD_SMALL_CY_4, "DEMOLITION TIME");
			dmd_show_low();
			task_sleep_sec (3);
}//end of mode_effect_deff










CALLSET_ENTRY (dm_amode, amode_page) {
	U8 numOfAlts = 4;
	alternator++;
	dmd_clean_page_low ();
	font_render_string_center (&font_steel, 64, 7, "DEMOLITION");
	font_render_string_center (&font_steel, 64, 22, "TIME");
	dmd_show_low ();
	task_sleep(TIME_4S);

	if (alternator % numOfAlts == 0) {
		dmd_sched_transition (&trans_scroll_right);
		show_john_spartan_animation ();

	} else if (alternator % numOfAlts == 1) {
		dmd_sched_transition (&trans_scroll_right);
		show_assoc_bob_animation();

	} else if (alternator % numOfAlts == 2) {
		dmd_sched_transition (&trans_scroll_left);
		show_john_spartan2_animation ();

	} else if (alternator % numOfAlts == 3) {
		dmd_sched_transition (&trans_scroll_left);
		show_lenina1_animation();
	}

	dmd_sched_transition (&trans_bitfade_slow);
	dmd_clean_page_low ();
	font_render_string_center (&font_term6, 64, 7, "GAME RULES BY");
	font_render_string_center (&font_lithograph, 64, 20, "CARDONA");
	dmd_show_low ();
	task_sleep(TIME_2S);

	dmd_sched_transition (&trans_scroll_up_slow);
	dmd_clean_page_low ();
	font_render_string_center (&font_lithograph, 64, 7, "DOTS BY");
	font_render_string_center (&font_term6, 64, 20, "SJI");
	dmd_show_low ();
	task_sleep(TIME_2S);

	if (alternator % numOfAlts == 0) {
		dmd_sched_transition (&trans_scroll_right);
		show_simon_phoenix_animation ();

	} else if (alternator % numOfAlts == 1) {
		dmd_sched_transition (&trans_scroll_left);
		show_cocteau_animation();

	} else if (alternator % numOfAlts == 2) {
	dmd_sched_transition (&trans_scroll_right);
	show_simon_phoenix2_animation ();

	} else if (alternator % numOfAlts == 3) {
		dmd_sched_transition (&trans_scroll_left);
		show_chief_earle_animation();
		}

	dmd_sched_transition (&trans_bitfade_slow);
	dmd_clean_page_low ();
	font_render_string_center (&font_steel, 64, 7, "DEMOLITION");
	font_render_string_center (&font_steel, 64, 22, "TIME");
	dmd_show_low ();
	task_sleep(TIME_4S);

	if (alternator % numOfAlts == 0) {
		dmd_sched_transition (&trans_scroll_right);
		show_huxley_info ();

	} else if (alternator % numOfAlts == 1) {
		dmd_sched_transition (&trans_scroll_left);
		show_rollovers1_info ();

	} else if (alternator % numOfAlts == 2) {
		dmd_sched_transition (&trans_scroll_right);
		show_standups_info ();

	} else if (alternator % numOfAlts == 3) {
		dmd_sched_transition (&trans_scroll_left);
		show_demotime_info ();
		}

}//end of function

