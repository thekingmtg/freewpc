/*
 * demolition man
 * cryoclaw.c
 *
 * written by James Cardona
 *
 * Location Description:
 *
 *
 */

#include <freewpc.h>
#include "dm/global_constants.h"
#include "elevator.h"
#include "claw.h"
#include "clawmagnet.h"
#include "search.h"

//local variables
U8			cryoclaw_mode_timer;

//internally called function prototypes  --external found at protos.h
void cryoclaw_mode_init (void);
void cryoclaw_mode_expire (void);
void cryoclaw_mode_exit (void);
void cryoclaw_start_effect_deff(void);
void cryoclaw_effect_deff(void);
void cryoclaw_end_effect_deff(void);

/****************************************************************************
 * mode definition structure
 ***************************************************************************/
struct timed_mode_ops cryoclaw_mode = {
	DEFAULT_MODE,
	.init = cryoclaw_mode_init,
	.exit = cryoclaw_mode_exit,
	.gid = GID_CRYOCLAW_MODE_RUNNING,
	.music = MUS_CLAW,
//	.deff_starting = DEFF_CRYOCLAW_START_EFFECT,
	.deff_running = DEFF_CRYOCLAW_EFFECT,
//	.deff_ending = DEFF_CRYOCLAW_END_EFFECT,
	.prio = PRI_GAME_MODE1,
	.init_timer = 14,
	.timer = &cryoclaw_mode_timer,
	.grace_timer = 0, //default is 2
//	.pause = system_timer_pause,
};


/****************************************************************************
 * initialize  and exit
 ***************************************************************************/
CALLSET_ENTRY (cryoclaw, start_ball) {
	flag_off(FLAG_IS_BALL_ON_CLAW);
	flag_off (FLAG_IS_CRYOCLAW_ACTIVATED);
}//end of function


void cryoclaw_mode_init (void) {
		ball_search_monitor_stop ();
		flag_on (FLAG_IS_CRYOCLAW_ACTIVATED);
		sound_start (ST_SPEECH, SPCH_USE_TRIGGERS_TO_MOVE_CRYOCLAW, SL_4S, PRI_GAME_QUICK5);
		flipper_disable ();							//approximately 4 secs, the player cannot control anything
		task_sleep(TIME_2S);
		clawmagnet_on ();
		elevator_move_up();							//move up
		task_sleep(TIME_100MS);						//then back down back to home
		elevator_move_down();
		task_sleep(TIME_500MS);
		claw_go_left_to_center();
		task_sleep(TIME_1S);
		sound_start (ST_SPEECH, SPCH_USE_BUTTONS_TO_RELEASE_BALL, SL_4S, PRI_GAME_QUICK5);
		flag_on(FLAG_IS_BALL_ON_CLAW);							//this enables control of claw with flippers
		rramp_clawready_off();		//turn off diverter
}//end of fufnction


void cryoclaw_mode_expire (void) {
	flag_off (FLAG_IS_CRYOCLAW_ACTIVATED);
	flipper_enable ();
	clawmagnet_off ();
	flag_off(FLAG_IS_BALL_ON_CLAW);
	task_sleep(TIME_1S);
	claw_go_right();		//move claw back to home position
	ball_search_monitor_start ();
}//end of function

void cryoclaw_mode_exit (void) { cryoclaw_mode_expire();}

/****************************************************************************
 * external event listeners
 ****************************************************************************/
CALLSET_ENTRY (cryoclaw, music_refresh)  	{ timed_mode_music_refresh (&cryoclaw_mode); }
CALLSET_ENTRY (cryoclaw, end_ball) 			{ if (timed_mode_running_p(&cryoclaw_mode) ) timed_mode_end (&cryoclaw_mode); }
CALLSET_ENTRY (cryoclaw, display_update) 	{ timed_mode_display_update (&cryoclaw_mode); }


/****************************************************************************
 * body
 ****************************************************************************/
//if ball lands on top of elevator then switch will be made
CALLSET_ENTRY (cryoclaw, sw_elevator_hold) {
	if (valid_playfield
		&& !timed_mode_running_p(&cryoclaw_mode) )
					timed_mode_begin (&cryoclaw_mode);//start mode
}//end of function



//release ball
CALLSET_ENTRY (cryoclaw, sw_left_handle_button, sw_launch_button) {
	if (flag_test(FLAG_IS_BALL_ON_CLAW)) clawmagnet_off ();
}//end of function

//end mode
CALLSET_ENTRY (cryoclaw, end_claw_mode) {
	 if (timed_mode_running_p(&cryoclaw_mode) ) timed_mode_end (&cryoclaw_mode);
}//end of function

//move claw left
CALLSET_ENTRY (cryoclaw, sw_left_button, sw_upper_left_button) {
	if (flag_test(FLAG_IS_BALL_ON_CLAW) ) claw_bump_left ();
}//end of function

//move claw right
CALLSET_ENTRY (cryoclaw, sw_right_button, sw_upper_right_button) {
	if (flag_test(FLAG_IS_BALL_ON_CLAW) ) claw_bump_right ();
}//end of function



//ensure claw is all the way to the right
CALLSET_ENTRY (cryoclaw, amode_start) {
	flag_off(FLAG_IS_BALL_ON_CLAW);
	clawmagnet_off ();
	task_sleep(TIME_1S);
	claw_go_left();
	task_sleep(TIME_2S);
	claw_go_right();
}//end of function


CALLSET_ENTRY (cryoclaw, player_start) {
	flag_off(FLAG_IS_BALL_ON_CLAW);
	clawmagnet_off ();
}//end of function


/****************************************************************************
 * DMD display and sound effects
 ****************************************************************************/
void cryoclaw_effect_deff(void) {
	for (;;) {
		dmd_alloc_low_clean ();
		sprintf ("%d                          %d", cryoclaw_mode_timer, cryoclaw_mode_timer);
		font_render_string_center (&font_var5, DMD_MIDDLE_X, DMD_SMALL_CY_1, sprintf_buffer);

		font_render_string_center (&font_steel, DMD_MIDDLE_X, DMD_BIG_CY_Top, "CRYO CLAW");

		font_render_string_center (&font_var5, DMD_MIDDLE_X, DMD_SMALL_CY_4, sprintf_buffer);
		dmd_show_low ();
		task_sleep (TIME_500MS);
	}//END OF ENDLESS LOOP
}//end of mode_effect_deff

