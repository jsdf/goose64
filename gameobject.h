
#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "animation.h"
#include "modeltype.h"
#include "vec3d.h"

typedef struct GameObject {
  int id;
  Vec3d position;
  Vec3d rotation;
  ModelType modelType;
  AnimationState* animState;
} GameObject;

GameObject* GameObject_alloc();
GameObject* GameObject_init(GameObject* self, int id, Vec3d* initPos);

#endif /* !GAMEOBJECT_H */
