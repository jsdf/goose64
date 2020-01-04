
#ifndef _COLLISION_H_
#define _COLLISION_H_

#include "vec3d.h"

typedef struct Triangle {
  Vec3d a;
  Vec3d b;
  Vec3d c;
} Triangle;

typedef struct AABB {
  Vec3d min;
  Vec3d max;
} AABB;

typedef struct SphereTriangleCollision {
  int index;
  float distance;
  Vec3d posInTriangle;
  Triangle* triangle;
  AABB triangleAABB;
} SphereTriangleCollision;

typedef struct SpatialHashBucket {
  int size;
  int* data;
} SpatialHashBucket;

typedef struct SpatialHash {
  int numBuckets;
  float gridCellSize;
  int cellsInDimension;
  int cellOffsetInDimension;
  SpatialHashBucket** data;
} SpatialHash;

#ifndef __N64__
#ifdef __cplusplus

#include <map>
extern int testCollisionResult;
extern int testCollisionTrace;

extern std::map<int, SphereTriangleCollision> testCollisionResults;
#endif
#endif

void Triangle_getCentroid(Triangle* triangle, Vec3d* result);
void Triangle_getNormal(Triangle* triangle, Vec3d* result);
float Triangle_comparePoint(Triangle* triangle, Vec3d* point);

void AABB_fromTriangle(Triangle* triangle, AABB* result);

int Collision_sphereTriangleIsSeparated(Triangle* triangle,
                                        Vec3d* sphereCenter,
                                        double sphereRadius);

void Collision_distancePointTriangleExact(Vec3d* point,
                                          Triangle* triangle,
                                          Vec3d* closest);

int Collision_testMeshSphereCollision(Triangle* triangles,
                                      int trianglesLength,
                                      Vec3d* objCenter,
                                      float objRadius,
                                      SpatialHash* spatialHash,
                                      SphereTriangleCollision* result);

int SpatialHash_unitsToGridForDimension(float unitsPos,
                                        float gridCellSz,
                                        int cellOffsetInDimension);
SpatialHashBucket* SpatialHash_getBucket(float x,
                                         float y,
                                         SpatialHash* spatialHash);

int SpatialHash_getTriangles(Vec3d* position,
                             float radius,
                             SpatialHash* spatialHash,
                             int* results,
                             int maxResults);
#endif /* !_COLLISION_H_ */
