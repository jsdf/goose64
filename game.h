
#ifndef _GAME_H_
#define _GAME_H_

#include "gameobject.h"

typedef struct Game {
  GameObject* worldObjects;
  int worldObjectsCount;
} Game;

void Game_init();
Game* Game_get();


#endif /* !_GAME_H_ */
