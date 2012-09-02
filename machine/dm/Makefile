#
# Machine-specific Makefile
#
MACHINE_FILE = dm.md

# Define the name of the GAME_ROM.  This is
# optional; you can delete it and a suitable
# default will be taken.
GAME_ROM_PREFIX = fdm

# Set ROM_PAGE_COUNT to the number of 16KB pages
# that should be present in the output image.
# The hardware supports a maximum of 64 pages;
# you can't exceed this.
#
# The 'system' will always reside in the uppermost
# two pages.  If the ROM is less than a full 64
# pages, then the pages are numbered from [N..64]
# anyway.
#
# 128KB.....8 pages
# 256KB.....16 pages
# 512KB.....32 pages
# 1MB...... 64 pages
#
ROM_PAGE_COUNT = 32

#MACHINE_MAJOR = 0
#MACHINE_MINOR = 62

#################################################
#
# Simulation options.
#
# These options control the way that the game
# ROM is made available to pinmame for testing.
# They are only used during installation into
# the pinmame roms directory.
#
#################################################
#PINMAME_MACHINE = dm_5_1
#PINMAME_GAME_ROM = dm_5_1.rom
#PINMAME_OTHER_ROMS = dmu*.rom

#
# Object files exported by this game
#
# GAME_OBJS RTTs must go here.  also place files that are called often here.
# 
# GAME_TEST_OBJS = page 58 = 	All feature tests can go here.  
# 
# GAME_PAGED_OBJS = page 59 =	Preferably, game functions go here.
# This works safely for any file that only uses CALLSET_ENTRY functions, deffs, and leffs.  
# 
# GAME2_OBJS = page 55
# GAME3_OBJS = page 54
# GAME4_OBJS = page 53
# GAME5_OBJS = page 52
#
# note: if we put tabs in between the file names you will get a linker error
# note: continue line with a '\' --make sure to leave off on last line or you will get a linker error

#test 1	good ---call_tester.o inlanes.o rollovers.o simple_switches.o  
#test 2 good ---call_tester.o inlanes.o rollovers.o simple_switches.o orbits.o ramps.o   
#test 3 fail ---call_tester.o inlanes.o rollovers.o simple_switches.o orbits.o ramps.o acmag.o 	capture_simon.o 
#test 3 good ---call_tester.o inlanes.o rollovers.o simple_switches.o orbits.o ramps.o 			capture_simon.o prison_break.o jets_superjets.o   
#test 4 good ---call_tester.o inlanes.o rollovers.o simple_switches.o orbits.o ramps.o 			capture_simon.o prison_break.o jets_superjets.o   
#								lock_freeze_mbstart.o fortressMB.o sound_handler.o




GAME_OBJS =	orbits.o   	ramps.o   
#	call_tester.o  

GAME2_OBJS = lock_freeze_mbstart.o   fortressMB.o  sound_handler.o car_crash.o underground.o     eyeball_explode.o capture_simon.o  prison_break.o   jets_superjets.o   car_chase_mode.o   acmag.o   standupfrenzy.o inlanes.o   rollovers.o   simple_switches.o  


GAME_TEST_OBJS =

GAME_INCLUDES =

GAME_PAGED_OBJS =    
