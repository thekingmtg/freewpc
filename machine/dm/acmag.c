/*
 * demolition man
 * acmag.c
 *
 * Location Description:
 * mode started by claw switch -points scored by center ramp.
 *
 * Scoring Description: (original game)
 * counts up as time goes down from 6 million to 13.5 million
 *
 * Scoring Description: (my rules)
 * same as above except:
 *
 */
#include <freewpc.h>
//constants

//local variables
U8 			acmag_mode_counter;
score_t 	acmag_mode_score;
__boolean 	acmag_mode_activated;

//prototypes

/****************************************************************************
 * initialize  and exit
 ***************************************************************************/
void acmag_reset (void) {
	acmag_mode_counter = 0;
	acmag_mode_activated = FALSE;
	score_zero(acmag_mode_score);
	}

CALLSET_ENTRY (acmag, start_player) { acmag_reset(); }
CALLSET_ENTRY (acmag, start_ball) { acmag_reset(); }


/****************************************************************************
 * body
 *
 ***************************************************************************/
CALLSET_ENTRY (acmag, sw_claw_acmag) {
	score (SC_250K);
	acmag_mode_activated = TRUE;

	}//end of function

CALLSET_ENTRY (acmag, acmag_made) {
	++acmag_mode_counter;
	//TODO: score rolls up as time goes?
	//score_add(acmag_mode_score, score_table[SC_6M]);
	score (SC_6M);
	}//end of function



/****************************************************************************
 * DMD display and sound effects
 ****************************************************************************/
