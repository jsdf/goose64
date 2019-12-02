#include <assert.h>
#include <math.h>

#include "character.h"
#include "game.h"
#include "modeltype.h"
#include "player.h"
#include "vec2d.h"
#include "vec3d.h"

#include "gameutils.h"

#include "constants.h"

#define ITEM_PICKUP_COOLDOWN 60
#define ITEM_STEAL_COOLDOWN 120

#ifndef __N64__
char* ItemHolderTypeStrings[] = {
    "PlayerItemHolder",
    "CharacterItemHolder",
};
#endif

void Item_init(Item* self, GameObject* obj, Game* game) {
  self->obj = obj;
  self->holder = NULL;
  self->lastPickedUpTick = 0;
  self->initialLocation = obj->position;
}

void Item_take(Item* self, ItemHolder* newHolder) {
  ItemHolder* originalHolder;
  originalHolder = NULL;

  if (self->holder
          // ensure player can't pick up or drop this really quickly
          ? Game_get()->tick > self->lastPickedUpTick + ITEM_PICKUP_COOLDOWN
          // ensure that characters can't steal this object back from each other
          // too frequently
          : Game_get()->tick > self->lastPickedUpTick + ITEM_STEAL_COOLDOWN) {
    if (self->holder) {
#ifndef __N64__
      debugPrintf("item taken from %s by %s\n",
                  ItemHolderTypeStrings[self->holder->itemHolderType],
                  ItemHolderTypeStrings[newHolder->itemHolderType]);
#endif
    } else {
#ifndef __N64__
      debugPrintf("item picked up by %s\n",
                  ItemHolderTypeStrings[newHolder->itemHolderType]);
#endif
    }

    originalHolder = self->holder;
    if (originalHolder) {
      assert(originalHolder->heldItem == self);
      // take from original holder
      originalHolder->heldItem = NULL;

      // shitty dynamic dispatch
      switch (originalHolder->itemHolderType) {
        case PlayerItemHolder:
          Player_haveItemTaken((Player*)originalHolder->owner);
          break;
        case CharacterItemHolder:
          Character_haveItemTaken((Character*)originalHolder->owner);
          break;
      }
    }

    assert(newHolder->heldItem == NULL);
    // give to new holder
    newHolder->heldItem = self;
    // be held by new holder
    self->holder = newHolder;
    self->lastPickedUpTick = Game_get()->tick;

    // disable rendering and physics (character will show as attachment instead)
    self->obj->visible = FALSE;
    self->obj->solid = FALSE;
  }
}

void Item_drop(Item* self) {
  assert(self->holder != NULL);

#ifndef __N64__
  debugPrintf("item dropped by %s\n",
              ItemHolderTypeStrings[self->holder->itemHolderType]);
#endif
  // clear back reference
  self->holder->heldItem = NULL;
  // no longer held
  self->holder = NULL;
  // put it back on the ground
  self->obj->position.y = 0;

  // re-enable rendering and physics
  self->obj->visible = TRUE;
  self->obj->solid = TRUE;
}

void ItemHolder_init(ItemHolder* self,
                     ItemHolderType itemHolderType,
                     void* owner) {
  self->itemHolderType = itemHolderType;
  self->owner = owner;
  self->heldItem = NULL;
  self->acquiredTick = 0;
}

#ifndef __N64__
#include <stdio.h>

void Item_print(Item* self) {
  printf("Item type=%s pos=", ModelTypeStrings[self->obj->modelType]);
  Vec3d_print(&self->obj->position);
}

#endif
