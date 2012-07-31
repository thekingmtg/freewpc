/*
 * demolition man
 * rollovers.c
 *
 * Location Description:
 * MTL rollovers: In the top right corner are three rollovers which advance
 * the bonus multiplier. Balls leave the rollovers into the jet bumpers.
 * These are very hard to see from the player's point of view.
 * Especially the leftmost (M) rollover.
 *
 * MTL rollovers
 * are actually left, center, right but
 * are labeled middle, top, lower where
 * left is called middle
 * center is called top
 * right is called lower
 *
 * Scoring Description: (original game)
 * MTL Rollovers light an increasing bonus (2x, 3x, 4x, 5x) multiplier. The bonus
 * multiplier is not scored until the end of the ball.
 * Getting the 5x bonus multiplier also lights the extra ball on the left loop (orbit).
 * Each completed MTL rollover sequence lights Access Claw at the left inlane.
 * Hitting an unlit 250k, lit one 100k
 * all 3 500k bonus
 *
 * Scoring Description: (my rules)
 * same as original
 *
 */

#include <freewpc.h>
//constants
U8 					max_rollover_bonus_multiplier = 5;

//local variables
__boolean 			middle_rollover_activated;
__boolean 			top_rollover_activated;
__boolean 			lower_rollover_activated;
U8 					rollover_bonus_multiplier; // 0 to 5

//prototypes
void rollover_reset (void);
void all_rollover_made (void);

/****************************************************************************
 * initialize  and exit
 ***************************************************************************/
void rollover_reset (void) {
	middle_rollover_activated = TRUE;
	top_rollover_activated = FALSE;
	lower_rollover_activated = FALSE;
	rollover_bonus_multiplier = 0;

	lamp_tristate_on(LM_MIDDLE_ROLLOVER);
	lamp_tristate_off(LM_TOP_ROLLOVER);
	lamp_tristate_off(LM_LOWER_ROLLOVER);
}//end of rollover_reset

CALLSET_ENTRY (rollovers, start_player) {  rollover_reset(); }
CALLSET_ENTRY (rollovers, start_ball) { rollover_reset(); }


/****************************************************************************
 * body
 ***************************************************************************/
void all_rollover_made (void){
	lamp_tristate_off(LM_MIDDLE_ROLLOVER);
	lamp_tristate_off(LM_TOP_ROLLOVER);
	lamp_tristate_off(LM_LOWER_ROLLOVER);
	middle_rollover_activated = FALSE;
	top_rollover_activated = FALSE;
	lower_rollover_activated = FALSE;
	score (SC_500K);
	//light access claw
	callset_invoke(Access_Claw_Light_On);
	if (rollover_bonus_multiplier < max_rollover_bonus_multiplier) ++rollover_bonus_multiplier;
	else if (rollover_bonus_multiplier == max_rollover_bonus_multiplier) callset_invoke(ExtraBall_Light_On);

	//TODO: DISPLAY EFFECTS HERE FOR ADVANCING MULTIPLIER
	}//end of function


CALLSET_ENTRY (rollovers, sw_middle_rollover) {
	//if already lit
	if (middle_rollover_activated) {
		sound_start (ST_SAMPLE, SPCH_WOAH, SL_1S, PRI_GAME_QUICK5);
		score (SC_100K);
		}
	else { //else - not already lit, so activate rollover
		lamp_tristate_on(LM_MIDDLE_ROLLOVER);
		middle_rollover_activated = TRUE;
		sound_start (ST_SAMPLE, SPCH_WOAH, SL_1S, PRI_GAME_QUICK5);
		score (SC_250K);

		//check to see if this is the third rollover to activate
		if (top_rollover_activated && lower_rollover_activated) all_rollover_made();
		}//end of else - not already lit, so activate rollover
	}//end of function rollovers_sw_middle_rollover


CALLSET_ENTRY (rollovers, sw_top_rollover) {
	if (top_rollover_activated) {
		sound_start (ST_SAMPLE, SPCH_WOAH, SL_1S, PRI_GAME_QUICK5);
		score (SC_100K);
		}
	else { //activate rollover
		lamp_tristate_on(LM_TOP_ROLLOVER);
		top_rollover_activated = TRUE;
		sound_start (ST_SAMPLE, SPCH_WOAH, SL_1S, PRI_GAME_QUICK5);
		score (SC_250K);

		//check to see if this is the third rollover to activate
		if (middle_rollover_activated && lower_rollover_activated)  all_rollover_made();
		}//end of else - not already lit, so activate rollover
	}//end of function rollovers_sw_top_rollover


CALLSET_ENTRY (rollovers, sw_lower_rollover) {
	if (lower_rollover_activated) {
		sound_start (ST_SAMPLE, SPCH_WOAH, SL_1S, PRI_GAME_QUICK5);
		score (SC_100K);
		}
	else { //activate rollover
		lamp_tristate_on(LM_LOWER_ROLLOVER);
		top_rollover_activated = TRUE;
		sound_start (ST_SAMPLE, SPCH_WOAH, SL_1S, PRI_GAME_QUICK5);
		score (SC_250K);

		//check to see if this is the third rollover to activate
		if (middle_rollover_activated && top_rollover_activated)  all_rollover_made();
		}//end of else - not already lit, so activate rollover
	}//end of function rollovers_sw_lower_rollover


	/****************************************************************************
	 * rotate rollovers when buttons pressed
	 ***************************************************************************/

CALLSET_ENTRY (rollovers, sw_left_button) {
	if (top_rollover_activated && lower_rollover_activated) { //left not activated
		lamp_tristate_on(LM_MIDDLE_ROLLOVER);
		lamp_tristate_off(LM_LOWER_ROLLOVER);
		middle_rollover_activated = TRUE;
		lower_rollover_activated = FALSE;
		}
	else
	if (middle_rollover_activated && lower_rollover_activated) { //center not activated
		lamp_tristate_off(LM_MIDDLE_ROLLOVER);
		lamp_tristate_on(LM_TOP_ROLLOVER);
		middle_rollover_activated = FALSE;
		top_rollover_activated = TRUE;
		}
	else
	if (middle_rollover_activated && top_rollover_activated) { //right not activated
		lamp_tristate_off(LM_TOP_ROLLOVER);
		lamp_tristate_on(LM_LOWER_ROLLOVER);
		top_rollover_activated = FALSE;
		lower_rollover_activated = TRUE;
		}
	else
	if (middle_rollover_activated) { //center and right not activated
		lamp_tristate_off(LM_MIDDLE_ROLLOVER);
		lamp_tristate_on(LM_LOWER_ROLLOVER);
		middle_rollover_activated = FALSE;
		lower_rollover_activated = TRUE;
		}
	else
	if (top_rollover_activated) { //left and right not activated
		lamp_tristate_on(LM_MIDDLE_ROLLOVER);
		lamp_tristate_off(LM_TOP_ROLLOVER);
		middle_rollover_activated = TRUE;
		top_rollover_activated = FALSE;
		}
	else
	if (lower_rollover_activated) { //left and center not activated
		lamp_tristate_on(LM_TOP_ROLLOVER);
		lamp_tristate_off(LM_LOWER_ROLLOVER);
		top_rollover_activated = TRUE;
		lower_rollover_activated = FALSE;
		}
}//end of function to rotate left

//rotate rollovers when buttons pressed
CALLSET_ENTRY (rollovers, sw_right_button) {
	if (top_rollover_activated && lower_rollover_activated) { //left not activated
		lamp_tristate_on(LM_MIDDLE_ROLLOVER);
		lamp_tristate_off(LM_TOP_ROLLOVER);
		middle_rollover_activated = TRUE;
		top_rollover_activated = FALSE;
		}
	else
	if (middle_rollover_activated && lower_rollover_activated) { //center not activated
		lamp_tristate_on(LM_TOP_ROLLOVER);
		lamp_tristate_off(LM_LOWER_ROLLOVER);
		top_rollover_activated = TRUE;
		lower_rollover_activated = FALSE;
		}
	else
	if (middle_rollover_activated && top_rollover_activated) { //right not activated
		lamp_tristate_off(LM_MIDDLE_ROLLOVER);
		lamp_tristate_on(LM_LOWER_ROLLOVER);
		middle_rollover_activated = FALSE;
		lower_rollover_activated = TRUE;
		}
	else
	if (middle_rollover_activated) { //center and right not activated
		lamp_tristate_off(LM_MIDDLE_ROLLOVER);
		lamp_tristate_on(LM_TOP_ROLLOVER);
		middle_rollover_activated = FALSE;
		top_rollover_activated = TRUE;
		}
	else
	if (top_rollover_activated) { //left and right not activated
		lamp_tristate_off(LM_TOP_ROLLOVER);
		lamp_tristate_on(LM_LOWER_ROLLOVER);
		top_rollover_activated = FALSE;
		lower_rollover_activated = TRUE;
		}
	else
	if (lower_rollover_activated) { //left and center not activated
		lamp_tristate_on(LM_MIDDLE_ROLLOVER);
		lamp_tristate_off(LM_LOWER_ROLLOVER);
		middle_rollover_activated = TRUE;
		lower_rollover_activated = FALSE;
		}
}//end of function to rotate right






/****************************************************************************
 * DMD display and sound effects
 ****************************************************************************/



