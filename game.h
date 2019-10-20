
#ifndef _GAME_H_
#define _GAME_H_

#include "gameobject.h"

typedef struct Player {
  GameObject* goose;
} Player;

typedef struct Game {
  Vec3d viewPos;
  Vec3d viewRot;
  GameObject* worldObjects;
  int worldObjectsCount;

  Player player;
} Game;

void Game_init(GameObject* worldObjects, int worldObjectsCount);
Game* Game_get();

GameObject* Game_findObjectByType(ModelType modelType);

void Game_update();

#endif /* !_GAME_H_ */
