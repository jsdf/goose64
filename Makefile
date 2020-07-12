# makefile for nintendo sdk toolchain

include $(ROOT)/usr/include/make/PRdefs

N64KITDIR    = c:/nintendo/n64kit
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

include ./common.makefile

LCINCS =	-I. -nostdinc -I- -I$(NUSTDINCDIR) -I$(NUSYSINCDIR) -I$(ROOT)/usr/include/PR
LCOPTS =	-G 0

# the order of $(NUAUDIOLIB) and -lgultra_d (CORELIBS) matter :|
LDFLAGS = $(MKDEPOPT) -L$(LIB)  -L$(NUSYSLIBDIR) -L$(NUSTDLIBDIR) $(NUAUDIOLIB) $(CORELIBS) -L$(GCCDIR)/mipse/lib -lkmc 


default:        $(TARGETS)

include $(COMMONRULES)

$(CODESEGMENT):	$(CODEOBJECTS) Makefile $(HFILES)
# use -M to print memory map from ld
		$(LD) -o $(CODESEGMENT) -r $(CODEOBJECTS) $(LDFLAGS) 

$(TARGETS):	$(OBJECTS)
		# use -m to print memory map from mild
		$(MAKEROM) spec -I$(NUSYSINCDIR) -r $(TARGETS) -e $(APP) -E  -m
		makemask $(TARGETS) 

