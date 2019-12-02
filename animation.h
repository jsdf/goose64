
#ifndef ANIMATION_H
#define ANIMATION_H

#include "rotation.h"
#include "vec3d.h"

typedef struct AnimationFrame {
  int frame;
  int object;
  Vec3d position;
  EulerDegrees rotation;

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

void AnimationFrame_get(
    AnimationInterpolation* interp,  // result of AnimationInterpolation_calc()
    AnimationFrame* animData,        // pointer to start of AnimationFrame list
                                     // exported for some rig
    int animDataNumBones,            // num bones in rig used by animData
    int boneIdx,            // index of bone in rig to produce transform for
    AnimationFrame* result  // the resultant   animation frame
);

void AnimationFrame_lerp(
    AnimationInterpolation* interp,  // result of AnimationInterpolation_calc()
    AnimationFrame* animData,        // pointer to start of AnimationFrame list
                                     // exported for some rig
    int animDataNumBones,            // num bones in rig used by animData
    int boneIdx,            // index of bone in rig to produce transform for
    AnimationFrame* result  // the resultant interpolated animation frame
);

#endif /* !ANIMATION_H */
