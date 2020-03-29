
#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "animation.h"
#include "modeltype.h"
#include "n64compat.h"
#include "physics.h"
#include "rotation.h"
#include "vec3d.h"

typedef struct GameObject {
  int id;
  Vec3d position;
  EulerDegrees rotation;
  ModelType modelType;
  int subtype;
  AnimationState* animState;
  PhysBody* physBody;
  int visible;
  int solid;
} GameObject;

GameObject* GameObject_alloc();
GameObject* GameObject_init(GameObject* self, int id, Vec3d* initPos);

#endif /* !GAMEOBJECT_H */
