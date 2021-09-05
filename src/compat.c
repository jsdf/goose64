
#include <sys/time.h>

#include "compat.h"

#ifndef __N64__
#include <stdio.h>
static double startTime = 0;

double getElapsedTimeMS(void) {
  struct timeval tv;

  gettimeofday(&tv, NULL);
  double curTime = (((long long)tv.tv_sec) * 1000) + (tv.tv_usec / 1000);

  if (startTime == 0) {
    startTime = curTime;
  }
  return curTime - startTime;
}
#endif