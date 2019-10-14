
#ifndef _VEC3D_H_
#define _VEC3D_H_

typedef struct Vec3d {
  float x;
  float y;
  float z;
} Vec3d;

Vec3d* Vec3d_alloc(float x, float y, float z) ;

Vec3d* Vec3d_init(Vec3d* self, float x, float y, float z);

Vec3d* Vec3d_identity(Vec3d* self) ;

Vec3d* Vec3d_copyFrom(Vec3d* self, Vec3d* other) ;

#endif /* !_VEC3D_H_ */