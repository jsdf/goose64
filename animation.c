#include <math.h>

#include "animation.h"
#include "constants.h"
#include "rotation.h"

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

// gets a non-interpolated AnimationFrame for one bone in a rig
void AnimationFrame_get(
    AnimationInterpolation* interp,  // result of AnimationInterpolation_calc()
    AnimationFrame* animData,        // pointer to start of AnimationFrame list
                                     // exported for some rig
    int animDataNumBones,            // num bones in rig used by animData
    int boneIdx,            // index of bone in rig to produce transform for
    AnimationFrame* result  // the resultant animation frame
) {
  int frameDataOffset;

  frameDataOffset = interp->currentFrame * animDataNumBones + boneIdx;
  *result = *(animData + frameDataOffset);
}

// produces an interpolated AnimationFrame for one bone in a rig
void AnimationFrame_lerp(
    AnimationInterpolation* interp,  // result of AnimationInterpolation_calc()
    AnimationFrame* animData,        // pointer to start of AnimationFrame list
                                     // exported for some rig
    int animDataNumBones,            // num bones in rig used by animData
    int boneIdx,            // index of bone in rig to produce transform for
    AnimationFrame* result  // the resultant interpolated animation frame
) {
  Quaternion quaternionA, quaternionB;
  Euler radiansA, radiansB, radiansResult;

  int frameDataOffsetA, frameDataOffsetB;
  AnimationFrame *a, *b;

  frameDataOffsetA = interp->currentFrame * animDataNumBones + boneIdx;
  frameDataOffsetB = interp->nextFrame * animDataNumBones + boneIdx;
  a = animData + frameDataOffsetA;
  b = animData + frameDataOffsetB;

  *result = *a;

  Vec3d_lerp(&result->position, &b->position, interp->t);

  Euler_fromEulerDegrees(&radiansA, &a->rotation);
  Euler_fromEulerDegrees(&radiansB, &b->rotation);

  Quaternion_fromEuler(&quaternionA, &radiansA);
  Quaternion_fromEuler(&quaternionB, &radiansB);

  Quaternion_slerp(&quaternionA, &quaternionB, interp->t);

  Euler_setFromQuaternion(&radiansResult, &quaternionA);
  EulerDegrees_fromEuler(&result->rotation, &radiansResult);
}
