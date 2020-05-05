# currently doesn't seem to produce working build
# to use this, first source ~/code/n64cc/env.sh
# then use make

DRIVE_C = /Users/jfriend/.wine/drive_c
ROOT = $(DRIVE_C)/ultra
CC = mips64-elf-gcc
LD = mips64-elf-ld
INC = $(ROOT)/usr/include
EXEGCC_INC = $(ROOT)/GCC/MIPSE/INCLUDE

LIB = $(ROOT)/usr/lib

N64KITDIR = $(DRIVE_C)/nintendo/n64kit
NUSYSINCDIR = $(N64KITDIR)/nusys/include
NUSYSLIBDIR = $(N64KITDIR)/nusys/lib
NUSTDINCDIR = $(N64KITDIR)/nustd/include
NUSTDLIBDIR = $(N64KITDIR)/nustd/lib

LCDEFS = -DNU_DEBUG -DF3DEX_GBI_2 -DN_AUDIO  -D__N64__
LCINCS = -I. -I$(NUSYSINCDIR)  -I$(NUSTDINCDIR) -I$(ROOT)/usr/include/PR
LCOPTS = -G 0
LDFLAGS = -L$(LIB) -L$(NUSYSLIBDIR) -L$(NUSTDLIBDIR)  -lnusys_d -lgultra_d # -L$(GCCDIR)/mipse/lib -lkm
GCCFLAG = -c -I$(INC) -I$(EXEGCC_INC) -D_MIPS_SZLONG=32 -D_MIPS_SZINT=32 -D_LANGUAGE_C -D_ULTRA64 -D__EXTENSIONS__ -mabi=32 -march=vr4300 -mtune=vr4300
CFLAGS = $(LCDEFS) $(LCINCS) $(LCOPTS) $(GCCFLAG) $(OPTIMIZER)

ED64CODEFILES = ed64io_usb.c ed64io_sys.c ed64io_everdrive.c ed64io_fault.c

CODEFILES   =   main.c stage00.c graphic.c gfxinit.c vec3d.c vec2d.c gameobject.c game.c modeltype.c renderer.c input.c character.c characterstate.c player.c gameutils.c item.c animation.c physics.c rotation.c collision.c  pathfinding.c frustum.c  university_map_graph.c sprite.c $(ED64CODEFILES)

CODEOBJECTS = $(CODEFILES:.c=.o)  $(NUSYSLIBDIR)/nusys.o

DATAFILES   = mem_heap.c trace.c university_map_collision.c models.c sprite_data.c
DATAOBJECTS = $(DATAFILES:.c=.o)

CODESEGMENT = codesegment.o

OBJECTS = $(CODESEGMENT) $(MODELSSEGMENT) $(DATAOBJECTS)


default:        $(TARGETS)

# include $(COMMONRULES)
.c.o:
	$(CC) $(CFLAGS) $<

clean: 
	rm -f *.o


clobber:
	rm -f *.o *.n64 *.out


$(CODESEGMENT): $(CODEOBJECTS) GNUMakefile $(HFILES)
# use -M to print memory map from ld
	$(LD) -o $(CODESEGMENT) -r $(CODEOBJECTS) $(LDFLAGS) 


$CC $CFLAGS -g $@
