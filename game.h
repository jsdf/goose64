
#ifndef _GAME_H_
#define _GAME_H_

#include "gameobject.h"
#include "input.h"

#ifdef __N64__
#define GOOSE_SPEED 5.0F
#else
#define GOOSE_SPEED 20.0F
#endif

#define GOOSE_MAX_TURN_SPEED 15.0f

typedef struct Player {
  GameObject* goose;
} Player;

typedef struct Game {
  Vec3d viewPos;
  Vec3d viewRot;
  Vec3d viewTarget;
  int freeView;
  GameObject* worldObjects;
  int worldObjectsCount;

  Player player;
} Game;

void Game_init(GameObject* worldObjects, int worldObjectsCount);
Game* Game_get();

GameObject* Game_findObjectByType(ModelType modelType);

void Game_update(Input* input);

#endif /* !_GAME_H_ */
