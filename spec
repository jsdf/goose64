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
  maxsize 0xDA400
	include "codesegment.o"
	include "$(ROOT)/usr/lib/PR/rspboot.o"
	include "$(ROOT)/usr/lib/PR/gspF3DEX2.fifo.o"
	include "$(ROOT)/usr/lib/PR/gspL3DEX2.fifo.o"
	include "$(ROOT)/usr/lib/PR/gspF3DEX2.Rej.fifo.o"
        include "$(ROOT)/usr/lib/PR/gspF3DEX2.NoN.fifo.o"
        include "$(ROOT)/usr/lib/PR/gspF3DLX2.Rej.fifo.o"
	include "$(ROOT)/usr/lib/PR/gspS2DEX2.fifo.o"
endseg

beginseg
  name  "models"
  flags OBJECT
  after "code"
  include "models.o"
endseg

beginseg
  name  "memheap"
  flags OBJECT
  after "models"
  include "mem_heap.o"
endseg



beginwave
	name	"goose"
  include "code"
  include "models"
  include "memheap"
endwave
