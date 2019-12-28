
#ifndef CONSTANTS_H
#define CONSTANTS_H

#define MAX_WORLD_OBJECTS 100
// we scale the models up by this much to avoid n64 fixed point precision issues
#define N64_SCALE_FACTOR 30

#define CONST_PI 3.14159265358979323846
#define degToRad(angleInDegrees) ((angleInDegrees)*CONST_PI / 180.0)
#define radToDeg(angleInRadians) ((angleInRadians)*180.0 / CONST_PI)

#ifndef __N64__
// dumb
#include <stdio.h>
#endif

#ifdef __N64__
#define RAND(x) (guRandom() % x) /* random number between 0 to x */
#else
#define RAND(x) (rand() % x) /* random number between 0 to x */
#endif

#define MEM_HEAP_BYTES 524288

#define USE_PHYSICS_MOVEMENT 0

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

#ifndef FLT_EPSILON
#define FLT_EPSILON 0.0000001192
#endif

#ifndef FLT_MAX
#define FLT_MAX 3.402823e+38
#endif
#endif

#endif /* CONSTANTS_H */
