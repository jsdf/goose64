/*
	ROM spec file

	Main memory map

  	0x80000000  exception vectors, ...
  	0x80000400  zbuffer (size 320*240*2)
  	0x80025c00  codesegment
                heap (512 * 1024)
    0x80025c00
	      :  
    0x80100000  end of 1st mb (codesegment must end before this)
  	0x8038F800  cfb 16b 3buffer (size 320*240*2*3)

*/

#include <nusys.h>

/* Use all graphic micro codes */
beginseg
	name	"code"
	flags	BOOT OBJECT
	entry 	nuBoot
	address NU_SPEC_BOOT_ADDR
        stack   NU_SPEC_BOOT_STACK

  /*       maxsize 0xDA400  keep inside first mb of RDRAM */

	include "codesegment.o"
  include "$(ROOT)/usr/lib/PR/rspboot.o"   

  /*
  gfx microcodes should match the order of these defines:
  NU_GFX_UCODE_F3DEX2 0   // F3DEX microcode  
  NU_GFX_UCODE_F3DEX2_NON 1   // F3DEX.NoN microcode  
  NU_GFX_UCODE_F3DEX2_REJ 2   // F3DEX.ReJ microcode  
  NU_GFX_UCODE_F3DLX2_REJ 3   // F3DLX2.ReJ microcode  
  NU_GFX_UCODE_L3DEX2 4   // L3DEX microcode  
  NU_GFX_UCODE_S2DEX2 5   // S2DEX microcode  
  */

	include "$(ROOT)/usr/lib/PR/gspF3DEX2.fifo.o"
  include "$(ROOT)/usr/lib/PR/gspF3DEX2.NoN.fifo.o"
  include "$(ROOT)/usr/lib/PR/gspF3DEX2.Rej.fifo.o"
  include "$(ROOT)/usr/lib/PR/gspF3DLX2.Rej.fifo.o"
	include "$(ROOT)/usr/lib/PR/gspL3DEX2.fifo.o"
	include "$(ROOT)/usr/lib/PR/gspS2DEX2.fifo.o"
endseg

beginseg
  name  "models"
  flags OBJECT
  after "code"
  include "modelssegement.o"
endseg

beginseg
  name  "collision"
  flags OBJECT
  after "models"
  include "university_map_collision.o"
endseg


beginseg
  name  "memheap"
  flags OBJECT
  after "collision"
  include "mem_heap.o"
endseg


beginseg
  name  "trace"
  flags OBJECT

  address 0x80400000
  include "trace.o"
endseg



beginwave
	name	"goose"
  include "code"
  include "models"
  include "memheap"
  include "collision"
  include "trace"
endwave
