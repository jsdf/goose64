// #include <dma.h>
// #include <n64sys.h>
#include <nusys.h>

#include "constants.h"

#include "ed64io_sys.h"

#include "ed64io_errors.h"
#include "ed64io_everdrive.h"
#include "ed64io_types.h"
#include "ed64io_usb.h"
// #include "gui.h"
// #include "graphics.h"

u32 asm_date;

Options_st options;

u32 native_tv_mode;

#define CIC_6101 1
#define CIC_6102 2
#define CIC_6103 3
#define CIC_6104 4
#define CIC_6105 5
#define CIC_6106 6

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

void evdPiReadRom(u32 rom_addr, void* buf_ptr, u32 size) {
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
    (void)osRecvMesg(&dmaMesgQ, NULL, OS_MESG_BLOCK);

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
                unsigned long len) {
  u32 buff[256];
  u32* bptr;
  u32* rptr = (u32*)ram_address;
  u16 i;

  // u16 blen = 512;
  // if (len < 512)blen = len;
  //*(volatile u32*) 0x1FC007FC = 0x08;

  IO_WRITE(PI_STATUS_REG, 3);
  while (len) {
    evdPiReadRom(pi_address, buff, 512);
    while ((IO_READ(PI_STATUS_REG) & 3) != 0)
      ;
    // while ((*((volatile u32*) PI_STATUS_REG) & 0x02) != 1);
    osInvalDCache(buff, 512);
    bptr = buff;
    for (i = 0; i < 512 && i < len; i += 4)
      *rptr++ = *bptr++;
    len = len < 512 ? 0 : len - 512;
    pi_address += 512;
  }
}

void dma_write_s(void* ram_address,
                 unsigned long pi_address,
                 unsigned long len) {
  // data_cache_writeback(ram_address, len);
  osWritebackDCache(ram_address, len);

  // dma_write(ram_address, pi_address, len);
  evdPiWriteRom(pi_address, ram_address, len);
}
void sleep(u32 ms) {
  u32 current_ms = CUR_TIME_MS();

  while (CUR_TIME_MS() - current_ms < ms)
    ;
}

void dma_read_sram(void* dest, u32 offset, u32 size) {
  /*
      PI_DMAWait();

      IO_WRITE(PI_STATUS_REG, 0x03);
      IO_WRITE(PI_DRAM_ADDR_REG, K1_TO_PHYS(dest));
      IO_WRITE(PI_CART_ADDR_REG, (0xA8000000 + offset));
      data_cache_invalidate_all();
      IO_WRITE(PI_WR_LEN_REG, (size - 1));
   * 0xA8000000
   * 0xb0000000
   *  0x4000000
   * */
  dma_read_s(dest, 0xA8000000 + offset, size);
  // data_cache_invalidate(dest,size);
}

void dma_write_sram(void* src, u32 offset, u32 size) {
  /*
      PI_DMAWait();

      IO_WRITE(PI_STATUS_REG, 0x02);
      IO_WRITE(PI_DRAM_ADDR_REG, K1_TO_PHYS(src));
      IO_WRITE(PI_CART_ADDR_REG, (0xA8000000 + offset));
      data_cache_invalidate_all();
      IO_WRITE(PI_RD_LEN_REG, (size - 1));*/

  dma_write_s(src, 0xA8000000 + offset, size);
}

// #define DO1(buf) crc = crc_table[((int)crc ^ (*buf++)) & 0xff] ^ (crc >> 8);
// #define DO2(buf)  DO1(buf); DO1(buf);
// #define DO4(buf)  DO2(buf); DO2(buf);
// #define DO8(buf)  DO4(buf); DO4(buf);

// unsigned int CRC_Calculate(unsigned int crc, unsigned char* buf, unsigned int
// len) {
//     static unsigned int crc_table[256];
//     static int make_crc_table = 1;

//     if (make_crc_table) {
//         unsigned int c, n;
//         int k;
//         unsigned int poly;
//         const unsigned char p[] = {0, 1, 2, 4, 5, 7, 8, 10, 11, 12, 16, 22,
//         23, 26};

//         /* make exclusive-or pattern from polynomial (0xedb88320L) */
//         poly = 0L;
//         for (n = 0; n < sizeof (p) / sizeof (unsigned char); n++)
//             poly |= 1L << (31 - p[n]);

//         for (n = 0; n < 256; n++) {
//             c = n;
//             for (k = 0; k < 8; k++)
//                 c = c & 1 ? poly ^ (c >> 1) : c >> 1;
//             crc_table[n] = c;
//         }
//         make_crc_table = 0;
//     }

//     if (buf == (void*) 0) return 0L;

//     crc = crc ^ 0xffffffffL;
//     while (len >= 8) {
//         DO8(buf);
//         len -= 8;
//     }
//     if (len)
//         do {
//             DO1(buf);
//         } while (--len);

//     return crc ^ 0xffffffffL;
// }

// int get_cic(unsigned char *buffer) {
//     unsigned int crc;
//     // figure out the CIC
//     crc = CRC_Calculate(0, buffer, 1000);
//     switch (crc) {
//         case 0x303faac9:
//         case 0xf0da3d50:
//             return 1;
//         case 0xf3106101:
//             return 2;
//         case 0xe7cd9d51:
//             return 3;
//         case 0x7ae65c9:
//             return 5;
//         case 0x86015f8f:
//             return 6;
//     }
//     return 2;
// }

// u8 getCicType(u8 bios_cic) {

//     u8 cic_buff[2048];
//     volatile u8 cic_chip;
//     volatile u32 val; // = *(u32 *) 0xB0000170;
//     if (bios_cic) {
//         evd_setCfgBit(ED_CFG_SDRAM_ON, 0);
//         sleep(10);
//         val = *(u32 *) 0xB0000170;
//         dma_read_s(cic_buff, 0xB0000040, 1024);
//         cic_chip = get_cic(cic_buff);
//         evd_setCfgBit(ED_CFG_SDRAM_ON, 1);
//         sleep(10);

//     } else {
//         val = *(u32 *) 0xB0000170;
//         dma_read_s(cic_buff, 0xB0000040, 1024);
//         cic_chip = get_cic(cic_buff);

//     }

//     /*
//         if (val == 0x0D0001EC)cic_chip = CIC_6106;
//         else
//             if (val == 0x0D0001DB)cic_chip = CIC_6103;
//         else
//             if (val == 0xAD400014)cic_chip = CIC_6105;
//         else
//             cic_chip = CIC_6102;*/
//     /*
//         if (bios_cic) {
//             console_printf("sys cic: %d\n", cic_chip);
//         } else {
//             console_printf("gam cic: %d\n", cic_chip);
//         }*/

//     //joyWait();

//     return cic_chip;
// }

// u32 ii;
// volatile u32 *pt;
// void clean();

// #define MEM32(addr) *((volatile u32 *)addr)

// u32 getColor(u32 fcolor) {

//     u8 r = fcolor >> 16;
//     u8 g = fcolor >> 8;
//     u8 b = fcolor >> 0;
//     r >>= 3;
//     g >>= 3;
//     b >>= 3;
//     fcolor = r << 11 | g << 6 | b << 1 | 1;

//     return fcolor;
// }

// u8 str_buff[128];

// u8 STR_intToDecString(u32 val, u8 *str) {

//     int len;

//     if (val < 10)len = 1;
//     else
//         if (val < 100)len = 2;
//     else
//         if (val < 1000)len = 3;
//     else
//         if (val < 10000)len = 4;
//     else
//         if (val < 100000)len = 5;
//     else
//         if (val < 1000000)len = 6;
//     else
//         if (val < 10000000)len = 7;
//     else
//         if (val < 100000000)len = 8;
//     else
//         if (val < 1000000000)len = 9;
//     else len = 10;

//     str += len;
//     str[0] = 0;
//     if (val == 0)*--str = '0';
//     while (val) {

//         *--str = '0' + val % 10;
//         val /= 10;
//     }

//     return len;
// }

// void STR_intToDecStringMin(u32 val, u8 *str, u8 min_size) {

//     int len;
//     u8 i;

//     if (val < 10)len = 1;
//     else
//         if (val < 100)len = 2;
//     else
//         if (val < 1000)len = 3;
//     else
//         if (val < 10000)len = 4;
//     else
//         if (val < 100000)len = 5;
//     else
//         if (val < 1000000)len = 6;
//     else
//         if (val < 10000000)len = 7;
//     else
//         if (val < 100000000)len = 8;
//     else
//         if (val < 1000000000)len = 9;
//     else len = 10;

//     if (len < min_size) {

//         i = min_size - len;
//         while (i--)str[i] = '0';
//         len = min_size;
//     }
//     str += len;
//     str[0] = 0;
//     if (val == 0)*--str = '0';
//     while (val) {

//         *--str = '0' + val % 10;
//         val /= 10;
//     }
// }

// u8 streq(u8 *str1, u8 *str2) {

//     u8 s1;
//     u8 s2;

//     for (;;) {
//         s1 = *str1++;
//         s2 = *str2++;
//         if (s1 >= 'a' && s1 <= 'z')s1 -= 0x20;
//         if (s2 >= 'a' && s2 <= 'z')s2 -= 0x20;

//         if (s1 != s2) return 0;

//         if (*str1 == 0 && *str2 == 0)return 1;
//     }
// }

// u8 streql(u8 *str1, u8 *str2, u8 len) {

//     u8 s1;
//     u8 s2;
//     while (len--) {

//         s1 = *str1++;
//         s2 = *str2++;
//         if (s1 >= 'a' && s1 <= 'z')s1 -= 0x20;
//         if (s2 >= 'a' && s2 <= 'z')s2 -= 0x20;

//         if (s1 != s2) return 0;
//     }

//     return 1;
// }

// u16 strContain(u8 *target, u8 *str) {

//     u16 targ_len = slen(target);
//     u16 eq_len;

//     for (eq_len = 0; eq_len < targ_len;) {

//         if (*str == 0)return 0;
//         if (*str++ == target[eq_len]) {
//             eq_len++;
//         } else {
//             eq_len = 0;
//         }
//     }

//     if (eq_len != targ_len)return 0;
//     return 1;

// }

// u8 slen(u8 *str) {

//     u8 len = 0;
//     while (*str++)len++;
//     return len;
// }

// u8 scopy(u8 *src, u8 *dst) {

//     u8 len = 0;
//     while (*src != 0) {
//         *dst++ = *src++;
//         len++;
//     }
//     *dst = 0;
//     return len;
// }

// void strhicase(u8 *str, u8 len) {

//     if (len) {
//         while (len--) {
//             if (*str >= 'a' && *str <= 'z')*str -= 0x20;
//             str++;
//         }
//     } else {
//         while (*str != 0) {
//             if (*str >= 'a' && *str <= 'z')*str -= 0x20;
//             str++;
//         }
//     }

// }