/*
 * demolition man
 * inlanes.c
 * 
 * Location Description:
 *
 * Scoring Description: (original game)
 *
 * Scoring Description: (my rules)
 *
 * */

#include <freewpc.h>

//local variables
__boolean 	left_inlane_Access_Claw_activated;
__boolean 	right_inlane_Light_Quick_Freeze_activated;

//prototypes


/****************************************************************************
 * initialize  and exit
 ***************************************************************************/
void inlanes_reset (void) {
	left_inlane_Access_Claw_activated = FALSE;
	lamp_tristate_off(LM_ACCESS_CLAW);
	right_inlane_Light_Quick_Freeze_activated = FALSE;
	lamp_tristate_off(LM_LIGHT_QUICK_FREEZE);
}//end of reset

CALLSET_ENTRY (inlanes, start_player) {  inlanes_reset(); }
CALLSET_ENTRY (inlanes, start_ball) { inlanes_reset(); }


/****************************************************************************
 * left inlane
 ***************************************************************************/
CALLSET_ENTRY (inlanes, Access_Claw_Light_On) {
	left_inlane_Access_Claw_activated = TRUE;
	lamp_tristate_on (LM_ACCESS_CLAW);
	}

CALLSET_ENTRY (inlanes, Access_Claw_Light_Off) {
	left_inlane_Access_Claw_activated = FALSE;
	lamp_tristate_off (LM_ACCESS_CLAW);
	}

CALLSET_ENTRY (inlanes, sw_left_inlane) {
	left_inlane_Access_Claw_activated = FALSE;
	lamp_tristate_off (LM_ACCESS_CLAW);
	sound_start (ST_SAMPLE, EXPLOSION, SL_1S, PRI_GAME_QUICK5);
	//move the right ramp diverter here
	}


/****************************************************************************
 * right inlane
 ***************************************************************************/
CALLSET_ENTRY (inlanes, Light_Quick_Freeze_Light_On) {
	right_inlane_Light_Quick_Freeze_activated = TRUE;
	lamp_tristate_on (LM_LIGHT_QUICK_FREEZE);
	}

CALLSET_ENTRY (inlanes, Light_Quick_Freeze_Light_Off) {
	right_inlane_Light_Quick_Freeze_activated = FALSE;
	lamp_tristate_off (LM_LIGHT_QUICK_FREEZE);
	}

CALLSET_ENTRY (inlanes, sw_right_inlane) {
	right_inlane_Light_Quick_Freeze_activated = FALSE;
	lamp_tristate_off (LM_LIGHT_QUICK_FREEZE);
	sound_start (ST_SAMPLE, EXPLOSION, SL_1S, PRI_GAME_QUICK5);
	//add another freeze or enable MB here
	}

