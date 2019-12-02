
#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "animation.h"
#include "modeltype.h"
#include "n64compat.h"
#include "rotation.h"
#include "vec3d.h"

typedef struct GameObject {
  int id;
  Vec3d position;
  EulerDegrees rotation;
  ModelType modelType;
  AnimationState* animState;
  Mtx objTransform;
} GameObject;

GameObject* GameObject_alloc();
GameObject* GameObject_init(GameObject* self, int id, Vec3d* initPos);

#endif /* !GAMEOBJECT_H */
