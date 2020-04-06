#include "sprite.h"

#include "honk1spr.h"
#include "honk2spr.h"
#include "honk3spr.h"
#include "testspr.h"

unsigned short* HonkSpriteAnimFrames[] = {
    Sprite_honk1spr,
    Sprite_honk2spr,
    Sprite_honk3spr,
};

unsigned short* getSpriteForSpriteType(SpriteType spriteType, int frame) {
  switch (spriteType) {
    case HonkSprite:
      return HonkSpriteAnimFrames[frame];
    default:
      return Sprite_testspr;
  }
}
