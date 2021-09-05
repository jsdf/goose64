

#include <PR/os_internal_error.h>

#include <ultra64.h>

#include <string.h>

#include "ed64io_sys.h"
#include "ed64io_usb.h"

#include "ed64io_everdrive.h"
#include "ed64io_fault.h"

#define PRINTF ed64PrintfSync2
#define DEBUGPRINT 0
#if DEBUGPRINT
#define DBGPRINT ed64PrintfSync2
#else
#define DBGPRINT(args...)
#endif

#define MSG_FAULT 0x10

typedef struct {
  u32 mask;
  u32 value;
  char* string;
} regDesc_t;

enum DebuggerPacketType { NonePacket, RegistersPacket, ThreadPacket };

#define MAX_STACK_TRACE 100

static void* stackTraceReturnAddresses[MAX_STACK_TRACE];

#define MAX_STOPPED_THREADS 200
static int stoppedThreadsCount = 0;
static OSThread* stoppedThreads[MAX_STOPPED_THREADS];
static u16 stoppedThreadsStates[MAX_STOPPED_THREADS];

static OSMesgQueue faultMsgQ;
static OSMesg faultMsgBuf;
static int faultThreadPri;

static OSThread faultThread;
static char faultThreadStack[ED64IO_FAULT_STACKSIZE];

int startedfaultproc = 0;

static OSMesg breakMsg = (OSMesg)0xaaaaaaaa;

static void printRegister(u32 regValue, char* regName, regDesc_t* regDesc);
static void printFaultData(OSThread* t);

static void setBreakpoint(u32* addr1, u32* addr2);

static int doSingleStep(int thread, u32* instptr);
static void clearBreakpoint(void);

static void walkFaultedThreads(void);

static regDesc_t causeDesc[] = {
    {CAUSE_BD, CAUSE_BD, "BD"},
    {CAUSE_IP8, CAUSE_IP8, "IP8"},
    {CAUSE_IP7, CAUSE_IP7, "IP7"},
    {CAUSE_IP6, CAUSE_IP6, "IP6"},
    {CAUSE_IP5, CAUSE_IP5, "IP5"},
    {CAUSE_IP4, CAUSE_IP4, "IP4"},
    {CAUSE_IP3, CAUSE_IP3, "IP3"},
    {CAUSE_SW2, CAUSE_SW2, "IP2"},
    {CAUSE_SW1, CAUSE_SW1, "IP1"},
    {CAUSE_EXCMASK, EXC_INT, "Interrupt"},
    {CAUSE_EXCMASK, EXC_MOD, "TLB modification exception"},
    {CAUSE_EXCMASK, EXC_RMISS, "TLB exception on load or instruction fetch"},
    {CAUSE_EXCMASK, EXC_WMISS, "TLB exception on store"},
    {CAUSE_EXCMASK, EXC_RADE, "Address error on load or instruction fetch"},
    {CAUSE_EXCMASK, EXC_WADE, "Address error on store"},
    {CAUSE_EXCMASK, EXC_IBE, "Bus error exception on instruction fetch"},
    {CAUSE_EXCMASK, EXC_DBE, "Bus error exception on data reference"},
    {CAUSE_EXCMASK, EXC_SYSCALL, "System call exception"},
    {CAUSE_EXCMASK, EXC_BREAK, "Breakpoint exception"},
    {CAUSE_EXCMASK, EXC_II, "Reserved instruction exception"},
    {CAUSE_EXCMASK, EXC_CPU, "Coprocessor unusable exception"},
    {CAUSE_EXCMASK, EXC_OV, "Arithmetic overflow exception"},
    {CAUSE_EXCMASK, EXC_TRAP, "Trap exception"},
    {CAUSE_EXCMASK, EXC_VCEI,
     "Virtual coherency exception on intruction fetch"},
    {CAUSE_EXCMASK, EXC_FPE, "Floating point exception (see fpcsr)"},
    {CAUSE_EXCMASK, EXC_WATCH, "Watchpoint exception"},
    {CAUSE_EXCMASK, EXC_VCED, "Virtual coherency exception on data reference"},
    {0, 0, ""}};

static regDesc_t srDesc[] = {{SR_CU3, SR_CU3, "CU3"},
                             {SR_CU2, SR_CU2, "CU2"},
                             {SR_CU1, SR_CU1, "CU1"},
                             {SR_CU0, SR_CU0, "CU0"},
                             {SR_RP, SR_RP, "RP"},
                             {SR_FR, SR_FR, "FR"},
                             {SR_RE, SR_RE, "RE"},
                             {SR_BEV, SR_BEV, "BEV"},
                             {SR_TS, SR_TS, "TS"},
                             {SR_SR, SR_SR, "SR"},
                             {SR_CH, SR_CH, "CH"},
                             {SR_CE, SR_CE, "CE"},
                             {SR_DE, SR_DE, "DE"},
                             {SR_IBIT8, SR_IBIT8, "IM8"},
                             {SR_IBIT7, SR_IBIT7, "IM7"},
                             {SR_IBIT6, SR_IBIT6, "IM6"},
                             {SR_IBIT5, SR_IBIT5, "IM5"},
                             {SR_IBIT4, SR_IBIT4, "IM4"},
                             {SR_IBIT3, SR_IBIT3, "IM3"},
                             {SR_IBIT2, SR_IBIT2, "IM2"},
                             {SR_IBIT1, SR_IBIT1, "IM1"},
                             {SR_KX, SR_KX, "KX"},
                             {SR_SX, SR_SX, "SX"},
                             {SR_UX, SR_UX, "UX"},
                             {SR_KSU_MASK, SR_KSU_USR, "USR"},
                             {SR_KSU_MASK, SR_KSU_SUP, "SUP"},
                             {SR_KSU_MASK, SR_KSU_KER, "KER"},
                             {SR_ERL, SR_ERL, "ERL"},
                             {SR_EXL, SR_EXL, "EXL"},
                             {SR_IE, SR_IE, "IE"},
                             {0, 0, ""}};

static regDesc_t fpcsrDesc[] = {{FPCSR_FS, FPCSR_FS, "FS"},
                                {FPCSR_C, FPCSR_C, "C"},
                                {FPCSR_CE, FPCSR_CE, "Unimplemented operation"},
                                {FPCSR_CV, FPCSR_CV, "Invalid operation"},
                                {FPCSR_CZ, FPCSR_CZ, "Division by zero"},
                                {FPCSR_CO, FPCSR_CO, "Overflow"},
                                {FPCSR_CU, FPCSR_CU, "Underflow"},
                                {FPCSR_CI, FPCSR_CI, "Inexact operation"},
                                {FPCSR_EV, FPCSR_EV, "EV"},
                                {FPCSR_EZ, FPCSR_EZ, "EZ"},
                                {FPCSR_EO, FPCSR_EO, "EO"},
                                {FPCSR_EU, FPCSR_EU, "EU"},
                                {FPCSR_EI, FPCSR_EI, "EI"},
                                {FPCSR_FV, FPCSR_FV, "FV"},
                                {FPCSR_FZ, FPCSR_FZ, "FZ"},
                                {FPCSR_FO, FPCSR_FO, "FO"},
                                {FPCSR_FU, FPCSR_FU, "FU"},
                                {FPCSR_FI, FPCSR_FI, "FI"},
                                {FPCSR_RM_MASK, FPCSR_RM_RN, "RN"},
                                {FPCSR_RM_MASK, FPCSR_RM_RZ, "RZ"},
                                {FPCSR_RM_MASK, FPCSR_RM_RP, "RP"},
                                {FPCSR_RM_MASK, FPCSR_RM_RM, "RM"},
                                {0, 0, ""}};

// http://yosefk.com/blog/getting-the-call-stack-without-a-frame-pointer.html

/* get previous stack pointer and return address
   given the current ones */
int getPrevStackPointerAndReturnAddress(void** prev_sp,
                                        void** prev_ra,
                                        void* sp,
                                        void* ra) {
  unsigned* wra = (unsigned*)ra;
  unsigned* k0base = (unsigned*)K0BASE;
  int spofft;

  if (wra < k0base) {
    return 0;
  }
  /* scan towards the beginning of the function -
     addui sp,sp,spofft should be the first command */
  while ((*wra >> 16) != 0x27bd) {
    /* test for "scanned too much" */
    if (wra < k0base) {
      return 0;
    }
    wra--;
  }
  spofft = ((int)*wra << 16) >> 16; /* sign-extend */
  *prev_sp = (char*)sp - spofft;
  /* now scan forward for sw r31,raofft(sp) */
  while (wra < (unsigned*)ra) {
    if ((*wra >> 16) == 0xafbf) {
      int raofft = ((int)*wra << 16) >> 16; /* sign */
      *prev_ra = *(void**)((char*)sp + raofft);
      return 1;
    }
    wra++;
  }
  return 0; /* failed to find where ra is saved */
}

int getCallStackNoFp(u64 sp_val, u64 ra_val) {
  void* sp = (void*)(u32)sp_val; /* stack pointer from thread state */
  void* ra = (void*)(u32)ra_val; /* return address from thread state */
  int i = 0;

  while (i < MAX_STACK_TRACE &&
         getPrevStackPointerAndReturnAddress(&sp, &ra, sp, ra) && ra != 0) {
    stackTraceReturnAddresses[i++] = ra;
  }
  return i; /* stack size */
}

void ed64PrintStackTrace(OSThread* t, int framesToSkip) {
  __OSThreadContext* tc = &t->context;

  int stackTraceSize = getCallStackNoFp(tc->sp, tc->ra);
  PRINTF("stacktrace:\n");
  if (framesToSkip == 0) {
    PRINTF("%08x ", (u32)(tc->pc));
    PRINTF("%08x ", (u32)(tc->ra));
  }
  {
    int i;
    for (i = framesToSkip; i < stackTraceSize; ++i) {
      PRINTF("%08x ", (u32)stackTraceReturnAddresses[i]);
    }
  }

  PRINTF("\n");
}

// the compiler is confused because there's a type called s8 but also
// a struct member called s8 so we have to rename the member to _s8
typedef struct {
  u64 at, v0, v1, a0, a1, a2, a3;
  u64 t0, t1, t2, t3, t4, t5, t6, t7;
  u64 s0, s1, s2, s3, s4, s5, s6, s7;
  u64 t8, t9, gp, sp, _s8, ra;
  u64 lo, hi;
  u32 sr, pc, cause, badvaddr, rcp;
  u32 fpcsr;
  __OSfp fp0, fp2, fp4, fp6, fp8, fp10, fp12, fp14;
  __OSfp fp16, fp18, fp20, fp22, fp24, fp26, fp28, fp30;
} __OSThreadContextHack;

void printFaultData(OSThread* t) {
  __OSThreadContextHack* tc = (__OSThreadContextHack*)&t->context;

  PRINTF("\nFault in thread %d:\n\n", t->id);
  PRINTF("epc\t\t0x%08x\n", tc->pc);
  printRegister(tc->cause, "cause", causeDesc);
  printRegister(tc->sr, "sr", srDesc);
  PRINTF("badvaddr\t0x%08x\n\n", tc->badvaddr);

  PRINTF("register contents:\n");
  PRINTF("at 0x%016llx v0 0x%016llx v1 0x%016llx\n", tc->at, tc->v0, tc->v1);
  PRINTF("a0 0x%016llx a1 0x%016llx a2 0x%016llx\n", tc->a0, tc->a1, tc->a2);
  PRINTF("a3 0x%016llx t0 0x%016llx t1 0x%016llx\n", tc->a3, tc->t0, tc->t1);
  PRINTF("t2 0x%016llx t3 0x%016llx t4 0x%016llx\n", tc->t2, tc->t3, tc->t4);
  PRINTF("t5 0x%016llx t6 0x%016llx t7 0x%016llx\n", tc->t5, tc->t6, tc->t7);
  PRINTF("s0 0x%016llx s1 0x%016llx s2 0x%016llx\n", tc->s0, tc->s1, tc->s2);
  PRINTF("s3 0x%016llx s4 0x%016llx s5 0x%016llx\n", tc->s3, tc->s4, tc->s5);
  PRINTF("s6 0x%016llx s7 0x%016llx t8 0x%016llx\n", tc->s6, tc->s7, tc->t8);
  PRINTF("t9 0x%016llx gp 0x%016llx sp 0x%016llx\n", tc->t9, tc->gp, tc->sp);
  // hack for tc->s8
  PRINTF("s8 0x%016llx ra 0x%016llx\n\n", tc->_s8, tc->ra);

  printRegister(tc->fpcsr, "fpcsr", fpcsrDesc);

  // commented out because something in here crashes the program
  // PRINTF("floating point register contents:\n");
  // PRINTF("\nf0  %.7e f2  %.7e f4  %.7e f6  %.7e\n", tc->fp0.f.f_even,
  //        tc->fp2.f.f_even, tc->fp4.f.f_even, tc->fp6.f.f_even);
  // PRINTF("f8  %.7e f10 %.7e f12 %.7e f14 %.7e\n", tc->fp8.f.f_even,
  //        tc->fp10.f.f_even, tc->fp12.f.f_even, tc->fp14.f.f_even);
  // PRINTF("f16 %.7e f18 %.7e f20 %.7e f22 %.7e\n", tc->fp16.f.f_even,
  //        tc->fp18.f.f_even, tc->fp20.f.f_even, tc->fp22.f.f_even);
  // PRINTF("f24 %.7e f26 %.7e f28 %.7e f30 %.7e\n\n", tc->fp24.f.f_even,
  //        tc->fp26.f.f_even, tc->fp28.f.f_even, tc->fp30.f.f_even);

  // PRINTF("d0  %.15e\td2  %.15e\n", tc->fp0.d, tc->fp2.d);
  // PRINTF("d4  %.15e\td6  %.15e\n", tc->fp4.d, tc->fp6.d);
  // PRINTF("d8  %.15e\td10 %.15e\n", tc->fp8.d, tc->fp10.d);
  // PRINTF("d12 %.15e\td14 %.15e\n", tc->fp12.d, tc->fp14.d);
  // PRINTF("d16 %.15e\td18 %.15e\n", tc->fp16.d, tc->fp18.d);
  // PRINTF("d20 %.15e\td22 %.15e\n", tc->fp20.d, tc->fp22.d);
  // PRINTF("d24 %.15e\td26 %.15e\n", tc->fp24.d, tc->fp26.d);
  // PRINTF("d28 %.15e\td30 %.15e\n", tc->fp28.d, tc->fp30.d);

  ed64PrintStackTrace(t, 0);
}

typedef struct RegistersState {
  u64 at, v0, v1, a0, a1, a2, a3;
  u64 t0, t1, t2, t3, t4, t5, t6, t7;
  u64 s0, s1, s2, s3, s4, s5, s6, s7;
  u64 t8, t9, gp, sp, _s8, ra;
  u32 id;
} RegistersState;

static void sendRegisters(OSThread* t) {
  __OSThreadContextHack* tc = (__OSThreadContextHack*)&t->context;

  RegistersState reg = (RegistersState){
      tc->at, tc->v0, tc->v1, tc->a0,  tc->a1, tc->a2, tc->a3, tc->t0,
      tc->t1, tc->t2, tc->t3, tc->t4,  tc->t5, tc->t6, tc->t7, tc->s0,
      tc->s1, tc->s2, tc->s3, tc->s4,  tc->s5, tc->s6, tc->s7, tc->t8,
      tc->t9, tc->gp, tc->sp, tc->_s8, tc->ra,
      t->id,  // thread id
  };

  ed64SendBinaryData(&reg, RegistersPacket, sizeof(RegistersState));
}

static void sendStack(OSThread* t) {
  //   __OSThreadContextHack* tc = (__OSThreadContextHack*)&t->context;
  //   int size =
  // tc->sp - tc->_s8

  //   ed64SendBinaryData(&reg, RegistersPacket, sizeof(RegistersState));
}

static void printRegister(u32 regValue, char* regName, regDesc_t* regDesc) {
  int first = 1;

  PRINTF("%s\t\t0x%08x ", regName, regValue);
  PRINTF("<");
  while (regDesc->mask != 0) {
    if ((regValue & regDesc->mask) == regDesc->value) {
      if (first)
        first = 0;
      else
        PRINTF(",");
      PRINTF("%s", regDesc->string);
    }
    regDesc++;
  }
  PRINTF(">\n");
}

extern OSThread* __osGetActiveQueue(void);

static u16 getThreadStateForDebugger(OSThread* tptr) {
  int i;
  if (tptr->state != OS_STATE_STOPPED) {
    return tptr->state;
  }
  for (i = 0; i < stoppedThreadsCount; ++i) {
    if (stoppedThreads[i] == tptr) {
      // this is a user thread stopped by the debugger, return the state it was
      // in before it was stopped
      return stoppedThreadsStates[i];
    }
  }
  // actually stopped
  return tptr->state;
}

static void stopUserThreads() {
  register OSThread* tptr = __osGetActiveQueue();
  int i;
  OSThread* curThread = &faultThread;

  if (stoppedThreadsCount > 0) {
    return;
  }

  while (tptr->priority != -1) {
    if (stoppedThreadsCount == MAX_STOPPED_THREADS) {
      break;
    }
    if (tptr->priority > 0 && tptr->priority < 128 && tptr != curThread) {
      int stateBeforeStopping = tptr->state;
      osStopThread(tptr);
      if (tptr->state != OS_STATE_STOPPED) {
        DBGPRINT("Couldn't stop thread %d\n", tptr->id);
      } else {
        i = stoppedThreadsCount++;
        stoppedThreads[i] = tptr;
        stoppedThreadsStates[i] = stateBeforeStopping;
      }
    }
    tptr = tptr->tlnext;
  }
}

static void resumeUserThreads() {
  int i;
  for (i = 0; i < stoppedThreadsCount; ++i) {
    osStartThread(stoppedThreads[i]);
  }
  stoppedThreadsCount = 0;
}

/*
 * Fault handler: simply waits for the fault message, gets the current
 *  faulted thread, and prints out all the registers of this thread.
 */
static void faultproc(char* argv) {
  OSMesg msg;
  static OSThread* curr;
  startedfaultproc = 1;

  DBGPRINT("=> faultproc started...\n");

  while (1) {
    (void)osRecvMesg(&faultMsgQ, (OSMesg*)&msg, OS_MESG_BLOCK);
    DBGPRINT("=> faultproc - got a fault message... %x\n", msg);

    if (msg == breakMsg) {
      clearBreakpoint();
#ifdef ED64IO_DEBUGGER
      walkFaultedThreads();
#else
      ed64PrintThreads(FALSE);
#endif
    } else if (msg == NULL) {
      // an actual fault

      /* This routine returns the most recent faulted thread */
      curr = __osGetCurrFaultedThread();

      if (curr) {
        printFaultData(curr);
      }
      ed64PrintThreads(FALSE);
      break;
    } else {
      // just reusing the fault handler code to print a stack trace of the
      // thread structure passed as the message
      curr = (OSThread*)msg;
      ed64PrintStackTrace(curr, 1);
      ed64PrintThreads(FALSE);
    }
  }

  DBGPRINT("=> faultproc reached end...\n");
  while (1) {
  }
}

/*
 * start a thread which will be entered when a fault occurs, and prints debug
 * info
 */
void ed64StartFaultHandlerThread(int mainThreadPri) {
  int currentThreadOriginalPri = osGetThreadPri(NULL);

  // we rely on the PI manager so we must be lower pri than it is
  faultThreadPri = OS_PRIORITY_PIMGR - 1;

  // set current thread to same pri as new thread to make sure this is the
  // thread we'll return to after creation
  osSetThreadPri(NULL, faultThreadPri - 1);

  // Create message queue and thread structures for fault handling thread
  osCreateMesgQueue(&faultMsgQ, &faultMsgBuf, 1);

  osSetEventMesg(OS_EVENT_FAULT, &faultMsgQ, NULL);
  osSetEventMesg(OS_EVENT_CPU_BREAK, &faultMsgQ, breakMsg);

  DBGPRINT("creating fault thread with pri=%d, current thread pri=%d\n",
           faultThreadPri, currentThreadOriginalPri);

  osCreateThread(&faultThread, /*id*/ 60, (void (*)(void*))faultproc,
                 /*argv*/ NULL, faultThreadStack + ED64IO_FAULT_STACKSIZE / 8,
                 /*priority*/ (OSPri)faultThreadPri);

  DBGPRINT("=> ed64StartFaultHandlerThread - starting fault thread...\n");
  osStartThread(&faultThread);

  if (startedfaultproc) {
    DBGPRINT("=> returned from osStartThread(&faultThread); state=%d\n",
             faultThread.state);

    // drop current thread back to its correct priority
    osSetThreadPri(NULL, currentThreadOriginalPri);
  }
}

typedef struct {
  u32* breakAddress;
  u32 oldInstruction;
} Breakpoint;

#define BREAK_INSTRUCTION_MASK 0xfc00003f
/*
  The following macro defines a breakpoint instruction with a
  built in breakpoint number. Thus, we always know which
  breakpoint was hit as soon as the break occurs, saving the
  time and trouble of actually looking it up. We add 16 to the
  break number to stay away from compiler-defined breaks and
  user set breaks.
*/

#define breakinst(n) (0x0d | (((n + 16) & 0xfffff) << 6))

#define breakinst_no_offset(n) (0x0d | (((n)&0xfffff) << 6))

#define NUM_BREAKPOINTS 2
static Breakpoint breakpoints[NUM_BREAKPOINTS]; /* the CPU breaks */
static Breakpoint altBreak;                     /* the other temp break */

void ed64SetBreakpoint(u32* address) {
  setBreakpoint(address, NULL);
}

// takes 2 args so in the case of a branch we can set both at the same time
static void setBreakpoint(u32* addr1, u32* addr2) {
  DBGPRINT("Set temp BP at %08x", addr1);
  if (addr2)
    DBGPRINT(" and %08x", addr2);
  DBGPRINT("\n");

  breakpoints[0].oldInstruction = *addr1;
  *addr1 = breakinst(0);
  osWritebackDCache(addr1, 4);
  osInvalICache(addr1, 4);
  breakpoints[0].breakAddress = addr1;
  if (addr2) {
    altBreak.oldInstruction = *addr2;
    *addr2 = breakinst(0);
    osWritebackDCache(addr2, 4);
    osInvalICache(addr2, 4);
    altBreak.breakAddress = addr2;
  }
}

static void clearBreakpoint(void) {
  u32 inst;

  if (breakpoints[0].breakAddress) {
    DBGPRINT("clearBreakpoint at %08x\n", breakpoints[0].breakAddress);
    /* Check to make sure one was really there */

    inst = *breakpoints[0].breakAddress;
    if ((inst & BREAK_INSTRUCTION_MASK) == 0xd) {
      *breakpoints[0].breakAddress = breakpoints[0].oldInstruction;
      osWritebackDCache(breakpoints[0].breakAddress, 4);
      osInvalICache(breakpoints[0].breakAddress, 4);
    }
    breakpoints[0].breakAddress = 0;
  }
  if (altBreak.breakAddress) {
    DBGPRINT("clearBreakpoint at %08x\n", altBreak.breakAddress);
    inst = *altBreak.breakAddress;
    if ((inst & BREAK_INSTRUCTION_MASK) == 0xd) {
      *altBreak.breakAddress = altBreak.oldInstruction;
      osWritebackDCache(altBreak.breakAddress, 4);
      osInvalICache(altBreak.breakAddress, 4);
    }
    altBreak.breakAddress = 0;
  }
}

static int isJumpInstruction(u32 inst) {
  switch ((inst >> 26) & 0x3f) {
    case 0x0:  // jump register
      if ((((inst >> 5) & 0x7fff) == 0) && ((inst & 0x3f) == 8))
        return 1;

      // jump and link register
      if ((((inst >> 16) & 0x1f) == 0) && ((inst & 0x7ff) == 9))
        return 1;
      break;

    case 0x2:  // jump
    case 0x3:  // jump and link
      return 1;
  }
  return 0;
}

extern u32 __rmonGetBranchTarget(int, int, char*);

static int doSingleStep(int thread, u32* instptr) {
  u32 branchTarget;
  branchTarget =
      __rmonGetBranchTarget(/*METHOD_NORMAL*/ 0, thread, (char*)instptr);
  if (branchTarget & 3) {
    setBreakpoint(instptr + 1, 0);
  }

  else if (branchTarget == (u32)instptr)
    return 0;
  else if (isJumpInstruction(*instptr) || (branchTarget == (u32)(instptr + 2)))
    setBreakpoint((u32*)branchTarget, 0);
  else
    setBreakpoint((u32*)branchTarget, instptr + 2);
  return 1;
}

extern OSThread* __osRunningThread;

extern OSThread* __osRunQueue;

static char* threadStateStrings[] = {"stopped", "runnable", "running",
                                     "waiting", "invalid"};

static char* getThreadStateName(int state) {
  switch (state) {
    case OS_STATE_STOPPED:
      return threadStateStrings[0];
    case OS_STATE_RUNNABLE:
      return threadStateStrings[1];
    case OS_STATE_RUNNING:
      return threadStateStrings[2];
    case OS_STATE_WAITING:
      return threadStateStrings[3];
  }
  return threadStateStrings[4];
}

// TODO: this code keeps breaking. figure out how to safely traverse thread list
void ed64PrintThreads(int withStackTrace) {
  // register OSThread* tptr = __osRunQueue;
  // register OSThread* tptr = __osRunningThread;
  register OSThread* tptr = __osGetActiveQueue();
  OSThread* startPtr = tptr;
  PRINTF("printing threads\n");

  while (TRUE) {
    if (tptr->priority == -1 || tptr->id < 0) {
      break;  // hack to unbreak this code for now
      // this is the sentinel marking the end of the thread list, skip over it
      tptr = tptr->tlnext;
      continue;
    }
    if (tptr->id == 0) {
      PRINTF("OS Thread at %08x [%s] pri=%d pc=%08x\n", tptr,
             getThreadStateName(getThreadStateForDebugger(tptr)),
             tptr->priority, tptr->context.pc);
    } else {
      PRINTF("Thread %d [%s] pri=%d pc=%08x\n", tptr->id,
             getThreadStateName(getThreadStateForDebugger(tptr)),
             tptr->priority, tptr->context.pc);
    }
    if (withStackTrace) {
      ed64PrintStackTrace(tptr, 0);
    }

    tptr = tptr->tlnext;
    // the thread queue is a circular list so we need to break out once we get
    // back to the start
    if (tptr == startPtr) {
      break;
    }
  }

  PRINTF("done\n");
}

typedef struct ThreadStateMessage {
  // using u32 for these so struct fields all have 32bit alignment
  u32 id, state, priority;
  u32 pc, ra;
  // u32 stacktrace[100];
} ThreadStateMessage;

// static void sendRegisters(OSThread* t) {
//   __OSThreadContextHack* tc = (__OSThreadContextHack*)&t->context;
// }

// includes extra space at the end for the stacktrace
#define THREADSTATE_MESSAGE_MAX_SIZE \
  sizeof(ThreadStateMessage) + (100 * sizeof(u32))

static void sendThreadStates() {
  register OSThread* tptr = __osGetActiveQueue();
  OSThread* startPtr = tptr;
  u8 message[THREADSTATE_MESSAGE_MAX_SIZE];

  while (TRUE) {
    if (tptr->priority == -1 || tptr->id < 0) {
      break;
    }
    // skip OS threads
    if (tptr->id != 0 && tptr->id < 255 && tptr != &faultThread) {
      {
        u8* messageEnd = message;  // ptr to where we're adding the next field
        __OSThreadContext* tc = &tptr->context;
        ThreadStateMessage threadState;

        int stackTraceSize = getCallStackNoFp(tc->sp, tc->ra);
        // PRINTF("sendThreadStates thread %d\n", tptr->id);
        threadState = (ThreadStateMessage){
            tptr->id,
            getThreadStateForDebugger(tptr),  // state
            // tptr->state,
            tptr->priority,
            tptr->context.pc,
            tc->ra,
        };

        *((ThreadStateMessage*)messageEnd) = threadState;
        messageEnd += sizeof(ThreadStateMessage);

        {
          int i;
          for (i = 0; i < stackTraceSize; ++i) {
            *((u32*)messageEnd) = (u32)stackTraceReturnAddresses[i];
            messageEnd += sizeof(u32);
            // threadState.stacktrace[i] = (u32)stackTraceReturnAddresses[i];
          }
        }

        // PRINTF("ed64SendBinaryData for threadstate %d type=%d length=%d\n",
        //        tptr->id, ThreadPacket, messageEnd - message);
        if (messageEnd - message > THREADSTATE_MESSAGE_MAX_SIZE) {
          PRINTF("sendThreadStates memory corruption (%d bytes)\n",
                 (messageEnd - message) - THREADSTATE_MESSAGE_MAX_SIZE);
        }
        ed64SendBinaryData(message, ThreadPacket, messageEnd - message);
        // if (ed64SendBinaryData(message, ThreadPacket, messageEnd - message))
        // {
        //   // PRINTF("ed64SendBinaryData failed\n");
        // } else {
        //   // PRINTF("ed64SendBinaryData success\n");
        // }
      }
    }
    // ed64PrintStackTrace(tptr, 0);
    tptr = tptr->tlnext;
    // the thread queue is a circular list so we need to break out once we get
    // back to the start
    if (tptr == startPtr) {
      break;
    }
  }
}

static void dumpThreads() {
  register OSThread* tptr = __osGetActiveQueue();
  OSThread* startPtr = tptr;

  while (ed64AsyncLoggerFlush() != -1) {
    evd_sleep(1);
  }
  while (TRUE) {
    if (tptr->priority == -1 || tptr->id < 0) {
      break;  // hack to unbreak this code for now
      // this is the sentinel marking the end of the thread list, skip over it
      tptr = tptr->tlnext;
      continue;
    }
    if (tptr->id != 0) {
      ed64Printf("EDBG=thread %d %s %d %08x ", tptr->id,
                 getThreadStateName(getThreadStateForDebugger(tptr)),
                 tptr->priority, tptr->context.pc);
      {
        __OSThreadContext* tc = &tptr->context;

        int stackTraceSize = getCallStackNoFp(tc->sp, tc->ra);

        ed64Printf("%08x ", (u32)(tc->pc));
        ed64Printf("%08x ", (u32)(tc->ra));
        {
          int i;
          for (i = 0; i < stackTraceSize; ++i) {
            ed64Printf("%08x ", (u32)stackTraceReturnAddresses[i]);
          }
        }

        ed64Printf("\n");
        while (ed64AsyncLoggerFlush() != -1) {
          evd_sleep(1);
        }
      }
    }
    // ed64PrintStackTrace(tptr, 0);

    tptr = tptr->tlnext;
    // the thread queue is a circular list so we need to break out once we get
    // back to the start
    if (tptr == startPtr) {
      break;
    }
  }
}

int ed64DebuggerUsbListener(OSThread* tptr);

static void walkFaultedThreads(void) {
  register OSThread* tptr = __osGetActiveQueue();
  OSThread* threadAtBreakpoint = NULL;

  // find thread at breakpoint (currently we can only handle one at a time)
  while (tptr->priority != -1) {
    if (tptr->priority > OS_PRIORITY_IDLE &&
        tptr->priority <= OS_PRIORITY_APPMAX) {
      if (tptr->flags & OS_FLAG_CPU_BREAK) {
        threadAtBreakpoint = tptr;
      }
    }
    tptr = tptr->tlnext;
  }

  if (threadAtBreakpoint) {
    tptr = threadAtBreakpoint;
    // first, if any threads are at a breakpoint, we need to stop all user
    // threads so they don't interfere the debugger, or get out of sync with the
    // thread being debugged
    stopUserThreads();

    // thread at a breakpoint
    DBGPRINT("Brk in thread %d @ %08x, inst %08x\r\n", tptr->id,
             tptr->context.pc, *(int*)tptr->context.pc);

    printFaultData(tptr);

    PRINTF("EDBG=break %d\n", tptr->id);
    PRINTF("dumpThreads\n");
    dumpThreads();
    PRINTF("sendThreadStates \n");
    sendThreadStates();
    PRINTF("sendRegisters\n");
    sendRegisters(tptr);
    PRINTF("stopping  \n");

    // don't allow user threads to continue while debugger is active
    // stopUserThreads();

    // if the current instruction is `break 1000`, automatically resume
    if (*(int*)tptr->context.pc == breakinst_no_offset(1000)) {
      // *(int*)tptr->context.pc = 0; // replace current instruction with nop
      tptr->context.pc += 4;  // advance PC past the breakpoint
    } else {
      // poll for command from debugger client
      while (!ed64DebuggerUsbListener(tptr)) {
        evd_sleep(1000);
      }
    }
    DBGPRINT("continuing\n");
  }
}

#define USB_BUFFER_SIZE 128

int ed64DebuggerUsbListener(OSThread* tptr) {
  char cmd;
  u32 usb_rx_buff32[USB_BUFFER_SIZE];
  char* usb_rx_buff8 = (char*)usb_rx_buff32;
  memset(usb_rx_buff8, 0, USB_BUFFER_SIZE * 4);

  if (evd_fifoRxf())  // when pin low, receive buffer not empty yet
    return FALSE;

  DBGPRINT("starting read\n");
  // returns timeout error, at which time we just try again
  while (evd_fifoRd(usb_rx_buff32, 1)) {
    DBGPRINT("sleeping\n");
    evd_sleep(100);
  }
  DBGPRINT("dma read done\n");

  DBGPRINT("message: %c%c%c%c\n", usb_rx_buff8[0], usb_rx_buff8[1],
           usb_rx_buff8[2], usb_rx_buff8[3]);

  if (usb_rx_buff8[0] != 'C' || usb_rx_buff8[1] != 'M' ||
      usb_rx_buff8[2] != 'D') {
    PRINTF("invalid message\n");
    return FALSE;
  }

  cmd = usb_rx_buff8[3];
  DBGPRINT("got command: '%c'\n", cmd);

  switch (cmd) {
    case 'b': {
      u32 breakpointAddr = *((u32*)usb_rx_buff32 + 1);  // start + 4 bytes (u32)
      PRINTF("set breakpoint at: 0x%x\n", breakpointAddr);
      setBreakpoint((u32*)breakpointAddr, NULL);
      resumeUserThreads();
      return TRUE;
    }
    case 's':
      PRINTF("single step, thread=%d pc=%x\n", tptr->id, tptr->context.pc);
      if (doSingleStep(tptr->id, (u32*)tptr->context.pc) != 1) {
        PRINTF("failed to set single step\n");
      }
      resumeUserThreads();
      return TRUE;
    case 'r':
      PRINTF("resuming normal execution\n");
      // don't set another breakpoint
      resumeUserThreads();
      return TRUE;
    default:
      PRINTF("invalid command: '%c'\n", cmd);
  }

  return FALSE;
}
