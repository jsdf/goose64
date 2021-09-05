
#ifndef _ED64IO_WATCHDOG_H
#define _ED64IO_WATCHDOG_H

#include <ultra64.h>

void ed64StartWatchdogThread(int* valToWatch, int intervalMS);

#endif /* _ED64IO_WATCHDOG_H */
