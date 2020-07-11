/*
 * File:   fifo.h
 * Author: KRIK
 *
 * Created on 22 јпрель 2011 г., 20:46
 */

#ifndef _ED64IO_USB_H
#define _ED64IO_USB_H

#include <stdarg.h>

#include "ed64io_types.h"

int ed64AsyncLoggerFlush();
// for backwards compat
#define usbLoggerFlush ed64AsyncLoggerFlush

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

void ed64PrintfSync2(const char* fmt, ...);

void ed64VPrintfSync2(const char* fmt, va_list ap);

int ed64SendBinaryData(const void* data, u16 type, u16 length);

#endif /* _ED64IO_USB_H */
