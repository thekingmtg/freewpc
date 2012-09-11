#--------------------------------------------------------------------------
# Demolition Man Machine description for FreeWPC
# See tools/genmachine for more information about the format of this file.
#--------------------------------------------------------------------------
# This file describes many characteristics of a pinball machine,
# mostly physical stuff, although some rules can be coded here also.
#--------------------------------------------------------------------------
##########################################################################
# General section (before a [section] header is given.
# Miscellaneous parameters are specified here.
##########################################################################
Title: Demolition Time
#the following tags are required because the wpc-dcs md is not finished
DMD: Yes
Fliptronic: Yes
DCS: Yes
WPC95: No

Lamp-Matrix-Width: 18
Lamp-Matrix-Height: 34

##########################################################################
# Include standard definitions that apply to all WPC games.
##########################################################################
include platform/wpc/wpc-dcs.md
#nclude platform/wpc/wpc-fliptronic.md

##########################################################################
# Use 'define' to emit a plain #define for anything not covered by
# some other means.
##########################################################################
	#see code changes in /kernal/flip.c
define MACHINE_HAS_FLIPPER_GUN_HANDLES
define MACHINE_HAS_UPPER_LEFT_FLIPPER
define MACHINE_START_BALL_MUSIC   MUS_PLUNGER
define MACHINE_BALL_IN_PLAY_MUSIC 	MUS_BG 
define MACHINE_BALL_SAVE_TIME       4
define MACHINE_MAX_BALLS            5
define MACHINE_CUSTOM_AMODE
define MACHINE_AMODE_EFFECTS

	#see code changes in ballsave.c
#define CUSTOM_BALL_SAVE_DEFF
	#see code changes in shootalert.c
#define CUSTOM_SHOOT_REMINDER_DEFF

#this does not play
define MACHINE_END_GAME_MUSIC 		MUS_END_GAME

##########################################################################
# Lamp Description
# The key is given in column/row format.  The first parameter must be
# the lamp name.  Optionally you can specify a color, and x() and y()
# for the location on the playfield.  Only a subset of lamp colors are
# recognized; see tools/genmachine for details.  Lamp location is
# given in terms of the Lamp-Matrix-Width and Lamp-Matrix-Height.
##########################################################################
[lamps]
11: Ball Save, 						red, 		x(28), 		y(6)
12: Fortress Multiball, 			white, 		x(33), 		y(9)
13: Museum Multiball, 				white, 		x(31), 		y(9)
14: Cryoprison Multiball, 			white, 		x(29), 		y(9)
15: Wasteland Multiball, 			white, 		x(30), 		y(9)
16: Shoot Again, ball-save, 		red, 		x(34), 		y(9)
17: Access Claw, 					yellow, 	x(27), 		y(1)
18: Left Ramp Explode, 				yellow, 	x(22), 		y(9)
21: Right Ramp Jackpot, 			red, 		x(22), 		y(14)
22: Right Loop Explode, 			yellow,		x(23), 		y(15)
23: Light Quick Freeze, 			yellow, 	x(27), 		y(15)
24: Freeze 4, 						white, 		x(26), 		y(14)
25: Claw Ready, 					yellow, 	x(25), 		y(12)
26: Freeze 3, 						white, 		x(28), 		y(12)
27: Freeze 2, 						white, 		x(28), 		y(6)
28: Freeze 1, 						white, 		x(26), 		y(2)
31: Right Loop Jackpot, 			red, 		x(21), 		y(16)
32: Standup 5, 						blue, 		x(19), 		y(15)
33: Right Ramp Arrow, 				red, 		x(19), 		y(14)
34: Left Ramp Jackpot, 				red, 		x(20), 		y(8)
35: Left Loop Jackpot, 				red, 		x(19), 		y(6)
36: Car Crash Top, 					yellow, 	x(20), 		y(2)
37: Standup 1, 						blue, 		x(21), 		y(5)
38: Car Crash Center, 				yellow, 	x(22), 		y(3)
41: Right Ramp Explode, 			yellow, 	x(23), 		y(14)
42: Right Ramp Car Chase, 			yellow, 	x(24), 		y(13)
43: Quick Freeze, 					yellow, 	x(24), 		y(10)
44: Left Ramp Car Chase, 			yellow, 	x(23), 		y(9)
45: Extra Ball, extra-ball, 		yellow, 	x(24), 		y(9)
46: Start Multiball, 				yellow, 	x(23), 		y(8)
47: Car Crash Bottom, 				yellow, 	x(23), 		y(4)
48: Left Loop Explode, 				yellow, 	x(22), 		y(7)
51: Underground Arrow, 				red, 		x(11), 		y(10)
52: Underground Jackpot, 			red, 		x(12), 		y(9)
53: Standup 2, 						blue, 		x(13), 		y(8)
54: Left Ramp Arrow, 				red, 		x(14), 		y(7)
55: Side Ramp Jackpot, 				red, 		x(16), 		y(9)
56: Side Ramp Arrow, 				red, 		x(15), 		y(9)
57: left Loop Arrow, 				red, 		x(14), 		y(4)
58: Center Ramp Jackpot, 			red, 		x(10), 		y(9)
61: Claw Capture Simon,				red, 		x(1), 		y(7)
62: Claw Super Jets,				red, 		x(3), 		y(6)
63: Claw Prison Break,				red, 		x(4), 		y(5)
64: Claw Freeze, 					red, 		x(5), 		y(2)
65: Claw Acmag, 					red, 		x(5), 		y(1)
66: Middle Rollover, 				green, 		x(2), 		y(14)
67: Top Rollover, 					green, 		x(2), 		y(15)
68: Lower Rollover, 				green, 		x(2), 		y(17)
71: Super Jackpot,					yellow, 	x(7), 		y(11)
72: Computer,						blue, 		x(8), 		y(11)
73: Demo Time, 						yellow, 	x(9), 		y(11)
74: Not Used 1
75: Not Used 2
76: Standup 4, 						blue, 		x(18), 		y(13)
77: Standup 3, 						blue, 		x(17), 		y(10)
78: Retina Scan, 					yellow, 	x(23), 		y(1)
81: Center Ramp Middle, 			red,		x(6), 		y(9)
82: Center Ramp Outer, 				red,		x(6), 		y(8)
83: Center Ramp Inner, 				red, 		x(6), 		y(9)
84: Center Ramp Arrow, 				red, 		x(9), 		y(9)
85: Right Loop Arrow, 				red, 		x(16), 		y(18)
86: Buy In Button, buyin, cabinet, 	yellow, 	x(33), 		y(18)
87: Ball Launch, launch, cabinet, 	red, 		x(34), 		y(16)
88: Start Button, start, cabinet, 	yellow, 	x(1), 		y(1)

##########################################################################
# Switch Description
# The key is in column/row format.  The first parameter must be the switch
# name.  Options can be given in any order:
#    ingame - only service the switch during a game
#    intest - also service the switch in test mode
#    novalid - tripping this switch does NOT mark ball in play
#    standup - this is a standup
#    button - this is a button
#    edge - this switch should be serviced on either transition
#    opto - this switch is optical and activates on closed->open
#
# These parameters mark various well-known switches.  Only one of these
# can exist per type:
#    outhole, slam-tilt, tilt, shooter, start-button, buyin-button
#
# Use sound() to invoke a sound call automatically when the switch activates.
# Use lamp() to flicker a lamp automatically on activation.  TODO:
#   this requires the c_name, and not the friendly md name.
# Use c_decl() to override the default name of the switch event.
#
##########################################################################
[switches]
11: Launch Button, launch-button, cabinet, novalid
12: Left Handle Button, cabinet, novalid
13: Start Button, start-button, cabinet, intest, novalid, debounce(TIME_100MS)
14: Tilt, tilt, ingame, novalid, cabinet, sound(TILT)
15: Left Outlane, ingame 
16: Left Inlane, ingame
17: Right Inlane, ingame
18: Right Outlane, ingame

21: Slam Tilt, slam-tilt, ingame, novalid, cabinet, sound(TILT)
22: Coin Door Closed, novalid, cabinet
23: Buyin Button, buyin-button, cabinet, novalid
24: Always Closed, novalid
25: Claw Position 1, opto, noscore, novalid
26: Claw Position 2, opto, noscore, novalid
27: Shooter, shooter, noscore, novalid, edge
28: Not Used 1, novalid

31: Trough 1, noscore, novalid, opto
32: Trough 2, noscore, novalid, opto
33: Trough 3, noscore, novalid, opto
34: Trough 4, noscore, novalid, opto
35: Trough 5, noscore, novalid, opto
36: Trough Jam, noscore, novalid, opto
37: Not Used 2, novalid
38: Standup 5, ingame, standup

41: Left Sling, ingame, c_decl(sw_sling)
42: Right Sling, ingame, c_decl(sw_sling)
43: Left Jet, ingame, c_decl(sw_jet)
44: Top Sling, ingame, c_decl(sw_jet)
45: Right Jet, ingame, c_decl(sw_jet)
46: Right Ramp Enter, ingame
47: Right Ramp Exit, ingame
48: Right Freeway, ingame

51: Left Ramp Enter, ingame
52: Left Ramp Exit, ingame
53: Center Ramp, ingame
54: Upper Rebound, ingame, novalid
55: Left Loop, ingame 
56: Standup 2, standup, ingame
57: Standup 3, standup, ingame
58: Standup 4, standup, ingame

61: Side Ramp Enter, ingame
62: Side Ramp Exit, ingame
63: Left Rollover, ingame
64: Center Rollover, ingame
65: Right Rollover, ingame
66: Eject, ingame
67: Elevator Index, opto, novalid
68: Not Used 3, novalid

71: Chase Car 1, opto, ingame
72: Chase Car  2, opto, ingame
73: Top Popper, opto, ingame, edge
74: Elevator Hold, opto, novalid
75:  Not Used 4, novalid
76: Bottom Popper, opto, ingame, edge, debounce(TIME_200MS)
77: Eyeball Standup, standup, ingame
78: Standup 1, standup, ingame

81: Claw Capture Simon, ingame
82: Claw Super Jets, ingame
83: Claw Prison Break, ingame
84: Claw Freeze, ingame
85: Claw Acmag, ingame
86: Upper Left Flipper Gate, ingame
87: Car Chase Standup, ingame
88: Lower Rebound, ingame

##########################################################################
# Drives
# This describes the names of the solenoid/motor drives.
# Hn = high power solenoids
# Ln = low power solenoids
# Gn = general purpose solenoids
# etc.
#
# The following options are supported:
#    flash - this is a flasher (default is solenoid, flash implies nosearch)
#    motor - this is a motor (default is solenoid)
#    nosearch - do not activate this during ball search
#
# The following options denote well-known drives:
#    knocker, ballserve, launch
#
##########################################################################
# THIS IS LIST IS SHOWN IN THE SAME ORDER AS THE SOLENOID/FLASHER TABLE
[drives]
#H = high power J130
H1: BallServe, ballserve, duty(SOL_DUTY_100), time(TIME_66MS)
H2: Bottom Popper, time(TIME_200MS)
H3: Launch, launch, nosearch, duty(SOL_DUTY_100), time(TIME_133MS)
H4: Top Popper, time(TIME_100MS)
H5: Diverter Power, duty(SOL_DUTY_75)
H6: Not Used 1
H7: Knocker, knocker, time(TIME_16MS)

#L = low power J127
L1: Left Sling, duty(SOL_DUTY_100)
L2: Right Sling, duty(SOL_DUTY_100)
L3: Left Jet, duty(SOL_DUTY_100)
L4: Top Sling, duty(SOL_DUTY_100)
L5: Right Jet, duty(SOL_DUTY_100)

L6: Eject, duty(SOL_DUTY_75), time(TIME_133MS)
L7: Diverter Hold, nosearch
L8: : Not Used 2

# G = J126 on Power Driver Board
G1: Claw Flasher, flash, nosearch
G2: Elevator Motor, motor
G3: Claw Motor Left, motor
G4: Claw motor Right, motor
G5: Jets Flasher, flash, nosearch
G6: Side Ramp Flasher, flash, nosearch
G7: Left Ramp Up Flasher, flash, nosearch
G8: Left Ramp Lower Flasher, flash, nosearch

# A = J122
A1: Car Chase Center Flasher, flash, nosearch
A2: Car Chase Lower Flasher, flash, nosearch
A3: Right Ramp Flasher, flash, nosearch
A4: Eject Flasher, flash, nosearch

# F = J902 on Fliptronic II
F1: L.R. Flip Power, time(TIME_33MS)
F2: L.R. Flip Hold, time(TIME_100MS)
F3: L.L. Flip Power, time(TIME_33MS)
F4: L.L. Flip Hold, time(TIME_100MS)
F5: Claw Magnet 
F6: Not Used
F7: U.L. Flip Power, time(TIME_33MS)
F8: U.L. Flip Hold, time(TIME_100MS)

#X = J4 on 8 driver board
X1: Car Chase Upper Flasher, flash, nosearch
X2: Lower Rebound Flasher, flash, nosearch
X3: Eyeball Flasher, flash, nosearch
X4: Center Ramp Flasher, flash, nosearch
X5: Elevator 2 Flasher, flash, nosearch
X6: Elevator 1 Flasher, flash, nosearch
X7: Diverter Flasher, flash, nosearch
X8: Right Ramp Up Flasher, flash, nosearch


##########################################################################
# templates
# This section contains template definitions for device drivers.
# For each device requiring a driver, it specifies the file that
# contains the template code, and any parameters for the specific
# device.
# 
# some of the templates have auto generated function calls in them that you will
# use to start and stop the coil.  For example divhold.ct creates functions
# self_start and self_stop, where self is the name of the diverter that you specify.
#
# look in the drivers directory for details on each driver
# the templates are autogenrated by ctemp in tools directory
#
# some notes on the various kinds of parameters
#
# note - spaces between parameters and arguments will cause crashes
#
# divhold
# mode --Set to 1 if the solenoids are fliptronic powered, 0 == normal
# power_pulse_ms --Durations of the pulse for the full power solenoid (in ms) must be divisible by the value used by the schedule_ms parameter
# schedule_ms  --try 32 first as highest number, then experiment down from there
#
# spsol
# ontime - how long the coil is kept on
# offtime - how long the coil is kept off after it was fired, before polling the switch again.  This prevents constant lock-on.
# auto 	- if nonzero, then automatically enable during start ball.

#
##########################################################################
[templates]
Left Sling: driver(sling), sw=SW_LEFT_SLING, sol=SOL_LEFT_SLING, ontime=3, offtime=16
Right Sling: driver(sling), sw=SW_RIGHT_SLING, sol=SOL_RIGHT_SLING, ontime=3, offtime=16
Left Jet: driver(jet), sw=SW_LEFT_JET, sol=SOL_LEFT_JET, ontime=3, offtime=16
Right Jet: driver(jet), sw=SW_RIGHT_JET, sol=SOL_RIGHT_JET, ontime=3, offtime=16
Top Sling: driver(jet), sw=SW_TOP_SLING, sol=SOL_TOP_SLING, ontime=3, offtime=16
Diverter: driver(divhold), power_sol=SOL_DIVERTER_POWER, hold_sol=SOL_DIVERTER_HOLD, mode=0, power_pulse_ms=96, schedule_ms=32, includetest(yes)
SubwayVUK: driver(spsol), sol=SOL_BOTTOM_POPPER, sw=SW_BOTTOM_POPPER, ontime=8, offtime=15, auto=1
TopSol: driver(spsol), sol=SOL_TOP_POPPER, sw=SW_TOP_POPPER, ontime=8, offtime=15, auto=1

Elevator: driver(motorbank2), sol=SOL_ELEVATOR_MOTOR, up_sw_event=SW_ELEVATOR_HOLD, down_sw_event=SW_ELEVATOR_INDEX, initial_position=MOTOR_BANK_DOWN, includetest(yes)

#This probably will not work and will have to write a new driver for this motor
#CryoClawLeft: driver(motorbank), sol=SOL_CLAW_MOTOR_LEFT, up_sw_event=SW_CLAW_POSITION_1, down_sw_event=SW_CLAW_POSITION_2
#CryoClawRight: driver(motorbank), sol=SOL_CLAW_MOTOR_RIGHT, up_sw_event=SW_CLAW_POSITION_2, down_sw_event=SW_CLAW_POSITION_1

# sw_on - the switch number or software event that triggers it
# sw_off - the switch number or software event that triggers it
#ClawMagnet: driver(magnet), sol=CLAW_MAGNET, sw_on=magnet_on, sw_off=magnet_off, mode=1

##########################################################################
# General Illumination
##########################################################################
[gi]
0: Backbox
1: U.Right Playfield
2: U.Left Playfield
3: L.Right Playfield
4: L.Left Playfield

##########################################################################
# Tests
# These are additional test items that should appear in the TESTS menu.
##########################################################################
[tests]
#claw:
diverter: 	#autogenerated-test, see defintion in [templates] section.
elevator:	#autogenerated-test, see defintion in [templates] section.


##########################################################################
# Lampsets
# These denote logical groupings of lamps.
# The key is a textual name for the set.  The options specify which lamps
# are a part of that set.  You can specify 1 or more lamp values,
# separated by commas, where each value can be: 1) a single lamp name,
# 2) another lampset name, already defined, or 3) a lamp range in the
# form lamp1..lampN.
#
# The special construct PF:function indicates that a Perl subroutine
# should be used to select and sort the lamps.  Some functions are
# builtin to genmachine; others can be defined by you and included
# via the 'perlinclude' directive.
#
# Note that a lampset may contain only one lamp.  The lampset is the
# unit of 'allocation' for a lamp effect.
##########################################################################
[lamplists]
Amode All: Right Ramp Awards, Left Ramp Awards, Right Loop Awards, Left Loop Awards, Freeze, Rollovers , Loops, Multiballs, 
Claw, Underground Scoop, Standups, Center Ramp, Car Crash
Amode Rand: Amode All
Sort1: PF:lamp_sort_bottom_to_top
Sort2: PF:lamp_sort_top_to_bottom
Sort3: PF:lamp_sort_left_to_right
Sort4: PF:lamp_sort_right_to_left
Circle Out: PF:lamp_sort_circle_out
Red Lamps: COLOR:red
White Lamps: COLOR:white
Orange Lamps: COLOR:orange
Yellow Lamps: COLOR:yellow
Green Lamps: COLOR:green
Blue Lamps: COLOR:blue
Amber Lamps: COLOR:amber
Right Ramp Awards: Right Ramp Arrow, Right Ramp Jackpot, Right Ramp Explode, Right Ramp Car Chase, Claw Ready
Left Ramp Awards: Left Ramp Arrow, Left Ramp Jackpot, Left Ramp Explode, Left Ramp Car Chase, Quick Freeze
Right Loop Awards: Right Loop Arrow, Right Loop Jackpot, Right Loop Explode
Left Loop Awards: left Loop Arrow, Left Loop Jackpot, Left Loop Explode, Start Multiball, Extra Ball
Freeze: Freeze 1, Freeze 2, Freeze 3, Freeze 4
Rollovers: Middle Rollover, Top Rollover, Lower Rollover
Loops: Left Loop Awards, Right Loop Awards
Multiballs: Fortress Multiball, Museum Multiball, Cryoprison Multiball, Wasteland Multiball
Claw: Claw Capture Simon, Claw Super Jets, Claw Prison Break, Claw Freeze, Claw Acmag 
Underground Scoop: Underground Arrow, Underground Jackpot
Standups: Standup 1, Standup 2, Standup 3, Standup 4, Standup 5
Center Ramp: Center Ramp Arrow, Center Ramp Middle, Center Ramp Outer, Center Ramp Inner, Center Ramp Jackpot
Car Crash: Car Crash Top, Car Crash Center, Car Crash Bottom

##########################################################################
# Containers
# These denote devices that can hold, count, and eject pinballs.
# The key is the name of the device.  The first parameter is the name
# of the solenoid that kicks out a ball.  The 'trough' parameter should
# be specified on the single trough device.  The 'init_max_count'
# parameter should say how many balls are normally held in this device
# at startup.  The remaining parameters are the names of the switches
# that count pinball, in front from entry to exit.
##########################################################################
[containers]
#name: solenoid, switch
Top Sol: Top Popper, Top Popper

Eyeball Eject: Eject, Eject

Subway VUK: Bottom Popper, Bottom Popper

Trough: BallServe, init_max_count(5), Trough 1, Trough 2, Trough 3, Trough 4, Trough 5





#------------------------------------------------------------------------
# The remaining sections describe software aspects, and not the physical
# machine.
#------------------------------------------------------------------------

##########################################################################
# Items for the Feature Adjustments menu.  Parameters indicate the
# type of adjustment and the default value.
##########################################################################
[adjustments]

##########################################################################
# Items for the Feature Audits menu.
##########################################################################
[audits]

##########################################################################
# Sound calls for well-known events
##########################################################################
[system_sounds]

##########################################################################
# Music calls for well-known events
##########################################################################
[system_music]

##########################################################################
# A list of all scores needed by the game rules.
##########################################################################
[scores]
540:
1010:
5K:
5770:
10K:
25K:
50K:
75K:
100K:
150K:
200K:
250K:
500K:
750K:
1M:
2M:
3M:
4M:
5M:
6M:
7M:
8M:
9M:
10M:
11M:
13M:
12M:
15M:
20M:
25M:
30M:
35M:
40M:
45M:
50M:
55M:
60M:
80M:
100M:

##########################################################################
# The default high scores.  Use GC to indicate the grand champion.
# The parameters are the initials followed by the score value.  Periods
# may optionally be used to group digits, but they are not necessary.
# Commas _cannot_ be used for this purpose since they separate parameters.
##########################################################################
[highscores]
GC: DAD, 000.900.000
1: SAM, 000.800.000
2: JOE, 000.700.000
3: ISA, 000.600.000
4: MOM, 000.500.000

##########################################################################
# Per-player bit flags.
##########################################################################
[flags]
IS_ACMAG_ACTIVATED:
IS_CARCHASE_MODE_ACTIVATED:
IS_EXPLODE_MODE_ACTIVATED:

IS_CAPSIM_SIDERAMP_ACTIVATED:
IS_CAPSIM_LEFTRAMP_ACTIVATED:
IS_CAPSIM_RIGHTRAMP_ACTIVATED:
IS_CAPSIM_UNDER_ACTIVATED:
IS_CAPSIM_CENTERRAMP_ACTIVATED:
IS_CAPSIM_LEFTORB_ACTIVATED:
IS_CAPSIM_RIGHTORB_ACTIVATED:

IS_PBREAK_SIDERAMP_ACTIVATED:
IS_PBREAK_LEFTRAMP_ACTIVATED:
IS_PBREAK_RIGHTRAMP_ACTIVATED:
IS_PBREAK_UNDER_ACTIVATED:
IS_PBREAK_CENTERRAMP_ACTIVATED:
IS_PBREAK_LEFTORB_ACTIVATED:
IS_PBREAK_RIGHTORB_ACTIVATED:

IS_COMBO_SIDERAMP_ACTIVATED:
IS_COMBO_LEFTRAMP_ACTIVATED:
IS_COMBO_RIGHTRAMP_ACTIVATED:
IS_COMBO_UNDER_ACTIVATED:
IS_COMBO_CENTERRAMP_ACTIVATED:
IS_COMBO_LEFTORB_ACTIVATED:
IS_COMBO_RIGHTORB_ACTIVATED:

IS_UGROUND_ARROW_ACTIVATED:
IS_L_LOOP_ARROW_ACTIVATED:
IS_R_LOOP_ARROW_ACTIVATED:
IS_L_RAMP_ARROW_ACTIVATED:
IS_C_RAMP_ARROW_ACTIVATED:
IS_S_RAMP_ARROW_ACTIVATED:
IS_R_RAMP_ARROW_ACTIVATED:


#flag_test (FLAG_IS_R_LOOP_ARROW_ACTIVATED)
#flag_off (FLAG_IS_R_LOOP_ARROW_ACTIVATED);

##########################################################################
# System-wide bit flags.
##########################################################################
[globalflags]

##########################################################################
# Display effects
##########################################################################
[deffs]
Acmag Effect: 					page(MACHINE_PAGE), PRI_GAME_QUICK1, D_SCORE+D_RESTARTABLE
Acmag Hit Effect: 				page(MACHINE_PAGE), PRI_GAME_QUICK1, D_SCORE+D_RESTARTABLE
Acmag Start Effect: 			page(MACHINE_PAGE), PRI_GAME_QUICK1, D_SCORE+D_RESTARTABLE
Acmag End Effect: 				page(MACHINE_PAGE), PRI_GAME_QUICK1, D_SCORE+D_RESTARTABLE

Capture Simon Start Effect: 		page(MACHINE_PAGE), PRI_GAME_QUICK1, D_RESTARTABLE
Capture Simon Effect: 			page(MACHINE_PAGE), PRI_GAME_QUICK1, D_SCORE+D_RESTARTABLE
Capture Simon Hit Effect: 		page(MACHINE_PAGE), PRI_GAME_QUICK1, D_SCORE+D_RESTARTABLE
Capture Simon End Effect: 		page(MACHINE_PAGE), PRI_GAME_QUICK1, D_SCORE+D_RESTARTABLE

Car Chase Effect: 				page(MACHINE_PAGE), PRI_GAME_QUICK1, D_RESTARTABLE
Car Chase Hit Effect: 			page(MACHINE_PAGE), PRI_GAME_QUICK1, D_RESTARTABLE
Car Chase Start Effect: 		page(MACHINE_PAGE), PRI_GAME_QUICK1, D_RESTARTABLE
Car Chase End Effect: 			page(MACHINE_PAGE), PRI_GAME_QUICK1, D_RESTARTABLE

Carcrash Effect: 				page(MACHINE_PAGE), PRI_GAME_QUICK1, D_SCORE+D_RESTARTABLE

Explode Start Effect:			page(MACHINE_PAGE), PRI_GAME_QUICK1, D_SCORE+D_RESTARTABLE
Explode Effect: 				page(MACHINE_PAGE), PRI_GAME_QUICK1, D_SCORE+D_RESTARTABLE
Explode Hit Effect: 			page(MACHINE_PAGE), PRI_GAME_QUICK1, D_SCORE+D_RESTARTABLE
Explode End: 					page(MACHINE_PAGE), PRI_GAME_QUICK1, D_SCORE+D_RESTARTABLE

fortress jackpot effect:		page(MACHINE_PAGE), PRI_GAME_QUICK1, D_SCORE+D_RESTARTABLE
fortress start effect:			page(MACHINE_PAGE), PRI_GAME_QUICK1, D_SCORE+D_RESTARTABLE
fortress effect:				page(MACHINE_PAGE), PRI_GAME_QUICK1, D_SCORE+D_RESTARTABLE

Jets Effect: 					page(MACHINE_PAGE), PRI_GAME_QUICK1, D_RESTARTABLE
Jets Completed Effect: 			page(MACHINE_PAGE), PRI_GAME_QUICK1, D_RESTARTABLE
Superjets Effect: 				page(MACHINE_PAGE), PRI_GAME_QUICK1, D_RESTARTABLE
Superjets Completed Effect: 	page(MACHINE_PAGE), PRI_GAME_QUICK1, D_RESTARTABLE


Prison Break Effect: 			page(MACHINE_PAGE), PRI_GAME_QUICK1, D_SCORE+D_RESTARTABLE
Prison Break Hit Effect: 		page(MACHINE_PAGE), PRI_GAME_QUICK1, D_SCORE+D_RESTARTABLE
Prison Break Start Effect: 		page(MACHINE_PAGE), PRI_GAME_QUICK1, D_SCORE+D_RESTARTABLE
Prison Break End Effect: 		page(MACHINE_PAGE), PRI_GAME_QUICK1, D_SCORE+D_RESTARTABLE



#############custom_deffs.c###############################################################
Ball Save Effect:				page(MACHINE_PAGE), PRI_GAME_QUICK1, D_SCORE+D_RESTARTABLE
Shoot Reminder Effect:			page(MACHINE_PAGE), PRI_GAME_QUICK1, D_SCORE+D_RESTARTABLE
Troubleshooting:				page(MACHINE_PAGE), PRI_GAME_QUICK1, D_SCORE+D_RESTARTABLE
Freeze Effect: 					page(MACHINE_PAGE), PRI_GAME_QUICK1, D_SCORE+D_RESTARTABLE
Clw Inlanes Effect: 			page(MACHINE_PAGE), PRI_GAME_QUICK1, D_SCORE+D_RESTARTABLE
Qf Inlanes Effect: 				page(MACHINE_PAGE), PRI_GAME_QUICK1, D_SCORE+D_RESTARTABLE
Eject Effect: 					page(MACHINE_PAGE), PRI_GAME_QUICK1, D_SCORE+D_RESTARTABLE
Underground Effect: 			page(MACHINE_PAGE), PRI_GAME_QUICK1, D_SCORE+D_RESTARTABLE
Rollovers Effect: 				page(MACHINE_PAGE), PRI_GAME_QUICK1, D_SCORE+D_RESTARTABLE
All Rollovers Effect: 			page(MACHINE_PAGE), PRI_GAME_QUICK1, D_SCORE+D_RESTARTABLE
#############custom_deffs.c###############################################################


standupFrenzy start effect: 	page(MACHINE_PAGE), PRI_GAME_QUICK1, D_SCORE+D_RESTARTABLE
Standupfrenzytotalscore Effect: page(MACHINE_PAGE), PRI_GAME_QUICK1, D_SCORE+D_RESTARTABLE
Standupfrenzy Mode Effect: 		page(MACHINE_PAGE), PRI_GAME_QUICK1, D_SCORE+D_RESTARTABLE
StandupfrenzyHit Effect: 		page(MACHINE_PAGE), PRI_GAME_QUICK1, D_SCORE+D_RESTARTABLE
Standup Effect: 				page(MACHINE_PAGE), PRI_GAME_QUICK1, D_SCORE+D_RESTARTABLE



##########################################################################
# Lamp effects
##########################################################################
[leffs]

##########################################################################
# Fonts used in this game.
# these are in addition to default fonts
# default fonts do not have to be listed (they are declared under wpc-common.md) and are:
# font_mono5
# font_mono9
# font_num5x7
# font_lucida9			---good only for very short words
# font_tinynum
# font_var5			---small text messages
# font_fixed6 		---standups and standup frenzy
# font_fixed10 		---fortress MB
# font_cu17				---good for large single letters only
# font_term6     	---car crash and car chase and many smaller lines
# font_times8		---capture simon
# font_bitmap8
#
# The number at the end doesn't correspond to their point size like you would think.
# The best thing to do is look at the actual font in the development menu
# Here are others that are available - from the fonts directory

#pcsenior:			---good only for very short words
#lithograph:	---EXPLODE, amode title, ball save, shoot reminder
#times10:
#steel:			---ACMAG
#addlg:				---good only for very short words
#Adore64:		---freeze  ---remember to drop the cap A when using this in your code
#arcadepi:
#micron55:
#pixchicago:
#px_sans_nouveaux:
#twizone:
#uni05_54:
#uni05_63:
#uni05_64:
#v5prc:			---prison break
#xpaiderp:
#fixed12:

#these won't compile
#amiga4ever:
#arial11:
#arial11a:
#c64:
#miscfixed:
#luctype:

#these don't look good or don't show up
#ffextra:		---these are not letters, but symbols, like wingdings
#utopia:		---looks garbled
#schu:			---looks garbled
#joystix:		---don't show
#ambitsek:		---don't show
#uni05_53:
#v5prd:
#v5prf:

##########################################################################
#can pick, at most, 4 to 6 extra fonts, depending on file size, outside of the system fonts without
#receiving compile errors for overflowing the fonts page 
[fonts]
lithograph:
steel:
v5prc: