#include <math.h>
#ifndef __N64__
#include <assert.h>
#endif

#include "animation.h"
#include "constants.h"
#include "rotation.h"
#include "trace.h"

void AnimationState_init(AnimationState* self) {
  self->state = 0;
  self->progress = 0.0f;
  AnimationBoneAttachment_init(&self->attachment);
  AnimationBoneSpriteAttachment_init(&self->spriteAttachment);
}

void AnimationBoneAttachment_init(AnimationBoneAttachment* self) {
  self->boneIndex = 0;
  self->modelType = NoneModel;
  Vec3d_origin(&self->offset);
  EulerDegrees_origin(&self->rotation);
}

void AnimationBoneSpriteAttachment_init(AnimationBoneSpriteAttachment* self) {
  self->boneIndex = 0;
  self->spriteType = NoneSprite;
  Vec3d_origin(&self->offset);
  self->startTick = 0;
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

#ifndef __N64__
  invariant(animDataNumBones <= MAX_ANIM_MESH_PARTS);
#endif

  frameDataOffset = interp->currentFrame * animDataNumBones + boneIdx;
  *result = *(animData + frameDataOffset);

#ifndef __N64__
  invariant(result->object == boneIdx);
#endif
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
  // float profStartAnimLerp;
  AnimationFrame *a, *b;

#ifndef __N64__
  invariant(animDataNumBones <= MAX_ANIM_MESH_PARTS);
#endif

  frameDataOffsetA = interp->currentFrame * animDataNumBones + boneIdx;
  frameDataOffsetB = interp->nextFrame * animDataNumBones + boneIdx;
  a = animData + frameDataOffsetA;
  b = animData + frameDataOffsetB;

#ifndef __N64__
  // if either of these fail, the animation data is messed up
  invariant(a->object == boneIdx);
  invariant(b->object == boneIdx);
#endif

  // start with data from A
  *result = *a;

  Vec3d_lerp(&result->position, &b->position, interp->t);

  Euler_fromEulerDegrees(&radiansA, &a->rotation);
  Euler_fromEulerDegrees(&radiansB, &b->rotation);

  // profStartAnimLerp = CUR_TIME_MS();
  Quaternion_fromEuler(&quaternionA, &radiansA);
  Quaternion_fromEuler(&quaternionB, &radiansB);
  // Trace_addEvent(AnimLerpTraceEvent, profStartAnimLerp, CUR_TIME_MS());

  Quaternion_slerp(&quaternionA, &quaternionB, interp->t);

  Euler_setFromQuaternion(&radiansResult, &quaternionA);
  EulerDegrees_fromEuler(&result->rotation, &radiansResult);
}
