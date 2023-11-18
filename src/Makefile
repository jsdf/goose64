# to use this, first add $N64CHAIN/tools/bin to your path
# (or source a file which does, on my system that's ~/code/n64cc/env.sh)
# then use make
 
# include $(ROOT)/usr/include/make/PRdefs 

# expects the following environment variables to be defined
# ROOT="${SDK_BASE_DIR}/ultra"
# N64KITDIR="${SDK_BASE_DIR}/nintendo/n64kit"
# $(info PATH is $(PATH))

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
MAKEROM = /workspace/spicy/cmd/spicy/spicy
MAKEMASK = /workspace/makemask/makemask

NAWK=awk

N64SDK_LIB_DIR = $(ROOT)/usr/lib
N64SDK_INC_DIR = $(ROOT)/usr/include

N64_GCC_INC_DIR = $(shell $(N64_CC) -print-file-name=include)
N64_GCC_LIB_DIR = $(dir $(shell $(N64_CC) -print-file-name=libgcc.a))
 
LCINCS =	-I. -I$(N64_GCC_INC_DIR) -I$(N64SDK_INC_DIR) -I$(NUSYSINCDIR) -I$(NUSTDINCDIR) -I$(ROOT)/usr/include/PR 
LCOPTS =	-G 0 -g3  -Wno-comment -Werror-implicit-function-declaration 

# load the lists of source files and flag-driven defines & libs
include ./common.makefile

CFLAGS = $(LCDEFS) $(LCINCS) $(LCOPTS) $(OPTIMIZER)

# the order of $(NUAUDIOLIB) and -lgultra_d (CORELIBS) matter :|
LDFLAGS = $(MKDEPOPT) -L$(N64SDK_LIB_DIR) -L$(NUSYSLIBDIR) -L$(NUSTDLIBDIR)  $(CORELIBS)  -L$(N64_GCC_LIB_DIR)  -lgcc 


DEPS = $(CODEFILES:.c=.d)
 


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

$(CODESEGMENT): $(CODEOBJECTS) Makefile $(HFILES)
# use -M to print memory map from ld
	$(LD) -o $(CODESEGMENT) -r $(CODEOBJECTS) $(LDFLAGS)


$(TARGETS):	$(OBJECTS)
		$(MAKEROM) spec -I$(NUSYSINCDIR) -r $(TARGETS) -s 9 -e $(APP) --ld_command="$(N64_LD)" --as_command="$(N64_AS)" --cpp_command="$(N64_CC)" --objcopy_command="$(N64_OBJCOPY)"
		$(MAKEMASK) $(TARGETS)

-include $(DEPS)
