
#include "frustum.h"
#include <math.h>

#include "constants.h"

char *FrustumTestResultStrings[MAX_FRUSTUM_TEST_RESULT] = {
    "Inside",
    "Outside",
    "Intersecting",
};

char *FrustumPlanesStrings[NUM_FRUSTUM_PLANES] = {
    "NearFrustumPlane",   //
    "FarFrustumPlane",    //
    "TopFrustumPlane",    //
    "BottomFrustumPlane", //
    "LeftFrustumPlane",   //
    "RightFrustumPlane",  //
};

void Plane_setNormalAndPoint(Plane *self, Vec3d *normal, Vec3d *point)
{
  self->normal = *normal;
  Vec3d_normalise(&self->normal);
  self->point = *point;

  self->d = -(Vec3d_dot(&self->normal, &self->point));
}

void Plane_set3Points(Plane *self, Vec3d *v1, Vec3d *v2, Vec3d *v3)
{
  Vec3d aux1, aux2;

  aux1 = *v1;
  Vec3d_sub(&aux1, v2);
  aux2 = *v3;
  Vec3d_sub(&aux2, v2);

  Vec3d_cross(&aux2, &aux1, &self->normal);

  Vec3d_normalise(&self->normal);
  self->point = *v2;
  self->d = -(Vec3d_dot(&self->normal, &self->point));
}

float Plane_distance(Plane *self, Vec3d *p)
{
  return (self->d + Vec3d_dot(&self->normal, p));
}

void Plane_pointClosestPoint(Plane *p, Vec3d *q, Vec3d *result)
{
  // float t = Dot(p.n, q) - p.d;
  // return q - t * p.n;

  Vec3d tmp;
  float t = Vec3d_dot(&p->normal, q) + p->d;

  *result = *q;
  tmp = p->normal;
  Vec3d_mulScalar(&tmp, t);
  Vec3d_sub(result, &tmp);
}

float Plane_distPointToPlane(Plane *p, Vec3d *q)
{
  // return Dot(q, p.n) - p.d; if plane equation normalized (||p.n||==1)
  return (Vec3d_dot(&p->normal, q) + p->d) / Vec3d_dot(&p->normal, &p->normal);
}

// based on
// http://www.lighthouse3d.com/tutorials/view-frustum-culling/geometric-approach-implementation/

// This function takes exactly the same parameters as the function
// guPerspective/gluPerspective. Each time the perspective definitions change,
// for instance when a window is resized, this function should be called as
// well.
void Frustum_setCamInternals(Frustum *self,
                             float fovy,
                             float aspect,
                             float nearD,
                             float farD)
{
  self->aspect = aspect;
  self->fovy = fovy;
  self->nearD = nearD;
  self->farD = farD;

  self->tang = (float)tanf(degToRad(fovy) * 0.5f);
  self->nh = nearD * self->tang;
  self->nw = self->nh * aspect;
  self->fh = farD * self->tang;
  self->fw = self->fh * aspect;
}

// This function takes three vectors that contain the information for the
// guLookAt/gluLookAt function: the position of the camera (p), a point to where
// the camera is pointing (l) and the up vector (u). Each time the camera
// position or orientation changes, this function should be called as well.
void Frustum_setCamDef(Frustum *self, Vec3d *p, Vec3d *l, Vec3d *u)
{
  Vec3d nc, fc, X, Y, Z;
  // compute the Z axis of camera
  // this axis points in the opposite direction from
  // the looking direction
  // Z = p - l;
  Z = *p;
  Vec3d_sub(&Z, l);
  Vec3d_normalise(&Z);

  // X axis of camera with given "up" vector and Z axis
  // X = u * Z;
  Vec3d_cross(u, &Z, &X);
  Vec3d_normalise(&X);

  // the real "up" vector is the cross product of Z and X
  // Y = Z * X;
  Vec3d_cross(&Z, &X, &Y);

  // compute the centers of the near and far planes
  {
    // nc = p - Z * nearD;
    Vec3d ZNearD;
    ZNearD = Z;
    Vec3d_mulScalar(&ZNearD, self->nearD);
    nc = *p;
    Vec3d_sub(&nc, &ZNearD);
  }

  {
    // fc = p - Z * farD;
    Vec3d ZFarD;
    ZFarD = Z;
    Vec3d_mulScalar(&ZFarD, self->farD);
    fc = *p;
    Vec3d_sub(&fc, &ZFarD);
  }

  {
    Vec3d Ynh, Xnw, Yfh, Xfw;

    Ynh = Y;
    Vec3d_mulScalar(&Ynh, self->nh);
    Xnw = X;
    Vec3d_mulScalar(&Xnw, self->nw);
    Yfh = Y;
    Vec3d_mulScalar(&Yfh, self->fh);
    Xfw = X;
    Vec3d_mulScalar(&Xfw, self->fw);

    // ntl = nc + Ynh - Xnw;
    self->ntl = nc;
    Vec3d_add(&self->ntl, &Ynh);
    Vec3d_sub(&self->ntl, &Xnw);

    // ntr = nc + Ynh + Xnw;
    self->ntr = nc;
    Vec3d_add(&self->ntr, &Ynh);
    Vec3d_add(&self->ntr, &Xnw);

    // nbl = nc - Ynh - Xnw;
    self->nbl = nc;
    Vec3d_sub(&self->nbl, &Ynh);
    Vec3d_sub(&self->nbl, &Xnw);

    // nbr = nc - Ynh + Xnw;
    self->nbr = nc;
    Vec3d_sub(&self->nbr, &Ynh);
    Vec3d_add(&self->nbr, &Xnw);

    // ftl = fc + Yfh - Xfw;
    self->ftl = fc;
    Vec3d_add(&self->ftl, &Yfh);
    Vec3d_sub(&self->ftl, &Xfw);

    // ftr = fc + Yfh + Xfw;
    self->ftr = fc;
    Vec3d_add(&self->ftr, &Yfh);
    Vec3d_add(&self->ftr, &Xfw);

    // fbl = fc - Yfh - Xfw;
    self->fbl = fc;
    Vec3d_sub(&self->fbl, &Yfh);
    Vec3d_sub(&self->fbl, &Xfw);

    // fbr = fc - Yfh + Xfw;
    self->fbr = fc;
    Vec3d_sub(&self->fbr, &Yfh);
    Vec3d_add(&self->fbr, &Xfw);
  }

  Plane_set3Points(&self->planes[TopFrustumPlane], &self->ntr, &self->ntl,
                   &self->ftl);
  Plane_set3Points(&self->planes[BottomFrustumPlane], &self->nbl, &self->nbr,
                   &self->fbr);
  Plane_set3Points(&self->planes[LeftFrustumPlane], &self->ntl, &self->nbl,
                   &self->fbl);
  Plane_set3Points(&self->planes[RightFrustumPlane], &self->nbr, &self->ntr,
                   &self->fbr);
  Plane_set3Points(&self->planes[NearFrustumPlane], &self->ntl, &self->ntr,
                   &self->nbr);
  Plane_set3Points(&self->planes[FarFrustumPlane], &self->ftr, &self->ftl,
                   &self->fbl);
}

void Frustum_setCamDef2(Frustum *self, Vec3d *p, Vec3d *l, Vec3d *u)
{
  Vec3d aux, normal;

  Vec3d nc, fc, X, Y, Z;
  // compute the Z axis of camera
  // this axis points in the opposite direction from
  // the looking direction
  // Z = p - l;
  Z = *p;
  Vec3d_sub(&Z, l);
  Vec3d_normalise(&Z);

  // X axis of camera with given "up" vector and Z axis
  // X = u * Z;
  Vec3d_cross(u, &Z, &X);
  Vec3d_normalise(&X);

  // the real "up" vector is the cross product of Z and X
  // Y = Z * X;
  Vec3d_cross(&Z, &X, &Y);

  // compute the centers of the near and far planes
  {
    // nc = p - Z * nearD;
    Vec3d tempZ;
    tempZ = Z;
    Vec3d_mulScalar(&tempZ, self->nearD);
    nc = *p;
    Vec3d_sub(&nc, &tempZ);
  }

  {
    // fc = p - Z * farD;
    Vec3d tempZ;
    tempZ = Z;
    Vec3d_mulScalar(&tempZ, self->farD);
    fc = *p;
    Vec3d_sub(&fc, &tempZ);
  }

  // near
  {
    Vec3d negZ;
    negZ = Z;
    Vec3d_mulScalar(&negZ, -1.0);
    Plane_setNormalAndPoint(&self->planes[NearFrustumPlane], &negZ, &nc);
  }
  // far
  Plane_setNormalAndPoint(&self->planes[FarFrustumPlane], &Z, &fc);

  {
    Vec3d Ynh;
    Ynh = Y;
    Vec3d_mulScalar(&Ynh, self->nh);
    // top
    {
      // aux = (nc + Y * nh) - p;
      Vec3d nsAddYnh;
      nsAddYnh = nc;
      Vec3d_add(&nsAddYnh, &Ynh);
      aux = nsAddYnh;
      Vec3d_sub(&aux, p);
      Vec3d_normalise(&aux);
      // normal = aux * X;
      Vec3d_cross(&aux, &X, &normal);
      Plane_setNormalAndPoint(&self->planes[TopFrustumPlane], &normal,
                              &nsAddYnh);
    }
    // bottom
    {
      // aux = (nc - Y * nh) - p;
      Vec3d ncSubYnh;
      ncSubYnh = nc;
      Vec3d_sub(&ncSubYnh, &Ynh);
      aux = ncSubYnh;
      Vec3d_sub(&aux, p);
      Vec3d_normalise(&aux);
      // normal = X * aux;
      Vec3d_cross(&aux, &X, &normal);
      Plane_setNormalAndPoint(&self->planes[BottomFrustumPlane], &normal,
                              &ncSubYnh);
    }
  }
  {
    Vec3d Xnw;
    Xnw = X;
    Vec3d_mulScalar(&Xnw, self->nw);
    // left
    {
      // aux = (nc - X * nw) - p;
      Vec3d ncSubXnw;
      ncSubXnw = nc;
      Vec3d_sub(&ncSubXnw, &Xnw);

      aux = ncSubXnw;
      Vec3d_sub(&aux, p);
      Vec3d_normalise(&aux);
      Vec3d_cross(&aux, &Y, &normal);
      Plane_setNormalAndPoint(&self->planes[LeftFrustumPlane], &normal,
                              &ncSubXnw);
    }
    // right
    {
      // aux = (nc + X * nw) - p;
      Vec3d ncAddXnw;
      ncAddXnw = nc;
      Vec3d_add(&ncAddXnw, &Xnw);
      aux = ncAddXnw;
      Vec3d_sub(&aux, p);
      Vec3d_normalise(&aux);
      Vec3d_cross(&aux, &Y, &normal);
      Plane_setNormalAndPoint(&self->planes[RightFrustumPlane], &normal,
                              &ncAddXnw);
    }
  }
}

void Frustum_getAABBVertexP(AABB *self, Vec3d *normal, Vec3d *result)
{
  *result = self->min;

  if (normal->x >= 0)
    result->x = self->max.x;

  if (normal->y >= 0)
    result->y = self->max.y;

  if (normal->z >= 0)
    result->z = self->max.z;
}

void Frustum_getAABBVertexN(AABB *self, Vec3d *normal, Vec3d *result)
{
  *result = self->max;

  if (normal->x >= 0)
    result->x = self->min.x;

  if (normal->y >= 0)
    result->y = self->min.y;

  if (normal->z >= 0)
    result->z = self->min.z;
}

// FrustumTestResult Frustum_boxInFrustum(Frustum* frustum, AABB* aabb) {
//   int i;
//   float r, s;
//   Vec3d center, positiveExtents;
//   Plane* plane;
//   FrustumTestResult result;
//   result = InsideFrustum;
//   // for each plane do ...
//   for (i = 0; i < NUM_FRUSTUM_PLANES; i++) {
//     plane = &frustum->planes[i];
//     // Compute AABB center
//     center = aabb->max;
//     Vec3d_add(&center, &aabb->min);
//     Vec3d_mulScalar(&center, 0.5f);

//     // Compute positive extents
//     positiveExtents = aabb->max;
//     Vec3d_sub(&positiveExtents, &center);

//     // Compute the projection interval radius of b onto L(t) = aabb->c + t *
//     p.n r = positiveExtents.x * fabsf(plane->normal.x) +
//         positiveExtents.y * fabsf(plane->normal.y) +
//         positiveExtents.z * fabsf(plane->normal.z);
//     // Compute distance of box center from plane
//     s = Vec3d_dot(&plane->normal, &center) - plane->d;
//     // Intersection occurs when distance s falls within [-r,+r] interval
//     // fabsf(s) <= r;

//     if (fabsf(s) <= r) {
//       result = IntersectingFrustum;
//     } else if (s < -r) {
//       return OutsideFrustum;
//     }
//   }

//   return result;
// }

// from Real Time Collision Detection ch 5.2.3
// currently doesn't work, maybe plane representation is wrong?
FrustumTestResult Frustum_boxFrustumPlaneTestRTCD(Frustum *frustum,
                                                  AABB *aabb,
                                                  int planeIdx)
{
  float r, s;
  Vec3d center, positiveExtents;
  Plane *plane;
  FrustumTestResult result = InsideFrustum;
  // for each plane do ...
  plane = &frustum->planes[planeIdx];
  // Compute AABB center
  // center = (min + max) * 0.5
  center = aabb->min;
  Vec3d_add(&center, &aabb->max);
  Vec3d_mulScalar(&center, 0.5f);

  // Compute positive extents
  // extents = max - center
  positiveExtents = aabb->max;
  Vec3d_sub(&positiveExtents, &center);

  // Compute the projection interval radius of b onto L(t) = center + t *
  // plane.normal
  r = positiveExtents.x * fabsf(plane->normal.x) +
      positiveExtents.y * fabsf(plane->normal.y) +
      positiveExtents.z * fabsf(plane->normal.z);
  // Compute distance of box center from plane
  s = Vec3d_dot(&plane->normal, &center) - plane->d;
  // Intersection occurs when distance s falls within [-r,+r] interval
  // fabsf(s) <= r;

  if (s + r > 0)
  {
    result = IntersectingFrustum;
  }
  else if (s - r >= 0)
  {
    // fully inside
  }
  else
  {
    return OutsideFrustum;
  }

  return result;
}

FrustumTestResult Frustum_boxFrustumPlaneTestPN(Frustum *frustum,
                                                AABB *aabb,
                                                int planeIdx)
{
  Vec3d vertexP, vertexN;
  FrustumTestResult result = InsideFrustum;
  Frustum_getAABBVertexP(aabb, &frustum->planes[planeIdx].normal, &vertexP);
  Frustum_getAABBVertexN(aabb, &frustum->planes[planeIdx].normal, &vertexN);
  // is the positive vertex outside?
  if (Plane_distance(&frustum->planes[planeIdx], &vertexP) < 0)
  {
    return OutsideFrustum;
    // is the negative vertex outside?
  }
  else if (Plane_distance(&frustum->planes[planeIdx], &vertexN) < 0)
  {
    result = IntersectingFrustum;
  }
  return result;
}

FrustumTestResult Frustum_boxInFrustum(Frustum *frustum, AABB *aabb)
{
  FrustumTestResult result = InsideFrustum;
  FrustumTestResult planeResult;
  int i;
  // for each plane do ...
  for (i = 0; i < NUM_FRUSTUM_PLANES; i++)
  {
    planeResult = Frustum_boxFrustumPlaneTestPN(frustum, aabb, i);

    if (planeResult == OutsideFrustum)
    {
      return OutsideFrustum;
    }
    else if (planeResult == IntersectingFrustum)
    {
      result = planeResult;
    }
  }
  return result;
}

void Frustum_getAABBVertex(AABB *aabb, int vertex, Vec3d *result)
{
  switch (vertex)
  {
  case 0:
    *result = (Vec3d){aabb->min.x, aabb->min.y, aabb->min.z};
    return;
  case 1:
    *result = (Vec3d){aabb->max.x, aabb->min.y, aabb->min.z};
    return;
  case 2:
    *result = (Vec3d){aabb->min.x, aabb->max.y, aabb->min.z};
    return;
  case 3:
    *result = (Vec3d){aabb->min.x, aabb->min.y, aabb->max.z};
    return;
  case 4:
    *result = (Vec3d){aabb->max.x, aabb->max.y, aabb->max.z};
    return;
  case 5:
    *result = (Vec3d){aabb->min.x, aabb->max.y, aabb->max.z};
    return;
  case 6:
    *result = (Vec3d){aabb->max.x, aabb->min.y, aabb->max.z};
    return;
  case 7:
    *result = (Vec3d){aabb->max.x, aabb->max.y, aabb->min.z};
    return;
  }
}

FrustumTestResult Frustum_boxInFrustumNaive(Frustum *frustum, AABB *aabb)
{
  int i, k;
  int out;
  int in;
  FrustumTestResult result = InsideFrustum;
  Vec3d vertex;

  // for each plane do ...
  for (i = 0; i < NUM_FRUSTUM_PLANES; i++)
  {
    // reset counters for corners in and out
    out = 0;
    in = 0;
    // for each corner of the box do ...
    // get out of the cycle as soon as a box as corners
    // both inside and out of the frustum
    for (k = 0; k < 8 && (in == 0 || out == 0); k++)
    {
      Frustum_getAABBVertex(aabb, k, &vertex);
      // is the corner outside or inside
      if (Plane_distance(&frustum->planes[i], &vertex) < 0)
      {
        out++;
      }
      else
      {
        in++;
      }
    }
    // if all corners are out
    if (!in)
    {
      return OutsideFrustum;
      // if some corners are out and others are in
    }
    else if (out)
    {
      result = IntersectingFrustum;
    }
  }
  return result;
}
