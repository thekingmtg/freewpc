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
//#define JETS
//#define COMBOS
//#define JACKPOTS
//#define INLANES
//#define ARROWS
//#define DEMOTIME
//#define CRYOPRISON
//#define WASTELAND
//#define CRASH
//#define UGROUND
//#define HUXLEY


#ifndef HUXLEY
void huxley_increment(void){}
void huxley_begin(void){}
void huxley_mode_shot_made(void){}
void huxley_made(void){}
#endif

#ifndef ARROWS
void center_ramp_arrow_update(void){}
void all_arrow_update(void){}
#endif


#ifndef INLANES
void access_claw_light_on(void){}
void light_quick_freeze_light_on (void){}
void light_quick_freeze_light_off (void){}
#endif


#ifndef JETS
const U8 jet_shots_made = 0; 	//needed by eyeball.c
#endif

#ifndef COMBOS
void comp_award_light_arrows(void){}
void combo_hit(void ){}
void combo_init(void){}
#endif

#ifndef JACKPOTS
void jackpot_reset (void){}
void choose_random_jackpot(void){}
void score_jackpot(void){}
void set_all_jackpots (void){}
#endif

#ifndef EXPLO
void start_explode(void){}
void explode_made(void){}
#endif


#ifndef CMAG
void acmag_made(void){ } 	//needed by ramps.c
#endif

#ifndef RAMPS
#endif

#ifndef LOCK
//lock_freeze_mbstart.c
void maximize_freeze(void){}
void increment_freeze(void){}
void multiball_start (void){}
void multiball_started (void){}
#endif

#ifndef CAPTURE
void capture_simon_made(void){}
#endif

#ifndef CARCHASE
void start_car_chase(void){}
void car_chase_ramp_made(void){}

void comp_award_trip_car_crash(void){}
#endif

#ifndef PRISON
//prison_break.c
void prison_break_made (void){}
#endif

#ifndef FORTRESS
void fortress_start(void){}
void fortress_jackpot_made(void){}
#endif


#ifndef UGROUND
void underground_jackpot_light_on(void){}
void underground_jackpot_light_off(void){}
void underground_arrow_light_on(void){}
void underground_arrow_light_off(void){}
void computer_light_on(void){}
void computer_light_off(void){}
#endif

#ifndef DEMOTIME
void demotime_increment (void){}
void demotime_start(void){}
void demotime_jackpot_made(void){}
#endif

#ifndef CRYOPRISON
void cryoprison_start(void){}
void cryoprison_jackpot_made(void){}
#endif

#ifndef WASTELAND
void wasteland_start(void){}
void wasteland_jackpot_made(void){}
#endif
