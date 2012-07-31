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
Title: Demolition Man
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

##########################################################################
# Use 'define' to emit a plain #define for anything not covered by
# some other means.
##########################################################################
#define MACHINE_LACKS_ALL_INCLUDES
define MACHINE_HAS_UPPER_LEFT_FLIPPER
#define MACHINE_SOL_EXTBOARD1
#define MACHINE_CUSTOM_AMODE
#define MACHINE_SCORE_DIGITS
#define MACHINE_MUSIC_GAME
#define MACHINE_MUSIC_PLUNGER
#define MACHINE_REPLAY_CODE_TO_SCORE
#define MACHINE_DEBUGGER_HOOK
#define MACHINE_REPLAY_SCORE_CHOICES 10
#define MACHINE_OUTHOLE_KICK_HOOK
#define MACHINE_BALL_SAVE_TIME             0
#define MACHINE_MAX_BALLS                  2

##########################################################################
# Lamp Description
# The key is given in column/row format.  The first parameter must be
# the lamp name.  Optionally you can specify a color, and x() and y()
# for the location on the playfield.  Only a subset of lamp colors are
# recognized; see tools/genmachine for details.  Lamp location is
# given in terms of the Lamp-Matrix-Width and Lamp-Matrix-Height.
##########################################################################

#TODO colors of lights 
[lamps]
11: Ball Save, amber ,x(28), y( 6)
12: Fortress Multiball, amber ,x(33), y( 9)
13: Museum Multiball, amber ,x(31), y( 9)
14: Cryoprison Multiball, amber ,x(29), y( 9)
15: Wasteland Multiball, amber ,x(30), y( 9)
16: Shoot Again, amber ,x(34), y( 9)
17: Access Claw, amber ,x(27), y(1)
18: Left Ramp Explode, amber ,x(22), y( 9)
21: Right Ramp Jackpot, red ,x(22), y(14)
22: Right Loop Explode, amber ,x(23), y(15)
23: Light Quick Freeze, amber ,x(27), y(15)
24: Freeze 4, amber ,x(26), y(14)
25: Claw Ready, amber ,x(25), y(12)
26: Freeze 3, amber ,x(28), y(12)
27: Freeze 2, amber ,x(28), y( 6)
28: Freeze 1, red ,x(26), y( 2)
31: Right Loop Jackpot, red ,x(21), y(16)
32: Standup 5, red, x(19), y(15)
33: Right Ramp Arrow, white ,x(19), y(14)
34: Left Ramp Jackpot, white ,x(20), y( 8)
35: Left Loop Jackpot, white ,x(19), y( 6)
36: Car Crash Top, red ,x(20), y( 2)
37: Standup 1, yellow ,x(21), y( 5)
38: Car Crash Center, white ,x(22), y( 3)
41: Right Ramp Explode, yellow ,x(23), y(14)
42: Right Ramp Car Chase, yellow ,x(24), y(13)
43: Quick Freeze, yellow ,x(24), y(10)
44: Left Ramp Car Chase, yellow ,x(23), y( 9)
45: Extra Ball, yellow ,x(24), y( 9)
46: Start Multiball, red ,x(23), y( 8)
47: Car Crash Bottom, red , shoot-again, x(23), y( 4)
48: Left Loop Explode, white ,x(22), y( 7)
51: Underground Arrow, white ,x(11), y(10)
52: Underground Jackpot, white ,x(12), y( 9)
53: Standup 2, orange ,x(13), y( 8)
54: Left Ramp Arrow, white ,x(14), y( 7)
55: Side Ramp Jackpot, white ,x(16), y( 9)
56: Side Ramp Arrow, orange ,x(15), y( 9)
57: left Loop Arrow, red , extra-ball, x(14), y( 4)
58: Center Ramp Jackpot, orange ,x(10), y( 9)
61: Claw Capture Simon,red, x( 1), y( 7)
62: Claw Super Jets,orange, x( 3), y( 6)
63: Claw Prison Break,yellow, x( 4), y( 5 )
64: Claw Freeze, yellow ,x( 5), y( 2)
65: Claw Acmag, yellow ,x( 5), y( 1)
66: Middle Rollover, red ,x( 2), y(14)
67: Top Rollover, white ,x( 2), y(15)
68: Lower Rollover, orange ,x( 2), y(17)
71: Super Jackpot,yellow, x( 7), y(11)
72: Computer,yellow, x( 8), y(11)
73: Demo Time, yellow, x( 9), y(11)
74: Not Used
75: Not Used 
76: Standup 4, yellow, x(18), y(13)
77: Standup 3, orange, x(17), y(10)
78: Retina Scan, red, x(23), y( 1)
81: Center Ramp Middle, yellow ,x( 6), y( 9)
82: Center Ramp Outer, white ,x( 6), y( 8)
83: Center Ramp Inner, yellow ,x( 6), y( 9)
84: Center Ramp Arrow, red ,x( 9), y( 9)
85: Right Loop Arrow, yellow ,x(16), y(18)
86: Buy In Button, yellow, buyin, cabinet, x(33), y(18)
87: Ball Launch, yellow, launch, cabinet, x(34), y(16)
88: Start Button, yellow, start, cabinet, x( 1), y( 1)

##########################################################################
# Switch Description
# The key is in column/row format.  The first parameter must be the switch
# name.  Options can be given in any order:
#    ingame - only service the switch during a game
#    intest - also service the switch in test mode
#    noplay - tripping this switch does NOT mark ball in play
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
11: Ball Launch, launch-button, noplay, sound(EXPLOSION)
12: Left Handle Button, ingame, noplay
13: Start Button, start-button, intest, noplay, debounce(TIME_100MS)
14: Tilt, tilt, ingame, noplay, cabinet, sound(TILT)
15: Left Outlane, ingame, sound(OUTLANE_PIANO)
16: Left Inlane, ingame
17: Right Inlane, ingame
18: Right Outlane, ingame, sound(OUTLANE_PIANO)
21: Slam Tilt, slam-tilt, ingame, noplay, cabinet, sound(TILT)
22: Coin Door Closed, noplay
23: Buyin Button, buyin-button, noplay
24: Always Closed, noplay
25: Claw Position 1, opto, noscore, noplay
26: Claw Position 2, opto, noscore, noplay
27: Shooter Lane, shooter, edge, noscore, debounce(TIME_200MS), noplay
28: Not Used, noplay
31: Trough 1, noscore, noplay, opto
32: Trough 2, noscore, noplay, opto
33: Trough 3, noscore, noplay, opto
34: Trough 4, noscore, noplay, opto
35: Trough 5, noscore, noplay, opto
36: Trough Jam, noscore, noplay, opto
37: Not Used, noplay
38: Standup 5, ingame, standup
41: Left Sling, ingame, c_decl(sw_sling), noplay 
42: Right Sling, ingame, c_decl(sw_sling), noplay
43: Left Jet, ingame, c_decl(sw_jet), noplay
44: Top Sling, ingame, c_decl(sw_jet), noplay
45: Right Jet, ingame, c_decl(sw_jet), noplay
46: Right Ramp Enter, ingame, sound(RACE_BY_LONG)
47: Right Ramp Exit, ingame
48: Right Freeway, ingame
51: Left Ramp Enter, ingame, sound(RACE_BY)
52: Left Ramp Exit, ingame
53: Center Ramp, ingame, sound(RACE_BY_SHORT)
54: Upper Rebound, ingame, noplay
55: Left Loop, ingame 
56: Standup 2, standup, ingame
57: Standup 3, standup, ingame
58: Standup 4, standup, ingame
61: Side Ramp Enter, ingame
62: Side Ramp Exit, ingame
63: Left Rollover, ingame
64: Center Rollover, ingame
65: Right Rollover, ingame
66: Eject, ingame, sound(EJECT)
67: Elevator Index, ingame, noplay, opto
68: Not Used, noplay
71: Chase Car 1, opto, noplay, ingame
72: Chase Car  2, opto, noplay, ingame
73: Top Popper, opto, ingame, noplay
74: Elevator Hold, opto, noplay
75: Elevator Ramp, ingame
76: Bottom Popper, opto, ingame
77: Eyeball Standup, standup, ingame
78: Standup 1, standup, ingame
81: Claw Capture Simon, noplay, ingame
82: Claw Super Jets, noplay, ingame
83: Claw Prison Break, noplay, ingame
84: Claw Freeze, noplay, ingame
85: Claw Acmag, noplay, ingame
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
#    knocker, ballserve
#
##########################################################################
# THIS IS LIST IS SHOWN IN THE SAME ORDER AS THE SOLENOID/FLASHER TABLE
[drives]
#H = high power J130
H1: Ball Release, duty(SOL_DUTY_25), time(TIME_200MS)
H2: Bottom Popper, time(TIME_200MS)
H3: Auto Plunger, nosearch, launch, duty(SOL_DUTY_100), time(TIME_200MS)
H4: Top Popper, time(TIME_100MS)
H5: Diverter Power, duty(SOL_DUTY_50), time(TIME_133MS)
H6: Not Used
H7: Knocker, knocker, time(TIME_16MS)

#L = low power J127
L1: Left Sling, duty(SOL_DUTY_100)
L2: Right Sling, duty(SOL_DUTY_100)
L3: Left Jet, duty(SOL_DUTY_100)
L4: Top Sling, duty(SOL_DUTY_100)
L5: Right Jet, duty(SOL_DUTY_100)
L6: Eject, duty(SOL_DUTY_50)
L7: Diverter Hold, nosearch
L8: : Not Used

# G = J126 on Power Driver Board
G1: Claw Flasher, flash, nosearch
G2: Elevator Motor, motor, nosearch
G3: Claw Motor Left, motor, nosearch
G4: Claw motor Right, motor, nosearch
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


[templates]
Left Sling: driver(sling), sw=SW_LEFT_SLING, sol=SOL_LEFT_SLING, ontime=3, offtime=16
Right Sling: driver(sling), sw=SW_RIGHT_SLING, sol=SOL_RIGHT_SLING, ontime=3, offtime=16
Left Jet: driver(jet), sw=SW_LEFT_JET, sol=SOL_LEFT_JET, ontime=3, offtime=16
Right Jet: driver(jet), sw=SW_RIGHT_JET, sol=SOL_RIGHT_JET, ontime=3, offtime=16
Top Sling: driver(jet), sw=SW_TOP_SLING, sol=SOL_TOP_SLING, ontime=3, offtime=16

#Clock Mech: driver(bivar),
#	forward_sol=SOL_CLOCK_FORWARD,
#	reverse_sol=SOL_CLOCK_REVERSE


Ramp Div: driver(duty2),
	sol=SOL_DIVERTER_POWER, timeout=TIME_1S, ontime=TIME_33MS, duty_mask=DUTY_MASK_50

#Eject: driver(eject), sol=SOL_EJECT, swno=SW_EJECT, swevent=sw_eject




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
#magnet:
#claw:

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
Computer: Top Popper, \
	Top Popper

Eyeball Eject: Eject, \
	Eject

Subway VUK: Bottom Popper, \
	Bottom Popper

Trough: Ball Release, init_max_count(5), \
	Trough 1, Trough 2, Trough 3, Trough 4, Trough 5

#Elevator:

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
9M:
10M:
11M:
13M:
15M:
20M:
30M:
40M:
50M:
100M:

##########################################################################
# The default high scores.  Use GC to indicate the grand champion.
# The parameters are the initials followed by the score value.  Periods
# may optionally be used to group digits, but they are not necessary.
# Commas _cannot_ be used for this purpose since they separate parameters.
##########################################################################
[highscores]
GC: DAD, 000.000.000
1: SAM, 000.000.000
2: JOE, 000.000.000
3: ISA, 000.000.000
4: MOM, 000.000.000

##########################################################################
# Per-player bit flags.
##########################################################################
[flags]

##########################################################################
# System-wide bit flags.
##########################################################################
[globalflags]

##########################################################################
# Display effects
##########################################################################
[deffs]

##########################################################################
# Lamp effects
##########################################################################
[leffs]

##########################################################################
# Fonts used in this game.
##########################################################################
[fonts]
