#include <nusys.h>

// this must come after nusys.h
#include "main.h"
#include "malloc.h"

#include <nualstl_n.h>

#include <PR/os_convert.h>
#ifdef ED64
#include "ed64io.h"
#endif
#include "graphic.h"
#include "mem_heap.h"
#include "trace.h"

// #define DEBUGSTARTUP

#ifdef DEBUGSTARTUP
#define DBGPRINT debugPrintfSync
#else
#define DBGPRINT(args...)
#endif

/* The global variable  */
NUContData contdata[1]; /* Read data of 1 controller  */
u8 contPattern;         /* The pattern connected to the controller  */

extern char mem_heap[MEM_HEAP_SIZE];

EXTERN_SEGMENT_WITH_BSS(memheap);
EXTERN_SEGMENT_WITH_BSS(trace);

int systemHeapMemoryInit(void) {
  int initHeapResult;
  nuPiReadRom((u32)_memheapSegmentRomStart, _memheapSegmentStart,
              (u32)_memheapSegmentRomEnd - (u32)_memheapSegmentRomStart);

  bzero(_memheapSegmentBssStart,
        _memheapSegmentBssEnd - _memheapSegmentBssStart);

  /* Reserve system heap memory */
  initHeapResult = InitHeap(mem_heap, MEM_HEAP_SIZE);

  if (initHeapResult == -1) {
    die("failed to init heap\n");
  } else {
    debugPrintfSync("init heap success, allocated=%d\n", MEM_HEAP_SIZE);
  }

  if (osGetMemSize() == 0x00800000) {
    debugPrintfSync("have expansion pack\n");
    nuPiReadRom((u32)_traceSegmentRomStart, _traceSegmentStart,
                (u32)_traceSegmentRomEnd - (u32)_traceSegmentRomStart);
    bzero(_traceSegmentBssStart, _traceSegmentBssEnd - _traceSegmentBssStart);

    debugPrintfSync("init trace buffer at %p\n", _traceSegmentStart);
  } else {
    die("expansion pack missing\n");
  }
  return 0;
}

extern void* __printfunc;
/*------------------------
        Main
--------------------------*/
void mainproc(void) {
#ifdef ED64
  // start everdrive communication
  evd_init();

  // register libultra error handler
  // ed64RegisterOSErrorHandler();

  // start thread which will catch and log errors
  ed64StartFaultHandlerThread(NU_GFX_TASKMGR_THREAD_PRI);

  // overwrite osSyncPrintf impl
  __printfunc = (void*)ed64PrintFuncImpl;
#endif

  DBGPRINT("hello\n");

  DBGPRINT("systemHeapMemoryInit\n");
  systemHeapMemoryInit();

  /* The initialization of the controller manager  */
  contPattern = nuContInit();

  /* The initialization of audio */
  DBGPRINT("nuAuStlInit\n");
  nuAuStlInit();
  // nuAuStlInit() also does:
  // nuAuStlSeqPlayerInit(
  //   NU_AU_SEQ_PLAYER0,
  //   NU_AU_SONG_SIZE /*16kb*/
  // );

  /* The initialization of graphic  */
  // nuGfxInit();
  DBGPRINT("gfxInit\n");
  gfxInit();

  /* The initialization for stage00()  */
  DBGPRINT("initStage00\n");
  initStage00();

  /* Register call-back  */
  DBGPRINT("nuGfxFuncSet\n");
  nuGfxFuncSet((NUGfxFunc)stage00);

  /* The screen display is ON */
  DBGPRINT("nuGfxDisplayOn\n");
  nuGfxDisplayOn();

  while (1)
    ;
}
