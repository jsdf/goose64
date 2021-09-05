/*
 * File:   tools.h
 * Author: KRIK
 *
 * Created on 16 јпрель 2011 г., 2:30
 */

#ifndef _ED64IO_SYS_H
#define _ED64IO_SYS_H
#include "ed64io_types.h"

void dma_read_s(void* ram_address,
                unsigned long pi_address,
                unsigned long len,
                int usePolling);
void dma_write_s(void* ram_address,
                 unsigned long pi_address,
                 unsigned long len);
void evd_sleep(u32 ms);

void evdPiReadRom(u32 rom_addr, void* buf_ptr, u32 size, int usePolling);

#endif /* _ED64IO_SYS_H */
