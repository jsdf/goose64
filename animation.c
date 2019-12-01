#include <math.h>

#include "animation.h"
#include "constants.h"

void AnimationState_init(AnimationState* self) {
  self->state = 0;
  self->progress = 0.0f;
}

void AnimationInterpolation_calc(AnimationInterpolation* self,
                                 AnimationState* state,
                                 AnimationRange* animRange) {
  int animDuration, currentFrame;
  // frames since range start, with intra-frame decimal part
  float integralFrameRel;
  animDuration = animRange->end - animRange->start;
  integralFrameRel = state->progress * animDuration;
  currentFrame = animRange->start + (int)floorf(integralFrameRel);
  self->currentFrame = currentFrame;
  // calculate next frame, wrapping around to range start offset
  self->nextFrame =
      animRange->start + (((int)ceilf(integralFrameRel)) % animDuration);
  self->t = fmodf(integralFrameRel, 1.0);
}

void AnimationFrame_lerp(AnimationFrame* self,
                         AnimationFrame* a,
                         AnimationFrame* b,
                         AnimationInterpolation* interp) {
  *self = *a;

  Vec3d_lerp(&self->position, &b->position, interp->t);
}
