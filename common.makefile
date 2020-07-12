# shared config between classic and modern makefiles

# include overrides file from local repo
-include ./localdefs.makefile


# this improves CPU perf (but we're usually RDP-bound)
# OPTIMIZE = y 

ifdef OPTIMIZE
NUAUDIOLIB = -lnualstl_n -ln_gmus -ln_gaudio_sc
else
NUAUDIOLIB = -lnualstl_n_d -ln_gmus_d -ln_gaudio_sc
endif

LCDEFS = -DN_AUDIO -DF3DEX_GBI_2 -D__N64__

ifndef OPTIMIZE
LCDEFS += -DNU_DEBUG -DDEBUG
endif

ifdef ED64
LCDEFS += -DED64
endif

ifdef OPTIMIZE
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

ED64CODEFILES = ed64io_usb.c ed64io_sys.c ed64io_everdrive.c ed64io_fault.c ed64io_os_error.c

CODEFILES   = 	main.c stage00.c graphic.c gfxinit.c vec3d.c vec2d.c gameobject.c game.c modeltype.c renderer.c input.c character.c characterstate.c player.c gameutils.c item.c animation.c physics.c rotation.c collision.c  pathfinding.c frustum.c  garden_map_graph.c sprite.c

ifdef ED64
CODEFILES  += $(ED64CODEFILES)
endif

CODEOBJECTS =	$(CODEFILES:.c=.o)  $(NUSYSLIBDIR)/nusys.o

DATAFILES   = mem_heap.c trace.c garden_map_collision.c models.c sprite_data.c
DATAOBJECTS =	$(DATAFILES:.c=.o)

CODESEGMENT =	codesegment.o

OBJECTS =	$(CODESEGMENT) $(MODELSSEGMENT) $(DATAOBJECTS)
