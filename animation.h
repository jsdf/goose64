
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

void AnimationState_init(AnimationState* self);

int AnimationState_getAnimFrame(AnimationState* self,
                                AnimationRange* animRange);

#endif /* !ANIMATION_H */
