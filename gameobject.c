
#include "gameobject.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "vec3d.h"

GameObject* GameObject_alloc() {
  GameObject* obj = (GameObject*)malloc(sizeof(GameObject));
  return obj;
}

GameObject* GameObject_init(GameObject* self, int id, Vec3d* initPos) {
  self->id = id;
  Vec3d_set(&self->position, 0, 0, 0);
  self->rotationZ = 0.0F;
  self->modelType = NoneModel;

  if (initPos) {
    Vec3d_copyFrom(&self->position, initPos);
  }
  return self;
}
