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

  Vec3d_init(&game.viewPos, 0.0F, 0.0F, -400.0F);
  Vec3d_init(&game.viewRot, 0.0F, 0.0F, 0.0F);

  game.player.goose = Game_findObjectByType(GooseModel);
  assert(game.player.goose != NULL);
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

void Game_update() {
  int i;
  Game* game;
  GameObject* obj;

  game = Game_get();
  obj = game->worldObjects;
  for (i = 0; i < game->worldObjectsCount; i++) {
    // rotate goose
    if (obj->modelType == GooseModel) {
      obj->rotationZ = _Game_util_fwrap(obj->rotationZ + 2.0F, 0.0F, 360.0F);
    }

    obj++;
  }
}
