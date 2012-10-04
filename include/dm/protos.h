/*******************************************************************
 * machine page 1
 ********************************************************************/
/* acmag.c */
__machine__ void acmag_reset (void);
__machine__ void acmag_player_reset (void);
__machine__ void acmag_effect_deff(void);
__machine__ void acmag_mode_init (void);
__machine__ void acmag_mode_expire (void);
__machine__ void acmag_mode_exit (void);
__machine__ void star_draw (void);
__machine__ void show_text_on_stars (U8 times);



/* car_chase_mode.c */
__machine__ void car_chase_reset (void);
__machine__ void car_chase_player_reset (void);
__machine__ void car_chase_effect_deff(void);
__machine__ void car_chase_mode_init (void);
__machine__ void car_chase_mode_expire (void);
__machine__ void car_chase_mode_exit (void);



/* eyeball.c */
__machine__ void eyeball_reset (void);
__machine__ void player_eyeball_reset (void);
__machine__ void eyeball_goal_award (void);
__machine__ void eyeball_eb_award (void);
__machine__ void eyeball_effect_chooser(void);



/* jets.c */
__machine__ void jet_flasher(void);
__machine__ void jets_effect_deff(void);
__machine__ void jet_goal_reset(void);
__machine__ void jet_goal_award(void);
__machine__ void player_jet_goal_reset(void);



/* rollovers.c */
__machine__ void rollover_reset (void);
__machine__ void all_rollover_made (void);
__machine__ void rollover_sounds (void);
__machine__ void rollover_sounds_all_rollovers (void);
__machine__ void rollover_sounds_already_lit(void);
__machine__ void rollovers_effect(void);



/* standupFrenzy.c */
__machine__ void standupFrenzy_mode_init (void);
__machine__ void standupFrenzy_mode_exit (void);
__machine__ void standupHandler (U8 target);
__machine__ void standupHandler1 (U8 target, U8 lamp);
__machine__ void frenzyHandler (U8 target, U8 lamp);
__machine__ void standup_lamp_update1 (U8 mask, U8 lamp);
__machine__ void standupFrenzyTotalScore_effect_deff (void);
__machine__ void standupFrenzy_mode_effect_deff (void);
__machine__ void standupFrenzy_sounds (void);
__machine__ void standup_sounds (void);
__machine__ void standup_effect(void);


/* superjets.c */
__machine__ void superjets_reset (void);
__machine__ void superjets_player_reset (void);
__machine__ void superjets_effect_deff(void);
__machine__ void superjets_mode_init (void);
__machine__ void superjets_mode_expire (void);
__machine__ void superjets_mode_exit (void);
__machine__ void superjets_goal_award(void);


/*******************************************************************
 * machine page 2
 ********************************************************************/
/* capture_simon.c */
__machine2__ void capture_simon_reset (void);
__machine2__ void capture_simon_player_reset (void);
__machine2__ void capture_simon_effect_deff(void);
__machine2__ void capture_simon_mode_init (void);
__machine2__ void capture_simon_mode_expire (void);
__machine2__ void capture_simon_mode_exit (void);



/* car_crash.c */
__machine2__ void car_crash_reset (void);
__machine2__ void player_car_crash_reset (void);
__machine2__ void car_crash_first_switch_task (void);
__machine2__ void car_crash_second_switch_task (void);
__machine2__ void car_crash_third_switch_task (void);
__machine2__ void carcrash_mode_effect_deff(void);
__machine2__ void carcrash_effect_deff(void);



/* combos.c */
__machine2__ void combo_reset (void);
__machine2__ void player_combo_reset (void);
__machine2__ void flash_combos(void);
__machine2__ void choose_random_flag_set(void);
__machine2__ void combo_task (void);



/* cryoclaw.c */
__machine2__ void cryoclaw_mode_init (void);
__machine2__ void cryoclaw_mode_expire (void);
__machine2__ void cryoclaw_mode_exit (void);
__machine2__ void cryoclaw_start_effect_deff(void);
__machine2__ void cryoclaw_effect_deff(void);
__machine2__ void cryoclaw_end_effect_deff(void);



/* eject.c */
__machine2__ void eject_reset (void);
__machine2__ void player_eject_reset (void);
__machine2__ void eject_killer_task (void);
__machine2__ void eyeball_effect_deff(void);



/* explode.c */
__machine2__ void explode_mode_init (void);
__machine2__ void explode_mode_expire (void);
__machine2__ void explode_mode_exit (void);
__machine2__ void explode_reset (void);
__machine2__ void player_explode_reset (void);
__machine2__ void explode_effect_deff(void);
__machine2__ void explode_effect(void);



/* fortress.c */
__machine2__ void fortress_player_reset (void);
__machine2__ void fortress_start_effect_deff(void);
__machine2__ void fortress_jackpot_effect1_deff(void);
__machine2__ void fortress_jackpot_effect2_deff(void);
__machine2__ void fortress_jackpot_effect3_deff(void);
__machine2__ void fortress_jackpot_effect(void);
__machine2__ void fortress_effect_deff (void);



/* lock_freeze_mbstart.c */
__machine2__ void lock_reset (void);
__machine2__ void player_reset (void);
__machine2__ void check_multiball_requirements(void);



/* prison_break.c */
__machine2__ void prison_break_reset (void);
__machine2__ void prison_break_player_reset (void);
__machine2__ void prison_break_effect_deff(void);
__machine2__ void prison_break_mode_init (void);
__machine2__ void prison_break_mode_expire (void);
__machine2__ void prison_break_mode_exit (void);



/* underground.c */
__machine2__ void underground_reset (void);
__machine2__ void underground_task (void);
__machine2__ void square_draw (U8 sq);
__machine2__ void show_text_on_squares (U8 times);
__machine2__ void square_init (void);
__machine2__ void computer_award(void);
__machine2__ void computer_light_off (void);




/*******************************************************************
 * machine page 3
 ********************************************************************/
/* bonus.c */
__machine3__ void bonus_button_monitor (void);
__machine3__ void bonus_sched_transition (void);
__machine3__ void bonus_pause (void);
__machine3__ bool check_for_big_score (void);
__machine3__ bool check_for_puny_score (void);



/*******************************************************************
 * machine page 4
 ********************************************************************/
/* top_popper.c */
__machine4__ void top_popper_reset (void);
__machine4__ void top_popper_task (void);



/* bar.c */
struct progress_bar_ops
{
	U8 x;
	U8 y;
	U8 *fill_level;
	U8 *max_level;
	U8 bar_width; //Needs to be a multiple of 5 +
};

__machine4__ void draw_progress_bar (struct progress_bar_ops *ops);


/* dm_millions.c */
__machine4__ void display_dm_millions(U8 num);
__machine4__ void display_0_top(void);



