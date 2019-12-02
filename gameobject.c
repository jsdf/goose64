
#include <math.h>
#include <stdlib.h>

#include "constants.h"
#include "gameobject.h"
#include "rotation.h"
#include "vec3d.h"

GameObject* GameObject_alloc() {
  GameObject* obj = (GameObject*)malloc(sizeof(GameObject));
  return obj;
}

GameObject* GameObject_init(GameObject* self, int id, Vec3d* initPos) {
  self->id = id;
  Vec3d_origin(&self->position);
  EulerDegrees_origin(&self->rotation);
  self->modelType = NoneModel;
  self->animState = NULL;
  self->visible = TRUE;
  self->solid = TRUE;

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
