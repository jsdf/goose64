#include <assert.h>
#include <math.h>
#include <stdlib.h>

#include "constants.h"

#include "collision.h"
#include "trace.h"
#include "vec3d.h"

#ifndef __N64__
#include "float.h"
// otherwise this stuff is in constants.h
#endif

void Triangle_getCentroid(Triangle* triangle, Vec3d* result) {
  *result = triangle->a;
  Vec3d_add(result, &triangle->b);
  Vec3d_add(result, &triangle->c);
  Vec3d_divScalar(result, 3.0);
}

void Triangle_getNormal(Triangle* triangle, Vec3d* result) {
  Vec3d edgeAB, edgeAC;
  edgeAB = triangle->b;
  Vec3d_sub(&edgeAB, &triangle->a);
  edgeAC = triangle->c;
  Vec3d_sub(&edgeAC, &triangle->a);

  Vec3d_cross(&edgeAB, &edgeAC, result);
  Vec3d_normalise(result);
}

// if result > 0: point is in front of triangle
// if result = 0: point is coplanar with triangle
// if result < 0: point is behind triangle
float Triangle_comparePoint(Triangle* triangle, Vec3d* point) {
  Vec3d normal, toPoint;

  // normal . (point - triangleVert)
  Triangle_getNormal(triangle, &normal);
  toPoint = *point;
  Vec3d_sub(&toPoint, &triangle->a);
  return Vec3d_dot(&normal, &toPoint);
}

void AABB_fromSphere(Vec3d* sphereCenter, float sphereRadius, AABB* result) {
  result->min = *sphereCenter;
  result->min.x -= sphereRadius;
  result->min.y -= sphereRadius;
  result->min.z -= sphereRadius;

  result->max = *sphereCenter;
  result->max.x += sphereRadius;
  result->max.y += sphereRadius;
  result->max.z += sphereRadius;
}

void AABB_fromTriangle(Triangle* triangle, AABB* result) {
  result->min = triangle->a;
  result->min.x = MIN(result->min.x, triangle->b.x);
  result->min.x = MIN(result->min.x, triangle->c.x);
  result->min.y = MIN(result->min.y, triangle->b.y);
  result->min.y = MIN(result->min.y, triangle->c.y);
  result->min.z = MIN(result->min.z, triangle->b.z);
  result->min.z = MIN(result->min.z, triangle->c.z);

  result->max = triangle->a;
  result->max.x = MAX(result->max.x, triangle->b.x);
  result->max.x = MAX(result->max.x, triangle->c.x);
  result->max.y = MAX(result->max.y, triangle->b.y);
  result->max.y = MAX(result->max.y, triangle->c.y);
  result->max.z = MAX(result->max.z, triangle->b.z);
  result->max.z = MAX(result->max.z, triangle->c.z);
}

void AABB_expandByPoint(AABB* self, Vec3d* point) {
  self->min.x = MIN(self->min.x, point->x);
  self->min.y = MIN(self->min.y, point->y);
  self->min.z = MIN(self->min.z, point->z);

  self->max.x = MAX(self->max.x, point->x);
  self->max.y = MAX(self->max.y, point->y);
  self->max.z = MAX(self->max.z, point->z);
}

int Collision_intersectAABBAABB(AABB* a, AABB* b) {
  // Exit with no intersection if separated along an axis
  if (a->max.x < b->min.x || a->min.x > b->max.x)
    return FALSE;
  if (a->max.y < b->min.y || a->min.y > b->max.y)
    return FALSE;
  if (a->max.z < b->min.z || a->min.z > b->max.z)
    return FALSE;  // Overlapping on all axes means AABBs are intersecting
  return TRUE;
}

// not tested
int Collision_intersectRayTriangle(Vec3d* pt,
                                   Vec3d* dir,
                                   Triangle* tri,
                                   Vec3d* out) {
  Vec3d edge1, edge2, tvec, pvec, qvec;
  float det, u, v, t;

  edge1 = tri->b;

  Vec3d_sub(&edge1, &tri->a);
  edge2 = tri->c;
  Vec3d_sub(&edge2, &tri->a);

  Vec3d_cross(dir, &edge2, &pvec);
  det = Vec3d_dot(&edge1, &pvec);

  if (det < FLT_EPSILON)
    return FALSE;

  tvec = *pt;
  Vec3d_sub(&tvec, &tri->a);
  u = Vec3d_dot(&tvec, &pvec);
  if (u < 0 || u > det)
    return FALSE;
  Vec3d_cross(&tvec, &edge1, &qvec);
  v = Vec3d_dot(dir, &qvec);
  if (v < 0 || u + v > det)
    return FALSE;

  t = Vec3d_dot(&edge2, &qvec) / det;
  out->x = pt->x + t * dir->x;
  out->y = pt->y + t * dir->y;
  out->z = pt->z + t * dir->z;

  return TRUE;
}

// http://realtimecollisiondetection.net/blog/?p=103
int Collision_sphereTriangleIsSeparated(Triangle* triangle,
                                        Vec3d* P,
                                        double r) {
  double rr, d, e, aa, ab, ac, bb, bc, cc, d1, e1, d2, e2, d3, e3;
  int sep1, sep2, sep3, sep4, sep5, sep6, sep7;
  Vec3d A, B, C;
  Vec3d V, BSubA, CSubA;
  Vec3d AB, BC, CA;
  Vec3d Q1, ABd1;
  Vec3d QC;
  Vec3d Q2, BCd2;
  Vec3d QA;
  Vec3d Q3, CAd3;
  Vec3d QB;
  // Translate problem so sphere is centered at origin
  // A = A - P
  A = triangle->a;
  Vec3d_sub(&A, P);
  // B = B - P
  B = triangle->b;
  Vec3d_sub(&B, P);
  // C = C - P
  C = triangle->c;
  Vec3d_sub(&C, P);

  rr = r * r;

  // Testing if sphere lies outside the triangle plane
  {
    // Compute a vector normal to triangle plane (V), normalize it
    // V = (B - A).cross(C - A)
    BSubA = B;
    Vec3d_sub(&BSubA, &A);
    CSubA = C;
    Vec3d_sub(&CSubA, &A);
    Vec3d_cross(&BSubA, &CSubA, &V);
    // Compute distance d of sphere center to triangle plane
    d = Vec3d_dot(&A, &V);
    e = Vec3d_dot(&V, &V);
    // d > r
    sep1 = d * d > rr * e;

    if (sep1)
      return TRUE;
  }
  // Testing if sphere lies outside a triangle vertex
  {
    // for triangle vertex A

    // Compute distance between sphere center and vertex A
    aa = Vec3d_dot(&A, &A);
    ab = Vec3d_dot(&A, &B);
    ac = Vec3d_dot(&A, &C);

    sep2 =
        // The plane through A with normal A ("A - P") separates sphere if:
        // (1) A lies outside the sphere, and
        (aa > rr) &
        // (2) if B and C lie on the opposite side of the plane w.r.t. the
        // sphere center
        (ab > aa) & (ac > aa);

    if (sep2)
      return TRUE;
  }
  {
    // for triangle vertex B
    bb = Vec3d_dot(&B, &B);
    bc = Vec3d_dot(&B, &C);
    sep3 = (bb > rr) & (ab > bb) & (bc > bb);

    if (sep3)
      return TRUE;
  }
  {
    // for triangle vertex C
    cc = Vec3d_dot(&C, &C);
    sep4 = (cc > rr) & (ac > cc) & (bc > cc);

    if (sep4)
      return TRUE;
  }

  // Testing if sphere lies outside a triangle edge
  {
    // AB = B - A
    AB = B;
    Vec3d_sub(&AB, &A);

    d1 = ab - aa;
    e1 = Vec3d_dot(&AB, &AB);

    // Q1 = A * e1 - AB * d1
    Q1 = A;
    Vec3d_mulScalar(&Q1, e1);
    ABd1 = AB;
    Vec3d_mulScalar(&ABd1, d1);
    Vec3d_sub(&Q1, &ABd1);

    // QC = C * e1 - Q1
    QC = C;
    Vec3d_mulScalar(&QC, e1);
    Vec3d_sub(&QC, &Q1);

    sep5 = (Vec3d_dot(&Q1, &Q1) > rr * e1 * e1) & (Vec3d_dot(&Q1, &QC) > 0);

    if (sep5)
      return TRUE;
  }
  {
    // BC = C - B
    BC = C;
    Vec3d_sub(&BC, &B);

    d2 = bc - bb;
    e2 = Vec3d_dot(&BC, &BC);

    // Q2 = B * e2 - BC * d2
    Q2 = B;
    Vec3d_mulScalar(&Q2, e2);
    BCd2 = BC;
    Vec3d_mulScalar(&BCd2, d2);
    Vec3d_sub(&Q2, &BCd2);

    // QA = A * e2 - Q2
    QA = A;
    Vec3d_mulScalar(&QA, e2);
    Vec3d_sub(&QA, &Q2);

    sep6 = (Vec3d_dot(&Q2, &Q2) > rr * e2 * e2) & (Vec3d_dot(&Q2, &QA) > 0);

    if (sep6)
      return TRUE;
  }
  {
    // CA = A - C
    CA = A;
    Vec3d_sub(&CA, &C);

    d3 = ac - cc;
    e3 = Vec3d_dot(&CA, &CA);

    // Q3 = C * e3 - CA * d3
    Q3 = C;
    Vec3d_mulScalar(&Q3, e3);
    CAd3 = CA;
    Vec3d_mulScalar(&CAd3, d3);
    Vec3d_sub(&Q3, &CAd3);

    // QB = B * e3 - Q3
    QB = B;
    Vec3d_mulScalar(&QB, e3);
    Vec3d_sub(&QB, &Q3);

    sep7 = (Vec3d_dot(&Q3, &Q3) > rr * e3 * e3) & (Vec3d_dot(&Q3, &QB) > 0);

    if (sep7)
      return TRUE;
  }
  return FALSE;
}

void Collision_distancePointTriangleExact(Vec3d* point,
                                          Triangle* triangle,
                                          Vec3d* closest) {
  Vec3d diff, edge0, edge1, t0edge0, t1edge1;
  double a00, a01, a11, b0, b1, zero, one, det, t0, t1;
  double invDet;
  double tmp0, tmp1, numer, denom;
  // diff = point - triangle->a
  diff = *point;
  Vec3d_sub(&diff, &triangle->a);
  // edge0 = triangle->b - triangle->a
  edge0 = triangle->b;
  Vec3d_sub(&edge0, &triangle->a);

  // edge1 = triangle->c - triangle->a
  edge1 = triangle->c;
  Vec3d_sub(&edge1, &triangle->a);

  a00 = Vec3d_dot(&edge0, &edge0);
  a01 = Vec3d_dot(&edge0, &edge1);
  a11 = Vec3d_dot(&edge1, &edge1);
  b0 = -Vec3d_dot(&diff, &edge0);
  b1 = -Vec3d_dot(&diff, &edge1);
  zero = (double)0;
  one = (double)1;
  det = a00 * a11 - a01 * a01;
  t0 = a01 * b1 - a11 * b0;
  t1 = a01 * b0 - a00 * b1;

  if (t0 + t1 <= det) {
    if (t0 < zero) {
      if (t1 < zero)  // region 4
      {
        if (b0 < zero) {
          t1 = zero;
          if (-b0 >= a00)  // V1
          {
            t0 = one;
          } else  // E01
          {
            t0 = -b0 / a00;
          }
        } else {
          t0 = zero;
          if (b1 >= zero)  // V0
          {
            t1 = zero;
          } else if (-b1 >= a11)  // V2
          {
            t1 = one;
          } else  // E20
          {
            t1 = -b1 / a11;
          }
        }
      } else  // region 3
      {
        t0 = zero;
        if (b1 >= zero)  // V0
        {
          t1 = zero;
        } else if (-b1 >= a11)  // V2
        {
          t1 = one;
        } else  // E20
        {
          t1 = -b1 / a11;
        }
      }
    } else if (t1 < zero)  // region 5
    {
      t1 = zero;
      if (b0 >= zero)  // V0
      {
        t0 = zero;
      } else if (-b0 >= a00)  // V1
      {
        t0 = one;
      } else  // E01
      {
        t0 = -b0 / a00;
      }
    } else  // region 0, interior
    {
      invDet = det == 0 ? 0 : one / det;
      t0 *= invDet;
      t1 *= invDet;
    }
  } else {
    if (t0 < zero)  // region 2
    {
      tmp0 = a01 + b0;
      tmp1 = a11 + b1;
      if (tmp1 > tmp0) {
        numer = tmp1 - tmp0;
        denom = a00 - ((double)2) * a01 + a11;
        if (numer >= denom)  // V1
        {
          t0 = one;
          t1 = zero;
        } else  // E12
        {
          t0 = numer / denom;
          t1 = one - t0;
        }
      } else {
        t0 = zero;
        if (tmp1 <= zero)  // V2
        {
          t1 = one;
        } else if (b1 >= zero)  // V0
        {
          t1 = zero;
        } else  // E20
        {
          t1 = -b1 / a11;
        }
      }
    } else if (t1 < zero)  // region 6
    {
      tmp0 = a01 + b1;
      tmp1 = a00 + b0;
      if (tmp1 > tmp0) {
        numer = tmp1 - tmp0;
        denom = a00 - ((double)2) * a01 + a11;
        if (numer >= denom)  // V2
        {
          t1 = one;
          t0 = zero;
        } else  // E12
        {
          t1 = numer / denom;
          t0 = one - t1;
        }
      } else {
        t1 = zero;
        if (tmp1 <= zero)  // V1
        {
          t0 = one;
        } else if (b0 >= zero)  // V0
        {
          t0 = zero;
        } else  // E01
        {
          t0 = -b0 / a00;
        }
      }
    } else  // region 1
    {
      numer = a11 + b1 - a01 - b0;
      if (numer <= zero)  // V2
      {
        t0 = zero;
        t1 = one;
      } else {
        denom = a00 - ((double)2) * a01 + a11;
        if (numer >= denom)  // V1
        {
          t0 = one;
          t1 = zero;
        } else  // 12
        {
          t0 = numer / denom;
          t1 = one - t0;
        }
      }
    }
  }

  // closest = triangle->a + t0 * edge0 + t1 * edge1;
  t0edge0 = edge0;
  Vec3d_mulScalar(&t0edge0, t0);
  t1edge1 = edge1;
  Vec3d_mulScalar(&t1edge1, t1);
  *closest = triangle->a;
  Vec3d_add(closest, &t0edge0);
  Vec3d_add(closest, &t1edge1);

  if (closest->x != closest->x) {
#ifndef __N64__
    debugPrintf("got NAN\n");
    // Collision_distancePointTriangleExact(point, triangle, closest);
#endif
    Vec3d_origin(closest);
  }

  // other things we could calculate:
  // parameter[0] = one - t0 - t1;
  // parameter[1] = t0;
  // parameter[2] = t1;
  // diff = point - closest;
  // sqrDistance = Vec3d_dot(diff, diff);
}

#ifndef __N64__
#ifdef __cplusplus

#include <map>
int testCollisionResult;
int testCollisionTrace = FALSE;  // set to true to capture trace

std::map<int, SphereTriangleCollision> testCollisionResults;
#endif
#endif

#define COLLISION_SPATIAL_HASH_MAX_RESULTS 100
#define COLLISION_SPATIAL_HASH_PRUNING_ENABLED 1

float Collision_sqDistancePointAABB(Vec3d* p, AABB* b) {
  float v, dist;
  float sqDist = 0.0f;
  // For each axis count any excess distance outside box extents
  v = p->x;
  if (v < b->min.x) {
    dist = (b->min.x - v);
    sqDist += dist * dist;
  }
  if (v > b->max.x) {
    dist = (v - b->max.x);
    sqDist += dist * dist;
  }

  v = p->y;
  if (v < b->min.y) {
    dist = (b->min.y - v);
    sqDist += dist * dist;
  }
  if (v > b->max.y) {
    dist = (v - b->max.y);
    sqDist += dist * dist;
  }

  v = p->z;
  if (v < b->min.z) {
    dist = (b->min.z - v);
    sqDist += dist * dist;
  }
  if (v > b->max.z) {
    dist = (v - b->max.z);
    sqDist += dist * dist;
  }
  return sqDist;
}

// Returns true if sphere intersects AABB, false otherwise
int Collision_testSphereAABBCollision(Vec3d* sphereCenter,
                                      float sphereRadius,
                                      AABB* aabb) {
  // Compute squared distance between sphere center and AABB
  float sqDist = Collision_sqDistancePointAABB(sphereCenter, aabb);
  // Sphere and AABB intersect if the (squared) distance
  // between them is less
  // than the (squared) sphere radius
  return sqDist <= sphereRadius * sphereRadius;
}

int Collision_testMeshSphereCollision(Triangle* triangles,
                                      int trianglesLength,
                                      Vec3d* objCenter,
                                      float objRadius,
                                      SpatialHash* spatialHash,
                                      SphereTriangleCollision* result) {
  int i, k;
  Triangle* tri;
  Vec3d closestPointOnTriangle;

  float closestHitDistSq;
  float hitDistSq;
  float objRadiusSq;
  AABB triangleAABB;
  // AABB sphereAABB;
  int hit, closestHitTriangleIndex, spatialHashResultsCount;
  int spatialHashResults[COLLISION_SPATIAL_HASH_MAX_RESULTS];
  // float profStartTriangleExact = CUR_TIME_MS();
  closestHitDistSq = FLT_MAX;
  closestHitTriangleIndex = -1;
  objRadiusSq = objRadius * objRadius;

  // AABB_fromSphere(objCenter, objRadius, &sphereAABB);

#ifndef __N64__
#ifdef __cplusplus
  if (testCollisionTrace) {
    testCollisionResults.clear();
  }
#endif
#endif

  spatialHashResultsCount = SpatialHash_getTriangles(
      objCenter, objRadius, spatialHash, spatialHashResults,
      COLLISION_SPATIAL_HASH_MAX_RESULTS);

#if COLLISION_SPATIAL_HASH_PRUNING_ENABLED
  for (k = 0; k < spatialHashResultsCount; k++) {
    i = spatialHashResults[k];
    tri = triangles + i;
#else
  for (i = 0, tri = triangles; i < trianglesLength; i++, tri++) {
#endif
    // as an optimization, first test AABB overlap
    AABB_fromTriangle(tri, &triangleAABB);
    if (!Collision_testSphereAABBCollision(objCenter, objRadius,
                                           &triangleAABB)) {
      continue;
    }
    // if (!Collision_intersectAABBAABB(&sphereAABB, &triangleAABB)) {
    //   continue;
    // }

    // then test triangle intersection
    // hit = !Collision_sphereTriangleIsSeparated(tri, objCenter, objRadius);
    hit = TRUE;

    if (hit) {
      Collision_distancePointTriangleExact(objCenter, tri,
                                           &closestPointOnTriangle);

      hitDistSq = Vec3d_distanceToSq(objCenter, &closestPointOnTriangle);
      if (hitDistSq > objRadiusSq) {
        // not really a collision, separating axis test fucked up
        continue;
      }

#ifndef __N64__
#ifdef __cplusplus
      if (testCollisionTrace) {
        SphereTriangleCollision debugResult = {
            i, hitDistSq, closestPointOnTriangle, tri, triangleAABB};
        testCollisionResults.insert(
            std::pair<int, SphereTriangleCollision>(i, debugResult));
      }
#endif
#endif

      if (hitDistSq < closestHitDistSq) {
        closestHitDistSq = hitDistSq;
        closestHitTriangleIndex = i;

        result->index = i;
        result->distance = sqrtf(closestHitDistSq);
        result->triangle = tri;
        result->posInTriangle = closestPointOnTriangle;
        result->triangleAABB = triangleAABB;
      }
    }
  }

#ifndef __N64__
#ifdef __cplusplus
  if (testCollisionTrace) {
    testCollisionResult = closestHitTriangleIndex;
  }
#endif
#endif

  // Trace_addEvent(CollisionTestMeshSphereTraceEvent, profStartTriangleExact,
  //                CUR_TIME_MS());
  return closestHitTriangleIndex > -1;
}

// Test if segment specified by points p0 and p1 intersects AABB b
// from Real Time Collision Detection ch5.3
int Collision_testSegmentAABBCollision(Vec3d* p0, Vec3d* p1, AABB* b) {
  Vec3d c;
  Vec3d e;
  Vec3d m;
  Vec3d d;
  float adx;
  float ady;
  float adz;

  // Box center-point
  // c = (b->min + b->max) * 0.5f;
  c = b->min;
  Vec3d_add(&c, &b->max);
  Vec3d_mulScalar(&c, 0.5f);

  // Box halflength extents
  // e = b->max - c;
  e = b->max;
  Vec3d_sub(&e, &c);

  // Segment midpoint
  // (p0 + p1) * 0.5f;
  m = *p0;
  Vec3d_add(&m, p1);
  Vec3d_mulScalar(&m, 0.5f);

  // Segment halflength vector
  // d = p1 - m;
  d = *p1;
  Vec3d_sub(&d, &m);

  // Translate box and segment to origin
  // m = m - c;
  Vec3d_sub(&m, &c);

  // Try world coordinate axes as separating axes
  adx = fabsf(d.x);
  if (fabsf(m.x) > e.x + adx)
    return FALSE;
  ady = fabsf(d.y);
  if (fabsf(m.y) > e.y + ady)
    return FALSE;
  adz = fabsf(d.z);
  if (fabsf(m.z) > e.z + adz)
    return FALSE;
  // Add in an epsilon term to counteract arithmetic errors when segment is
  // (near) parallel to a coordinate axis (see text for detail)
  adx += FLT_EPSILON;
  ady += FLT_EPSILON;
  adz += FLT_EPSILON;
  // Try cross products of segment direction vector with coordinate axes
  if (fabsf(m.y * d.z - m.z * d.y) > e.y * adz + e.z * ady)
    return FALSE;
  if (fabsf(m.z * d.x - m.x * d.z) > e.x * adz + e.z * adx)
    return FALSE;
  if (fabsf(m.x * d.y - m.y * d.x) > e.x * ady + e.y * adx)
    return FALSE;  // No separating axis found; segment must be overlapping AABB
  return TRUE;
}

int SpatialHash_getBucketIndex(int cellX, int cellY, int cellsInDimension) {
  // layout is rows then columns (row-major)
  // (x, y) is a cell pos, not unit pos
  return cellY * cellsInDimension + cellX;
}

// quantize world pos to containing grid cell
int SpatialHash_unitsToGridForDimension(float unitsPos,
                                        SpatialHash* spatialHash) {
  return floorf(unitsPos / spatialHash->gridCellSize) +
         spatialHash->cellOffsetInDimension;
}

// convert world pos to integral pos in grid cell
float SpatialHash_unitsToGridFloatForDimension(float unitsPos,
                                               SpatialHash* spatialHash) {
  return unitsPos / spatialHash->gridCellSize +
         spatialHash->cellOffsetInDimension;
}

// localize grid cell to world pos of bottom of cell
float SpatialHash_gridToUnitsForDimension(float gridCell,
                                          SpatialHash* spatialHash) {
  return (gridCell - spatialHash->cellOffsetInDimension) *
         spatialHash->gridCellSize;
}

void SpatialHash_raycast(float x0,
                         float y0,
                         float x1,
                         float y1,
                         SpatialHashRaycastCallback traversalVisitor,
                         void* traversalState) {
  float dx;
  float dy;
  int x;
  int y;
  int n;
  int x_inc, y_inc;
  float error;

  dx = fabs(x1 - x0);
  dy = fabs(y1 - y0);

  x = (int)(floor(x0));
  y = (int)(floor(y0));

  n = 1;

  if (dx == 0) {
    x_inc = 0;
    error = FLT_MAX;
  } else if (x1 > x0) {
    x_inc = 1;
    n += (int)(floor(x1)) - x;
    error = (floor(x0) + 1 - x0) * dy;
  } else {
    x_inc = -1;
    n += x - (int)(floor(x1));
    error = (x0 - floor(x0)) * dy;
  }

  if (dy == 0) {
    y_inc = 0;
    error -= FLT_MAX;
  } else if (y1 > y0) {
    y_inc = 1;
    n += (int)(floor(y1)) - y;
    error -= (floor(y0) + 1 - y0) * dx;
  } else {
    y_inc = -1;
    n += y - (int)(floor(y1));
    error -= (y0 - floor(y0)) * dx;
  }

  for (; n > 0; --n) {
    // visit(x, y);
    traversalVisitor(x, y, traversalState);

    if (error > 0) {
      y += y_inc;
      error -= dx;
    } else {
      x += x_inc;
      error += dy;
    }
  }
}

SpatialHashBucket* SpatialHash_getBucket(float x,
                                         float y,
                                         SpatialHash* spatialHash) {
  int bucketIndex, cellX, cellY;

  cellX = SpatialHash_unitsToGridForDimension(x, spatialHash);
  cellY = SpatialHash_unitsToGridForDimension(y, spatialHash);
  bucketIndex =
      SpatialHash_getBucketIndex(cellX, cellY, spatialHash->cellsInDimension);

  assert(bucketIndex < spatialHash->numBuckets);

  return *(spatialHash->data + bucketIndex);
}

typedef struct GetTrianglesVisitBucketState {
  SpatialHash* spatialHash;
  int* results;
  int maxResults;
  int resultsFound;
} GetTrianglesVisitBucketState;

void SpatialHash_getTrianglesVisitBucket(int cellX,
                                         int cellY,
                                         GetTrianglesVisitBucketState* state) {
  int bucketIndex, bucketItemIndex, resultIndex;
  SpatialHashBucket* bucket;
  int *bucketItem, *currentResult;

  bucketIndex = SpatialHash_getBucketIndex(
      cellX, cellY, state->spatialHash->cellsInDimension);

  assert(bucketIndex < state->spatialHash->numBuckets);

  bucket = *(state->spatialHash->data + bucketIndex);
  if (!bucket) {
    // nothing in this bucket
    return;
  }
  // collect results from this bucket
  for (bucketItemIndex = 0; bucketItemIndex < bucket->size; ++bucketItemIndex) {
    bucketItem = bucket->data + bucketItemIndex;

    // look through results and add if not duplicate
    // TODO: optimize this, as it's currently O(n^2)
    // could qsort then remove duplicates to get to O(n log n)
    for (resultIndex = 0; resultIndex < state->maxResults; ++resultIndex) {
      currentResult = state->results + resultIndex;
      if (resultIndex < state->resultsFound) {
        if (*currentResult == *bucketItem) {
          // already have this triangle in the results
          break;
        } else {
          continue;
        }
      } else {
        // at end of found results and this result is not already in the
        // list
        *currentResult = *bucketItem;
        state->resultsFound++;
        break;  // continue to next item in bucket
      }
    }
  }
}

int SpatialHash_getTrianglesForRaycast(Vec3d* rayStart,
                                       Vec3d* rayEnd,
                                       SpatialHash* spatialHash,
                                       int* results,
                                       int maxResults) {
  GetTrianglesVisitBucketState traversalState;
  traversalState.spatialHash = spatialHash;
  traversalState.results = results;
  traversalState.maxResults = maxResults;
  traversalState.resultsFound = 0;
  SpatialHash_raycast(
      SpatialHash_unitsToGridFloatForDimension(rayStart->x, spatialHash),
      SpatialHash_unitsToGridFloatForDimension(-rayStart->z, spatialHash),
      SpatialHash_unitsToGridFloatForDimension(rayEnd->x, spatialHash),
      SpatialHash_unitsToGridFloatForDimension(-rayEnd->z, spatialHash),
      (SpatialHashRaycastCallback)&SpatialHash_getTrianglesVisitBucket,
      (void*)&traversalState);

#ifndef __N64__
  if (traversalState.resultsFound == maxResults) {
    debugPrintf("possibly ran out of space in results array\n");
  }
#endif

  return traversalState.resultsFound;
}

int SpatialHash_getTriangles(Vec3d* position,
                             float radius,
                             SpatialHash* spatialHash,
                             int* results,
                             int maxResults) {
  int minCellX, minCellY, maxCellX, maxCellY, cellX, cellY;
  GetTrianglesVisitBucketState traversalState;
  // float profStartCollisionGetTriangles = CUR_TIME_MS();

  traversalState.spatialHash = spatialHash;
  traversalState.results = results;
  traversalState.maxResults = maxResults;
  traversalState.resultsFound = 0;

  minCellX =
      SpatialHash_unitsToGridForDimension(position->x - radius, spatialHash);
  minCellY =
      SpatialHash_unitsToGridForDimension(-position->z - radius, spatialHash);
  maxCellX =
      SpatialHash_unitsToGridForDimension(position->x + radius, spatialHash) +
      1;
  maxCellY =
      SpatialHash_unitsToGridForDimension(-position->z + radius, spatialHash) +
      1;

  // walk range of overlapping buckets and collect (unique) set of triangles
  for (cellX = minCellX; cellX < maxCellX; ++cellX) {
    for (cellY = minCellY; cellY < maxCellY; ++cellY) {
      SpatialHash_getTrianglesVisitBucket(cellX, cellY, &traversalState);
    }
  }

#ifndef __N64__
  if (traversalState.resultsFound == maxResults) {
    debugPrintf("possibly ran out of space in results array\n");
  }
#endif

  // Trace_addEvent(CollisionGetTrianglesTraceEvent,
  //                profStartCollisionGetTriangles, CUR_TIME_MS());
  return traversalState.resultsFound;
}
