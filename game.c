
#include "game.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "gameobject.h"
#include "vec3d.h"

#include "constants.h"

GameObject worldObjects[MAX_WORLD_OBJECTS];
Game game;

void Game_init() {
  int i;
  for (i = 0; i < MAX_WORLD_OBJECTS; ++i) {
    GameObject_init(&worldObjects[i], NULL);
  }
  game.worldObjects = worldObjects;
  game.worldObjectsCount = MAX_WORLD_OBJECTS;
}

Game* Game_get() {
  return &game;
}
