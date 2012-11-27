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


/****************************************************************************
 *
 * trouble shooting
 *
 ****************************************************************************/
CALLSET_ENTRY (custom_deffs, sw_left_handle_button) {
//	deff_start (DEFF_TROUBLESHOOTING);
}
/*

void troubleshooting_deff(void) {
	dmd_alloc_low_clean ();
	if (in_game) sprintf ("IG"); 			font_render_string_center (&font_var5, 8, 5, sprintf_buffer);
	if (in_live_game) sprintf ("ILG"); 		font_render_string_center (&font_var5, 25, 5, sprintf_buffer);
	#if defined(MACHINE_LAUNCH_SWITCH)
		sprintf ("MLSW");
	#endif
												font_render_string_center (&font_var5, 50, 5, sprintf_buffer);
	#if defined(MACHINE_LAUNCH_SOLENOID)
		sprintf ("SOL");
	#endif
												font_render_string_center (&font_var5, 80, 5, sprintf_buffer);
	#if 	defined(MACHINE_SHOOTER_SWITCH)
		sprintf ("SHT");
	#endif
												font_render_string_center (&font_var5, 110, 5, sprintf_buffer);
	#if defined(INCLUDE_AUTOPLUNGER)
		sprintf ("PL");
	#endif
												font_render_string_center (&font_var5, 8, 11, sprintf_buffer);
	#if defined(HAVE_AUTO_SERVE)
		sprintf ("SRV");
	#endif
											font_render_string_center (&font_var5, 25 , 11, sprintf_buffer);
if (global_flag_test (GLOBAL_FLAG_BALL_AT_PLUNGER)) sprintf ("BALL");
											font_render_string_center (&font_var5, 50 , 11, sprintf_buffer);
	dmd_show_low ();
	task_sleep_sec (4);
	deff_exit ();
}//end of deff

*/
