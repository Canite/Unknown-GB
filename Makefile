#
# A Makefile that compiles all .c and .s files in "src" and "res" 
# subdirectories and places the output in a "obj" subdirectory
#

# If you move this project you can change the directory 
# to match your GBDK root directory (ex: GBDK_HOME = "C:/GBDK/"
ifdef OS
	GBDK_HOME = "F:/gbdk/"
	RGBDS_HOME = "F:/GBDev/rgbds-0.6.1-win64/"
	HUGE_HOME = "F:/hugetracker"
	EMULATOR_gb = "F:/bgb/bgb.exe"
	EMULATORFLAGS_gb = 
	EMULATOR_nes = 
	EMULATORFLAGS_nes = 
else
	GBDK_HOME = /home/canight/gbdk/
	RGBDS_HOME = /home/canight/rgbds
	HUGE_HOME = /home/canight/hugetracker
	EMULATOR_gb = java -jar /home/canight/Downloads/emulicious/Emulicious.jar
	EMULATORFLAGS_gb = 
	EMULATOR_nes = 
	EMULATORFLAGS_nes = 
endif

TARGETS = gb# nes

# Configure platform specific LCC flags here:
LCCFLAGS_gb      = -Wl-yt0x1B # Set an MBC for banking (1B-ROM+MBC5+RAM+BATT)
LCCFLAGS_pocket  = -Wl-yt0x1B # Usually the same as required for .gb
LCCFLAGS_duck    = -Wl-yt0x1B # Usually the same as required for .gb
LCCFLAGS_gbc     = -Wl-yt0x1B -Wm-yc # Same as .gb with: -Wm-yc (gb & gbc) or Wm-yC (gbc exclusive)
LCCFLAGS_nes 	 = -mmos6502:nes -Wm-yoA

LCC = $(GBDK_HOME)bin/lcc
PNGCONVERTER = $(GBDK_HOME)bin/png2asset
PNGCONVERTERFLAGS = -tiles_only -spr8x8 -noflip -keep_palette_order -repair_indexed_pal -no_palettes -b 255
RGBASM = $(RGBDS_HOME)/rgbasm
UGE2SOURCE = $(HUGE_HOME)/uge2source

EXTDIR 		= ext

# You can set flags for LCC here
CFLAGS += -Iinclude -Iext -debug 
LCCFLAGS += $(LCCFLAGS_$(EXT))
LCCFLAGS += -Wl-j -Wm-ya4 -autobank -Wb-ext=.rel -Wb-v
LCCFLAGS += -Wl-l$(EXTDIR)/hUGEDriver.lib
# LCCFLAGS += -v     # Uncomment for lcc verbose output

RGB2SDAS = python $(UTILDIR)/rgb2sdas.py

# You can set the name of the .gb ROM file here
PROJECTNAME    = SlimeTrials

SRCDIR      = src
OBJDIR      = obj/$(EXT)
RESDIR      = res
UTILDIR     = utils

BINS	    = $(OBJDIR)/$(PROJECTNAME).$(EXT)
CSOURCES    = $(foreach dir,$(SRCDIR),$(notdir $(wildcard $(dir)/*.c))) $(foreach dir,$(RESDIR),$(notdir $(wildcard $(dir)/*.c)))
ASMSOURCES  = $(foreach dir,$(SRCDIR),$(notdir $(wildcard $(dir)/*.asm)))
EXTASMSOURCES = $(foreach dir,$(EXTDIR),$(notdir $(wildcard $(dir)/*.asm)))
IMAGEFILES  = $(foreach dir,$(RESDIR),$(notdir $(wildcard $(dir)/*.png)))
TILEDFILES  = $(foreach dir,$(RESDIR),$(notdir $(wildcard $(dir)/*.tmj)))
HUGEFILES   = $(foreach dir,$(RESDIR),$(notdir $(wildcard $(dir)/*.uge)))
SFXFILES   = $(foreach dir,$(RESDIR),$(notdir $(wildcard $(dir)/*.sav)))
OBJS        = $(CSOURCES:%.c=$(OBJDIR)/%.o) $(ASMSOURCES:%.asm=$(OBJDIR)/%.o) $(EXTASMSOURCES:%.asm=$(OBJDIR)/%.o)
RESOBJS		= $(IMAGEFILES:%.png=$(RESDIR)/%.c) $(TILEDFILES:%.tmj=$(RESDIR)/%.c) $(HUGEFILES:%.uge=$(RESDIR)/%.c) $(SFXFILES:%.sav=$(RESDIR)/%.c)

all:	prepare $(TARGETS)

compile.bat: Makefile
	@echo "REM Automatically generated from Makefile" > compile.bat
	@make -sn | sed y/\\//\\\\/ | sed s/mkdir\ -p\/mkdir\/ | grep -v make >> compile.bat

# Compile .png files in "res/" to bin files
$(RESDIR)/%.c:  $(RESDIR)/%.png
	$(PNGCONVERTER) $< $(PNGCONVERTERFLAGS) -o $@
	$(LCC) $(CFLAGS) -c -o $(OBJDIR)/$(*F).o $@

$(RESDIR)/%.c: 	$(RESDIR)/%.tmj
	$(UTILDIR)/convert_tile_json.py -i $< -o $@
	$(LCC) $(CFLAGS) -c -o $(OBJDIR)/$(*F).o $@

$(RESDIR)/%.c: $(RESDIR)/%.uge
	$(UGE2SOURCE) $< -b 255 $(*F) $@
	$(UTILDIR)/create_huge_header.py -i $@
	$(LCC) $(CFLAGS) -c -o $(OBJDIR)/$(*F).o $@

$(RESDIR)/%.c: $(RESDIR)/%.sav
	$(UTILDIR)/fxhammer2data.py -o $@ $<
	$(LCC) $(CFLAGS) -c -o $(OBJDIR)/$(*F).o $@

# Compile .c files in "src/" to .o object files
$(OBJDIR)/%.o:	$(SRCDIR)/%.c
	$(LCC) $(CFLAGS) -c -o $@ $<

# Compile .c files in "res/" to .o object files
$(OBJDIR)/%.o:	$(RESDIR)/%.c
	$(LCC) $(CFLAGS) -c -o $@ $<

# Compile .s assembly files in "src/" to .o object files
$(OBJDIR)/%.o:	$(SRCDIR)/%.asm
	$(LCC) $(CFLAGS) -c -o $@ $<

$(OBJDIR)/%.o:	$(OBJDIR)/%.obj
	$(RGB2SDAS) $(RGB2SDASFLAGS) -o$@ $<

# Compile external .s assembly files in "src/" to .o object files
$(OBJDIR)/%.obj:	$(EXTDIR)/%.asm
	$(RGBASM) -i.. -DGBDK -o$@ $<

# If needed, compile .c files in "src/" to .s assembly files
# (not required if .c is compiled directly to .o)
$(OBJDIR)/%.s:	$(SRCDIR)/%.c
	$(LCC) $(CFLAGS) -S -o $@ $<

# Link the compiled object files into a .gb ROM file
$(BINS):	$(RESOBJS) $(OBJS)
	$(LCC) $(LCCFLAGS) $(CFLAGS) -o $(BINS) $(OBJS)

prepare:
	mkdir -p $(OBJDIR)

clean:
#	rm -f  *.gb *.ihx *.cdb *.adb *.noi *.map
	rm -rf  $(OBJDIR)/*
	rm -f  $(RESDIR)/*.c
	rm -f  $(RESDIR)/*.h
	rm -f  $(RESDIR)/*.2bpp

run:
	$(EMULATOR_$(EXT)) $(EMULATORFLAGS_$(EXT)) $(OBJDIR)/$(PROJECTNAME).$(EXT)

include Makefile.targets
