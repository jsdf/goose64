
#include "vec3d.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef __N64__
#include <gu.h>
#endif

Vec3d* Vec3d_alloc(float x, float y, float z) {
  Vec3d* created = (Vec3d*)malloc(sizeof(Vec3d));
  created->x = x;
  created->y = y;
  created->z = z;
  return created;
}

void Vec3d_set(Vec3d* self, float x, float y, float z) {
  self->x = x;
  self->y = y;
  self->z = z;
}

Vec3d* Vec3d_init(Vec3d* self, float x, float y, float z) {
  Vec3d_set(self, x, y, z);
  return self;
}

Vec3d* Vec3d_origin(Vec3d* self) {
  return Vec3d_init(self, 0.0F, 0.0F, 0.0F);
}

Vec3d* Vec3d_copyFrom(Vec3d* self, Vec3d* other) {
  self->x = other->x;
  self->y = other->y;
  self->z = other->z;
  return self;
}

float Vec3d_distanceTo(Vec3d* self, Vec3d* other) {
  float xDist, yDist, zDist;
  xDist = self->x - other->x;
  yDist = self->y - other->y;
  zDist = self->z - other->z;
  return sqrtf((xDist * xDist) + (yDist * yDist) + (zDist * zDist));
}

float Vec3d_distanceToSq(Vec3d* self, Vec3d* other) {
  float xDist, yDist, zDist;
  xDist = self->x - other->x;
  yDist = self->y - other->y;
  zDist = self->z - other->z;
  return (xDist * xDist) + (yDist * yDist) + (zDist * zDist);
}

Vec3d* Vec3d_normalise(Vec3d* self) {
  float magnitude;
  if (self->x == 0.0F && self->y == 0.0F && self->z == 0.0F) {
    return self;
  }

  magnitude = sqrtf(self->x * self->x + self->y * self->y + self->z * self->z);
  self->x /= magnitude;
  self->y /= magnitude;
  self->z /= magnitude;
  return self;
}

float Vec3d_magSq(Vec3d* self) {
  return self->x * self->x + self->y * self->y + self->z * self->z;
}

float Vec3d_mag(Vec3d* self) {
  return sqrtf(self->x * self->x + self->y * self->y + self->z * self->z);
}

Vec3d* Vec3d_directionTo(Vec3d* self, Vec3d* other, Vec3d* result) {
  Vec3d_copyFrom(result, other);
  Vec3d_sub(result, self);
  Vec3d_normalise(result);
  return result;
}

Vec3d* Vec3d_add(Vec3d* self, Vec3d* other) {
  self->x += other->x;
  self->y += other->y;
  self->z += other->z;
  return self;
}

Vec3d* Vec3d_sub(Vec3d* self, Vec3d* other) {
  self->x -= other->x;
  self->y -= other->y;
  self->z -= other->z;
  return self;
}

Vec3d* Vec3d_mulScalar(Vec3d* self, float scalar) {
  self->x *= scalar;
  self->y *= scalar;
  self->z *= scalar;
  return self;
}

Vec3d* Vec3d_divScalar(Vec3d* self, float scalar) {
  self->x /= scalar;
  self->y /= scalar;
  self->z /= scalar;
  return self;
}

float Vec3d_dot(Vec3d* self, Vec3d* other) {
  return self->x * other->x + self->y * other->y + self->z * other->z;
}

void Vec3d_cross(Vec3d* self, Vec3d* other, Vec3d* result) {
  result->x = self->y * other->z - self->z * other->y;
  result->y = self->z * other->x - self->x * other->z;
  result->z = self->x * other->y - self->y * other->x;
}

Vec3d* Vec3d_lerp(Vec3d* self, Vec3d* v, float alpha) {
  self->x += (v->x - self->x) * alpha;
  self->y += (v->y - self->y) * alpha;
  self->z += (v->z - self->z) * alpha;

  return self;
}

#ifndef __N64__
char* Vec3d_toString(Vec3d* self, char* buffer) {
  sprintf(buffer, "{x:%.3f, y:%.3f, z:%.3f}", self->x, self->y, self->z);
  return buffer;
}

void Vec3d_print(Vec3d* self) {
  printf("{x:%.3f, y:%.3f, z:%.3f}", self->x, self->y, self->z);
}

#ifdef __cplusplus
#include <string>
std::string Vec3d_toStdString(Vec3d* self) {
  char buffer[100];
  sprintf(buffer, "{x:%.3f, y:%.3f, z:%.3f}", self->x, self->y, self->z);
  return buffer;
}
#endif

#endif
