
#ifndef CONSTANTS_H
#define CONSTANTS_H

#define MAX_WORLD_OBJECTS 100
// we scale the models up by this much to avoid n64 fixed point precision issues
#define N64_SCALE_FACTOR 30
#define DEFAULT_FOVY 45.0f
#define DEFAULT_NEARPLANE 100
#define DEFAULT_FARPLANE 3000
#define WATER_HEIGHT -70.0

#define CONST_PI 3.14159265358979323846
#define degToRad(angleInDegrees) ((angleInDegrees)*CONST_PI / 180.0)
#define radToDeg(angleInRadians) ((angleInRadians)*180.0 / CONST_PI)

// TODO: switch based on region
#define VSYNC_FPS 60

// dumb
#ifdef __N64__
#ifdef __cplusplus  // only for clang linter
#include "ed64io_usb.h"
#endif
#else
#include <stdio.h>
#endif

#ifdef __N64__
#include <PR/gu.h>
#ifndef RAND
#define RAND(x) (guRandom() % x) /* random number between 0 to x */
#endif
#else
#define RAND(x) (rand() % x) /* random number between 0 to x */
#endif

#ifdef __N64__
// this include needs to be here or this macro will break sometimes
#include <PR/os.h>
#include "n64compat.h"
#define CUR_TIME_MS() OS_CYCLES_TO_USEC(osGetTime()) / 1000.0
#else
#include "compat.h"
#define CUR_TIME_MS() getElapsedTimeMS()
#endif

#define MEM_HEAP_BYTES 524288

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif
#ifndef NULL
#define NULL 0
#endif

#ifndef MIN
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#endif
#ifndef MAX
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#endif
#ifndef CLAMP
#define CLAMP(x, low, high) \
  (((x) > (high)) ? (high) : (((x) < (low)) ? (low) : (x)))
#endif
#define INTFLOOR(x) (int)x
#define INTCEIL(x) ((int)x) + 1

#ifdef __N64__
// hard coded arch-dependent values for n64
#include "mathdef.h"

#ifndef FLT_EPSILON
#define FLT_EPSILON 0.0000001192
#endif

#ifndef FLT_MAX
#define FLT_MAX 3.402823e+38
#endif

#endif

#ifdef __N64__
#define debugPrintf ed64Printf
#else
#define debugPrintf printf
#endif

#ifdef __N64__
#define debugPrintfSync ed64PrintfSync
#else
#define debugPrintfSync printf
#endif

#ifdef __N64__
#define die(msg)                                                            \
  debugPrintfSync("died: \"%s\" in %s at %s:%d\n", msg, __FILE__, __LINE__, \
                  __FUNCTION__);                                            \
  while (TRUE) {                                                            \
  }

#else

#define die(msg)                                                   \
  printf("died: \"%s\" in %s at %s:%d\n", msg, __FILE__, __LINE__, \
         __FUNCTION__);                                            \
  assert(FALSE);
#endif

#ifdef __N64__
#define invariant(expression)                                                \
  if (!(expression)) {                                                       \
    debugPrintfSync("assertion failed in %s at %s:%d\n", __FILE__, __LINE__, \
                    __FUNCTION__);                                           \
    while (TRUE) {                                                           \
    }                                                                        \
  }
#else
#define invariant(expression) assert(expression)
#endif

#endif /* CONSTANTS_H */
