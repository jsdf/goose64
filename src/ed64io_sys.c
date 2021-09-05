
#include "ed64io_everdrive.h"

#include "ed64io_sys.h"

#include "ed64io_errors.h"
#include "ed64io_usb.h"

u16 strcon(u8* str1, u8* str2, u8* dst, u16 max_len) {
  u16 len = 0;
  max_len -= 1;

  while (*str1 != 0 && len < max_len) {
    *dst++ = *str1++;
    len++;
  }

  while (*str2 != 0 && len < max_len) {
    *dst++ = *str2++;
    len++;
  }
  *dst++ = 0;
  return len;
}

#define NU_PI_CART_BLOCK_READ_SIZE 0x4000 /* cart read block size */

void evdPiReadRom(u32 rom_addr, void* buf_ptr, u32 size, int usePolling) {
  OSIoMesg dmaIoMesgBuf;
  OSMesgQueue dmaMesgQ;
  OSMesg dmaMesgBuf;
  u32 readSize;

  /* Disable the CPU cache. */
  osInvalDCache((void*)buf_ptr, (s32)size);

  /* Create message queue. */
  osCreateMesgQueue(&dmaMesgQ, &dmaMesgBuf, 1);

  while (size) {
    if (size > NU_PI_CART_BLOCK_READ_SIZE) {
    } else {
      readSize = size;
    }

    /* DMA read */
    osPiStartDma(&dmaIoMesgBuf, OS_MESG_PRI_NORMAL, OS_READ, rom_addr, buf_ptr,
                 readSize, &dmaMesgQ);

    /* Wait for end. */
    if (usePolling) {
      // for when you don't want to yield to other threads
      while (osRecvMesg(&dmaMesgQ, NULL, OS_MESG_NOBLOCK) != 0) {
        evd_sleep(1);
      }
    } else {
      (void)osRecvMesg(&dmaMesgQ, NULL, OS_MESG_BLOCK);
    }

    rom_addr += readSize;
    buf_ptr = (void*)((u8*)buf_ptr + readSize);
    size -= readSize;
  }
}

void evdPiWriteRom(u32 rom_addr, void* buf_ptr, u32 size) {
  OSIoMesg dmaIoMesgBuf;
  OSMesgQueue dmaMesgQ;
  OSMesg dmaMesgBuf;
  u32 writeSize;

  /* Create message queue. */
  osCreateMesgQueue(&dmaMesgQ, &dmaMesgBuf, 1);

  while (size) {
    if (size > NU_PI_CART_BLOCK_READ_SIZE) {
    } else {
      writeSize = size;
    }

    /* DMA write */
    osPiStartDma(&dmaIoMesgBuf, OS_MESG_PRI_NORMAL, OS_WRITE, rom_addr, buf_ptr,
                 writeSize, &dmaMesgQ);

    /* Wait for end. */
    (void)osRecvMesg(&dmaMesgQ, NULL, OS_MESG_BLOCK);

    rom_addr += writeSize;
    buf_ptr = (void*)((u8*)buf_ptr + writeSize);
    size -= writeSize;
  }
}

void dma_read_s(void* ram_address,
                unsigned long pi_address,
                unsigned long len,
                int usePolling) {
  evdPiReadRom(pi_address, ram_address, len, usePolling);
  osInvalDCache(ram_address, len);
  // u32 buff[256];
  // u32* bptr;
  // u32* rptr = (u32*)ram_address;
  // u16 i;

  // // u16 blen = 512;
  // // if (len < 512)blen = len;
  // //*(volatile u32*) 0x1FC007FC = 0x08;

  // // IO_WRITE(PI_STATUS_REG, 3);
  // while (len) {
  //   evdPiReadRom(pi_address, buff, 512, usePolling);
  //   // while ((IO_READ(PI_STATUS_REG) & 3) != 0)
  //   //   ;
  //   // while ((*((volatile u32*) PI_STATUS_REG) & 0x02) != 1);
  //   osInvalDCache(buff, 512);
  //   bptr = buff;
  //   for (i = 0; i < 512 && i < len; i += 4)
  //     *rptr++ = *bptr++;
  //   len = len < 512 ? 0 : len - 512;
  //   pi_address += 512;
  // }
}

void dma_write_s(void* ram_address,
                 unsigned long pi_address,
                 unsigned long len) {
  // data_cache_writeback(ram_address, len);
  osWritebackDCache(ram_address, len);

  // dma_write(ram_address, pi_address, len);
  evdPiWriteRom(pi_address, ram_address, len);
}

// blocking evd_sleep
void evd_sleep(u32 ms) {
  u32 current_ms = OS_CYCLES_TO_USEC(osGetTime()) / 1000.0;

  while ((OS_CYCLES_TO_USEC(osGetTime()) / 1000.0) - current_ms < ms)
    ;
}
