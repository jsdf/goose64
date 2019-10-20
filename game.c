
#include "game.h"
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
}

Game* Game_get() {
  return &game;
}

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
