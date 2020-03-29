#include <stdarg.h>
#include <string.h>

#include "ed64io_everdrive.h"
#include "ed64io_sys.h"
#include "ed64io_types.h"
#include "ed64io_usb.h"

#define USB_BUFFER_SIZE 128
#define USB_BUFFER_SIZE_BYTES 1024
#define USB_LOGGER_BUFFER_SIZE_BYTES 512

#ifndef MIN
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#endif

u64 usb_buff[USB_BUFFER_SIZE];
u8* usb_buff8;  // = (u8 *) usb_buff;

extern u8 system_cic;

char* cmdStart = "CMD";
char* overflowMsg = "LOGOVERFLOW!!!\n";

evd_fifoWrNonblockState fifoWriteState;
char usbLoggerData[USB_LOGGER_BUFFER_SIZE_BYTES];
int usbLoggerOffset = 0;
int usbLoggerFlushing = FALSE;
int usbLoggerOverflow = FALSE;
int usbLoggerCountDone = 0;

extern void _Printf(void (*)(void*), int, const char*, va_list);

int usbLoggerBufferRemaining() {
  return USB_LOGGER_BUFFER_SIZE_BYTES - usbLoggerOffset;
}

int usbLoggerLog(const char* str) {
  char* emptySpaceStart = usbLoggerData;
  int lengthToWrite;
  if (usbLoggerOverflow) {
    return -1;
  }
  lengthToWrite = strlen(str);

  if (usbLoggerOffset + lengthToWrite >= USB_LOGGER_BUFFER_SIZE_BYTES) {
    memcpy(usbLoggerData +
               (USB_LOGGER_BUFFER_SIZE_BYTES - 1 - (strlen(overflowMsg) + 1)),
           overflowMsg, (strlen(overflowMsg) + 1));
    usbLoggerOverflow = TRUE;
    return -1;
  }

  emptySpaceStart += usbLoggerOffset;
  strcpy(emptySpaceStart, str);
  usbLoggerOffset += lengthToWrite;

  return lengthToWrite;
}

void usbLoggerGetState(UsbLoggerState* res) {
  res->fifoWriteState = fifoWriteState.state;
  res->msgID = fifoWriteState.id;
  res->usbLoggerOffset = usbLoggerOffset;
  res->usbLoggerFlushing = usbLoggerFlushing;
  res->usbLoggerOverflow = usbLoggerOverflow;
  res->msgQSize = fifoWriteState.dmaMesgQ.validCount;
  res->countDone = usbLoggerCountDone;
  res->writeError = fifoWriteState.error;
}

int usbLoggerFlush() {
  if (!usbLoggerFlushing) {
    if (!usbLoggerOffset) {
      // nothing to write
      return -1;
    }
    memcpy(usb_buff, usbLoggerData,
           MIN(USB_LOGGER_BUFFER_SIZE_BYTES, usbLoggerOffset + 1));
    usbLoggerOffset = 0;
    usbLoggerOverflow = FALSE;

    evd_fifoWrNonblockStateInit(&fifoWriteState);
    usbLoggerFlushing = TRUE;
  }
  if (usbLoggerFlushing) {
    if (fifoWriteState.error != 0) {
      return -2;
    }
  }

  // step IO state machine
  evd_fifoWrNonblock(usb_buff, 1, &fifoWriteState);
  if (!fifoWriteState.done) {
    return fifoWriteState.state;
  }

  usbLoggerFlushing = FALSE;
  usbLoggerCountDone++;
  return 0;
}

static void* _PrintfImplUSBAsync(void* str,
                                 register const char* buf,
                                 register int n) {
  char tocopy[USB_LOGGER_BUFFER_SIZE_BYTES];

  memcpy(tocopy, buf, MIN(USB_LOGGER_BUFFER_SIZE_BYTES, n));
  if (n < USB_LOGGER_BUFFER_SIZE_BYTES) {
    tocopy[n] = '\0';
  }

  usbLoggerLog(tocopy);
  return ((void*)1);
}

static void* _PrintfImplUSBSync(void* str,
                                register const char* buf,
                                register int n) {
  u64 usbMsgBuff[USB_BUFFER_SIZE];
  char* usbBuffCharPtr = (char*)usbMsgBuff;

  memcpy(usbBuffCharPtr, buf, MIN(USB_BUFFER_SIZE, n));
  if (n < USB_BUFFER_SIZE) {
    usbBuffCharPtr[n] = '\0';
  }

  while (evd_fifoWr(usbMsgBuff, 1)) {
    // retry on timeout
  }

  return ((void*)1);
}

void ed64Printf(const char* fmt, ...) {
  va_list ap;

  va_start(ap, fmt);
  _Printf((void (*)(void*))_PrintfImplUSBAsync, 0, fmt, ap);
  va_end(ap);
}

void ed64PrintfSync(const char* fmt, ...) {
  va_list ap;
  return;

  va_start(ap, fmt);
  _Printf((void (*)(void*))_PrintfImplUSBSync, 0, fmt, ap);
  va_end(ap);
}
