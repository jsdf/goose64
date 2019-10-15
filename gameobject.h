
#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "modeltype.h"
#include "vec3d.h"

typedef struct GameObject {
  Vec3d position;
  float rotationZ;
  ModelType modelType;
} GameObject;

GameObject* GameObject_alloc();
GameObject* GameObject_init(GameObject* self, Vec3d* initPos);

#endif /* !GAMEOBJECT_H */
