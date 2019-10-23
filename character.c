#include <math.h>

#include "character.h"
#include "vec2d.h"
#include "vec3d.h"

#include "gameutils.h"

#include "constants.h"

#define CHARACTER_SPEED 2.0F

#define CHARACTER_MAX_TURN_SPEED 45.0f

#ifndef __N64__
#include <stdio.h>

void Character_print(Character* self) {
  printf("Character target=%s pos=",
         self->target ? ModelTypeStrings[self->target->modelType] : "none");
  Vec3d_print(&self->obj->position);
}

void Character_printStateTransition(Character* self, CharacterState nextState) {
  printf("%s: %s -> %s\n", ModelTypeStrings[self->obj->modelType],
         CharacterStateStrings[self->state], CharacterStateStrings[nextState]);
}
#endif

void Character_init(Character* self, GameObject* obj, Game* game) {
  self->obj = obj;
  self->target = 0;

  switch (obj->modelType) {
    case GroundskeeperCharacterModel:

      Character_setTarget(self, game->player.goose);
      break;
    default: {
    }
  }
}

void Character_update(Character* self, Game* game) {
  Vec3d movement;
  Vec2d movement2d;
  float destAngle;

  if (self->target) {
    Vec3d_directionTo(&self->obj->position, &self->target->position, &movement);

    Vec3d_multiplyScalar(&movement, CHARACTER_SPEED);

    Vec3d_add(&self->obj->position, &movement);

    // rotate towards target, but with a speed limit
    Vec2d_init(&movement2d, movement.x, movement.z);
    destAngle = 360.0 - radToDeg(Vec2d_angle(&movement2d));
    self->obj->rotationZ = GameUtils_rotateTowardsClamped(
        self->obj->rotationZ, destAngle, CHARACTER_MAX_TURN_SPEED);
  }

  Character_updateState(self, game);
}

void Character_transitionToState(Character* self, CharacterState nextState) {
#ifndef __N64__
  Character_printStateTransition(self, nextState);
#endif
  self->state = nextState;
}

void Character_updateIdleState(Character* self, Game* game) {}
void Character_updateDefaultActivityState(Character* self, Game* game) {}
void Character_updateSeekingItemState(Character* self, Game* game) {}
void Character_updateSeekingSoundSourceState(Character* self, Game* game) {}
void Character_updateFleeingState(Character* self, Game* game) {}

void Character_updateState(Character* self, Game* game) {
  switch (self->state) {
    case IdleState:
      Character_updateIdleState(self, game);
      break;
    case DefaultActivityState:
      Character_updateDefaultActivityState(self, game);
      break;
    case SeekingItemState:
      Character_updateSeekingItemState(self, game);
      break;
    case SeekingSoundSourceState:
      Character_updateSeekingSoundSourceState(self, game);
      break;
    case FleeingState:
      Character_updateFleeingState(self, game);
      break;
    default:
      break;
  }
}

void Character_setTarget(Character* self, GameObject* target) {
  self->target = target;
}
