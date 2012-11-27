/*
 * demolition man
 * global_constants.h
 *
 * written by James Cardona
 *
 */
#include <freewpc.h>


#ifndef _DM_GLOBAL_CONSTANTS_H
#define _DM_GLOBAL_CONSTANTS_H

/****************************************************************************
 * DMD display coordinates
 ****************************************************************************/
//max width is 128
#define DMD_MIDDLE_X 	64
#define DMD_MIDDLE_Y	16

#define DMD_LEFT_X 		10
#define DMD_RIGHT_X 	118

//max height is 32 - for fonts larger than 5 but less than 14
#define DMD_BIG_CY_Top 	8
#define DMD_BIG_CY_Bot 	24

//for large fonts of height > 16 but < 30
#define DMD_BIG_CY_Cent 16


//4, 11, 19, 26
//max height is 32 - these are for fonts of height 5 and number is center of font
#define DMD_SMALL_CY_1 	4
#define DMD_SMALL_CY_2 	10
#define DMD_SMALL_CY_3 	16
#define DMD_SMALL_CY_4 	22
#define DMD_SMALL_CY_5 	28



/****************************************************************************
 * SCORING CONSTANTS - CLAW MODES
 ****************************************************************************/
// no start points
// each successive mode start score higher
#define ACMAG_HIT_SCORE_1 		SC_3M
#define ACMAG_HIT_SCORE_2  		SC_6M
#define ACMAG_HIT_SCORE_3  		SC_10M

// no start points
// each successive mode start score higher
#define CAPSIM_HIT_SCORE_1 		SC_3M
#define CAPSIM_HIT_SCORE_2  	SC_6M
#define CAPSIM_HIT_SCORE_3  	SC_10M
#define CAP_SIM_COMPLETED_SCORE	SC_10M

//This is a "fast scoring" mode where all small point switches such as jets and rebounds score higher
#define PRIS_BREAK_START_SCORE1	SC_3M
#define PRIS_BREAK_START_SCORE2	SC_6M
#define PRIS_BREAK_START_SCORE3	SC_10M
#define PRIS_BREAK_HIT_SCORE	SC_500K

#define FRENZY_START_SCORE		SC_5M
#define FRENZY_HIT_SCORE		SC_3M
#define FRENZY_ADD_SCORE		SC_1M //NUMBER HITS * THIS NUMBER

#define SUPERJETS_START_SCORE1	SC_3M
#define SUPERJETS_START_SCORE2	SC_6M
#define SUPERJETS_START_SCORE3	SC_10M
#define SUPERJETS_HIT_SCORE1	SC_500K
#define SUPERJETS_HIT_SCORE2	SC_1M
#define SUPERJETS_HIT_SCORE3	SC_2M
#define SUPERJETS_GOAL_SCORE	SC_5M



/****************************************************************************
 * SCORING CONSTANTS - MULTIBALLS
 ****************************************************************************/
//multiball jackpot scores
#define FORTRESS_MB_SCORE		SC_10M
#define MUSEUM_MB_SCORE			SC_15M
#define CRYOPRISON_MB_SCORE		SC_15M
#define WASTELAND_MB_SCORE		SC_20M


/****************************************************************************
 * SCORING CONSTANTS - NON-CLAW MODES
 ****************************************************************************/
// no start points
// each successive mode start score higher
#define CARCHASE_HIT_SCORE_1 	SC_3M
#define CARCHASE_HIT_SCORE_2  	SC_6M
#define CARCHASE_HIT_SCORE_3  	SC_10M

#define EXPLODE_START  			SC_5M
#define EXPLODE_SCORE			SC_500K //this is multiplied by count down timer (30 to 0)


/****************************************************************************
 * SCORING CONSTANTS - WIZARD MODES
 ****************************************************************************/
//started by starting 4 non-claw modes and making a huxley
//which is hitting the retina scan in reverse
#define HUXLEY_START_SCORE		SC_15M
#define HUXLEY_MODE_HIT_SCORE		SC_1M

//like a normal MB mode except all jackpot shots are lit
//started by starting 5 claw modes and shooting underground shot
#define DEMOTIME_MB_SCORE		SC_15M


/****************************************************************************
 * SCORING CONSTANTS - REGULAR SWITCHES
 ****************************************************************************/
//NOTE: car switch two and end switch are hard set since their values are listed on the playfield lights
//they are 3M, 6M, and 10M
#define CARCRASH_SCORE_CAR1		SC_250K
#define CARCRASH_SCORE_CAR2		SC_500K //if playfield light not lit

//THIS IS MULTIPLIED BY RETINA SCAN MULTIPLIER - max of 2
#define EJECT_SCORE 			SC_1M

#define EYEBALL_SCORE			SC_3M

//LOCATED AT SIMPLE SWITCHES
#define OUTLANE_SCORE			SC_5770
#define UP_LEFT_FLIP_GATE_SCORE	SC_1010
#define LOWER_REBOUND_SCORE		SC_1010
#define UPPER_REBOUND_SCORE		SC_1010
#define SLING_SCORE				SC_540

#define INLANE_SCORE			SC_5770

#define HUXLEY_SCORE			SC_500K

#define STANDUP_SCORE			SC_100K

#define JETS_HIT_SCORE			SC_250K
#define JETS_GOAL_SCORE1		SC_1M
#define JETS_GOAL_SCORE2		SC_2M
#define JETS_GOAL_SCORE3		SC_3M
#define JETS_GOAL_SCORE4		SC_4M
#define JETS_GOAL_SCORE5		SC_5M

#define ORBIT_SCORE				SC_100K
#define ORBIT_GOAL_SCORE		SC_250K

#define ALL_ROLLOVERS_SCORE		SC_500K
#define ROLLOVERS_SCORE1		SC_100K //ROLLOVERS HIT THAT ARE ALREADY LIT
#define ROLLOVERS_SCORE2		SC_250K //ROLLOVERS HIT THAT ARE NOT ALREADY LIT

#define RAMPS_ENTER_SCORE		SC_100K
#define RAMPS_MADE_SCORE		SC_250K

#define TOP_POP_SCORE			SC_100K
#define BOT_POP_SCORE			SC_100K




#endif /* _DM_GLOBAL_CONSTANTS_H */
