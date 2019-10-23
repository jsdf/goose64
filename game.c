#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
// game
#include "character.h"
#include "game.h"
#include "gameobject.h"
#include "gameutils.h"
#include "player.h"
#include "vec3d.h"

#include "constants.h"

Game game;

#define NUM_CHARACTERS 1
Character characters[NUM_CHARACTERS];

void Game_init(GameObject* worldObjects, int worldObjectsCount) {
  GameObject* goose;
  game.worldObjects = worldObjects;
  game.worldObjectsCount = worldObjectsCount;

  // camera
  Vec3d_init(&game.viewPos, 0.0F, 0.0F, -400.0F);
  Vec3d_init(&game.viewRot, 0.0F, 0.0F, 0.0F);
  game.freeView = 0;

  goose = Game_findObjectByType(GooseModel);
  assert(goose != NULL);

  Player_init(&game.player, goose);

  // look at goose
  Vec3d_copyFrom(&game.viewTarget, &game.player.goose->position);

  Character_init(&characters[0],
                 Game_findObjectByType(GroundskeeperCharacterModel), &game);
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

void Game_updateCamera(Game* game) {
  Vec3d cameraOffset;
  Vec3d_set(&cameraOffset, 0.0F, 650.0F, -800.0F);

  Vec3d_copyFrom(&game->viewPos, &game->player.goose->position);
  Vec3d_add(&game->viewPos, &cameraOffset);

  // look at goose
  Vec3d_copyFrom(&game->viewTarget, &game->player.goose->position);
}

void Game_update(Input* input) {
  Game* game;
  int i;

  game = Game_get();

  for (i = 0; i < NUM_CHARACTERS; ++i) {
    Character_update(&characters[i], game);
  }

  Player_update(&game->player, input);

  Game_updateCamera(game);

  // reset inputs
  Input_init(input);
}
