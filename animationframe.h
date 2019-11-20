
#ifndef ANIMATION_FRAME_H
#define ANIMATION_FRAME_H

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

#endif /* !ANIMATION_FRAME_H */
