/*
 * demolition man
 * calltester.c
 *
 * written by James Cardona
 *
 *
 */
#include <freewpc.h>

#define RAMPS
//#define CAPTURE
//#define PRISON
//#define CMAG		//acmag.c
//#define LOCK
//#define CARCHASE
//#define FORTRESS
#define EXPLO		//expolde.c
#define JETS		//jets_superjets.c


#ifndef JETS
const U8 jet_shots_made = 0; 	//needed by eyeball.c
#endif


#ifndef EXPLO
ALLSET_ENTRY (calltester, explode_made){ } 	//needed by orbits.c/ramps.c
ALLSET_ENTRY (calltester, start_explode){ } 	//needed by eyeball.c
#endif


#ifndef CMAG
CALLSET_ENTRY (calltester, acmag_made){ } 	//needed by ramps.c
#endif

#ifndef RAMPS
//ramps.c
ALLSET_ENTRY (calltester, rramp_clawready_on) { } 					//needed by inlanes.c
ALLSET_ENTRY (calltester, activate_left_ramp_quickfreeze) { } 		//needed by inlanes.c
ALLSET_ENTRY (calltester, deactivate_left_ramp_quickfreeze) { } 		//needed by inlanes.c
ALLSET_ENTRY (calltester, extraball_light_on) { }					//needed by rollovers.c
ALLSET_ENTRY (calltester, access_claw_light_on) { } 				//needed by rollovers.c
ALLSET_ENTRY (calltester, cramp_jackpot_light_on) { } 				//needed by fortressMB
ALLSET_ENTRY (calltester, cramp_jackpot_light_off) { } 			//needed by fortressMB
ALLSET_ENTRY (calltester, activate_explode_inserts) { }
ALLSET_ENTRY (calltester, deactivate_explode_inserts) { }
#endif

#ifndef LOCK
//lock_freeze_mbstart.c
CALLSET_ENTRY (calltester, multiball_start){ } 	//needed by orbits.c
CALLSET_ENTRY (calltester, increment_freeze){ } 	//needed by ramps.c
#endif

#ifndef CAPTURE
//capture_simon.c
CALLSET_ENTRY (calltester, capture_simon_made){ }//needed by orbits.c and ramps.c
#endif

#ifndef CARCHASE
//carchase.c
CALLSET_ENTRY (calltester, car_chase_ramp_made){ } 	 //needed by ramps.c
CALLSET_ENTRY (calltester, start_car_chase){ } 	 //needed by car_crash.c
#endif

#ifndef PRISON
//prison_break.c
CALLSET_ENTRY (calltester, prison_break_made){ } 	 //needed by ramps.c
#endif

#ifndef FORTRESS
//fortressMB.c
CALLSET_ENTRY (calltester, fortress_jackpot_made){ } 	 //needed by ramps.c
#endif




