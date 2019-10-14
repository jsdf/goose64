
#ifndef _VEC2D_H_
#define _VEC2D_H_

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct Vec2d {
  float x;
  float y;
} Vec2d;

#define VEC2D_M_PI    3.14159265358979323846

float _Vec2dutil_lerp(float v0, float v1, float t) {
  return v0 * (1 - t) + v1 * t;
}

Vec2d* Vec2d_alloc() {
  Vec2d* created = (Vec2d*) malloc(sizeof(Vec2d));
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
  Vec2d* copy = Vec2d_alloc();
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
  if (self->x == 0.0F && self->y == 0.0F) return self;

  float magnitude = sqrtf(self->x * self->x + self->y * self->y);
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
  return sqrtf(powf(self->x - other->x, 2.0F) + powf(self->y - other->y, 2.0F));
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

float angle(Vec2d* self) {
  float angle = atan2f(self->y, self->x);

  if (angle < 0.0F) angle += 2.0F * VEC2D_M_PI;

  return angle;
}

char* toString(Vec2d* self, char* buffer) {
  sprintf(buffer, "{x:%f, y:%f}", self->x, self->y);
  return buffer;
}

bool equals(Vec2d* self, Vec2d* other) {
  return self->x == other->x && self->y == other->y;
}

#endif /* !_VEC2D_H_ */