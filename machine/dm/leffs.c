/* CALLSET_SECTION (leffs, __machine3__) */

#include <freewpc.h>


//cycle through one GI string at a time
void gi_cycle_leff (void) {
//	U8 i, j;
//	gi_leff_enable (PINIO_GI_STRINGS);
/*
	for (j = 0; j < 3; j++) {
		for (i = 0; i < 3; i++) {
				gi_leff_disable (TRIAC_GI_STRING (i) );
				task_sleep (TIME_100MS);
				gi_leff_enable (TRIAC_GI_STRING (i) );
				task_sleep (TIME_100MS);
		}//end of loop
	}//end of loop
*/
		gi_leff_disable (TRIAC_GI_STRING (0) );
		sound_start (ST_ANY, SPCH_PLAYER_ONE, SL_1S, PRI_GAME_QUICK5);
		task_sleep (TIME_500MS);
		task_sleep (TIME_500MS);
		gi_leff_enable (TRIAC_GI_STRING (0) );
		task_sleep (TIME_500MS);
		task_sleep (TIME_500MS);

		gi_leff_disable (TRIAC_GI_STRING (1) );
		sound_start (ST_ANY, SPCH_PLAYER_ONE, SL_1S, PRI_GAME_QUICK5);
		task_sleep (TIME_500MS);
		task_sleep (TIME_500MS);
		gi_leff_enable (TRIAC_GI_STRING (1) );
		task_sleep (TIME_500MS);
		task_sleep (TIME_500MS);

		gi_leff_disable (TRIAC_GI_STRING (2) );
		sound_start (ST_ANY, SPCH_PLAYER_TWO, SL_1S, PRI_GAME_QUICK5);
		task_sleep (TIME_500MS);
		task_sleep (TIME_500MS);
		gi_leff_enable (TRIAC_GI_STRING (2) );
		task_sleep (TIME_500MS);
		task_sleep (TIME_500MS);

		gi_leff_disable (TRIAC_GI_STRING (3) );
		sound_start (ST_ANY, SPCH_PLAYER_THREE, SL_1S, PRI_GAME_QUICK5);
		task_sleep (TIME_500MS);
		task_sleep (TIME_500MS);
		gi_leff_enable (TRIAC_GI_STRING (3) );
		task_sleep (TIME_500MS);
		task_sleep (TIME_500MS);

		gi_leff_disable (TRIAC_GI_STRING (4) );
		sound_start (ST_ANY, SPCH_PLAYER_FOUR, SL_1S, PRI_GAME_QUICK5);
		task_sleep (TIME_500MS);
		task_sleep (TIME_500MS);
		gi_leff_enable (TRIAC_GI_STRING (4) );
		task_sleep (TIME_500MS);
		task_sleep (TIME_500MS);

//		gi_leff_enable (PINIO_GI_STRINGS);
}// end of function



// turn off GIs for a time
void no_gi_onesec_leff (void) {
	gi_leff_disable (PINIO_GI_STRINGS);
	task_sleep_sec (1);
	gi_leff_enable (PINIO_GI_STRINGS);
	task_sleep_sec (1);
	leff_exit ();
}// end of function




// turn off GIs for a time
void no_gi_twosec_leff (void) {
	gi_leff_disable (PINIO_GI_STRINGS);
	task_sleep_sec (2);
	gi_leff_enable (PINIO_GI_STRINGS);
	task_sleep_sec (1);
	leff_exit ();
}// end of function



void turn_off_gi_leff (void) {
	gi_disable (PINIO_GI_STRINGS);
	task_sleep_sec (1);
	leff_exit ();
}// end of function



void turn_on_gi_leff (void) {
	gi_enable (PINIO_GI_STRINGS);
	task_sleep_sec (1);
	leff_exit ();
}// end of function




void flash_all_leff (void) {
	U8 i;
	lamplist_set_apply_delay (0);
	gi_leff_enable (PINIO_GI_STRINGS);
	lamplist_apply_leff_alternating (LAMPLIST_AMODE_ALL, 0); //turn all lamps off
	for (i=0; i < 28; i++) {
		lamplist_apply (LAMPLIST_AMODE_ALL, leff_toggle); //toggle
		task_sleep (TIME_66MS);
	}//end of loop
	lamplist_apply (LAMPLIST_AMODE_ALL, leff_on); //
	task_sleep_sec (1);
	leff_exit ();
}// end of function




void bot2top_leff (void) {
	lamplist_set_apply_delay (TIME_16MS);
//	gi_leff_enable (PINIO_GI_STRINGS);
//	lamplist_apply_leff_alternating (LAMPLIST_AMODE_ALL, 0); //turn all lamps off

	lamplist_apply (LAMPLIST_BOTTOM_TOP, leff_toggle); //toggle
	task_sleep (TIME_500MS);

	lamplist_apply (LAMPLIST_TOP_BOTTOM, leff_toggle); //toggle
	task_sleep (TIME_500MS);

//	lamplist_apply (LAMPLIST_AMODE_ALL, leff_on); //
	task_sleep_sec (1);
	leff_exit ();
}// end of function




void top2bot_leff (void) {
	lamplist_set_apply_delay (TIME_16MS);
//	gi_leff_enable (PINIO_GI_STRINGS);
//	lamplist_apply_leff_alternating (LAMPLIST_AMODE_ALL, 0); //turn all lamps off

	lamplist_apply (LAMPLIST_TOP_BOTTOM, leff_toggle); //toggle
	task_sleep (TIME_500MS);

	lamplist_apply (LAMPLIST_BOTTOM_TOP, leff_toggle); //toggle
	task_sleep (TIME_500MS);

//	lamplist_apply (LAMPLIST_AMODE_ALL, leff_on); //
	task_sleep_sec (1);
	leff_exit ();
}// end of function




void left2right_leff (void) {
	lamplist_set_apply_delay (TIME_16MS);
//	gi_leff_enable (PINIO_GI_STRINGS);
//	lamplist_apply_leff_alternating (LAMPLIST_AMODE_ALL, 0); //turn all lamps off

	lamplist_apply (LAMPLIST_LEFT_RIGHT, leff_toggle); //toggle
	task_sleep (TIME_500MS);

	lamplist_apply (LAMPLIST_RIGHT_LEFT, leff_toggle); //toggle
	task_sleep (TIME_500MS);

//	lamplist_apply (LAMPLIST_AMODE_ALL, leff_on); //
	task_sleep_sec (1);
	leff_exit ();
}// end of function




void right2left_leff (void) {
	lamplist_set_apply_delay (TIME_16MS);
//	gi_leff_enable (PINIO_GI_STRINGS);
//	lamplist_apply_leff_alternating (LAMPLIST_AMODE_ALL, 0); //turn all lamps off

	lamplist_apply (LAMPLIST_RIGHT_LEFT, leff_toggle); //toggle
	task_sleep (TIME_500MS);

	lamplist_apply (LAMPLIST_LEFT_RIGHT, leff_toggle); //toggle
	task_sleep (TIME_500MS);

//	lamplist_apply (LAMPLIST_AMODE_ALL, leff_on); //
	task_sleep_sec (1);
	leff_exit ();
}// end of function



void flash_gi_leff (void) {
	U8 i;
	for (i = 1; i < 7; i++) {
		gi_leff_disable (PINIO_GI_STRINGS);
		task_sleep (TIME_100MS);
		gi_leff_enable (PINIO_GI_STRINGS);
		task_sleep (TIME_100MS);
	}//end of loop
	gi_leff_enable (PINIO_GI_STRINGS);
	leff_exit ();
}// end of function
