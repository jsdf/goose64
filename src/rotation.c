#ifndef __N64__
#include <float.h>
#endif
#include <math.h>

#include "constants.h"
#include "rotation.h"
#include "vec3d.h"

static Vec3d _zero = {0, 0, 0};
static Vec3d _one = {1, 1, 1};

void Matrix4_compose(Matrix4* matrix,
                     Vec3d* position,
                     Quaternion* quaternion,
                     Vec3d* scale) {
  float x, y, z, w, x2, y2, z2, xx, xy, xz, yy, yz, zz, wx, wy, wz, sx, sy, sz;

  x = quaternion->x, y = quaternion->y, z = quaternion->z, w = quaternion->w;
  x2 = x + x, y2 = y + y, z2 = z + z;
  xx = x * x2, xy = x * y2, xz = x * z2;
  yy = y * y2, yz = y * z2, zz = z * z2;
  wx = w * x2, wy = w * y2, wz = w * z2;

  sx = scale->x, sy = scale->y, sz = scale->z;

  matrix->elements[0] = (1 - (yy + zz)) * sx;
  matrix->elements[1] = (xy + wz) * sx;
  matrix->elements[2] = (xz - wy) * sx;
  matrix->elements[3] = 0;

  matrix->elements[4] = (xy - wz) * sy;
  matrix->elements[5] = (1 - (xx + zz)) * sy;
  matrix->elements[6] = (yz + wx) * sy;
  matrix->elements[7] = 0;

  matrix->elements[8] = (xz + wy) * sz;
  matrix->elements[9] = (yz - wx) * sz;
  matrix->elements[10] = (1 - (xx + yy)) * sz;
  matrix->elements[11] = 0;

  matrix->elements[12] = position->x;
  matrix->elements[13] = position->y;
  matrix->elements[14] = position->z;
  matrix->elements[15] = 1;
}

void Matrix4_makeRotationFromQuaternion(Matrix4* self, Quaternion* q) {
  Matrix4_compose(self, &_zero, q, &_one);
}

void Matrix4_makeRotationFromEuler(Matrix4* self, Euler* euler) {
  float x, y, z, a, b, c, d, e, f, ae, af, be, bf;

  x = euler->x, y = euler->y, z = euler->z;
  a = cosf(x), b = sinf(x);
  c = cosf(y), d = sinf(y);
  e = cosf(z), f = sinf(z);

  ae = a * e, af = a * f, be = b * e, bf = b * f;

  self->elements[0] = c * e;
  self->elements[4] = -c * f;
  self->elements[8] = d;

  self->elements[1] = af + be * d;
  self->elements[5] = ae - bf * d;
  self->elements[9] = -b * c;

  self->elements[2] = bf - ae * d;
  self->elements[6] = be + af * d;
  self->elements[10] = a * c;

  // bottom row
  self->elements[3] = 0;
  self->elements[7] = 0;
  self->elements[11] = 0;

  // last column
  self->elements[12] = 0;
  self->elements[13] = 0;
  self->elements[14] = 0;
  self->elements[15] = 1;
}

void Euler_setFromRotationMatrix(Euler* self, Matrix4* m) {
  float m11, m12, m13, m21, m22, m23, m31, m32, m33;

  // assumes the upper 3x3 of m is a pure rotation matrix (i.e, unscaled)

  m11 = m->elements[0], m12 = m->elements[4], m13 = m->elements[8];
  m21 = m->elements[1], m22 = m->elements[5], m23 = m->elements[9];
  m31 = m->elements[2], m32 = m->elements[6], m33 = m->elements[10];

  self->y = asinf(CLAMP(m13, -1, 1));

  if (fabsf(m13) < 0.9999999) {
    self->x = atan2f(-m23, m33);
    self->z = atan2f(-m12, m11);

  } else {
    self->x = atan2f(m32, m22);
    self->z = 0;
  }
}

EulerDegrees* EulerDegrees_origin(EulerDegrees* self) {
  self->x = 0.0F;
  self->y = 0.0F;
  self->z = 0.0F;
  return self;
}

void EulerDegrees_fromEuler(EulerDegrees* degrees, Euler* radians) {
  degrees->x = radToDeg(radians->x);
  degrees->y = radToDeg(radians->y);
  degrees->z = radToDeg(radians->z);
}

void Euler_fromEulerDegrees(Euler* radians, EulerDegrees* degrees) {
  radians->x = degToRad(degrees->x);
  radians->y = degToRad(degrees->y);
  radians->z = degToRad(degrees->z);
}

void Euler_setFromQuaternion(Euler* self, Quaternion* quaternion) {
  Matrix4 matrix;

  Matrix4_makeRotationFromQuaternion(&matrix, quaternion);

  Euler_setFromRotationMatrix(self, &matrix);
}

// TODO: maybe delete. don't think this is used anywhere
void Quaternion_setFromRotationMatrix(Quaternion* self, Matrix4* m) {
  float m11, m12, m13, m21, m22, m23, m31, m32, m33, trace, s;

  //  http://www.euclideanspace.com/maths/geometry/rotations/conversions/matrixToQuaternion/index.htm

  // assumes the upper 3x3 of m is a pure rotation matrix (i.e, unscaled)

  m11 = m->elements[0], m12 = m->elements[4], m13 = m->elements[8],
  m21 = m->elements[1], m22 = m->elements[5], m23 = m->elements[9],
  m31 = m->elements[2], m32 = m->elements[6], m33 = m->elements[10],

  trace = m11 + m22 + m33;

  if (trace > 0.0) {
    s = 0.5 / sqrtf(trace + 1.0);

    self->w = 0.25 / s;
    self->x = (m32 - m23) * s;
    self->y = (m13 - m31) * s;
    self->z = (m21 - m12) * s;

  } else if (m11 > m22 && m11 > m33) {
    s = 2.0 * sqrtf(1.0 + m11 - m22 - m33);

    self->w = (m32 - m23) / s;
    self->x = 0.25 * s;
    self->y = (m12 + m21) / s;
    self->z = (m13 + m31) / s;

  } else if (m22 > m33) {
    s = 2.0 * sqrtf(1.0 + m22 - m11 - m33);

    self->w = (m13 - m31) / s;
    self->x = (m12 + m21) / s;
    self->y = 0.25 * s;
    self->z = (m23 + m32) / s;

  } else {
    s = 2.0 * sqrtf(1.0 + m33 - m11 - m22);

    self->w = (m21 - m12) / s;
    self->x = (m13 + m31) / s;
    self->y = (m23 + m32) / s;
    self->z = 0.25 * s;
  }
}

void Quaternion_fromEuler(Quaternion* self, Euler* euler) {
  float x, y, z, c1, c2, c3, s1, s2, s3;

  x = euler->x;
  y = euler->y;
  z = euler->z;

  // http://www.mathworks.com/matlabcentral/fileexchange/
  //  20696-function-to-convert-between-dcm-euler-angles-quaternions-and-euler-vectors/
  //  content/SpinCalc.m

  c1 = cosf(x / 2.0f);
  c2 = cosf(y / 2.0f);
  c3 = cosf(z / 2.0f);

  s1 = sinf(x / 2.0f);
  s2 = sinf(y / 2.0f);
  s3 = sinf(z / 2.0f);

  self->x = s1 * c2 * c3 + c1 * s2 * s3;
  self->y = c1 * s2 * c3 - s1 * c2 * s3;
  self->z = c1 * c2 * s3 + s1 * s2 * c3;
  self->w = c1 * c2 * c3 - s1 * s2 * s3;
}

float Quaternion_length(Quaternion* self) {
  return sqrtf(self->x * self->x + self->y * self->y + self->z * self->z +
               self->w * self->w);
}

void Quaternion_normalize(Quaternion* self) {
  float l;
  l = Quaternion_length(self);

  if (l == 0.0f) {
    self->x = 0.0f;
    self->y = 0.0f;
    self->z = 0.0f;
    self->w = 1.0f;

  } else {
    l = 1.0f / l;

    self->x = self->x * l;
    self->y = self->y * l;
    self->z = self->z * l;
    self->w = self->w * l;
  }
}

void Quaternion_slerp(Quaternion* self, Quaternion* qb, float t) {
  float x, y, z, w, cosHalfTheta, sqrSinHalfTheta, sinHalfTheta, halfTheta,
      ratioA, ratioB, s;

  if (t == 0.0) {
    return;
  }
  if (t == 1.0) {
    // copy other
    *self = *qb;
  }

  x = self->x, y = self->y, z = self->z, w = self->w;

  // http://www.euclideanspace.com/maths/algebra/realNormedAlgebra/quaternions/slerp/

  cosHalfTheta = w * qb->w + x * qb->x + y * qb->y + z * qb->z;

  if (cosHalfTheta < 0.0) {
    self->w = -qb->w;
    self->x = -qb->x;
    self->y = -qb->y;
    self->z = -qb->z;

    cosHalfTheta = -cosHalfTheta;

  } else {
    // copy other
    *self = *qb;
  }

  if (cosHalfTheta >= 1.0) {
    self->w = w;
    self->x = x;
    self->y = y;
    self->z = z;

    return;
  }

  sqrSinHalfTheta = 1.0 - cosHalfTheta * cosHalfTheta;

  // TODO: define FLT_EPSILON for N64???
  if (sqrSinHalfTheta <= FLT_EPSILON) {
    s = 1.0 - t;
    self->w = s * w + t * self->w;
    self->x = s * x + t * self->x;
    self->y = s * y + t * self->y;
    self->z = s * z + t * self->z;

    Quaternion_normalize(self);
    return;
  }

  sinHalfTheta = sqrtf(sqrSinHalfTheta);
  halfTheta = atan2f(sinHalfTheta, cosHalfTheta);
  ratioA = sinf((1 - t) * halfTheta) / sinHalfTheta,
  ratioB = sinf(t * halfTheta) / sinHalfTheta;

  self->w = (w * ratioA + self->w * ratioB);
  self->x = (x * ratioA + self->x * ratioB);
  self->y = (y * ratioA + self->y * ratioB);
  self->z = (z * ratioA + self->z * ratioB);
}
