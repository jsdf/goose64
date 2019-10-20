
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

Vec3d* Vec3d_identity(Vec3d* self) {
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

char* Vec3d_toString(Vec3d* self, char* buffer) {
  sprintf(buffer, "{x:%.3f, y:%.3f, z:%.3f}", self->x, self->y, self->z);
  return buffer;
}

void Vec3d_print(Vec3d* self) {
  printf("{x:%.3f, y:%.3f, z:%.3f}", self->x, self->y, self->z);
}
