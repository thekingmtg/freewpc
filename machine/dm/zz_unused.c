/*
 * demolition man
 * zz_unused
 * written by James Cardona
 *
 * This is a list of item that have not yet been implememented in the code yet
 *
 */
[lamps]
71: Super Jackpot,yellow, x( 7), y(11)
72: Computer,yellow, x( 8), y(11)
73: Demo Time, yellow, x( 9), y(11)
86: Buy In Button, yellow, buyin, cabinet, x(33), y(18)

[switches]
75: Elevator Ramp, ingame


#H = high power J130
H1: Ball Release, duty(SOL_DUTY_25), time(TIME_200MS)
H3: Auto Plunger, nosearch, launch, duty(SOL_DUTY_100), time(TIME_200MS)
H4: Top Popper, time(TIME_100MS)

# G = J126 on Power Driver Board
G1: Claw Flasher, flash, nosearch

F5: Claw Magnet 

#X = J4 on 8 driver board
X5: Elevator 2 Flasher, flash, nosearch
X6: Elevator 1 Flasher, flash, nosearch









#define MUS_BG_A 0x3
#define MUS_BG_B 0x4
#define MUS_CLAW 0xE
#define MUS_HIGH_SCORE 0x1F

#define SPECIAL 0x21
#define WATER_SPRAYER 0x68
#define MACHINE6 0x6C
#define PLONK 0x78
#define RICOCHET 0x7C
#define WIPE 0x7E
#define SPRAY_CAN 0x81
#define STEAM 0x8B
#define SPECIAL_EFFECT 0x8C
#define TRAIN_PULL_IN 0x8D
#define GUNSHOT_MUFFLED 0x98
#define GUNSHOT 0x99
#define LOW_HORN 0xAA
#define TIN_CAN 0xB0
#define RACE_BY_SHORT 0xB4
#define RACE_BY_LONG 0xB6
#define BONUS_SHORT 0xB9
#define BONUS_LONG 0xBA
#define MACHINE10 0xBC
#define EXPLOSION1_SHORT 0xCB
#define EXPLOSION1_MED 0xCC
#define EXPLOSION1_LONG 0xCD
#define MACHINE11 0xD1
#define TILT 0x01FB
#define SPARKS 0x01FC
#define ZAPP 0x0200
#define MACHINE3 0x0202
#define EJECT 0x0208
#define FREEZE5 0x0209
#define ORCHESTRA 0x0210
#define MACHINE14_SHORT 0x0212
#define MACHINE14_LONG 0x0216
#define COMPUTER3 0x0218
#define GROUND_HUMM 0x0219


#define SPCH_SOME_ACTION 0x0143
#define SPCH_CLOSE_ENOUGH 0x014A
#define SPCH_SIMON_SAYS_SLY 0x0154
#define SPCH_BACK_IN_FRIDGE 0x0155
#define SPCH_SIMON_LAUGH_LONG 0x0165
#define SPCH_JOHN_SCREAM 0x0166
#define SPCH_FEEL_GOOD_TOO 0x0182
#define SPCH_COWS 0x025A
#define SPCH_I_GOT_IT 0x0263
#define SPCH_COMPETITION 0x0264
#define SPCH_HOW_NICE 0x0289
#define SPCH_LOVE_THIS_GUY 0x028A
#define SPCH_UHHN 0x0292
#define SPCH_WHAT_YOU_DOING_HERE 0x02AC
#define SPCH_GET_THIS_PARTNER 0x02AE
#define SPCH_ARREST 0x02B0
#define SPCH_TRESPASSING 0x02B1
#define SPCH_NICE_MOVE 0x02C5
#define SPCH_HURRY_UP 0x02D2
#define SPCH_SIMON_SAYS 0x02FF
#define SPCH_YOU_GOT_IT 0x030C
#define SPCH_HURRY 0x033B
#define SPCH_SKILLSHOT 0x0361
#define SPCH_DOUBLE 0x0375
#define SPCH_TRIPLE 0x0376
#define SPCH_YOU_AND_ME 0x0385
#define SPCH_CAR_CRASH_TRIPLED 0x03EE
#define SPCH_USE_BUTTONS_TO_RELEASE_BALL 0x0417
#define SPCH_USE_TRIGGERS_TO_MOVE_CRYOCLAW 0x0418
#define SPCH_IN_THIS_CENTURY 0x041B
#define SPCH_STAYED_THERE 0x041E
#define SPCH_DONT_WORRY 0x0420
#define SPCH_MUST_HAVE_HURT 0x0422
#define SPCH_JOHN_SPARTAN 0x0423
#define SPCH_PHOENIX_SHORT 0x0425
#define SPCH_COW_MOO 0x0427
#define SPCH_WHO_IS_THAT 0x043E
#define SPCH_DREAMIN_KILLING_YOU 0x043F
#define SPCH_KEEP_DREAMING 0x0440


multiball sayings
#define SPCH_SUPER_JACKPOT 0x0378
#define SPCH_GET_SUPER_JACKPOT 0x02D6

top popper award sayings
#define SPCH_YOU_LOOK_GREAT_TODAY 0x0416
#define SPCH_ILUMINATE 0x018A
#define SPCH_AMAZING_WES 0x030D
#define SPCH_AMAZING_SLY 0x0320
#define SPCH_GLORIOUS 0x0140
#define SPCH_OUTSTANDING 0x02F6

end ball and match feature
#define SPCH_TWO_PRICE_ONE 0x02A9
#define SPCH_PLAY_ACCORDION_TOO 0x017F
#define SPCH_BE_WELL_PHOENIX 0x0341
#define BALL_DRAIN 0x1A
#define NO_MATCH_ACCORDION 0x0220
#define SPCH_REPLAY 0x0384
#define SPCH_NO_FREE_RIDES 0x0297

enter name for high scores
#define SPCH_PLAYER_ONE 0x0405
#define SPCH_PLAYER_TWO 0x0406
#define SPCH_PLAYER_THREE 0x0407
#define SPCH_PLAYER_FOUR 0x0408