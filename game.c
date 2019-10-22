#include "game.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "gameobject.h"
#include "vec3d.h"

#include "constants.h"

Game game;

void Game_init(GameObject* worldObjects, int worldObjectsCount) {
  game.worldObjects = worldObjects;
  game.worldObjectsCount = worldObjectsCount;

  // camera
  Vec3d_init(&game.viewPos, 0.0F, 0.0F, -400.0F);
  Vec3d_init(&game.viewRot, 0.0F, 0.0F, 0.0F);
  game.freeView = 0;

  game.player.goose = Game_findObjectByType(GooseModel);
  assert(game.player.goose != NULL);

  // look at goose
  Vec3d_copyFrom(&game.viewTarget, &game.player.goose->position);
}

Game* Game_get() {
  return &game;
}

// finds the first object with a particular modeltype
GameObject* Game_findObjectByType(ModelType modelType) {
  int i;
  GameObject* obj;

  for (i = 0, obj = game.worldObjects; i < game.worldObjectsCount; i++, obj++) {
    if (obj->modelType == modelType) {
      return obj;
    }
  }
  return NULL;
}

float _Game_util_lerpDegrees(float start, float end, float amount) {
  float shortestAngle;
  shortestAngle = fmodf((fmodf((end - start), 360) + 540), 360) - 180;
  return start + fmodf((shortestAngle * amount), 360);
}

float _Game_util_fclamp(float x, float lower, float upper) {
  return x > upper ? upper : x < lower ? lower : x;
}

float _Game_util_rotateTowardsClamped(float from,
                                      float to,
                                      float maxSpeed  // must be positive
) {
  float shortestAngle;
  shortestAngle = fmodf((fmodf((to - from), 360.0F) + 540.0F), 360.0F) - 180.0F;
  return from +
         fmodf(_Game_util_fclamp(shortestAngle, -maxSpeed, maxSpeed), 360.0F);
}

// this is a stupid function
float _Game_util_fwrap(float x, float lower, float upper) {
  if (x > upper) {
    return lower;
  } else if (x < lower) {
    return upper;
  }
  return x;
}

void Game_updateCamera(Game* game) {
  Vec3d cameraOffset;
  Vec3d_set(&cameraOffset, 0.0F, 650.0F, -800.0F);

  Vec3d_copyFrom(&game->viewPos, &game->player.goose->position);
  Vec3d_add(&game->viewPos, &cameraOffset);

  // look at goose
  Vec3d_copyFrom(&game->viewTarget, &game->player.goose->position);
}

void Game_updatePlayer(Game* game, Input* input) {
  Vec3d playerMovement;
  float destAngle;
  GameObject* goose;
  goose = game->player.goose;

  Vec3d_init(&playerMovement, input->direction.x * GOOSE_SPEED, 0.0F,
             input->direction.y * GOOSE_SPEED);
  // player movement
  Vec3d_add(&goose->position, &playerMovement);

  // rot
  if (Vec2d_lengthSquared(&input->direction) > 0) {
    destAngle = 360.0F - radToDeg(Vec2d_angle(&input->direction));
    // rotate towards dest, but with a speed limit
    goose->rotationZ = _Game_util_rotateTowardsClamped(
        goose->rotationZ, destAngle, GOOSE_MAX_TURN_SPEED);
  }
}

void Game_update(Input* input) {
  Game* game;

  game = Game_get();

  Game_updatePlayer(game, input);

  Game_updateCamera(game);

  // reset inputs
  Input_init(input);
}
