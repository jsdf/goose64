
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "vec3d.h"
#include "gameobject.h"

GameObject* GameObject_alloc() {
  GameObject* obj = (GameObject*) malloc(sizeof(GameObject));
  return obj;
}

GameObject* GameObject_init(GameObject* self, Vec3d* initPos) {
  self->position.x = 0.0F;
  self->position.y = 0.0F;
  self->position.z = 0.0F;
  self->rotationZ = 0.0F;
  self->modelType = Goose;

  if (initPos) {
    Vec3d_copyFrom(&self->position, initPos);
  }
  return self;
}


