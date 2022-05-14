
#ifndef MESH_H
#define MESH_H
#include "n64compat.h"


typedef struct MeshTri {
  Vtx * vertTable;
  int tri[3]; // indexes in vertTable
  // float
} MeshTri;


// TODO: 
typedef struct MeshInfo {
  MeshTri* tris;
  int triCount;
  unsigned short* texture;
  unsigned short textureWidth;
  unsigned short textureHeight;
} MeshInfo;
#endif /* !MESH_H */
