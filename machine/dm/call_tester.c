/*
 * demolition man
 * calltester.c
 *
 * written by James Cardona
 *
 *
 */
#include <freewpc.h>

//#define RAMPS
#define CAPTURE
#define PRISON
#define CMAG
#define EYE
#define LOCK
#define CARCHASE
#define FORTRESS

#ifndef CMAG
ALLSET_ENTRY (calltester, acmag_made){ } 	//needed by ramps.c
#endif

#ifndef RAMPS
//ramps.c
CALLSET_ENTRY (calltester, RRamp_ClawReady_On) { } 					//needed by inlanes.c
CALLSET_ENTRY (calltester, Activate_left_Ramp_QuickFreeze) { } 		//needed by inlanes.c
CALLSET_ENTRY (calltester, DeActivate_left_Ramp_QuickFreeze) { } 		//needed by inlanes.c
CALLSET_ENTRY (calltester, ExtraBall_Light_On) { }					//needed by rollovers.c
CALLSET_ENTRY (calltester, Access_Claw_Light_On) { } 				//needed by rollovers.c
CALLSET_ENTRY (calltester, CRamp_Jackpot_Light_On) { } 				//needed by fortressMB
CALLSET_ENTRY (calltester, CRamp_Jackpot_Light_Off) { } 			//needed by fortressMB
CALLSET_ENTRY (calltester, Activate_Explode_Inserts) { }
CALLSET_ENTRY (calltester, DeActivate_Explode_Inserts) { }
#endif

#ifndef EYE
//eyeball_explode.c
ALLSET_ENTRY (calltester, explode_made){ }		//needed by orbits.c and ramps.c
#endif

#ifndef LOCK
//lock_freeze_mbstart.c
ALLSET_ENTRY (calltester, multiball_start){ } 	//needed by orbits.c
ALLSET_ENTRY (calltester, increment_freeze){ } 	//needed by ramps.c
#endif

#ifndef CAPTURE
//capture_simon.c
ALLSET_ENTRY (calltester, capture_simon_made){ }//needed by orbits.c and ramps.c
#endif

#ifndef CARCHASE
//carchase.c
ALLSET_ENTRY (calltester, car_chase_ramp_made){ } 	 //needed by ramps.c
__boolean 	is_car_chase_mode_activated; //located in car_chase.c
#endif

#ifndef PRISON
//prison_break.c
ALLSET_ENTRY (calltester, prison_break_made){ } 	 //needed by ramps.c
#endif

#ifndef FORTRESS
//fortressMB.c
ALLSET_ENTRY (calltester, fortressMB_jackpot_made){ } 	 //needed by ramps.c
#endif




