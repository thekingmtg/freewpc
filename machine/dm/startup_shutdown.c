/*
 * demolition man
 * startup_shutdown.c
 *
 * written by James Cardona
 *
 */
/* CALLSET_SECTION (startup_shutdown, __machine5__) */

#include <freewpc.h>
#include "dm/global_constants.h"
//include "startup_shutdown.h"


//local variables
__boolean in_ending;


//external variables

//prototypes



/****************************************************************************
 * body
 ***************************************************************************/
void startup_effect_deff (void){
	U16 fno;
	dmd_alloc_pair_clean ();
	sound_start (ST_SPEECH, SPCH_DEMOLITION_MAN, SL_4S, PRI_GAME_QUICK5);

	for (fno = IMG_JAIL_A_START; fno <= IMG_JAIL_A_END; fno += 2) {
		dmd_alloc_pair ();
		frame_draw(fno);
		dmd_show2 ();
		task_sleep (TIME_100MS);
	}//end of loop
	deff_exit ();
}//end of function




CALLSET_ENTRY (startup_shutdown, start_player) 	{
	in_ending = FALSE;
//	task_sleep (TIME_500MS);
	deff_start (DEFF_STARTUP_EFFECT);
}//end of function




//default music handler --see sound_effect.c
CALLSET_ENTRY (startup_shutdown, music_refresh) {

	//note that there is a music disable call in /kernel/game.c:255 that is commented out
	if (in_bonus)										music_request (MUS_NONE, PRI_BONUS);
	else if (in_ending)									music_request (MUS_END_GAME, PRI_GAME_OVER);
	else if (!in_live_game)								music_request (0, PRI_GAME_OVER);

	//if this far then we are in a live game, but not in bonus
	else if (valid_playfield)							music_request (MUS_BG , PRI_SCORES);
	else												music_request (MUS_PLUNGER, PRI_EGG4);
}//end of function



//This is made into a task so that it will run concurrently
//with the enter initials screen
void dm_end_game_task (void) {
	task_sleep_sec (20);
	callset_invoke(fade_out);
	task_sleep_sec(25);
	in_ending = FALSE;
	task_exit();
}//end of function




// LET END GAME MUSIC PLAY WHILE INITIALS ARE BEING ENTERED
CALLSET_ENTRY (startup_shutdown, end_game) {
	in_ending = TRUE;
	task_create_gid1 (GID_END_GAME_DM, dm_end_game_task);
}//end of function




//this is invoked once initials are entered
//or after task time out (above) - which ever sooner
CALLSET_ENTRY (startup_shutdown, fade_out) {
	 U8 i;
	 U8 volume_restore_point;
	 volume_restore_point = get_volume();	 //record current volume level :: at kernal/sound.c

	// N-second fadeout
	//FIXME: this actually lowers volume permanently
	//so if game is turned off during this time, then volume will be permanently lowered
	 for (i = 0; i < volume_restore_point - 1; i++) { //go down to volume level 2
		 lower_volume (); //lower volume by 1 tick :: at sound.c
		 task_sleep (TIME_2S);
	 }//end of loop

	in_ending = FALSE; 					//turn off all music
	task_sleep_sec (2); 				//wait for all tasks to catch up
	volume_set (volume_restore_point); //restore volume level for next game
}//end of function



//U8 once_through = 0;
//this is to ensure all flashers/sols/flippers are off after a crash or bad boot
//ALLSET_ENTRY (startup_shutdown, amode_start) {
//	if (once_through == 0) {
//		kill_all_sols();
//		sol_init();
//		flash_all_flashers();
//	}
//	once_through++;
//}//end of function


//ensure no balls left on shooter lane
CALLSET_ENTRY (startup_shutdown, amode_start) {
	if (!in_test) {
		if (switch_poll_logical (MACHINE_SHOOTER_SWITCH)) launch_ball();
	}
}//end of function

