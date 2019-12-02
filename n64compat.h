
#ifndef N64COMPAT_H
#define N64COMPAT_H
#ifdef __N64__
#include <nusys.h>
#endif

#ifndef __N64__

/*
 * 4x4 matrix, fixed point s15.16 format.
 * First 8 words are integer portion of the 4x4 matrix
 * Last 8 words are the fraction portion of the 4x4 matrix
 */
typedef long Mtx_t[4][4];

typedef union {
  Mtx_t m;
  long long int force_structure_alignment;
} Mtx;

#endif

#endif /* !N64COMPAT_H */
