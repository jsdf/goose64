
#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_

#include "vec3d.h"
#include "modeltype.h"

typedef struct GameObject {
  Vec3d position;
  float rotationZ;
  ModelType modelType;
} GameObject;

GameObject* GameObject_alloc();
GameObject* GameObject_init(GameObject* self, Vec3d* initPos);

#endif /* !_GAMEOBJECT_H_ */