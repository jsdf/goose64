#include <math.h>

#include "character.h"
#include "modeltype.h"
#include "vec2d.h"
#include "vec3d.h"

#include "gameutils.h"

#include "constants.h"

void Item_init(Item* self, GameObject* obj, Game* game) {
  self->obj = obj;
  self->initialLocation = obj->position;
}

#ifndef __N64__
#include <stdio.h>

void Item_print(Item* self) {
  printf("Item type=%s pos=", ModelTypeStrings[self->obj->modelType]);
  Vec3d_print(&self->obj->position);
}

#endif
