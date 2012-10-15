/*******************************************************************
 * .text page == GAME_OBJS
 ********************************************************************/
/* orbits.c */
void extraball_light_on(void);
void extraball_light_off(void);
void multiball_light_on(void);
void multiball_light_off(void);

void activate_explode_inserts_orbits(void);
void deactivate_explode_inserts_orbits(void);

void ll_jackpot_light_on(void);
void ll_jackpot_light_off(void);
void rl_jackpot_light_on(void);
void rl_jackpot_light_off(void);

void ll_arrow_light_on(void);
void ll_arrow_light_off(void);
void rl_arrow_light_on(void);
void rl_arrow_light_off(void);



/* ramps.c*/
void activate_explode_inserts_ramps(void);
void deactivate_explode_inserts_ramps(void);
void carchase_mode_on(void);
void carchase_mode_off(void);

void activate_left_ramp_quickfreeze(void);
void deactivate_left_ramp_quickfreeze(void);
void rramp_clawready_on(void);
void rramp_clawready_off(void);

void lramp_jackpot_light_on(void);
void lramp_jackpot_light_off(void);
void rramp_jackpot_light_on(void);
void rramp_jackpot_light_off(void);
void cramp_jackpot_light_on(void);
void cramp_jackpot_light_off(void);
void sramp_jackpot_light_on(void);
void sramp_jackpot_light_off(void);

void lramp_arrow_light_on(void);
void lramp_arrow_light_off(void);
void rramp_arrow_light_on(void);
void rramp_arrow_light_off(void);
void sramp_arrow_light_on(void);
void sramp_arrow_light_off(void);
void cramp_arrow_light_on(void);
void cramp_arrow_light_off(void);



/*******************************************************************
 * machine page 1
 ********************************************************************/
/* acmag.c */
/* rollovers.c */

/*******************************************************************
 * machine page 2
 ********************************************************************/
/* car_crash.c */
/* cryoclaw.c */

/*jackpots.c*/
__machine2__ void jackpot_reset (void);
__machine2__ void choose_random_jackpot(void);
__machine2__ void score_jackpot(void);


/* combos.c */
__machine2__ void comp_award_light_arrows(void);
__machine2__ void combo_hit(void );
__machine2__ void combo_init(void);


/* arrow_handler.c*/
__machine2__ void center_ramp_arrow_update(void);
__machine2__ void all_arrow_update(void);


/* car_chase_mode.c */
__machine2__ void start_car_chase(void);
__machine2__ void car_chase_ramp_made(void);


/* car_crash.c */
__machine2__ void comp_award_trip_car_crash(void);


/* eject.c */
__machine2__ void comp_award_doub_retina(void);


/* explode.c */
__machine2__ void start_explode(void);
__machine2__ void explode_made(void);


/*inlanes.c*/
__machine2__ void access_claw_light_on(void);
__machine2__ void light_quick_freeze_light_on (void);
__machine2__ void light_quick_freeze_light_off (void);

/* underground.c */
__machine2__ void underground_jackpot_light_on(void);
__machine2__ void underground_jackpot_light_off(void);
__machine2__ void underground_arrow_light_on(void);
__machine2__ void underground_arrow_light_off(void);
__machine2__ void computer_light_on(void);
__machine2__ void computer_light_off(void);



/*******************************************************************
 * machine page 3
 ********************************************************************/
/* bonus.c */


/* capture_simon.c */
__machine3__ void capture_simon_made(void);


/* lock_freeze_mbstart.c */
__machine3__ void maximize_freeze(void);
__machine3__ void increment_freeze(void);
__machine3__ void multiball_start (void);
__machine3__ void multiball_started (void);


/* prison_break.c */
__machine3__ void prison_break_made (void);


/* fortress.c */
__machine3__ void fortress_start(void);
__machine3__ void fortress_jackpot_made(void);

/* museum.c */
__machine3__ void museum_start(void);
__machine3__ void museum_jackpot_made(void);

/* cryoprison.c */
__machine3__ void cryoprison_start(void);
__machine3__ void cryoprison_jackpot_made(void);

/* wasteland.c */
__machine3__ void wasteland_start(void);
__machine3__ void wasteland_jackpot_made(void);

/* demotime.c */
__machine3__ void demotime_start(void);
__machine3__ void demotime_jackpot_made(void);


/*******************************************************************
 * machine page 4
 ********************************************************************/
/* bar.c */
struct progress_bar_ops {
	U8 x;
	U8 y;
	U8 *fill_level;
	U8 *max_level;
	U8 bar_width; //Needs to be a multiple of 5 +
};
__machine4__ void draw_progress_bar (struct progress_bar_ops *ops);


/* dm_millions.c */
__machine4__ void display_dm_millions(U8 num);
__machine4__ void display_and_shake_dm_millions(U8 num);


/* wheelie.c */
__machine4__ void start_wheelie (void);


/* top_popper.c */
__machine4__ void video_mode_finished (void);


/* standupFrenzy.c */
void collect_standups(void );


/*******************************************************************
 * machine page 5
 ********************************************************************/
/* eyeball.c */
/* jets.c */
/* superjets.c */
