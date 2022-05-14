
#ifndef N64COMPAT_H
#define N64COMPAT_H
#ifdef __N64__
#include <nusys.h>
#endif

#ifndef __N64__


// N64 matrix types

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


// N64 vertex types

/*
 * Vertex (set up for use with colors)
 */
typedef struct {
  short   ob[3];  /* x, y, z */
  unsigned short  flag;
  short   tc[2];  /* texture coord */
  unsigned char cn[4];  /* color & alpha */
} Vtx_t;

/*
 * Vertex (set up for use with normals)
 */
typedef struct {
  short   ob[3];  /* x, y, z */
  unsigned short  flag;
  short   tc[2];  /* texture coord */
  signed char n[3]; /* normal */
  unsigned char   a;      /* alpha  */
} Vtx_tn;

typedef union {
    Vtx_t   v;  /* Use this one for colors  */
    Vtx_tn              n;  /* Use this one for normals */
    long long int force_structure_alignment;
} Vtx;

#endif

#endif /* !N64COMPAT_H */
