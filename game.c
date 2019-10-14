
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "gameobject.h"
#include "vec3d.h"
#include "game.h"

#define MAX_WORLD_OBJECTS 100

GameObject worldObjects[MAX_WORLD_OBJECTS];
Game game;

void Game_init() {
  int i;
  for (i = 0; i < MAX_WORLD_OBJECTS; ++i)
  {
    GameObject_init(&worldObjects[i], NULL);
  }
  game.worldObjects = worldObjects;
  game.worldObjectsCount  = MAX_WORLD_OBJECTS;
}

Game* Game_get() {
    return &game;
}
