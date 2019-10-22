
#ifndef VEC2D_H
#define VEC2D_H

typedef struct Vec2d {
  float x;
  float y;
} Vec2d;

#define VEC2D_M_PI 3.14159265358979323846

float _Vec2dutil_lerp(float v0, float v1, float t);

Vec2d* Vec2d_alloc();
Vec2d* Vec2d_init(Vec2d* self, float x, float y);
Vec2d* Vec2d_identity(Vec2d* self);
Vec2d* Vec2d_clone(Vec2d* v);
Vec2d* Vec2d_copyFrom(Vec2d* self, Vec2d* other);
Vec2d* Vec2d_add(Vec2d* self, Vec2d* other);
Vec2d* Vec2d_sub(Vec2d* self, Vec2d* other);
Vec2d* Vec2d_normalise(Vec2d* self);

Vec2d* Vec2d_addScalar(Vec2d* self, float scalar);

Vec2d* Vec2d_subScalar(Vec2d* self, float scalar);
Vec2d* Vec2d_multiplyScalar(Vec2d* self, float scalar);
Vec2d* Vec2d_divideScalar(Vec2d* self, float scalar);

float Vec2d_distanceTo(Vec2d* self, Vec2d* other);
Vec2d* Vec2d_directionTo(Vec2d* self, Vec2d* other, Vec2d* result);
Vec2d* Vec2d_lerp(Vec2d* self, Vec2d* v0, Vec2d* v1, float t);
float Vec2d_angle(Vec2d* self);
int Vec2d_equals(Vec2d* self, Vec2d* other);
float Vec2d_lengthSquared(Vec2d* self);
float Vec2d_length(Vec2d* self);

#ifndef __N64__
char* Vec2d_toString(Vec2d* self, char* buffer);
void Vec2d_print(Vec2d* self);
#endif

#endif /* !VEC2D_H */
