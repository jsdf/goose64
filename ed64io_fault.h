
#ifndef _ED64IO_FAULT_H
#define _ED64IO_FAULT_H

#include <ultra64.h>

#define ED64IO_FAULT_STACKSIZE 0x2000

void ed64StartFaultHandlerThread(int mainThreadPri);

void ed64PrintStackTrace(OSThread* t, int framesToSkip);

void ed64SetBreakpoint(u32* address);
#endif /* _ED64IO_FAULT_H */
