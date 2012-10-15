/*
 * demolition man
 * sound_handler.c
 *
 * written by James Cardona
 *
 */
#include <freewpc.h>
#include "dm/global_constants.h"

__boolean in_ending;

CALLSET_ENTRY (startup_shutdown, start_player) 	{
	in_ending = FALSE;
	sound_start (ST_SPEECH, SPCH_DEMOLITION_MAN, SL_4S, PRI_GAME_QUICK5);
}//end of function



//default music handler --dupe from sound_effect.c
CALLSET_ENTRY (startup_shutdown, music_refresh) {

	//note that there is a music disable call in game.c:255 that is commented out
	if (in_bonus)										music_request (MUS_NONE, PRI_BONUS);
	if (in_ending)										music_request (MUS_END_GAME, PRI_GAME_OVER);
	else if (!in_live_game)								return;

	if (config_timed_game && timed_game_timer == 0)		music_request (0, PRI_GAME_OVER);
	else if (valid_playfield)							music_request (MUS_BG , PRI_SCORES);
	else												music_request (MUS_PLUNGER, PRI_EGG4);
}//end of function


//TODO: fade out volume level
CALLSET_ENTRY (startup_shutdown, end_game) {
	in_ending = TRUE;
	//	music_set (MUS_END_GAME);
	task_sleep_sec (5); // not needed?
	task_sleep_sec (2);
	/*
	 U8 i;
	 //record current volume level
	 for (i = 0; i < 100; i++) {
	 //lower volume
	 task_sleep (TIME_100MS);
	 }//end of loop

	 */
	in_ending = FALSE;
	task_sleep_sec (2);//wait for all tasks to catch up
	 //restore volume level

}//end of function




