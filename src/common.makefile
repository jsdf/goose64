# shared config between classic and modern makefiles

# include overrides file from local repo
ifndef RELEASE
# not using -include method because it doesn't work in old make version
ifneq ("$(wildcard localdefs.makefile)","")
$(info using localdefs.makefile)
include localdefs.makefile
else
$(info no localdefs.makefile)
endif
endif

# this improves CPU perf (but we're usually RDP-bound)
# OPTIMIZE = y 
ifdef RELEASE
ED64 =
OPTIMIZE = y 
endif

ifdef OPTIMIZE
NUAUDIOLIB = -lnualsgi_n 
else
NUAUDIOLIB = -lnualsgi_n_d 
endif

LCDEFS = -DN_AUDIO -DF3DEX_GBI_2 -D__N64__

ifndef RELEASE
LCDEFS += -DNU_DEBUG -DDEBUG
endif

ifdef RELEASE
LCDEFS += -DRELEASE
endif

ifdef ED64
LCDEFS += -DED64
endif

ifdef RELEASE
CORELIBS = -lnusys -lnustd -lgultra 
else
CORELIBS = -lnusys_d -lnustd_d -lgultra_d 
endif

ifdef OPTIMIZE
OPTIMIZER =	-O2
else
OPTIMIZER =	-g
endif

APP =		goose64.out

TARGETS =	goose64.n64

HFILES =	main.h graphic.h testingCube.h vec3d.h vec2d.h gameobject.h game.h modeltype.h renderer.h input.h character.h player.h gameutils.h gametypes.h item.h animation.h physics.h rotation.h collision.h garden_map_collision.h pathfinding.h trace.h frustum.h garden_map_graph.h

ED64CODEFILES = ed64io_usb.c ed64io_sys.c ed64io_everdrive.c ed64io_fault.c ed64io_os_error.c ed64io_watchdog.c

CROSSPLAT_CODEFILES = vec3d.c vec2d.c gameobject.c game.c modeltype.c renderer.c input.c character.c characterstate.c player.c gameutils.c item.c animation.c physics.c rotation.c collision.c  pathfinding.c frustum.c  garden_map_graph.c sprite.c option.c

CODEFILES   = 	main.c stage00.c graphic.c gfxinit.c $(CROSSPLAT_CODEFILES)

ifdef ED64
CODEFILES  += $(ED64CODEFILES)
endif

CODEOBJECTS =	$(CODEFILES:.c=.o)  $(NUSYSLIBDIR)/nusys.o

CROSSPLAT_DATAFILES = garden_map_collision.c trace.c 

DATAFILES   = mem_heap.c models.c sprite_data.c $(CROSSPLAT_DATAFILES)
DATAOBJECTS =	$(DATAFILES:.c=.o)

CODESEGMENT =	codesegment.o

OBJECTS =	$(CODESEGMENT) $(MODELSSEGMENT) $(DATAOBJECTS)
