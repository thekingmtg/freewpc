/*******************************************************************
 * common page
 ********************************************************************/
/* ballsave.c */
__common__ void ballsave_add_time(U8 secs);
__common__ void ballsave_disable(void);


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


/* arrow_handler.c*/
void center_ramp_arrow_update(void);
void all_arrow_update(void);


/*jackpots.c*/
void jackpot_reset (void);
void choose_random_jackpot(void);
void choose_multiple_random_jackpot(U8 numOfJakpots);
void score_jackpot(void);
void set_all_jackpots (void);
void score_super_jackpot (void);


/*diverter_check.c*/
void diverter_check (void);





/*******************************************************************
 * machine page 1
 ********************************************************************/
/*inlanes.c*/
__machine__ void access_claw_light_on(void);
__machine__ void light_quick_freeze_light_on (void);
__machine__ void light_quick_freeze_light_off (void);
__machine__ void increment_access_claw_light_on(void);
__machine__ void check_access_claw_relight (void);
__machine__ void increment_light_quick_freeze_light (void);

/*extra_ball.c*/
__machine__ void start_extraball(void);
__machine__ void end_extraball(void);


/*start_super_jackpot_reminder.c*/
__machine__ void start_super_jackpot_reminder (void);
__machine__ void end_super_jackpot_reminder (void);


/* standupFrenzy.c */

/* acmag.c */
__machine__ void acmag_made(void);

__machine__ void disabled_claw_hit (void);

/*******************************************************************
 * machine page 2
 ********************************************************************/
/* car_chase_mode.c */
__machine2__ void start_car_chase(void);
__machine2__ void car_chase_ramp_made(void);


/* explode.c */
__machine2__ void start_explode(void);
__machine2__ void explode_made(void);


/* demotime.c */
__machine2__ void demotime_increment (void);
__machine2__ void demotime_increment_all (void);
__machine2__ void demotime_start(void);
__machine2__ void demotime_jackpot_made(void);


/* huxley.c*/
__machine2__ void huxley_increment(void);
__machine2__ void huxley_increment_all(void);
__machine2__ void huxley_begin(void);
__machine2__ void huxley_mode_shot_made(void);
__machine2__ void huxley_made(void);


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
__machine3__ void fortress_start(U8 num);
__machine3__ void fortress_jackpot_made(void);
__machine3__ void fortress_award_super_jackpot(void);


/* wasteland.c */
__machine3__ void wasteland_start(U8 num);
__machine3__ void wasteland_jackpot_made(void);
__machine3__ void wasteland_award_super_jackpot(void);


/*leffs.c*/




/*******************************************************************
 * machine page 4
 ********************************************************************/
/* top_popper.c */
__machine4__ void top_popper_video_mode_finished (void);


/* vm_handler.c */
__machine4__ void start_video_mode (U8 sender);
__machine4__ void video_mode_finished (void);


/*shooting_gallery.c*/
__machine4__ void start_shooting_gallery (void);


/* eject.c */
__machine4__ void comp_award_doub_retina(void);
__machine4__ void eject_video_mode_finished(void);


/*kaboom.c*/
__machine4__ void start_kaboom (void);


/*back_in_the_fridge.c*/
__machine4__ void enable_back_in_the_fridge(void);
__machine4__ void start_back_in_the_fridge(void);
__machine4__ void back_in_the_fridge_shot_made(void);




/*******************************************************************
 * machine page 5
 ********************************************************************/
/* museum.c */
__machine5__ void museum_start(U8 num);
__machine5__ void museum_jackpot_made(void);
__machine5__ void museum_award_super_jackpot(void);


/* cryoprison.c */
__machine5__ void cryoprison_start(U8 num);
__machine5__ void cryoprison_jackpot_made(void);
__machine5__ void cryoprison_award_super_jackpot(void);



/*skillshot.c*/
__machine5__ void award_skill_shot (U8 type_skill_shot_made);


/* combos.c */
__machine5__ void comp_award_light_arrows(void);
__machine5__ void combo_hit(void );
__machine5__ void combo_init(void);
__machine5__ void kill_combos(void);
__machine5__ void combo_rehit_check(void );

/* car_crash.c */
__machine5__ void comp_award_trip_car_crash(void);
__machine5__ void restore_carcrash_lights_state(void);


/* special_shots.c */
__machine5__ void laser_shot_made(void);

