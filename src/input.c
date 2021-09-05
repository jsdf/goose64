#include "input.h"
#include "constants.h"
#include "vec2d.h"

void Input_init(Input* self) {
  Vec2d_origin(&self->direction);
  self->run = FALSE;
  self->pickup = FALSE;
  self->zoomIn = FALSE;
  self->zoomOut = FALSE;
}
