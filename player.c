#include <math.h>

#include "animation.h"
#include "game.h"
#include "gameutils.h"
#include "gooseanimtypes.h"
#include "input.h"
#include "item.h"
#include "player.h"

#include "constants.h"

// max speed, slightly slower than character speed
#define GOOSE_SPEED 3.9F
#define GOOSE_WALK_SPEED_RATIO 0.5
#define GOOSE_FORCE 5000.0F
#define GOOSE_MAX_TURN_SPEED 15.0f
#define PLAYER_NEAR_OBJ_DIST 100.0f
#define PLAYER_PICKUP_COOLDOWN 10
#define PLAYER_WALK_ANIM_MOVEMENT_DIVISOR 100.0
#define PLAYER_PHYS_WALK_ANIM_MOVEMENT_DIVISOR 5200.0

static Vec3d playerItemOffset = {0.0F, 80.0F, 0.0F};

void Player_init(Player* self, GameObject* obj) {
  ItemHolder_init(&self->itemHolder, PlayerItemHolder, (void*)self);
  AnimationState_init(&self->animState);
  self->goose = obj;
  obj->animState = &self->animState;
  // setup picked-up object attachment point
  self->animState.attachment.boneIndex = (int)goosehead_gooseheadmesh;
  self->animState.attachment.offset.x = 14;
  self->animState.attachment.offset.z = -2;
  self->animState.attachment.rotation.x = 90;
  self->lastPickupTick = 0;
}

int Player_debounceInput(unsigned int lastTrigger, unsigned int cooldown) {
  if (Game_get()->tick > lastTrigger + cooldown) {
    return TRUE;
  }
  return FALSE;
}

void Player_setVisibleItemAttachment(Player* self, ModelType modelType) {
  self->animState.attachment.modelType = modelType;
}

void Player_update(Player* self, Input* input, Game* game) {
  Vec3d playerMovement, playerMovementScaled;
  float destAngle, movementSpeedRatio, resultantMovementSpeed;
  GameObject* goose;
  int i;
  Item* item;

  movementSpeedRatio = (input->run ? 1.0 : GOOSE_WALK_SPEED_RATIO);
  goose = self->goose;

  // prevent moving too fast diagonally
  if (Vec2d_length(&input->direction) > 1.0) {
    Vec2d_normalise(&input->direction);
  }

  // movement
  Vec3d_init(&playerMovement, input->direction.x * movementSpeedRatio, 0.0F,
             input->direction.y * movementSpeedRatio);
  playerMovementScaled = playerMovement;

#if USE_PHYSICS_MOVEMENT
  Vec3d_multiplyScalar(&playerMovementScaled, GOOSE_FORCE);
  Vec3d_add(&goose->physBody->acceleration, &playerMovementScaled);
  playerMovement = goose->physBody->nonIntegralVelocity;  // from last frame :(
  playerMovement.y = 0;

  resultantMovementSpeed = Vec3d_mag(&playerMovement);
  resultantMovementSpeed /= PLAYER_PHYS_WALK_ANIM_MOVEMENT_DIVISOR;
#else
  Vec3d_multiplyScalar(&playerMovementScaled, GOOSE_SPEED);

  Vec3d_add(&goose->position, &playerMovementScaled);
  // PhysBody_translateWithoutForce(goose->physBody, &playerMovementScaled);
  resultantMovementSpeed = Vec3d_mag(&playerMovementScaled);
  resultantMovementSpeed /= PLAYER_WALK_ANIM_MOVEMENT_DIVISOR;
#endif

  // update animation
  if (resultantMovementSpeed > 0.001) {
    if (self->animState.state != goose_walk_anim) {
      // enter walk anim
      self->animState.progress = 0.0;
    } else {
      // advance walk anim
      self->animState.progress =
          fmodf(self->animState.progress + resultantMovementSpeed, 1.0);
    }
    self->animState.state = goose_walk_anim;

  } else {
    self->animState.state = goose_idle_anim;
    self->animState.progress = 0.0;
  }

  // update held item visual attachment
  if (self->itemHolder.heldItem != NULL) {
    Player_setVisibleItemAttachment(self,
                                    self->itemHolder.heldItem->obj->modelType);
  } else {
    Player_setVisibleItemAttachment(self, NoneModel);
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
      Player_debounceInput(self->lastPickupTick, PLAYER_PICKUP_COOLDOWN)) {
    if (self->itemHolder.heldItem) {
      // drop item
      Item_drop(self->itemHolder.heldItem);
      self->lastPickupTick = game->tick;
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
  // react to item being taken
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

void Player_toString(Player* self, char* buffer) {
  char pos[60];
  char vel[60];
  Vec3d_toString(&self->goose->position, pos);
  Vec3d_toString(&self->goose->physBody->nonIntegralVelocity, vel);
  sprintf(buffer, "Player id=%d pos=%s vel=%s heldItem=%s", self->goose->id,
          pos, vel,
          self->itemHolder.heldItem
              ? ModelTypeStrings[self->itemHolder.heldItem->obj->modelType]
              : "none");
}
#endif
