
#include "animation.h"

void AnimationState_init(AnimationState* self) {
  self->state = 0;
  self->progress = 0.0f;
}

int AnimationState_getAnimFrame(AnimationState* self,
                                AnimationRange* animRange) {
  int animDuration;
  animDuration = animRange->end - animRange->start;
  return self->progress * animDuration + animRange->start;
}
