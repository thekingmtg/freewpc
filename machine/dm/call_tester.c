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
//#define EXPLO		//expolde.c
#define JETS
//#define COMBOS
//#define JACKPOTS
//#define INLANES


#ifndef INLANES
#endif


#ifndef JETS
const U8 jet_shots_made = 0; 	//needed by eyeball.c
#endif

#ifndef COMBOS
#endif

#ifndef JACKPOTS
#endif

#ifndef EXPLO
#endif


#ifndef CMAG
CALLSET_ENTRY (calltester, acmag_made){ } 	//needed by ramps.c
#endif

#ifndef RAMPS
#endif

#ifndef LOCK
//lock_freeze_mbstart.c
#endif

#ifndef CAPTURE
//capture_simon.c
#endif

#ifndef CARCHASE
//carchase.c
#endif

#ifndef PRISON
//prison_break.c
#endif

#ifndef FORTRESS
#endif




