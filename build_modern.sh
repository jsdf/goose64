#!/bin/bash

# to use this, update the specific c file to build at the bottom of this file
# then source ~/code/n64cc/env.sh and run this script,
# before running build_cached.sh
# then run disassemble_modern.sh


DRIVE_C="/Users/jfriend/.wine/drive_c"
ROOT="${DRIVE_C}/ultra"
CC=mips64-elf-gcc
# LD=mips64-elf-ld
INC="$ROOT/usr/include"
EXEGCC_INC="$ROOT/GCC/MIPSE/INCLUDE"

# LIB="${ROOT}/usr/lib"

N64KITDIR="${DRIVE_C}/nintendo/n64kit"
NUSYSINCDIR=${N64KITDIR}/nusys/include
# NUSYSLIBDIR=${N64KITDIR}/nusys/lib
NUSTDINCDIR=${N64KITDIR}/nustd/include
# NUSTDLIBDIR=${N64KITDIR}/nustd/lib

LCDEFS="-DNU_DEBUG -DF3DEX_GBI_2 -DN_AUDIO  -D__N64__"
LCINCS="-I. -I${NUSYSINCDIR}  -I${NUSTDINCDIR} -I${ROOT}/usr/include/PR"
LCOPTS="-G 0"
# LDFLAGS="-L${LIB} -L${NUSYSLIBDIR} -L${NUSTDLIBDIR}  -lnusys_d -lgultra_d" # -L${GCCDIR}/mipse/lib -lkmc
GCCFLAG="-c -I${INC} -I${EXEGCC_INC} -D_MIPS_SZLONG=32 -D_MIPS_SZINT=32 -D_LANGUAGE_C -D_ULTRA64 -D__EXTENSIONS__ -mabi=32 -march=vr4300 -mtune=vr4300"
CFLAGS="${LCDEFS} ${LCINCS} ${LCOPTS} ${GCCFLAG} ${OPTIMIZER}"



$CC $CFLAGS -g $@
