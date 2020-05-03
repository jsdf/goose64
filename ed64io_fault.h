
#ifndef _ED64IO_FAULT_H
#define _ED64IO_FAULT_H
#define ED64IO_FAULT_STACKSIZE 0x2000
#define ED64IO_FAULT_DMA_QUEUE_SIZE 200

void ed64StartFaultHandlerThread(int mainThreadPri);

void ed64PrintStackTrace(OSThread* t, int framesToSkip);
void ed64SendFaultMessage(OSThread* t);

#endif /* _ED64IO_FAULT_H */
