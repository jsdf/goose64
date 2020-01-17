/* 
 * File:   tools.h
 * Author: KRIK
 *
 * Created on 16 јпрель 2011 г., 2:30
 */

#ifndef _TOOLS_H
#define	_TOOLS_H
#include "ed64io_types.h"
// #include <libdragon.h>

#define PI_BSD_DOM1_LAT_REG	(PI_BASE_REG+0x14)

/* PI dom1 pulse width (R/W): [7:0] domain 1 device R/W strobe pulse width */
#define PI_BSD_DOM1_PWD_REG	(PI_BASE_REG+0x18)

/* PI dom1 page size (R/W): [3:0] domain 1 device page size */
#define PI_BSD_DOM1_PGS_REG	(PI_BASE_REG+0x1C)    /*   page size */

/* PI dom1 release (R/W): [1:0] domain 1 device R/W release duration */
#define PI_BSD_DOM1_RLS_REG	(PI_BASE_REG+0x20)
/* PI dom2 latency (R/W): [7:0] domain 2 device latency */
#define PI_BSD_DOM2_LAT_REG	(PI_BASE_REG+0x24)    /* Domain 2 latency */

/* PI dom2 pulse width (R/W): [7:0] domain 2 device R/W strobe pulse width */
#define PI_BSD_DOM2_PWD_REG	(PI_BASE_REG+0x28)    /*   pulse width */

/* PI dom2 page size (R/W): [3:0] domain 2 device page size */
#define PI_BSD_DOM2_PGS_REG	(PI_BASE_REG+0x2C)    /*   page size */

/* PI dom2 release (R/W): [1:0] domain 2 device R/W release duration */
#define PI_BSD_DOM2_RLS_REG	(PI_BASE_REG+0x30)    /*   release duration */

#define	PHYS_TO_K1(x)	((u32)(x)|0xA0000000)	/* physical to kseg1 */
#define	IO_WRITE(addr,data)	(*(volatile u32*)PHYS_TO_K1(addr)=(u32)(data))
#define	IO_READ(addr)		(*(vu32 *)PHYS_TO_K1(addr))

#define PI_STATUS_REG		(PI_BASE_REG+0x10)
#define PI_BASE_REG		0x04600000


void dma_read_s(void * ram_address, unsigned long pi_address, unsigned long len);
void dma_write_s(void * ram_address, unsigned long pi_address, unsigned long len);
void dma_write_s(void * ram_address, unsigned long pi_address, unsigned long len);
 void sleep(u32 ms);
void dma_write_sram(void* src, u32 offset, u32 size);
void dma_read_sram(void *dest, u32 offset, u32 size);
// u8 getSaveType();
// u8 getCicType(u8 bios_cic);
// void printCartInfo(); 
// u32 getColor(u32 fcolor);
//  u16 strcon(u8 *str1, u8 *str2, u8 *dst, u16 max_len);
// u8 slen(u8 *str);
// u8 scopy(u8 *src, u8 *dst);
// u8 STR_intToDecString(u32 val, u8 *str); 
// u16 strContain(u8 *target, u8 *str);
 
typedef struct SP_regs_s {
    u32 mem_addr;
    u32 dram_addr;
    u32 rd_len;
    u32 wr_len;
    u32 status;
} _SP_regs_s;

#define SP_PC *((volatile u32 *)0xA4080000)
#define SP_IBIST_REG *((volatile u32 *)0xA4080004)
 
extern u32 native_tv_mode;

typedef struct {
    u16 sd_speed;
    u16 font_size;
    u16 tv_mode;
    u8 wall[256];
} Options_st;


extern Options_st options;
extern u32 asm_date;
#endif	/* _TOOLS_H */

