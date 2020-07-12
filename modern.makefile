# include for modern n64 toolchain

# expects the following environment variables to be defined
# ROOT="${SDK_BASE_DIR}/ultra"
# N64KITDIR="${SDK_BASE_DIR}/nintendo/n64kit"

NUSYSINCDIR = $(N64KITDIR)/nusys/include
NUSYSLIBDIR = $(N64KITDIR)/nusys/lib
NUSTDINCDIR = $(N64KITDIR)/nustd/include
NUSTDLIBDIR = $(N64KITDIR)/nustd/lib

GCCLIBDIR = $(N64_TOOLCHAIN)/lib/gcc/mips64-elf/9.1.0
GCCINCDIR = $(N64_TOOLCHAIN)/lib/gcc/mips64-elf/9.1.0/include

INC = $(ROOT)/usr/include
LIB = $(ROOT)/usr/lib
EXEGCC_INC = $(ROOT)/GCC/MIPSE/INCLUDE

MAKEROM = spicy

N64_CC ?= mips64-elf-gcc
N64_CFLAGS ?= -c -D_MIPS_SZLONG=32 -D_MIPS_SZINT=32 -D_LANGUAGE_C -D_ULTRA64 -D__EXTENSIONS__ -mabi=32  -march=vr4300 -mtune=vr4300 -mfix4300 

N64_AS ?= mips64-elf-as
N64_ASFLAGS ?= -march=r4300 -mabi=32

N64_LD ?= mips64-elf-ld
N64_LDFLAGS ?= --no-check-sections

N64_SIZE ?= mips64-elf-size
N64_SIZEFLAGS ?=

N64_OBJCOPY ?= mips64-elf-objcopy
N64_OBJCOPYFLAGS ?=

CC=$(N64_CC) $(N64_CFLAGS)
AS=$(N64_AS) $(N64_ASFLAGS)
LD=$(N64_LD) $(N64_LDFLAGS)
SIZE=$(N64_SIZE) $(N64_SIZEFLAGS)
OBJCOPY=$(N64_OBJCOPY) $(N64_OBJCOPYFLAGS)
NAWK=awk