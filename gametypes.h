
#ifndef _GAMETYPES_H_
#define _GAMETYPES_H_

#include "characterstate.h"
#include "gameobject.h"

typedef enum ItemHolderType {
  PlayerItemHolder,
  CharacterItemHolder,
} ItemHolderType;

struct ItemStruct;

// composable struct for players and characters which hold things
typedef struct ItemHolder {
  ItemHolderType itemHolderType;
  void* owner;

  unsigned int acquiredTick;
  struct ItemStruct* heldItem;
} ItemHolder;

typedef struct Player {
  ItemHolder itemHolder;

  GameObject* goose;
  unsigned int lastPickupTick;
} Player;

typedef struct Character {
  ItemHolder itemHolder;

  GameObject* obj;

  struct ItemStruct* target;
  struct ItemStruct* defaultActivityItem;
  Vec3d defaultActivityLocation;
  CharacterState state;

  unsigned int enteredStateTick;
  unsigned int startedActivityTick;
} Character;

typedef struct ItemStruct {
  GameObject* obj;
  ItemHolder* holder;
  unsigned int lastPickedUpTick;
  Vec3d initialLocation;
} Item;

typedef struct Game {
  unsigned int tick;  // this will overflow if you run the game for 829 days :)
  int paused;
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
