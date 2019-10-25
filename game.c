#include <assert.h>
#include <math.h>
#ifndef __N64__
#include <stdio.h>
#endif
#include <stdlib.h>
// game
#include "character.h"
#include "game.h"
#include "gameobject.h"
#include "gameutils.h"
#include "item.h"
#include "player.h"
#include "vec3d.h"

#include "constants.h"

Game game;

#define NUM_CHARACTERS 1
Character characters[NUM_CHARACTERS];
#define NUM_ITEMS 2
Item items[NUM_ITEMS];

void Game_init(GameObject* worldObjects, int worldObjectsCount) {
  GameObject* goose;

  game.paused = FALSE;
  game.worldObjects = worldObjects;
  game.worldObjectsCount = worldObjectsCount;

  // camera
  Vec3d_init(&game.viewPos, 0.0F, 0.0F, -400.0F);
  Vec3d_init(&game.viewRot, 0.0F, 0.0F, 0.0F);
  game.freeView = 0;

  goose = Game_findObjectByType(GooseModel);
  assert(goose != NULL);

  Player_init(&game.player, goose);

  // setup camera
  Vec3d_copyFrom(&game.viewTarget, &game.player.goose->position);

  Item_init(&items[0], Game_findObjectByType(BookItemModel), &game);
  Item_init(&items[1], Game_findObjectByType(HomeworkItemModel), &game);
  Character_init(&characters[0],
                 Game_findObjectByType(GroundskeeperCharacterModel),
                 /*book*/ &items[0], &game);

  game.items = items;
  game.itemsCount = NUM_ITEMS;
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

  if (!game->paused) {
    game->tick++;

    for (i = 0; i < NUM_CHARACTERS; ++i) {
      Character_update(&characters[i], game);
    }

    Player_update(&game->player, input, game);

    Game_updateCamera(game);
  }

  // reset inputs
  Input_init(input);
}
