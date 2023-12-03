# this make-based build system builds the ROM for the n64

# first, ensure you have an n64-compatible mips cross-compiler installed
# e.g https://github.com/ModernN64SDK/n64sdkmod

# then invoke `make` with the following environment variables defined,
# based on where you have the n64 sdk installed
# ROOT="/ultra"
# N64KITDIR="nintendo/n64kit"

# you can also define these in localdefs.makefile

# --- OPTIONS FOR BUILD ---

# load the lists of source files shared between n64 and opengl builds
include ./common.makefile

# machine-specific variables for n64 build
-include localdefs.makefile

# OPTIMIZE = y 

ifdef RELEASE
ED64 =
OPTIMIZE = y 
endif

# --- SOURCE FILES AND OBJECTS ---

APP =		goose64.out

TARGETS =	goose64.n64

ED64CODEFILES = ed64io_usb.c ed64io_sys.c ed64io_everdrive.c ed64io_fault.c ed64io_os_error.c ed64io_watchdog.c

CODEFILES   = 	main.c stage00.c graphic.c gfxinit.c $(CROSSPLAT_CODEFILES)

ifdef ED64
CODEFILES  += $(ED64CODEFILES)
endif

CODEOBJECTS =	$(CODEFILES:.c=.o)  $(NUSYSLIBDIR)/nusys.o

DATAFILES   = mem_heap.c models.c sprite_data.c $(CROSSPLAT_DATAFILES)
DATAOBJECTS =	$(DATAFILES:.c=.o)

CODESEGMENT =	codesegment.o

OBJECTS =	$(CODESEGMENT) $(MODELSSEGMENT) $(DATAOBJECTS)

DEPS = $(CODEFILES:.c=.d)
 
# --- COMPILER OPTIONS ---

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


NUSYSINCDIR = $(N64KITDIR)/nusys/include
NUSYSLIBDIR = $(N64KITDIR)/nusys/lib
NUSTDINCDIR = $(N64KITDIR)/nustd/include
NUSTDLIBDIR = $(N64KITDIR)/nustd/lib

N64_CC ?= mips-n64-gcc
N64_CFLAGS ?= -c -D_MIPS_SZLONG=32 -D_MIPS_SZINT=32 -D_LANGUAGE_C -D_ULTRA64 -D__EXTENSIONS__ -mabi=32   -march=vr4300 -mtune=vr4300 -mfix4300

N64_AS ?= mips-n64-as
N64_ASFLAGS ?= -march=r4300 -mabi=32 

N64_LD ?= mips-n64-ld
N64_LDFLAGS ?= --no-check-sections

N64_SIZE ?= mips-n64-size
N64_SIZEFLAGS ?=

N64_OBJCOPY ?= mips-n64-objcopy
N64_OBJCOPYFLAGS ?=

CC=$(N64_CC) $(N64_CFLAGS)
AS=$(N64_AS) $(N64_ASFLAGS)
LD=$(N64_LD) $(N64_LDFLAGS)
SIZE=$(N64_SIZE) $(N64_SIZEFLAGS)
OBJCOPY=$(N64_OBJCOPY) $(N64_OBJCOPYFLAGS)

MAKEROM ?= spicy
MAKEMASK ?= makemask

NAWK=awk

N64SDK_LIB_DIR = $(ROOT)/usr/lib
N64SDK_INC_DIR = $(ROOT)/usr/include

N64_GCC_INC_DIR = $(shell $(N64_CC) -print-file-name=include)
N64_GCC_LIB_DIR = $(dir $(shell $(N64_CC) -print-file-name=libgcc.a))
 
LCINCS =	-I. -I$(N64_GCC_INC_DIR) -I$(N64SDK_INC_DIR) -I$(NUSYSINCDIR) -I$(NUSTDINCDIR) -I$(ROOT)/usr/include/PR 
LCOPTS =	-G 0 -g3  -Wno-comment -Werror-implicit-function-declaration 

CFLAGS = $(LCDEFS) $(LCINCS) $(LCOPTS) $(OPTIMIZER)

# the order of $(NUAUDIOLIB) and -lgultra_d (CORELIBS) matter :|
LDFLAGS = $(MKDEPOPT) -L$(N64SDK_LIB_DIR) -L$(NUSYSLIBDIR) -L$(NUSTDLIBDIR)  $(CORELIBS)  -L$(N64_GCC_LIB_DIR)  -lgcc 


# --- BUILD RULES ---

default: $(TARGETS)

# -c will compile but not link
# -MMD will generate a .d file with dependencies
.c.o:
	@echo 🔨 building $<
	$(CC) $(CFLAGS) -MMD -c $< -o $@

clean: 
	rm -f *.o *.d

clobber:
	rm -f *.o *.d *.n64 *.out

$(CODESEGMENT): $(CODEOBJECTS) Makefile common.makefile localdefs.makefile
# use -M to print memory map from ld
	$(LD) -o $(CODESEGMENT) -r $(CODEOBJECTS) $(LDFLAGS)


$(TARGETS):	$(OBJECTS)
		$(MAKEROM) spec -I$(NUSYSINCDIR) -r $(TARGETS) -s 9 -e $(APP) --ld_command="$(N64_LD)" --as_command="$(N64_AS)" --cpp_command="$(N64_CC)" --objcopy_command="$(N64_OBJCOPY)"
		$(MAKEMASK) $(TARGETS)

-include $(DEPS)
