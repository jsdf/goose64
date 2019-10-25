
#ifndef _GAME_H_
#define _GAME_H_

#include "gameobject.h"
#include "gametypes.h"
#include "input.h"

void Game_init(GameObject* worldObjects, int worldObjectsCount);
Game* Game_get();

GameObject* Game_findObjectByType(ModelType modelType);

void Game_update(Input* input);

#endif /* !_GAME_H_ */
