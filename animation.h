
#ifndef ANIMATION_H
#define ANIMATION_H

#include "vec3d.h"

typedef struct AnimationFrame {
  int frame;
  int object;
  Vec3d position;
  Vec3d rotation;

} AnimationFrame;

typedef struct AnimationRange {
  int start;
  int end;
} AnimationRange;

typedef struct AnimationState {
  int state;
  float progress;
} AnimationState;

typedef struct AnimationInterpolation {
  int currentFrame;
  int nextFrame;
  float t;
} AnimationInterpolation;

void AnimationState_init(AnimationState* self);

void AnimationInterpolation_calc(AnimationInterpolation* self,
                                 AnimationState* state,
                                 AnimationRange* animRange);

void AnimationFrame_lerp(AnimationFrame* self,
                         AnimationFrame* a,
                         AnimationFrame* b,
                         AnimationInterpolation* interp);
#endif /* !ANIMATION_H */
