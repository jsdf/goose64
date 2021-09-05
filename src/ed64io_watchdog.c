#include "ed64io.h"

#define PRINTF ed64PrintfSync2
#define DEBUGPRINT 0
#if DEBUGPRINT
#define DBGPRINT ed64PrintfSync2
#else
#define DBGPRINT(args...)
#endif

static OSMesgQueue watchdogMsgQ;
static OSMesg watchdogMsgBuf;
static int watchdogThreadPri;

static OSThread watchdogThread;
static char watchdogThreadStack[ED64IO_FAULT_STACKSIZE];

static int startedwatchdogproc = 0;

static int lastValToWatch = 0;
static int* valToWatch = NULL;

OSTimer watchdogTimer;

/*
 * Watchdog handler: simply waits for the watchdog message,
 * and checks if the watched value has changed since last time.
 * If it hasn't, we're hanged.
 */
static void watchdogproc(char* argv) {
  OSMesg msg;
  startedwatchdogproc = 1;

  DBGPRINT("=> watchdogproc started...\n");

  while (1) {
    (void)osRecvMesg(&watchdogMsgQ, (OSMesg*)&msg, OS_MESG_BLOCK);
    DBGPRINT("=> watchdogproc - got a timer message... %x\n", msg);

    if (valToWatch != NULL) {
      if (*valToWatch == lastValToWatch) {
        // value hasn't changed since last timer, we've hanged
        ed64PrintfSync2("watchdog detected thread hang\n");
// break in debugger
#ifdef ED64IO_DEBUGGER
        asm("break 1000");
#else
        ed64PrintThreads(TRUE);
#endif
        break;
      } else {
        lastValToWatch = *valToWatch;
      }
    }
  }

  DBGPRINT("=> watchdogproc reached end...\n");
  while (1) {
  }
}

/*
 * start a thread which will be entered when a watchdog timer occurs, and prints
 * debug info
 */
void ed64StartWatchdogThread(int* valToWatchInit, int intervalMS) {
  int currentThreadOriginalPri = osGetThreadPri(NULL);
  valToWatch = valToWatchInit;

  // debugger relies on the PI manager so we must be lower pri than it is
  watchdogThreadPri = OS_PRIORITY_PIMGR - 2;

  // set current thread to same pri as new thread to make sure this is the
  // thread we'll return to after creation
  osSetThreadPri(NULL, watchdogThreadPri - 1);

  // Create message queue and thread structures for watchdog timer handling
  // thread
  osCreateMesgQueue(&watchdogMsgQ, &watchdogMsgBuf, 1);

  DBGPRINT("creating watchdog thread with pri=%d, current thread pri=%d\n",
           watchdogThreadPri, currentThreadOriginalPri);

  osCreateThread(&watchdogThread, /*id*/ 60, (void (*)(void*))watchdogproc,
                 /*argv*/ NULL,
                 watchdogThreadStack + ED64IO_FAULT_STACKSIZE / 8,
                 /*priority*/ (OSPri)watchdogThreadPri);

  DBGPRINT("=> ed64StartFaultHandlerThread - starting watchdog thread...\n");
  osStartThread(&watchdogThread);

  lastValToWatch = *valToWatch;
  osSetTimer(&watchdogTimer, OS_NSEC_TO_CYCLES(intervalMS * 1000000UL),
             OS_NSEC_TO_CYCLES(intervalMS * 1000000UL), &watchdogMsgQ,
             /*OSMesg*/ NULL);

  if (startedwatchdogproc) {
    DBGPRINT("=> returned from osStartThread(&watchdogThread); state=%d\n",
             watchdogThread.state);

    // drop current thread back to its correct priority
    osSetThreadPri(NULL, currentThreadOriginalPri);
  }
}
