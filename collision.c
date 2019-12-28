#include <math.h>

#include "constants.h"

#include "collision.h"
#include "vec3d.h"

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
}

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

  // V = (B - A).cross(C - A)
  Vec3d_copyFrom(&BSubA, &B);
  Vec3d_sub(&BSubA, &A);
  Vec3d_copyFrom(&CSubA, &C);
  Vec3d_sub(&CSubA, &A);
  Vec3d_cross(&BSubA, &CSubA, &V);

  d = Vec3d_dot(&A, &V);
  e = Vec3d_dot(&V, &V);
  sep1 = d * d > rr * e;

  if (sep1)
    return TRUE;

  aa = Vec3d_dot(&A, &A);
  ab = Vec3d_dot(&A, &B);
  ac = Vec3d_dot(&A, &C);
  sep2 = (aa > rr) & (ab > aa) & (ac > aa);

  if (sep2)
    return TRUE;

  bb = Vec3d_dot(&B, &B);
  bc = Vec3d_dot(&B, &C);
  sep3 = (bb > rr) & (ab > bb) & (bc > bb);

  if (sep3)
    return TRUE;

  cc = Vec3d_dot(&C, &C);
  sep4 = (cc > rr) & (ac > cc) & (bc > cc);

  if (sep4)
    return TRUE;

  // AB = B - A
  // BC = C - B
  // CA = A - C
  Vec3d_copyFrom(&AB, &B);
  Vec3d_sub(&AB, &A);
  Vec3d_copyFrom(&BC, &C);
  Vec3d_sub(&BC, &B);
  Vec3d_copyFrom(&CA, &A);
  Vec3d_sub(&CA, &C);

  d1 = ab - aa;
  e1 = Vec3d_dot(&AB, &AB);

  // Q1 = A * e1 - AB * d1
  Vec3d_copyFrom(&Q1, &A);
  Vec3d_multiplyScalar(&Q1, e1);
  Vec3d_copyFrom(&ABd1, &AB);
  Vec3d_multiplyScalar(&ABd1, d1);
  Vec3d_sub(&Q1, &ABd1);

  // QC = C * e1 - Q1
  Vec3d_copyFrom(&QC, &C);
  Vec3d_multiplyScalar(&QC, e1);
  Vec3d_sub(&QC, &Q1);

  sep5 = (Vec3d_dot(&Q1, &Q1) > rr * e1 * e1) & (Vec3d_dot(&Q1, &QC) > 0);

  if (sep5)
    return TRUE;

  d2 = bc - bb;
  e2 = Vec3d_dot(&BC, &BC);

  // Q2 = B * e2 - BC * d2
  Vec3d_copyFrom(&Q2, &B);
  Vec3d_multiplyScalar(&Q2, e2);
  Vec3d_copyFrom(&BCd2, &BC);
  Vec3d_multiplyScalar(&BCd2, d2);
  Vec3d_sub(&Q2, &BCd2);

  // QA = A * e2 - Q2
  Vec3d_copyFrom(&QA, &A);
  Vec3d_multiplyScalar(&QA, e2);
  Vec3d_sub(&QA, &Q2);

  sep6 = (Vec3d_dot(&Q2, &Q2) > rr * e2 * e2) & (Vec3d_dot(&Q2, &QA) > 0);

  if (sep6)
    return TRUE;

  d3 = ac - cc;
  e3 = Vec3d_dot(&CA, &CA);

  // Q3 = C * e3 - CA * d3
  Vec3d_copyFrom(&Q3, &C);
  Vec3d_multiplyScalar(&Q3, e3);
  Vec3d_copyFrom(&CAd3, &CA);
  Vec3d_multiplyScalar(&CAd3, d3);
  Vec3d_sub(&Q3, &CAd3);

  // QB = B * e3 - Q3
  Vec3d_copyFrom(&QB, &B);
  Vec3d_multiplyScalar(&QB, e3);
  Vec3d_sub(&QB, &Q3);

  sep7 = (Vec3d_dot(&Q3, &Q3) > rr * e3 * e3) & (Vec3d_dot(&Q3, &QB) > 0);

  if (sep7)
    return TRUE;

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
  Vec3d_multiplyScalar(&t0edge0, t0);
  t1edge1 = edge1;
  Vec3d_multiplyScalar(&t1edge1, t1);
  *closest = triangle->a;
  Vec3d_add(closest, &t0edge0);
  Vec3d_add(closest, &t1edge1);

  if (closest->x != closest->x) {
#ifndef __N64__
    printf("got NAN\n");
    Collision_distancePointTriangleExact(point, triangle, closest);
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

std::map<int, SphereTriangleCollision> testCollisionResults;
#endif
#endif

int Collision_testMeshSphereCollision(Triangle* triangles,
                                      int trianglesLength,
                                      Vec3d* objCenter,
                                      float objRadius,
                                      SphereTriangleCollision* result) {
  int i;
  Triangle* tri;
  Vec3d closestPointOnTriangle;

  float closestHitDist;
  float hitDist;
  int hit, closestHitTriangleIndex;
  closestHitDist = 3.402823e+38;  // FLT_MAX;
  closestHitTriangleIndex = -1;

#ifndef __N64__
#ifdef __cplusplus
  testCollisionResults.clear();
#endif
#endif

  for (i = 0, tri = triangles; i < trianglesLength; i++, tri++) {
    hit = !Collision_sphereTriangleIsSeparated(tri, objCenter, objRadius);

    if (hit) {
      printf("collided with: %d ", i);
      Vec3d_print(&tri->a);
      Vec3d_print(&tri->b);
      Vec3d_print(&tri->c);
      printf("\n");

      Collision_distancePointTriangleExact(objCenter, tri,
                                           &closestPointOnTriangle);
      printf("objCenter:");
      Vec3d_print(objCenter);
      printf("closestPointOnTriangle:");
      Vec3d_print(&closestPointOnTriangle);
      printf(" \n");

      hitDist = Vec3d_distanceTo(objCenter, &closestPointOnTriangle);

#ifndef __N64__
#ifdef __cplusplus
      SphereTriangleCollision debugResult = {i, hitDist, closestPointOnTriangle,
                                             tri};
      testCollisionResults.insert(
          std::pair<int, SphereTriangleCollision>(i, debugResult));

      printf("hit dist: %f\n", hitDist);
#endif
#endif

      if (hitDist < closestHitDist) {
        closestHitDist = hitDist;
        closestHitTriangleIndex = i;

        result->index = i;
        result->distance = closestHitDist;
        result->triangle = tri;
        result->posInTriangle = closestPointOnTriangle;
      }
    }
  }

#ifndef __N64__
#ifdef __cplusplus
  testCollisionResult = closestHitTriangleIndex;
#endif
#endif

  if (closestHitTriangleIndex > -1) {
    return TRUE;
  } else {
    return FALSE;
  }
}
