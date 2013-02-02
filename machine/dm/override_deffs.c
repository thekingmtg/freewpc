/*
 * demolition man
 * override_deffs.c
 *
 * written by James Cardona
 *
 * */
/* CALLSET_SECTION (override_deffs, __machine2__) */


#include <freewpc.h>
#include "dm/global_constants.h"
#include <replay.h>

extern __local__ U8 jet_shots_made;//external ref to eject.c



void shoot_again_leff(void) {
	leff_start(LEFF_BOT2TOP);
	task_sleep_sec (TIME_1S);
	leff_exit();
}//end of function




void shoot_again_deff (void) {
	U8 sa_rnd_sound;
	U16 fno;
	dmd_alloc_pair_clean ();// Clean both pages
	sa_rnd_sound = random_scaled(2);
	sound_start (ST_EFFECT, EXTRA_BALL_SOUND, SL_2S, SP_NORMAL);

	for (fno = IMG_FREEZER1_START; fno <= IMG_FREEZER1_END; fno += 2) {
				dmd_alloc_pair ();
				frame_draw(fno);
				dmd_show2 ();
				task_sleep (TIME_66MS);
	}//end of for loop

	for (fno = IMG_FREEZER2_START; fno <= IMG_FREEZER2_END; fno += 2) {
				dmd_alloc_pair ();
				frame_draw(fno);
				dmd_show2 ();
				task_sleep (TIME_66MS);
	}//end of for loop

	if (sa_rnd_sound) 	sound_start (ST_SPEECH, SPCH_SHOOT_AGAIN, SL_2S, SP_NORMAL);
	else 				sound_start (ST_SPEECH, SPCH_SHOOT_AGAIN_SLY, SL_2S, SP_NORMAL);

	for (fno = IMG_FREEZER3_START; fno <= IMG_FREEZER3_END; fno += 2) {
					dmd_map_overlay ();
					dmd_clean_page_low ();
					font_render_string_center (&font_fireball, DMD_MIDDLE_X - 25, DMD_BIG_CY_Top, "SHOOT");
					font_render_string_center (&font_fireball, DMD_MIDDLE_X - 25, DMD_BIG_CY_Bot, "AGAIN");
						dmd_text_outline ();
						dmd_alloc_pair ();
						frame_draw(fno);
						dmd_overlay_outline ();
						dmd_show2 ();
						task_sleep (TIME_66MS);
			}//end of for loop

	deff_exit ();
}//end of function




CALLSET_ENTRY (override_deffs, tilt) {
	sound_send (TILT);
	task_sleep_sec (2);
	sound_send (SPCH_PLAY_ACCORDION_TOO);
}//end of function



CALLSET_ENTRY (override_deffs, tilt_warning) {
	sound_send (TILT);
}//end of function






/****************************************************************************
 *
 * trouble shooting
 *
 ****************************************************************************/
CALLSET_ENTRY (override_deffs, sw_left_handle_button) {
	if (IN_TEST) {
	deff_start (DEFF_TROUBLESHOOTING);
	}
}//end of function




void troubleshooting_deff(void) {
	dmd_alloc_low_clean ();

/*	sprintf ("%d", jet_shots_made);
	font_render_string_left (&font_var5, 70, 1, sprintf_buffer);
	sprintf ("JETS");
	font_render_string_right (&font_var5, 64, 1, sprintf_buffer);

	sprintf_score (REPLAY_SCORE_OFFSET);
	font_render_string_left (&font_var5, 70, 8, sprintf_buffer);
	sprintf ("REPLAY SCORE OFFSET");
	font_render_string_right (&font_var5, 64, 8, sprintf_buffer);

	sprintf_score (current_score);
	font_render_string_left (&font_var5, 70, 16, sprintf_buffer);
	sprintf ("CURRENT SCORE");
	font_render_string_right (&font_var5, 64, 16, sprintf_buffer);

	sprintf_score (current_score + REPLAY_SCORE_OFFSET);
	font_render_string_left (&font_var5, 70, 23, sprintf_buffer);
	sprintf ("CURR *");
	font_render_string_right (&font_var5, 64, 23, sprintf_buffer);
*/

	//curr = (replay_score_t *)(current_score + REPLAY_SCORE_OFFSET);


/*trough and ball checking*/
/*
	sprintf ("LIVE BALLS   %d", live_balls);
	font_render_string_center (&font_var5, 64, 5, sprintf_buffer);

	sprintf ("TROUGH COUNT   %d", 	device_recount(device_entry (DEVNO_TROUGH)) );
	font_render_string_center (&font_var5, 64, 10, sprintf_buffer);

	if (switch_poll_logical(SW_TROUGH_1) ) font_render_string_center (&font_var5,  5, 19, "1");
	if (switch_poll_logical(SW_TROUGH_2) ) font_render_string_center (&font_var5, 10, 19, "2");
	if (switch_poll_logical(SW_TROUGH_3) ) font_render_string_center (&font_var5, 15, 19, "3");
	if (switch_poll_logical(SW_TROUGH_4) ) font_render_string_center (&font_var5, 20, 19, "4");
	if (switch_poll_logical(SW_TROUGH_5) ) font_render_string_center (&font_var5, 25, 19, "5");
 */




/*	if (in_live_game) sprintf ("ILG");
 	 font_render_string_center (&font_var5, 25, 5, sprintf_buffer);

	#if defined(HAVE_AUTO_SERVE) sprintf ("SRV");
	font_render_string_center (&font_var5, 25 , 11, sprintf_buffer);
	#endif

if (global_flag_test (GLOBAL_FLAG_BALL_AT_PLUNGER)) sprintf ("BALL");
	font_render_string_center (&font_var5, 50 , 11, sprintf_buffer);
*/

	dmd_show_low ();
	task_sleep_sec (1);
	deff_exit ();
}//end of deff


