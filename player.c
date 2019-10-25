#include "player.h"
#include "game.h"
#include "gameutils.h"
#include "input.h"

#include "constants.h"

#define PLAYER_NEAR_OBJ_DIST 100.0f
#define PLAYER_PICKUP_COOLDOWN 10

void Player_init(Player* self, GameObject* obj) {
  self->goose = obj;
  self->heldItem = NULL;
  self->lastPickupTick = 0;
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

  // rot
  if (Vec2d_lengthSquared(&input->direction) > 0) {
    destAngle = 360.0F - radToDeg(Vec2d_angle(&input->direction));
    // rotate towards dest, but with a speed limit
    goose->rotationZ = GameUtils_rotateTowardsClamped(
        goose->rotationZ, destAngle, GOOSE_MAX_TURN_SPEED);
  }

  if (self->heldItem) {
    // bring item with you
    self->heldItem->obj->position = self->goose->position;
  }

  if (input->pickup &&
      self->lastPickupTick < (game->tick - PLAYER_PICKUP_COOLDOWN)) {
    if (!self->heldItem) {
      for (i = 0, item = game->items; i < game->itemsCount; i++, item++) {
        if (Vec3d_distanceTo(&self->goose->position, &item->obj->position) <
            PLAYER_NEAR_OBJ_DIST) {
          // yes, pick up
          self->heldItem = item;
          // TODO: logic for letting item holders know you stole their item
        }
      }
    } else {
      // drop item
      self->heldItem = NULL;
    }
  }
}

#ifndef __N64__
#include <stdio.h>
void Player_print(Player* self) {
  printf("Player heldItem=%s pos=",
         self->heldItem ? ModelTypeStrings[self->heldItem->obj->modelType]
                        : "none");
  Vec3d_print(&self->goose->position);
}
#endif
