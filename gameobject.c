
#include "gameobject.h"
#include <math.h>
#include <stdlib.h>
#include "vec3d.h"

GameObject* GameObject_alloc() {
  GameObject* obj = (GameObject*)malloc(sizeof(GameObject));
  return obj;
}

GameObject* GameObject_init(GameObject* self, int id, Vec3d* initPos) {
  self->id = id;
  Vec3d_set(&self->position, 0, 0, 0);
  self->rotationY = 0.0F;
  self->modelType = NoneModel;

  if (initPos) {
    Vec3d_copyFrom(&self->position, initPos);
  }
  return self;
}

#ifndef __N64__
#include <stdio.h>
void GameObject_print(GameObject* self) {
  printf("%s pos=", ModelTypeStrings[self->modelType]);
  Vec3d_print(&self->position);
}
#endif
