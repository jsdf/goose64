
#ifndef _GAMETYPES_H_
#define _GAMETYPES_H_

#include "characterstate.h"
#include "gameobject.h"

typedef struct Player {
  GameObject* goose;
  GameObject* heldObj;
} Player;

typedef struct Character {
  GameObject* obj;
  GameObject* target;
  CharacterState state;
} Character;

typedef struct Game {
  Vec3d viewPos;
  Vec3d viewRot;
  Vec3d viewTarget;
  int freeView;
  GameObject* worldObjects;
  int worldObjectsCount;

  Player player;
} Game;

#endif /* !_GAMETYPES_H_ */
