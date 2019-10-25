#include "input.h"
#include "constants.h"
#include "vec2d.h"

void Input_init(Input* self) {
  Vec2d_identity(&self->direction);
  self->run = FALSE;
  self->pickup = FALSE;
}
