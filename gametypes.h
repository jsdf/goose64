
#ifndef _GAMETYPES_H_
#define _GAMETYPES_H_

#include "animation.h"
#include "characterstate.h"
#include "gameobject.h"
#include "physics.h"

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
  AnimationState animState;

  unsigned int lastPickupTick;
} Player;

typedef struct Character {
  ItemHolder itemHolder;

  GameObject* obj;
  AnimationState animState;

  struct ItemStruct* target;
  struct ItemStruct* defaultActivityItem;
  Vec3d defaultActivityLocation;
  Vec3d targetLocation;
  CharacterTarget targetType;
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

// this is here because of mutual dependency between game methods and objects
// with update methods which take Game arg
typedef struct Game {
  unsigned int tick;  // this will overflow if you run the game for 829 days :)
  int paused;
  Vec3d viewPos;
  Vec3d viewRot;
  Vec3d viewTarget;
  float viewZoom;
  int freeView;
  GameObject* worldObjects;
  int worldObjectsCount;
  Item* items;
  int itemsCount;
  Character* characters;
  int charactersCount;
  PhysBody* physicsBodies;
  int physicsBodiesCount;

  Player player;
  PhysState physicsState;

  // profiling
  float profTimeCharacters;
  float profTimePhysics;
  float profTimeDraw;
  float profTimePath;
} Game;

#endif /* !_GAMETYPES_H_ */
