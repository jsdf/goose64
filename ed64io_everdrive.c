
#include "ed64io_everdrive.h"
#include "ed64io_types.h"

#include <stdio.h>
#include "ed64io_errors.h"
#include "ed64io_sys.h"

#define CMD0 0x40   // software reset
#define CMD1 0x41   // brings card out of idle state
#define CMD2 0x42   // not used in SPI mode
#define CMD3 0x43   // not used in SPI mode
#define CMD4 0x44   // not used in SPI mode
#define CMD5 0x45   // Reserved
#define CMD6 0x46   // Reserved
#define CMD7 0x47   // not used in SPI mode
#define CMD8 0x48   // Reserved
#define CMD9 0x49   // ask card to send card speficic data (CSD)
#define CMD10 0x4A  // ask card to send card identification (CID)
#define CMD11 0x4B  // not used in SPI mode
#define CMD12 0x4C  // stop transmission on multiple block read
#define CMD13 0x4D  // ask the card to send it's status register
#define CMD14 0x4E  // Reserved
#define CMD15 0x4F  // not used in SPI mode
#define CMD16 0x50  // sets the block length used by the memory card
#define CMD17 0x51  // read single block
#define CMD18 0x52  // read multiple block
#define CMD19 0x53  // Reserved
#define CMD20 0x54  // not used in SPI mode
#define CMD21 0x55  // Reserved
#define CMD22 0x56  // Reserved
#define CMD23 0x57  // Reserved
#define CMD24 0x58  // writes a single block
#define CMD25 0x59  // writes multiple blocks
#define CMD26 0x5A  // not used in SPI mode
#define CMD27 0x5B  // change the bits in CSD
#define CMD28 0x5C  // sets the write protection bit
#define CMD29 0x5D  // clears the write protection bit
#define CMD30 0x5E  // checks the write protection bit
#define CMD31 0x5F  // Reserved
#define CMD32 0x60  // Sets the address of the first sector of the erase group
#define CMD33 0x61  // Sets the address of the last sector of the erase group
#define CMD34 0x62  // removes a sector from the selected group
#define CMD35 0x63  // Sets the address of the first group
#define CMD36 0x64  // Sets the address of the last erase group
#define CMD37 0x65  // removes a group from the selected section
#define CMD38 0x66  // erase all selected groups
#define CMD39 0x67  // not used in SPI mode
#define CMD40 0x68  // not used in SPI mode
#define CMD41 0x69  // Reserved
#define CMD42 0x6A  // locks a block
// CMD43 ... CMD57 are Reserved
#define CMD58 0x7A  // reads the OCR register
#define CMD59 0x7B  // turns CRC off
// CMD60 ... CMD63 are not used in SPI mode

#define ED_STATE_DMA_BUSY 0
#define ED_STATE_DMA_TOUT 1
#define ED_STATE_TXE 2
#define ED_STATE_RXF 3
#define ED_STATE_SPI 4

#define SPI_CFG_SPD0 0
#define SPI_CFG_SPD1 1
#define SPI_CFG_SS 2
#define SPI_CFG_RD 3
#define SPI_CFG_DAT 4
#define SPI_CFG_1BIT 5

#define SAV_EEP_ON 0
#define SAV_SRM_ON 1
#define SAV_EEP_SIZE 2
#define SAV_SRM_SIZE 3

void evd_setSpiSpeed(u8 speed);
u8 evd_mmcCmd(u8 cmd, u32 arg);

u8 sd_mode;
volatile u8 spi_cfg;
volatile u8 evd_cfg;
u8 sd_type;
volatile u32* regs_ptr = (u32*)0xA8040000;

/*
result[2] <= ad[15:8] == {ad[6], ad[1], ad[0], ad[7], ad[5], ad[4], ad[3],
ad[2]} ^ 8'h37 ^ prv[7:0]; prv[7:0] <= ad[15:8];
 */
void (*dma_busy_callback)();

void evd_setDmaAddr(u32 addr) {}

void evd_init() {
  volatile u8 val;
  sd_mode = 0;
  dma_busy_callback = 0;

  sleep(1);
  val = regs_ptr[0];

  spi_cfg = (0 << SPI_CFG_SPD0) | (1 << SPI_CFG_SPD1) | (1 << SPI_CFG_SS);
  evd_cfg = (1 << ED_CFG_SDRAM_ON);

  val = regs_ptr[0];
  regs_ptr[REG_KEY] = 0x1234;  // unlock everdrive
  // now you have access to everdrive hardware and cart momy is not read only
  // anymore

  val = regs_ptr[0];
  regs_ptr[REG_CFG] = evd_cfg;
  val = regs_ptr[0];
  regs_ptr[REG_SPI_CFG] = spi_cfg;

  evd_fifoRxf();
  if (!evd_fifoRxf()) {
    val = regs_ptr[0];
    regs_ptr[REG_DMA_LEN] = 7;  // clean 16k
    val = regs_ptr[0];
    regs_ptr[REG_DMA_RAM_ADDR] = (ROM_LEN - 0x200000) / 2048;
    val = regs_ptr[0];
    regs_ptr[REG_DMA_CFG] = DCFG_FIFO_TO_RAM;
    while (evd_isDmaBusy())
      ;
  }
}

void evd_lockRegs() {
  volatile u8 val;
  val = regs_ptr[0];
  regs_ptr[REG_KEY] = 0;
}

// some data availalbe on usb buffer
u8 evd_fifoRxf() {
  u16 val;
  // regs_ptr[REG_STATE]++;
  val = regs_ptr[REG_STATUS];
  return (val >> ED_STATE_RXF) & 1;
}

// usb ready to transfer
u8 evd_fifoTxe() {
  u16 val;
  // regs_ptr[REG_STATE]++;
  val = regs_ptr[REG_STATUS];
  return (val >> ED_STATE_TXE) & 1;
}

// everdrive dma controller state
u8 evd_isDmaBusy() {
  u16 val;
  // volatile u32 i;
  sleep(1);
  if (dma_busy_callback != 0)
    dma_busy_callback();
  // regs_ptr[REG_STATE]++;
  val = regs_ptr[REG_STATUS];
  return (val >> ED_STATE_DMA_BUSY) & 1;
}

u8 evd_isDmaBusyNoWait() {
  u16 val;
  val = regs_ptr[REG_STATUS];
  return (val >> ED_STATE_DMA_BUSY) & 1;
}

u8 evd_isDmaTimeout() {
  u16 val;
  // regs_ptr[REG_STATE]++;
  val = regs_ptr[REG_STATUS];

  return (val >> ED_STATE_DMA_TOUT) & 1;
}

// everdrive transfers data from/to rom memory, and not directly to n64 ram
// transfer from PC to cart memory
// dst address in cart address space. must me aligned to 2048. len in blocks.
// block = 512bytes
u8 evd_fifoRdToCart(u32 cart_addr, u16 blocks) {
  volatile u8 val;
  cart_addr /= 2048;

  val = regs_ptr[0];
  regs_ptr[REG_DMA_LEN] = (blocks - 1);
  val = regs_ptr[0];
  regs_ptr[REG_DMA_RAM_ADDR] = cart_addr;
  val = regs_ptr[0];
  regs_ptr[REG_DMA_CFG] = DCFG_FIFO_TO_RAM;

  while (evd_isDmaBusy())
    ;
  if (evd_isDmaTimeout())
    return EVD_ERROR_FIFO_TIMEOUT;

  return 0;
}

// transfer from cart memory to PC
// src address in cart address space. must me aligned to 2048. len in blocks.
// block = 512bytes
u8 evd_fifoWrFromCart(u32 cart_addr, u16 blocks) {
  volatile u8 val;
  cart_addr /= 2048;

  val = regs_ptr[0];
  regs_ptr[REG_DMA_LEN] = (blocks - 1);
  val = regs_ptr[0];
  regs_ptr[REG_DMA_RAM_ADDR] = cart_addr;
  val = regs_ptr[0];
  regs_ptr[REG_DMA_CFG] = DCFG_RAM_TO_FIFO;

  while (evd_isDmaBusy())
    ;
  if (evd_isDmaTimeout())
    return EVD_ERROR_FIFO_TIMEOUT;

  return 0;
}

u8 evd_fifoRd(void* buff, u16 blocks) {
  volatile u8 val;
  u32 len = blocks == 0 ? 65536 * 512 : blocks * 512;
  u32 ram_buff_addr =
      DMA_BUFF_ADDR / 2048;  //(ROM_LEN - len - 65536 * 4) / 2048;

  val = regs_ptr[0];
  regs_ptr[REG_DMA_LEN] = (blocks - 1);
  val = regs_ptr[0];
  regs_ptr[REG_DMA_RAM_ADDR] = ram_buff_addr;
  val = regs_ptr[0];
  regs_ptr[REG_DMA_CFG] = DCFG_FIFO_TO_RAM;

  while (evd_isDmaBusy())
    ;
  dma_read_s(buff, (0xb0000000 + ram_buff_addr * 2048), len);
  if (evd_isDmaTimeout())
    return EVD_ERROR_FIFO_TIMEOUT;

  return 0;
}

u8 evd_fifoWr(void* buff, u16 blocks) {
  volatile u8 val;
  u32 len = blocks == 0 ? 65536 * 512 : blocks * 512;
  u32 ram_buff_addr =
      DMA_BUFF_ADDR / 2048;  //(ROM_LEN - len - 65536 * 4) / 2048;

  dma_write_s(buff, (0xb0000000 + ram_buff_addr * 1024 * 2), len);

  val = regs_ptr[0];
  regs_ptr[REG_DMA_LEN] = (blocks - 1);
  val = regs_ptr[0];
  regs_ptr[REG_DMA_RAM_ADDR] = ram_buff_addr;
  val = regs_ptr[0];
  regs_ptr[REG_DMA_CFG] = DCFG_RAM_TO_FIFO;

  while (evd_isDmaBusy())
    ;
  if (evd_isDmaTimeout())
    return EVD_ERROR_FIFO_TIMEOUT;

  return 0;
}

#define EVERDRIVE_CART_BLOCK_WRITE_SIZE 0x4000 /* cart write block size */

void evd_fifoWrNonblockStateInit(evd_fifoWrNonblockState* state) {
  state->state = 0;
  state->done = FALSE;
}

// start DMA to cart memory
// use non blocking OS_MESG_NOBLOCK check for DMA-to-cart end
// use non blocking evd_isDmaBusyNoWait check for DMA-to-fifo end
void evd_fifoWrNonblock(void* buff,
                        u16 blocks,
                        evd_fifoWrNonblockState* state) {
  volatile u8 val;
  u32 len = blocks == 0 ? 65536 * 512 : blocks * 512;
  u32 ram_buff_addr =
      DMA_BUFF_ADDR / 2048;  //(ROM_LEN - len - 65536 * 4) / 2048;

  void* buf_ptr = buff;
  unsigned long pi_address = (0xb0000000 + ram_buff_addr * 1024 * 2);
  u32 rom_addr = pi_address;
  u32 size = len;
  OSIoMesg dmaIoMesgBuf;
  OSMesg dmaMesgBuf;
  int msgRet;

  // assert (size <= EVERDRIVE_CART_BLOCK_WRITE_SIZE);

  switch (state->state) {
    case 0:
      // write back CPU cache to RAM for consistency during DMA
      osWritebackDCache(buff, len);
      // Create message queue to track DMA-to-cart completion
      osCreateMesgQueue(&state->dmaMesgQ, &dmaMesgBuf, 1);
      // DMA write to cart memory space
      osPiStartDma(&dmaIoMesgBuf, OS_MESG_PRI_NORMAL, OS_WRITE, rom_addr,
                   buf_ptr, size, &state->dmaMesgQ);
      state->state = 1;
    case 1:
      // non-blocking check of DMA-to-cart message queue
      msgRet = osRecvMesg(&state->dmaMesgQ, NULL, OS_MESG_NOBLOCK);
      if (msgRet == -1) {
        // message queue is empty, DMA not finished
        return;
      }
      state->state = 2;
    case 2:
      if (evd_fifoTxe())
        return;

      // wait for cart-to-fifo dma
      val = regs_ptr[0];
      regs_ptr[REG_DMA_LEN] = (blocks - 1);
      val = regs_ptr[0];
      regs_ptr[REG_DMA_RAM_ADDR] = ram_buff_addr;
      val = regs_ptr[0];
      regs_ptr[REG_DMA_CFG] = DCFG_RAM_TO_FIFO;

      state->state = 3;
      return;  // always wait a little for DMA
    case 3:
      // wait for DMA write to host
      if (evd_isDmaBusyNoWait()) {
        return;
      }
      if (evd_isDmaTimeout()) {
        // if DMA timed out, end in failure
        // return EVD_ERROR_FIFO_TIMEOUT;
        state->done = TRUE;
        return;
      }
  }
  state->done = TRUE;
  return;
}

u8 evd_isSpiBusy() {
  volatile u16 val;
  regs_ptr[REG_STATUS];
  val = regs_ptr[REG_STATUS];
  return (val >> ED_STATE_SPI) & 1;
}

u8 evd_SPI(u8 dat) {
  volatile u8 val;
  val = regs_ptr[0];
  regs_ptr[REG_SPI] = dat;
  while (evd_isSpiBusy())
    ;
  // osInvalICache((u32*) & regs_ptr[REG_SPI], 1);
  val = regs_ptr[REG_SPI];
  return val;
}

void evd_spiSSOn() {
  volatile u8 val;
  if (sd_mode)
    return;
  spi_cfg &= ~(1 << SPI_CFG_SS);
  val = regs_ptr[0];
  regs_ptr[REG_SPI_CFG] = spi_cfg;
}

void evd_spiSSOff() {
  volatile u8 val;
  spi_cfg |= (1 << SPI_CFG_SS);
  val = regs_ptr[0];
  regs_ptr[REG_SPI_CFG] = spi_cfg;
}

void evd_enableSDMode() {
  sd_mode = 1;
}

void evd_enableSPIMode() {
  sd_mode = 0;
}

u8 evd_isSDMode() {
  return sd_mode;
}

void evd_SDcmdWriteMode(u8 bit1_mode) {
  volatile u8 val;
  if (!sd_mode)
    return;
  spi_cfg &= ~((1 << SPI_CFG_RD) | (1 << SPI_CFG_DAT));
  if (bit1_mode) {
    spi_cfg |= (1 << SPI_CFG_1BIT);
  } else {
    spi_cfg &= ~(1 << SPI_CFG_1BIT);
  }
  val = regs_ptr[0];
  regs_ptr[REG_SPI_CFG] = spi_cfg;
}

void evd_SDcmdReadMode(u8 bit1_mode) {
  volatile u8 val;
  if (!sd_mode)
    return;
  spi_cfg |= (1 << SPI_CFG_RD);
  spi_cfg &= ~(1 << SPI_CFG_DAT);
  if (bit1_mode) {
    spi_cfg |= (1 << SPI_CFG_1BIT);
  } else {
    spi_cfg &= ~(1 << SPI_CFG_1BIT);
  }
  val = regs_ptr[0];
  regs_ptr[REG_SPI_CFG] = spi_cfg;
}

void evd_SDdatWriteMode(u8 bit4_mode) {
  volatile u8 val;
  if (!sd_mode)
    return;
  spi_cfg &= ~(1 << SPI_CFG_RD);
  spi_cfg |= (1 << SPI_CFG_DAT);
  if (bit4_mode) {
    spi_cfg |= (1 << SPI_CFG_1BIT);
  } else {
    spi_cfg &= ~(1 << SPI_CFG_1BIT);
  }
  val = regs_ptr[0];
  regs_ptr[REG_SPI_CFG] = spi_cfg;
}

void evd_SDdatReadMode(u8 bit4_mode) {
  volatile u8 val;
  if (!sd_mode)
    return;
  spi_cfg |= (1 << SPI_CFG_RD) | (1 << SPI_CFG_DAT);
  if (bit4_mode) {
    spi_cfg |= (1 << SPI_CFG_1BIT);
  } else {
    spi_cfg &= ~(1 << SPI_CFG_1BIT);
  }
  val = regs_ptr[0];
  regs_ptr[REG_SPI_CFG] = spi_cfg;
}

void evd_setSpiSpeed(u8 speed) {
  volatile u8 val;
  spi_cfg &= ~3;  //((1 << SPI_CFG_SPD0) | (1 << SPI_CFG_SPD1));
  spi_cfg |= speed & 3;
  val = regs_ptr[0];
  regs_ptr[REG_SPI_CFG] = spi_cfg;
}

u8 evd_mmcReadToCart(u32 cart_addr, u32 len) {
  volatile u8 val;
  cart_addr /= 2048;

  val = regs_ptr[0];
  regs_ptr[REG_DMA_LEN] = (len - 1);
  val = regs_ptr[0];
  regs_ptr[REG_DMA_RAM_ADDR] = cart_addr;
  val = regs_ptr[0];
  regs_ptr[REG_DMA_CFG] = DCFG_SD_TO_RAM;

  while (evd_isDmaBusy())
    ;
  if (evd_isDmaTimeout())
    return EVD_ERROR_MMC_TIMEOUT;

  return 0;
}

void evd_setCfgBit(u8 option, u8 state) {
  volatile u8 val;

  if (state)
    evd_cfg |= (1 << option);
  else
    evd_cfg &= ~(1 << option);

  val = regs_ptr[0];
  regs_ptr[REG_CFG] = evd_cfg;
  val = regs_ptr[0];
}

u16 evd_readReg(u8 reg) {
  volatile u32 tmp;

  tmp = regs_ptr[0];

  return regs_ptr[reg];
}

void evd_setSaveType(u8 type) {
  // u8 eeprom_on, sram_on, eeprom_size, sram_size;
  // eeprom_on = 0;
  // sram_on = 0;
  // eeprom_size = 0;
  // sram_size = 0;

  // switch (type) {
  //     case SAVE_TYPE_EEP16k:
  //         eeprom_on = 1;
  //         eeprom_size = 1;
  //         break;
  //     case SAVE_TYPE_EEP4k:
  //         eeprom_on = 1;
  //         break;
  //     case SAVE_TYPE_SRAM:
  //         sram_on = 1;
  //         break;
  //     case SAVE_TYPE_SRAM128:
  //         sram_on = 1;
  //         sram_size = 1;
  //         break;
  //     case SAVE_TYPE_FLASH:
  //         sram_on = 0;
  //         sram_size = 1;
  //         break;
  //     default:
  //         sram_on = 0;
  //         sram_size = 0;
  //         break;
  // }

  // volatile u8 val;
  // val = regs_ptr[0];
  // regs_ptr[REG_SAV_CFG] = (eeprom_on << SAV_EEP_ON | sram_on << SAV_SRM_ON |
  // eeprom_size << SAV_EEP_SIZE | sram_size << SAV_SRM_SIZE);
}

void evd_writeReg(u8 reg, u16 val) {
  volatile u8 tmp;
  tmp = regs_ptr[0];
  regs_ptr[reg] = val;
}

void evd_mmcSetDmaSwap(u8 state) {
  evd_setCfgBit(ED_CFG_SWAP, state);
}

void evd_writeMsg(u8 dat) {
  evd_writeReg(REG_MSG, dat);
}

u8 evd_readMsg() {
  return evd_readReg(REG_MSG);
}

u16 evd_getFirmVersion() {
  return evd_readReg(REG_VER);
}

void evd_setDmaCallback(void (*callback)()) {
  dma_busy_callback = callback;
}
