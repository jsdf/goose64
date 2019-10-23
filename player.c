#include "game.h"
#include "gameutils.h"
#include "input.h"

#include "constants.h"

void Player_init(Player* self, GameObject* obj) {
  self->goose = obj;
}

void Player_update(Player* self, Input* input) {
  Vec3d playerMovement;
  float destAngle, movementSpeed;
  GameObject* goose;

  movementSpeed = GOOSE_SPEED * (input->run ? 2.0 : 0.75);
  goose = self->goose;

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
}

#ifndef __N64__
#include <stdio.h>
void Player_print(Player* self) {
  printf("Player heldObj=%s pos=",
         self->heldObj ? ModelTypeStrings[self->heldObj->modelType] : "none");
  Vec3d_print(&self->goose->position);
}
#endif
