#include <string.h>

#include "ed64io_everdrive.h"
#include "ed64io_sys.h"
#include "ed64io_types.h"
// #include "rom.h"

u8 cmdTest();
u8 cmdFill();
u8 cmdReadRom();
u8 cmdWriteRom();

#define USB_BUFFER_SIZE 128
#define USB_BUFFER_SIZE_BYTES 1024
#define USB_LOGGER_BUFFER_SIZE_BYTES 512

u64 usb_buff[USB_BUFFER_SIZE];
u8* usb_buff8;  // = (u8 *) usb_buff;

extern u8 system_cic;

char* cmdStart = "CMD";
char* overflowMsg = "LOGOVERFLOW!!!";

evd_fifoWrNonblockState fifoWriteState;
char usbLoggerData[USB_LOGGER_BUFFER_SIZE_BYTES];
int usbLoggerOffset = 0;
int usbLoggerFlushing = FALSE;
int usbLoggerOverflow = FALSE;

int usbLoggerLog(char* str) {
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

  // step IO state machine
  evd_fifoWrNonblock(usb_buff, 1, &fifoWriteState);
  if (!fifoWriteState.done) {
    return fifoWriteState.state;
  }

  usbLoggerFlushing = FALSE;
  return 0;
}

u8 usbLogger(char* str, int newTransfer) {
  if (newTransfer) {
    memcpy(usb_buff, str, USB_BUFFER_SIZE_BYTES);

    evd_fifoWrNonblockStateInit(&fifoWriteState);
  }

  evd_fifoWrNonblock(usb_buff, 1, &fifoWriteState);
  if (!fifoWriteState.done)
    return fifoWriteState.state;

  return 0;
}

u8 usbListener(int userData) {
  volatile u16 resp;
  volatile u8 cmd;
  usb_buff8 = (u8*)usb_buff;

  if (evd_fifoRxf())
    return 0;

  resp = evd_fifoRd(usb_buff, 1);

  if (resp != 0)
    return 1;

  if (usb_buff8[0] != 'C' || usb_buff8[1] != 'M' || usb_buff8[2] != 'D')
    return 2;

  cmd = usb_buff8[3];

  switch (cmd) {
    case 'R':
      resp = cmdReadRom();
      if (resp)
        return 10;
      break;
    case 'W':
      resp = cmdWriteRom();
      if (resp)
        return 11;
      break;
    case 'T':
      resp = cmdTest();
      if (resp)
        return 12;
      break;
    case 'F':
      resp = cmdFill();
      if (resp)
        return 13;
      break;
    case 'S':
      // IO_WRITE(PI_BSD_DOM1_PGS_REG, 0x0c);
      // IO_WRITE(PI_BSD_DOM1_PGS_REG, 0x80);
      // evdSetESaveType(SAVE_TYPE_EEP16k);
      // system_cic = CIC_6102;
      // evd_lockRegs();
      // IO_WRITE(PI_STATUS_REG, 3);
      // sleep(2);
      // pif_boot();
      break;
  }

  return -1;
}

u8 cmdTest() {
  u16 resp;
  usb_buff8[3] = 'k';
  resp = evd_fifoWr(usb_buff, 1);
  if (resp)
    return 1;

  return 0;
}

u8 cmdFill() {
  u16 resp;
  u32 i;
  // console_printf("fill...\n");

  for (i = 0; i < 512; i++) {
    usb_buff8[i] = 0;
  }
  // console_printf("buff prepared\n");
  // romFill(0, 0x200000, 0);
  // console_printf("fill done\n");

  usb_buff8[3] = 'k';
  resp = evd_fifoWr(usb_buff, 1);

  if (resp)
    return 1;
  // console_printf("resp sent ok\n");

  return 0;
}

u8 cmdReadRom() {
  u16 resp;
  u16 ptr;
  u16 len;
  u32 addr;
  ptr = 4;

  addr = usb_buff8[ptr++];
  addr <<= 8;
  addr |= usb_buff8[ptr++];
  addr *= 2048;

  len = usb_buff8[ptr++];
  len <<= 8;
  len |= usb_buff8[ptr++];

  resp = evd_fifoWrFromCart(addr, len);
  if (resp)
    return 1;

  return 0;
}

u8 cmdWriteRom() {
  u16 resp;
  u16 ptr;
  u16 len;
  u32 addr;
  ptr = 4;

  addr = usb_buff8[ptr++];
  addr <<= 8;
  addr |= usb_buff8[ptr++];
  addr *= 2048;

  len = usb_buff8[ptr++];
  len <<= 8;
  len |= usb_buff8[ptr++];

  resp = evd_fifoRdToCart(addr, len);
  if (resp)
    return 1;

  return 0;
}
