
#ifndef CONSTANTS_H
#define CONSTANTS_H

// needs to be at least num characters * num bones per character
#define MAX_ANIM_MESH_PARTS 50
#define MAX_WORLD_OBJECTS 100
// we scale the models up by this much to avoid n64 fixed point precision issues
#define N64_SCALE_FACTOR 30

#define CONST_PI 3.14159265358979323846
#define degToRad(angleInDegrees) ((angleInDegrees)*CONST_PI / 180.0)
#define radToDeg(angleInRadians) ((angleInRadians)*180.0 / CONST_PI)

#ifdef __N64__

#define RAND(x) (guRandom() % x) /* random number between 0 to x */
#else
#include <stdio.h>
#define debugPrintf(...) printf(__VA_ARGS__)

#define RAND(x) (rand() % x) /* random number between 0 to x */
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
#ifndef FLT_EPSILON
#define FLT_EPSILON 0.0000001192
#endif
#endif

#endif /* CONSTANTS_H */
