# makefile for modern toolchain

# currently doesn't seem to produce working build
# to use this, first source ~/code/n64cc/env.sh
# then use make
# ROOT="${SDK_BASE_DIR}/ultra"
# N64KITDIR="${SDK_BASE_DIR}/nintendo/n64kit"

# set up modern toolchain (gcc, etc)
include ./modern.makefile


LCINCS = -I. -I./include -I$(GCCINCDIR) -I$(NUSYSINCDIR) -I$(NUSTDINCDIR) -I$(ROOT)/usr/include/PR -I$(INC) -I$(EXEGCC_INC)
LCOPTS =	-G 0 -std=gnu90 -nostdinc -Wno-comment -Werror-implicit-function-declaration 

# load the lists of source files and flag-driven defines & libs
include ./common.makefile


CFLAGS = $(LCDEFS) $(LCINCS) $(LCOPTS) $(OPTIMIZER) 

# the order of $(NUAUDIOLIB) and -lgultra_d (CORELIBS) matter :|
LDFLAGS = $(MKDEPOPT) -L$(LIB)  -L$(NUSYSLIBDIR) -L$(NUSTDLIBDIR) $(NUAUDIOLIB) $(CORELIBS) -L$(GCCLIBDIR) -lgcc



default:        $(TARGETS)

.c.o: 
	# to print resolved include paths, add -M flag
	$(CC) $(CFLAGS) $<

clean: 
	rm -f *.o


clobber:
	rm -f *.o *.n64 *.out

$(CODESEGMENT): $(CODEOBJECTS) GNUMakefile $(HFILES)
# use -M to print memory map from ld
	$(LD) -o $(CODESEGMENT) -r $(CODEOBJECTS) $(LDFLAGS) 

$(TARGETS):	$(OBJECTS) spec $(CODESEGMENT)
	$(MAKEROM) -I$(NUSYSINCDIR) -r $(TARGETS) -s 16 -e $(APP)    spec # -d  -m 
	makemask $(TARGETS)

