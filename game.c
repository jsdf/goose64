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
  GameObject* goose;
  goose = game->player.goose;

  Vec3d_init(&playerMovement, input->direction.x * GOOSE_SPEED, 0.0F,
             input->direction.y * GOOSE_SPEED);
  // player movement
  Vec3d_add(&goose->position, &playerMovement);
  if (Vec2d_lengthSquared(&input->direction) > 0) {
    goose->rotationZ = 360.0F - radToDeg(Vec2d_angle(&input->direction));
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
