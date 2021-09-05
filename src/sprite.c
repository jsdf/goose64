
#include "sprite.h"
#include "constants.h"

int Sprite_frameCycle(int numFrameAssets,
                      int frameDurationTicks,
                      int currentTick) {
  return (currentTick / frameDurationTicks) % numFrameAssets;
}
