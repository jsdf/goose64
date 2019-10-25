
#ifndef _GAMETYPES_H_
#define _GAMETYPES_H_

#include "characterstate.h"
#include "gameobject.h"

typedef struct Item {
  GameObject* obj;
  Vec3d initialLocation;
} Item;

typedef struct Player {
  GameObject* goose;
  Item* heldItem;
  unsigned int lastPickupTick;
} Player;

typedef struct Character {
  GameObject* obj;
  GameObject* target;
  Item* heldItem;
  Item* defaultActivityItem;
  CharacterState state;

  unsigned int enteredStateTick;
  unsigned int lastPickupTick;
} Character;

typedef struct Game {
  unsigned int tick;  // this will overflow if you run the game for 829 days :)
  Vec3d viewPos;
  Vec3d viewRot;
  Vec3d viewTarget;
  int freeView;
  GameObject* worldObjects;
  int worldObjectsCount;
  Item* items;
  int itemsCount;

  Player player;
} Game;

#endif /* !_GAMETYPES_H_ */
