#################################################
#
# Machine-specific Makefile
#
#################################################
MACHINE_FILE = dm.md

#################################################
# Define the name of the GAME_ROM.  This is
# optional; you can delete it and a suitable
# default will be taken.
#################################################
GAME_ROM_PREFIX = dman_lx4


#################################################
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
#################################################
ROM_PAGE_COUNT = 32


#################################################
# rev number for screen in format X.XX - must be an octal number
#################################################
MACHINE_MAJOR = 0
MACHINE_MINOR = 2

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
PINMAME_MACHINE = dm_lx4
PINMAME_GAME_ROM = dman_lx4.rom
#PINMAME_OTHER_ROMS = dmu*.rom



#################################################
# Object files exported by this game
#
# GAME_OBJS  = .text page -- 
# the system page.  No decorator needed on function prototypes.
# RTTs must go here.  also place files that are called often here.
#
#
# GAME_PAGED_OBJS = page 59 =	Preferably, game functions go here.
# This works safely for any file that only uses CALLSET_ENTRY functions, deffs, and leffs.  
# GAME_PAGED_OBJS (probably should have been called GAME1_OBJS) is page 
# 59 (labelled MACHINE in the display at the bottom of the "err" file). 
# Prototypes need __machine__.  The space is shared with "EVENT", which 
# means that it is not as large as the other pages, but since "EVENT" is 
# the machinery behind callsets, files which have lots of CALLSET_ENTRY 
# points are more efficient when placed here. 
#
# 
# GAME_TEST_OBJS = page 58 = 	All feature tests can go here.  
# 
# 
# GAME2_OBJS = page 55, labelled MACHINE2, prototypes use __machine2__
# GAME3_OBJS = page 54, labelled MACHINE3, prototypes use __machine3__
# GAME4_OBJS = page 53
# GAME5_OBJS = page 52
#
# note: if we put tabs in between the file names you will get a linker error
# note: continue line with a '\' --make sure to leave off on last line or you will get a linker error
#GAME_TEST_OBJS =
#GAME_INCLUDES =
#################################################

#only uses CALLSET_ENTRY functions, deffs, and leffs - no function calls
#this is about 30% full from the OS - page 59 
#also called MACHINE_PAGE - 
GAME_PAGED_OBJS = 

# page .text
GAME_OBJS = simple_switches.o  orbits.o  ramps.o 
#call_tester.o \


#all files here that have deffs and are less frequently called
#also called MACHINE2_PAGE - page 55
GAME2_OBJS = rollovers.o car_chase_mode.o acmag.o startup_shutdown.o dm_amode.o underground.o  \
jackpots.o arrow_handler.o  inlanes.o car_crash.o combos.o eject.o explode.o custom_deffs.o 

#all files here that have deffs and are less frequently called
#also called MACHINE3_PAGE - page 54
GAME3_OBJS = bonus.o capture_simon.o prison_break.o \
fortress.o museum.o cryoprison.o wasteland.o demotime.o lock_freeze_mbstart.o cryoclaw.o 


#all files here that have deffs and are less frequently called
#also called MACHINE4_PAGE - page 53
GAME4_OBJS = bar.o dm_millions.o wheelie.o  top_popper.o  standupfrenzy.o 

#also called MACHINE5_PAGE - page 52
GAME5_OBJS = jets.o  superjets.o eyeball.o 

#################################################
#listing of images used
#################################################
IMAGE_MAP += $(M)/dm.ild


