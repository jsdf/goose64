
#ifndef VEC3D_H
#define VEC3D_H

typedef struct Vec3d {
  float x;
  float y;
  float z;
} Vec3d;

Vec3d* Vec3d_alloc(float x, float y, float z);
void Vec3d_set(Vec3d* self, float x, float y, float z);
Vec3d* Vec3d_init(Vec3d* self, float x, float y, float z);
Vec3d* Vec3d_identity(Vec3d* self);
Vec3d* Vec3d_copyFrom(Vec3d* self, Vec3d* other);
float Vec3d_distanceTo(Vec3d* self, Vec3d* other);

Vec3d* Vec3d_add(Vec3d* self, Vec3d* other);
Vec3d* Vec3d_sub(Vec3d* self, Vec3d* other);

#ifndef __N64__
char* Vec3d_toString(Vec3d* self, char* buffer);
void Vec3d_print(Vec3d* self);
#endif

#endif /* !VEC3D_H */
