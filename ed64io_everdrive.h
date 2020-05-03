/*
 * File:   everdrive.h
 * Author: KRIK
 *
 * Created on 22 јпрель 2011 г., 20:46
 */

#ifndef _ED64IO_EVERDRIVE_H
#define _ED64IO_EVERDRIVE_H

#include <ultra64.h>

#include "ed64io_types.h"

#define OS_VER "1.29"

#define ROM_LEN 0x4000000
#define ROM_ADDR 0xb0000000
#define ROM_END_ADDR (0xb0000000 + 0x4000000)

#define SAVE_TYPE_OFF 0
#define SAVE_TYPE_SRAM 1
#define SAVE_TYPE_SRAM128 2
#define SAVE_TYPE_EEP4k 3
#define SAVE_TYPE_EEP16k 4
#define SAVE_TYPE_FLASH 5

#define DMA_BUFF_ADDR (ROM_LEN - 0x100000)

#define REG_CFG 0
#define REG_STATUS 1
#define REG_DMA_LEN 2
#define REG_DMA_RAM_ADDR 3
#define REG_MSG 4
#define REG_DMA_CFG 5
#define REG_SPI 6
#define REG_SPI_CFG 7
#define REG_KEY 8
#define REG_SAV_CFG 9
#define REG_SEC 10
#define REG_VER 11

#define REG_CFG_CNT 16
#define REG_CFG_DAT 17
#define REG_MAX_MSG 18
#define REG_CRC 19

#define DCFG_SD_TO_RAM 1
#define DCFG_RAM_TO_SD 2
#define DCFG_FIFO_TO_RAM 3
#define DCFG_RAM_TO_FIFO 4

#define ED_CFG_SDRAM_ON 0
#define ED_CFG_SWAP 1
#define ED_CFG_WR_MOD 2
#define ED_CFG_WR_ADDR_MASK 3

void evd_setCfgBit(u8 option, u8 state);

void evd_init();
u8 evd_fifoRxf();
u8 evd_fifoTxe();
u8 evd_isDmaBusy();
u8 evd_isDmaTimeout();
u8 evd_fifoRd(void* buff, u16 blocks);
u8 evd_fifoWr(void* buff, u16 blocks);

typedef struct evd_fifoWrNonblockState {
  int state;
  int done;
  int id;
  int error;

  OSIoMesg dmaIoMesgBuf;
  OSMesg dmaMesgBuf;
  OSMesgQueue dmaMesgQ;
} evd_fifoWrNonblockState;
void evd_fifoWrNonblockStateInit(evd_fifoWrNonblockState* state);
void evd_fifoWrNonblock(void* buff, u16 blocks, evd_fifoWrNonblockState* state);
u8 evd_fifoRdToCart(u32 cart_addr, u16 blocks);
u8 evd_fifoWrFromCart(u32 cart_addr, u16 blocks);

u8 evd_SPI(u8 dat);
void evd_mmcSetDmaSwap(u8 state);
u8 evd_mmcReadToCart(u32 cart_addr, u32 len);

void evd_lockRegs();
u16 evd_readReg(u8 reg);
void evd_writeReg(u8 reg, u16 val);
void evd_setSaveType(u8 type);

u8 romLoadSettingsFromSD();
u8 romSaveInfoToLog();
void evd_writeMsg(u8 dat);
u8 evd_readMsg();
u16 evd_getFirmVersion();

void evd_spiSSOn();
void evd_spiSSOff();
u8 evd_isSpiBusy();
void evd_setSpiSpeed(u8 speed);

void evd_SDcmdWriteMode(u8 bit1_mode);
void evd_SDcmdReadMode(u8 bit1_mode);
void evd_SDdatWriteMode(u8 bit4_mode);
void evd_SDdatReadMode(u8 bit4_mode);
void evd_enableSDMode();
void evd_enableSPIMode();
u8 evd_isSDMode();
void evd_setDmaCallback(void (*callback)());

/*
u8 evd_mmcWriteNextBlock(void *dat);
u8 evd_mmcOpenWrite(u32 addr);
u8 evd_mmcWriteBlock(void *dat, u32 addr);
u8 evd_mmcInit();
u8 evd_mmcReadBlock(void *dat, u32 addr);
u8 evd_mmcOpenRead(u32 addr);
u8 evd_mmcReadNextBlock(void *dat);
void evd_mmcCloseRW();
 */
#endif /* _ED64IO_EVERDRIVE_H */
