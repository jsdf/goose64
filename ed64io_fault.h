
#ifndef _ED64IO_FAULT_H
#define _ED64IO_FAULT_H
#define ED64IO_FAULT_STACKSIZE 0x2000
#define ED64IO_FAULT_DMA_QUEUE_SIZE 200

void ed64StartFaultHandlerThread(int mainThreadPri);

#endif /* _ED64IO_FAULT_H */
