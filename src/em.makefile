

CC=emcc
CXX=em++
NATIVE_CC=mips-n64-gcc
 
# load the lists of source files and flag-driven defines & libs
include ./common.makefile

SINGLE_FILE_CFLAGS = -c
CFLAGS = -I. -I/usr/include/glm -I./imgui 

CODEFILES  +=  arith64.c  

DEPS = $(CODEFILES:.c=.d)
 

CPP_SOURCE_FILES = glgoose.cpp gl/objloader.cpp gl/texture.cpp imgui/imgui.cpp imgui/imgui_widgets.cpp 

default: $(TARGETS)

.c.o:
	@echo 🔨 building $<
	# $(NATIVE_CC) $(SINGLE_FILE_CFLAGS) $(CFLAGS) -MMD $<
	$(CC) $(SINGLE_FILE_CFLAGS) $(CFLAGS) $<

clean: 
	rm -f *.o *.d

clobber:
	rm -f *.o *.d *.n64 *.out


CROSSPLAT_CODEOBJECTS = $(CROSSPLAT_CODEFILES:.c=.o)
CROSSPLAT_DATAOBJECTS = $(CROSSPLAT_DATAFILES:.c=.o)
NATIVE_FILES = compat.c

GLOBJECTS =	$(CROSSPLAT_CODEOBJECTS) $(CROSSPLAT_DATAOBJECTS)

$(TARGETS):	$(GLOBJECTS)
		$(CXX) $(CROSSPLAT_CODEFILES) $(CROSSPLAT_DATAFILES) $(NATIVE_FILES) $(CPP_SOURCE_FILES) $(CFLAGS)  -o emgoose 

-include $(DEPS)
