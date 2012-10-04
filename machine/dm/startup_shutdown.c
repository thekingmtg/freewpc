/*
 * demolition man
 * sound_handler.c
 *
 * written by James Cardona
 *
 */
#include <freewpc.h>

CALLSET_ENTRY (startup_shutdown, start_player) 	{ sound_start (ST_SPEECH, SPCH_DEMOLITION_MAN, SL_4S, PRI_GAME_QUICK5);}

//ALLSET_ENTRY (startup_shutdown, start_ball) 	{ enable_flippers(); }

//ALLSET_ENTRY (startup_shutdown, end_ball) 		{ disable_flippers(); }
