
#ifndef SPRITE_H
#define SPRITE_H

typedef enum SpriteType {
  NoneSprite,      //
  HonkSprite,      //
  MAX_SPRITE_TYPE  //
} SpriteType;

#define HONK_SPRITE_FRAMES 3

extern unsigned short* getSpriteForSpriteType(SpriteType spriteType, int frame);
int Sprite_frameCycle(int numFrameAssets,
                      int frameDurationTicks,
                      int currentTick);

#endif /* SPRITE_H */
