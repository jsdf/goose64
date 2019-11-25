#include <math.h>

#include "animation.h"
#include "game.h"
#include "gameutils.h"
#include "gooseanimtypes.h"
#include "input.h"
#include "item.h"
#include "player.h"

#include "constants.h"

#define PLAYER_NEAR_OBJ_DIST 100.0f
#define PLAYER_PICKUP_COOLDOWN 10
#define PLAYER_WALK_ANIM_MOVEMENT_DIVISOR 100.0

static Vec3d playerItemOffset = {0.0F, 80.0F, 0.0F};

void Player_init(Player* self, GameObject* obj) {
  ItemHolder_init(&self->itemHolder, PlayerItemHolder, (void*)&self);
  AnimationState_init(&self->animState);
  self->goose = obj;
  obj->animState = &self->animState;
  self->lastPickupTick = 0;
}

int Player_debounceInput(unsigned int* lastTrigger, unsigned int cooldown) {
  if (Game_get()->tick > *lastTrigger + cooldown) {
    *lastTrigger = Game_get()->tick;
    return TRUE;
  }
  return FALSE;
}

void Player_update(Player* self, Input* input, Game* game) {
  Vec3d playerMovement;
  float destAngle, movementSpeed;
  GameObject* goose;
  int i;
  Item* item;

  movementSpeed = GOOSE_SPEED * (input->run ? 2.0 : 0.75);
  goose = self->goose;

  // prevent moving too fast diagonally
  if (Vec2d_length(&input->direction) > 1.0) {
    Vec2d_normalise(&input->direction);
  }

  // movement
  Vec3d_init(&playerMovement, input->direction.x * movementSpeed, 0.0F,
             input->direction.y * movementSpeed);
  Vec3d_add(&goose->position, &playerMovement);

  if (Vec3d_magSq(&playerMovement) > 0) {
    if (self->animState.state != goose_walk_anim) {
      // enter walk anim
      self->animState.progress = 0.0;
    } else {
      // advance walk anim
      self->animState.progress = fmodf(
          self->animState.progress +
              Vec3d_mag(&playerMovement) / PLAYER_WALK_ANIM_MOVEMENT_DIVISOR,
          1.0);
    }
    self->animState.state = goose_walk_anim;

  } else {
    self->animState.state = goose_idle_anim;
    self->animState.progress = 0.0;
  }

  // rot
  if (Vec2d_lengthSquared(&input->direction) > 0) {
    destAngle = 360.0F - radToDeg(Vec2d_angle(&input->direction));
    // rotate towards dest, but with a speed limit
    goose->rotation.y = GameUtils_rotateTowardsClamped(
        goose->rotation.y, destAngle, GOOSE_MAX_TURN_SPEED);
  }

  if (self->itemHolder.heldItem) {
    // bring item with you
    self->itemHolder.heldItem->obj->position = self->goose->position;
    Vec3d_add(&self->itemHolder.heldItem->obj->position, &playerItemOffset);
  }

  if (input->pickup &&
      Player_debounceInput(&self->lastPickupTick, PLAYER_PICKUP_COOLDOWN)) {
    if (self->itemHolder.heldItem) {
      // drop item
      Item_drop(self->itemHolder.heldItem);
    } else {
      // TODO: should have a concept of target so goose can look at items to
      // pickup
      for (i = 0, item = game->items; i < game->itemsCount; i++, item++) {
        if (Vec3d_distanceTo(&self->goose->position, &item->obj->position) <
            PLAYER_NEAR_OBJ_DIST) {
          // yes, pick up
          Item_take(item, &self->itemHolder);
          if (self->itemHolder.heldItem == item) {
            self->lastPickupTick = game->tick;
            // one's enough
            break;
          }
        }
      }
    }
  }
}

void Player_haveItemTaken(Player* self) {
  // TODO: some reaction
}

#ifndef __N64__
#include <stdio.h>
void Player_print(Player* self) {
  printf("Player heldItem=%s pos=",
         self->itemHolder.heldItem
             ? ModelTypeStrings[self->itemHolder.heldItem->obj->modelType]
             : "none");
  Vec3d_print(&self->goose->position);
}
#endif
