#include "constants.h"
#include "vec3d.h"

int Collision_sphereTriangleIsSeparated(Vec3d* AOrig,
                                        Vec3d* BOrig,
                                        Vec3d* COrig,
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
  Vec3d_copyFrom(&A, AOrig);
  Vec3d_sub(&A, P);
  // B = B - P
  Vec3d_copyFrom(&B, BOrig);
  Vec3d_sub(&B, P);
  // C = C - P
  Vec3d_copyFrom(&C, COrig);
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
