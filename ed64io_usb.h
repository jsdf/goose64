/*
 * File:   fifo.h
 * Author: KRIK
 *
 * Created on 22 јпрель 2011 г., 20:46
 */

#ifndef _FIFO_H
#define _FIFO_H

#include "ed64io_types.h"
int usbLoggerLog(const char* str);
int usbLoggerFlush();

typedef struct UsbLoggerState {
  int fifoWriteState;
  int msgID;
  int usbLoggerOffset;
  int usbLoggerFlushing;
  int usbLoggerOverflow;
  int msgQSize;
  int countDone;
  int writeError;
} UsbLoggerState;

void usbLoggerGetState(UsbLoggerState* res);

int usbLoggerBufferRemaining();

void ed64Printf(const char* fmt, ...);

void ed64PrintfSync(const char* fmt, ...);

#endif /* _FIFO_H */
