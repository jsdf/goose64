
#ifndef FRUSTUM_H
#define FRUSTUM_H

#include "collision.h"
#include "vec3d.h"

typedef struct Plane {
  Vec3d normal;
  Vec3d point;
  float d;
} Plane;

typedef enum FrustumPlanes {
  NearFrustumPlane,
  FarFrustumPlane,
  TopFrustumPlane,
  BottomFrustumPlane,
  LeftFrustumPlane,
  RightFrustumPlane,
  NUM_FRUSTUM_PLANES
} FrustumPlanes;

typedef enum FrustumTestResult {
  InsideFrustum,
  OutsideFrustum,
  IntersectingFrustum,
  MAX_FRUSTUM_TEST_RESULT
} FrustumTestResult;

extern char* FrustumTestResultStrings[MAX_FRUSTUM_TEST_RESULT];

typedef struct Frustum {
  Plane planes[NUM_FRUSTUM_PLANES];
  Vec3d ntl, ntr, nbl, nbr, ftl, ftr, fbl, fbr;
  float nearD, farD, aspect, fovy, tang;
  float nw, nh, fw, fh;
} Frustum;

float Plane_distance(Plane* self, Vec3d* p);

void Frustum_setCamInternals(Frustum* self,
                             float fovy,
                             float aspect,
                             float nearD,
                             float farD);

void Frustum_setCamDef(Frustum* self, Vec3d* p, Vec3d* l, Vec3d* u);
FrustumTestResult Frustum_boxInFrustum(Frustum* frustum, AABB* aabb);
FrustumTestResult Frustum_boxFrustumPlaneTestRTCD(Frustum* frustum,
                                                  AABB* aabb,
                                                  int planeIdx);
FrustumTestResult Frustum_boxFrustumPlaneTestPN(Frustum* frustum,
                                                AABB* aabb,
                                                int planeIdx);

void Frustum_getAABBVertexP(AABB* self, Vec3d* normal, Vec3d* result);
void Frustum_getAABBVertexN(AABB* self, Vec3d* normal, Vec3d* result);

#endif /* !FRUSTUM_H_ */
