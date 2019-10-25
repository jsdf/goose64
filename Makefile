
include $(ROOT)/usr/include/make/PRdefs

N64KITDIR    = c:\nintendo\n64kit
NUSYSINCDIR  = $(N64KITDIR)/nusys/include
NUSYSLIBDIR  = $(N64KITDIR)/nusys/lib
NUSTDINCDIR = $(N64KITDIR)/nustd/include
NUSTDLIBDIR = $(N64KITDIR)/nustd/lib



LIB = $(ROOT)/usr/lib
LPR = $(LIB)/PR
INC = $(ROOT)/usr/include
CC  = gcc
LD  = ld
MAKEROM = mild

LCDEFS =	-DNU_DEBUG -DF3DEX_GBI_2 -D__N64__
LCINCS =	-I. -nostdinc -I- -I$(NUSTDINCDIR) -I$(NUSYSINCDIR) -I$(ROOT)/usr/include/PR
LCOPTS =	-G 0
LDFLAGS = $(MKDEPOPT) -L$(LIB)  -L$(NUSYSLIBDIR) -L$(NUSTDLIBDIR)  -lnusys_d -lnustd -lgultra_d -L$(GCCDIR)/mipse/lib -lkmc

OPTIMIZER =	-g

APP =		goose64.out

TARGETS =	goose64.n64

HFILES =	main.h graphic.h testingCube.h vec3d.h vec2d.h gameobject.h game.h modeltype.h renderer.h input.h character.h player.h gameutils.h gametypes.h item.h

CODEFILES   = 	main.c stage00.c graphic.c gfxinit.c om_mem_heap.c vec3d.c vec2d.c gameobject.c game.c renderer.c input.c character.c player.c gameutils.c item.c

CODEOBJECTS =	$(CODEFILES:.c=.o)  $(NUSYSLIBDIR)/nusys.o

DATAFILES   =	

DATAOBJECTS =	$(DATAFILES:.c=.o)

CODESEGMENT =	codesegment.o

OBJECTS =	$(CODESEGMENT) $(DATAOBJECTS)


default:        $(TARGETS)

include $(COMMONRULES)

$(CODESEGMENT):	$(CODEOBJECTS) Makefile $(HFILES)
		$(LD) -o $(CODESEGMENT) -r $(CODEOBJECTS) $(LDFLAGS)

$(TARGETS):	$(OBJECTS)
		$(MAKEROM) spec -I$(NUSYSINCDIR) -r $(TARGETS) -e $(APP)

