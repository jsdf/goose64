
#ifndef ROTATION_H
#define ROTATION_H

#include "vec3d.h"

typedef struct Matrix4 {
  float elements[16];
} Matrix4;

typedef struct Quaternion {
  float x;
  float y;
  float z;
  float w;
} Quaternion;

typedef struct Euler {
  float x;
  float y;
  float z;
} Euler;

typedef struct EulerDegrees {
  float x;
  float y;
  float z;
} EulerDegrees;

void Euler_fromEulerDegrees(Euler* radians, EulerDegrees* degrees);
void Euler_setFromQuaternion(Euler* self, Quaternion* quaternion);

void EulerDegrees_fromEuler(EulerDegrees* degrees, Euler* radians);
EulerDegrees* EulerDegrees_origin(EulerDegrees* self);

void Matrix4_makeRotationFromEuler(Matrix4* self, Euler* euler);

void Quaternion_fromEuler(Quaternion* self, Euler* euler);
void Quaternion_slerp(Quaternion* self, Quaternion* qb, float t);
void Quaternion_setFromRotationMatrix(Quaternion* self, Matrix4* m);

#endif /* !ROTATION_H */
