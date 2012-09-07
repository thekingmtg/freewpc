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
GAME2_OBJS = 
GAME_TEST_OBJS =
GAME_INCLUDES =


#all files here have NO deffs and may be frequently called
GAME_OBJS =	inlanes.o   eyeball.o   eject.o   lock_freeze_mbstart.o   orbits.o   \
ramps.o     rollovers.o   simple_switches.o   \
underground.o        
#	call_tester.o  

#all files here have NO deffs and are infrequently called
GAME3_OBJS = ball_handling.o   sound_handler.o  

#all files here that have deffs
GAME_PAGED_OBJS = acmag.o   capture_simon.o   car_chase_mode.o   car_crash.o \
custom_deffs.o   dm_amode.o   explode.o \
fortressMB.o \
jets_superjets.o   prison_break.o   standupfrenzy.o 



#listing of images used
IMAGE_MAP += $(M)/dm.ild


