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
CALLSET_ENTRY (calltester, rramp_clawready_on) { } 					//needed by inlanes.c
CALLSET_ENTRY (calltester, activate_left_ramp_quickfreeze) { } 		//needed by inlanes.c
CALLSET_ENTRY (calltester, deactivate_left_ramp_quickfreeze) { } 		//needed by inlanes.c
CALLSET_ENTRY (calltester, extraball_light_on) { }					//needed by rollovers.c
CALLSET_ENTRY (calltester, access_claw_light_on) { } 				//needed by rollovers.c
CALLSET_ENTRY (calltester, cramp_jackpot_light_on) { } 				//needed by fortressMB
CALLSET_ENTRY (calltester, cramp_jackpot_light_off) { } 			//needed by fortressMB
CALLSET_ENTRY (calltester, activate_explode_inserts) { }
CALLSET_ENTRY (calltester, deactivate_explode_inserts) { }
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
ALLSET_ENTRY (calltester, fortress_jackpot_made){ } 	 //needed by ramps.c
#endif




