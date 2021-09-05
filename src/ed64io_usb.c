

#include <string.h>

#include "ed64io_everdrive.h"
#include "ed64io_sys.h"
#include "ed64io_types.h"
#include "ed64io_usb.h"

// everdrive transfers blocks of 512 bytes at a time
// however, to ensure correct alignment for DMA, we allocate a 1024 byte buffer
#define USB_BUFFER_SIZE 128
#define USB_BUFFER_SIZE_BYTES 1024
#define USB_LOGGER_BUFFER_SIZE_BYTES 512
#define ED64_BLOCK_BYTES 512

#ifndef MIN
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#endif

// only a single transfer can be using this buffer at a time so we
// coordinate this with a lock mechanism
static u64 usb_buff[USB_BUFFER_SIZE];

static evd_fifoWrNonblockState fifoWriteState;
// lock to prevent any new transfer from starting while one is in progress
static int usbSendLocked = FALSE;
static int usbSendCountDone = 0;
static int usbSendCountFailed = 0;

static char* loggerOverflowMsg = "LOGOVERFLOW!!!\n";
static char loggerData[USB_LOGGER_BUFFER_SIZE_BYTES];
static int loggerOffset = 0;
static int loggerOverflow = FALSE;

extern void _Printf(void (*)(void*), int, const char*, va_list);

void ed64USBSendStart();
int ed64USBSendDrain();

int usbLoggerBufferRemaining() {
  return USB_LOGGER_BUFFER_SIZE_BYTES - loggerOffset;
}

typedef struct BinaryPacketHeader {
  char magic[4];
  u16 type;
  u16 length;
} BinaryPacketHeader;

// syncronously send binary data
int ed64SendBinaryData(const void* data, u16 type, u16 length) {
  u8* message = (u8*)usb_buff;
  u8* messageEnd = message;

  // first we need to finish any existing usb send (eg. logger) so we can have
  // exclusive control of send buffer
  while (ed64USBSendDrain() != 0) {
    evd_sleep(1);
  }

  // BinaryPacketHeader header =
  //     (BinaryPacketHeader){{'\0', 'b', 'i', 'n'}, type, length};

  // logging via printf after this point will mess up data in usb_buff
  memset(message, 0, ED64_BLOCK_BYTES);

  // memcpy(message, &header, sizeof(BinaryPacketHeader));

  message[0] = '\0';
  message[1] = 'b';
  message[2] = 'i';
  message[3] = 'n';
  messageEnd += 4;
  // write packet type into header
  *(u16*)(messageEnd) = type;
  messageEnd += 2;
  // write transfer size into header
  *(u16*)(messageEnd) = length;
  messageEnd += 2;
  if (length > ED64_BLOCK_BYTES - /*header size*/ (messageEnd - message)) {
    return 1;
  }

  if (messageEnd - message != 8) {
    ed64PrintfSync2("ed64SendBinaryData invalid header size: %d\n",
                    messageEnd - message);
    return 1;
  }

  memcpy(messageEnd, data, length);

  // do the transfer
  ed64USBSendStart();
  while (ed64USBSendDrain() != 0) {
    evd_sleep(1);
  }

  // ok to log via printf again
  if (fifoWriteState.error != 0) {
    // ed64PrintfSync2("ed64SendBinaryData write error\n");
    return 2;
  }
  return 0;
}

static int loggerAppendLog(const char* str) {
  char* emptySpaceStart = loggerData;
  int lengthToWrite;
  if (loggerOverflow) {
    return -1;
  }
  lengthToWrite = strlen(str);

  if (loggerOffset + lengthToWrite >= USB_LOGGER_BUFFER_SIZE_BYTES) {
    memcpy(loggerData + (USB_LOGGER_BUFFER_SIZE_BYTES - 1 -
                         (strlen(loggerOverflowMsg) + 1)),
           loggerOverflowMsg, (strlen(loggerOverflowMsg) + 1));
    loggerOverflow = TRUE;
    return -1;
  }

  emptySpaceStart += loggerOffset;
  strcpy(emptySpaceStart, str);
  loggerOffset += lengthToWrite;

  return lengthToWrite;
}

// for debugging
void usbLoggerGetState(UsbLoggerState* res) {
  res->fifoWriteState = fifoWriteState.state;
  res->msgID = fifoWriteState.id;
  res->usbLoggerOffset = loggerOffset;
  res->usbLoggerFlushing = usbSendLocked;
  res->usbLoggerOverflow = loggerOverflow;
  res->msgQSize = fifoWriteState.dmaMesgQ.validCount;
  res->countDone = usbSendCountDone;
  res->writeError = fifoWriteState.error;
}

void ed64USBSendStart() {
  // ideally we'd assert here but how do you log a failure in your logging
  // system?

  // assert(!usbSendLocked);

  evd_fifoWrNonblockStateInit(&fifoWriteState);
  usbSendLocked = TRUE;
}

// step io for current send
// returns non-zero if send is not finished
int ed64USBSendDrain() {
  if (!usbSendLocked) {
    return 0;
  }

  // cleanup in error case and unlock
  if (fifoWriteState.error != 0) {
    // this is basically swallowing the error, but in the general case there's
    // not much we can do about errors, so they're only useful when debugging
    // more low level methods
    usbSendLocked = FALSE;
    usbSendCountFailed++;
    return 0;
  }

  // step IO state machine
  evd_fifoWrNonblock(usb_buff, 1, &fifoWriteState);
  if (!fifoWriteState.done) {
    return 1;
  }

  // unlock for next transfer
  usbSendLocked = FALSE;
  usbSendCountDone++;
  return 0;
}

// call this regularly to allow the delivery of ed64Printf logs to the host
int ed64AsyncLoggerFlush() {
  // if not already busy, start the next transfer
  if (!usbSendLocked) {
    if (!loggerOffset) {
      // nothing to write
      return -1;
    }

    memset(usb_buff, 0, USB_LOGGER_BUFFER_SIZE_BYTES);
    memcpy(usb_buff, loggerData,
           MIN(USB_LOGGER_BUFFER_SIZE_BYTES, loggerOffset + 1));

    loggerOffset = 0;
    loggerOverflow = FALSE;

    ed64USBSendStart();
  }
  // in either case, step io for the current transfer (if any)
  return ed64USBSendDrain();
}

static void* _PrintfImplUSBAsync(void* str,
                                 register const char* buf,
                                 register int n) {
  char tocopy[USB_LOGGER_BUFFER_SIZE_BYTES];

  memcpy(tocopy, buf, MIN(USB_LOGGER_BUFFER_SIZE_BYTES, n));
  if (n < USB_LOGGER_BUFFER_SIZE_BYTES) {
    tocopy[n] = '\0';
  }

  loggerAppendLog(tocopy);
  return ((void*)1);
}

static void* _PrintfImplUSBSync(void* str,
                                register const char* buf,
                                register int n) {
  char usbMsgBuff[USB_LOGGER_BUFFER_SIZE_BYTES];
  char* usbBuffCharPtr = (char*)usbMsgBuff;
  memset(usbMsgBuff, 0, USB_LOGGER_BUFFER_SIZE_BYTES);

  memcpy(usbBuffCharPtr, buf, MIN(USB_LOGGER_BUFFER_SIZE_BYTES, n));
  if (n < USB_LOGGER_BUFFER_SIZE_BYTES) {
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

  va_start(ap, fmt);
  _Printf((void (*)(void*))_PrintfImplUSBSync, 0, fmt, ap);
  va_end(ap);
}

// version of ed64PrintfSync implemented in terms of async transfer method
void ed64PrintfSync2(const char* fmt, ...) {
  va_list ap;

  va_start(ap, fmt);
  _Printf((void (*)(void*))_PrintfImplUSBAsync, 0, fmt, ap);
  va_end(ap);
  // wait for previous flush to finish, and drain logger buffer
  while (ed64AsyncLoggerFlush() != -1) {
    evd_sleep(1);
  }
  // flush current and wait
  while (ed64AsyncLoggerFlush() != -1) {
    evd_sleep(1);
  }
}

// to make osSyncPrintf work:
// __printfunc = ed64PrintFuncImpl;
void* ed64PrintFuncImpl(void* str, register const char* buf, register int n) {
  void* ret = _PrintfImplUSBAsync(str, buf, n);
  // wait for previous flush to finish, and drain logger buffer
  while (ed64AsyncLoggerFlush() != -1) {
    evd_sleep(1);
  }
  // flush current and wait
  while (ed64AsyncLoggerFlush() != -1) {
    evd_sleep(1);
  }
  return ret;
}

// same but with takes varargs pointer as an arg
void ed64VPrintfSync2(const char* fmt, va_list ap) {
  _Printf((void (*)(void*))_PrintfImplUSBAsync, 0, fmt, ap);
}

void ed64Assert(int expression) {
  if (!(expression)) {
    ed64PrintfSync("assertion failed in %s at %s:%d\n", __FUNCTION__, __FILE__,
                   __LINE__);
    while (1) {
    }
  }
}

void ed64AssertMsg(int expression, char* msg) {
  if (!(expression)) {
    ed64PrintfSync(msg);
    while (1) {
    }
  }
}
