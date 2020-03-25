#include <nusys.h>

// this must come after nusys.h
#include "main.h"
#include "malloc.h"

#include <PR/os_convert.h>
#include "mem_heap.h"
#include "trace.h"

/* The global variable  */
NUContData contdata[1]; /* Read data of 1 controller  */
u8 contPattern;         /* The pattern connected to the controller  */

extern char mem_heap[MEM_HEAP_SIZE];

int systemHeapMemoryInit(void) {
  /* Reserve system heap memory */
  int initHeapResult = InitHeap(mem_heap, MEM_HEAP_SIZE);
  }
  return 0;
}

/*------------------------
        Main
--------------------------*/
void mainproc(void) {
  /* The initialization of graphic  */
  nuGfxInit();
  systemHeapMemoryInit();

  /* The initialization of the controller manager  */
  contPattern = nuContInit();

  /* The initialization for stage00()  */
  initStage00();
  /* Register call-back  */
  nuGfxFuncSet((NUGfxFunc)stage00);
  /* The screen display is ON */
  nuGfxDisplayOn();

  while (1)
    ;
}

/*-----------------------------------------------------------------------------
  The call-back function

  pendingGfx which is passed from Nusystem as the argument of the call-back
  function is the total of RCP tasks that are currently processing and
  waiting for the process.
-----------------------------------------------------------------------------*/
void stage00(int pendingGfx) {
  float skippedGfxTime, profStartUpdate, profStartFrame, profEndFrame;
  profStartFrame = CUR_TIME_MS();
  /* Provide the display process if n or less RCP tasks are processing or
        waiting for the process.  */
  if (pendingGfx < GFX_TASKS_PER_MAKEDL * 2) {
    makeDL00();
    Trace_addEvent(MainMakeDisplayListTraceEvent, profStartFrame,
                   CUR_TIME_MS());
  } else {
    skippedGfxTime = CUR_TIME_MS();
    Trace_addEvent(SkippedGfxTaskTraceEvent, skippedGfxTime,
                   skippedGfxTime + 16.0f);
  }

  profStartUpdate = CUR_TIME_MS();
  /* The process of game progress  */
  updateGame00();
  profEndFrame = CUR_TIME_MS();
  Trace_addEvent(MainCPUTraceEvent, profStartFrame, profEndFrame);
  Trace_addEvent(MainUpdateTraceEvent, profStartUpdate, profEndFrame);
  profilingAccumulated[MainCPUTraceEvent] += profEndFrame - profStartFrame;
}
