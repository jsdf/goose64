
#ifndef _COLLISION_H_
#define _COLLISION_H_

#include "vec3d.h"

typedef struct Triangle {
  Vec3d a;
  Vec3d b;
  Vec3d c;
} Triangle;

int Collision_sphereTriangleIsSeparated(Vec3d* AOrig,
                                        Vec3d* BOrig,
                                        Vec3d* COrig,
                                        Vec3d* P,
                                        double r);

#endif /* !_COLLISION_H_ */
