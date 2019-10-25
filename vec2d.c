
#include <math.h>
#ifndef __N64__
#include <stdio.h>
#endif
#include <stdlib.h>

#include "vec2d.h"

#ifdef __N64__
#include <gu.h>
#endif

#define VEC2D_M_PI 3.14159265358979323846

float _Vec2dutil_lerp(float v0, float v1, float t) {
  return v0 * (1 - t) + v1 * t;
}

Vec2d* Vec2d_alloc() {
  Vec2d* created;
  created = (Vec2d*)malloc(sizeof(Vec2d));
  return created;
}

Vec2d* Vec2d_init(Vec2d* self, float x, float y) {
  self->x = x;
  self->y = y;
  return self;
}

Vec2d* Vec2d_identity(Vec2d* self) {
  return Vec2d_init(self, 0.0F, 0.0F);
}

Vec2d* Vec2d_clone(Vec2d* v) {
  Vec2d* copy;
  copy = Vec2d_alloc();
  copy->x = v->x;
  copy->y = v->y;
  return copy;
}

Vec2d* Vec2d_copyFrom(Vec2d* self, Vec2d* other) {
  self->x = other->x;
  self->y = other->y;
  return self;
}

Vec2d* Vec2d_add(Vec2d* self, Vec2d* other) {
  self->x += other->x;
  self->y += other->y;
  return self;
}

Vec2d* Vec2d_sub(Vec2d* self, Vec2d* other) {
  self->x -= other->x;
  self->y -= other->y;
  return self;
}

Vec2d* Vec2d_normalise(Vec2d* self) {
  float magnitude;
  if (self->x == 0.0F && self->y == 0.0F) {
    return self;
  }

  magnitude = sqrtf(self->x * self->x + self->y * self->y);
  self->x /= magnitude;
  self->y /= magnitude;
  return self;
}

Vec2d* Vec2d_addScalar(Vec2d* self, float scalar) {
  self->x += scalar;
  self->y += scalar;
  return self;
}

Vec2d* Vec2d_subScalar(Vec2d* self, float scalar) {
  self->x -= scalar;
  self->y -= scalar;
  return self;
}

Vec2d* Vec2d_multiplyScalar(Vec2d* self, float scalar) {
  self->x *= scalar;
  self->y *= scalar;
  return self;
}

Vec2d* Vec2d_divideScalar(Vec2d* self, float scalar) {
  self->x /= scalar;
  self->y /= scalar;
  return self;
}

float Vec2d_distanceTo(Vec2d* self, Vec2d* other) {
  float xDist, yDist;
  xDist = self->x - other->x;
  yDist = self->y - other->y;
  return sqrtf((xDist * xDist) + (yDist * yDist));
}

Vec2d* Vec2d_directionTo(Vec2d* self, Vec2d* other, Vec2d* result) {
  Vec2d_copyFrom(result, other);
  Vec2d_sub(result, self);
  Vec2d_normalise(result);
  return result;
}

Vec2d* Vec2d_lerp(Vec2d* self, Vec2d* v0, Vec2d* v1, float t) {
  self->x = _Vec2dutil_lerp(v0->x, v1->x, t);
  self->y = _Vec2dutil_lerp(v0->y, v1->y, t);
  return self;
}

float Vec2d_angle(Vec2d* self) {
  float angle;
  angle = atan2f(self->y, self->x);

  if (angle < 0.0F) {
    angle += 2.0F * VEC2D_M_PI;
  }

  return angle;
}

int Vec2d_equals(Vec2d* self, Vec2d* other) {
  return self->x == other->x && self->y == other->y;
}

float Vec2d_lengthSquared(Vec2d* self) {
  return self->x * self->x + self->y * self->y;
}

float Vec2d_length(Vec2d* self) {
  return sqrtf(self->x * self->x + self->y * self->y);
}

#ifndef __N64__
char* Vec2d_toString(Vec2d* self, char* buffer) {
  sprintf(buffer, "{x:%f, y:%f}", self->x, self->y);
  return buffer;
}

void Vec2d_print(Vec2d* self) {
  printf("{x:%f, y:%f}", self->x, self->y);
}
#endif
