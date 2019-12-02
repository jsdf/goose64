#include <math.h>

#include "character.h"
#include "game.h"
#include "item.h"
#include "modeltype.h"
#include "vec2d.h"
#include "vec3d.h"

#include "gameutils.h"

#include "constants.h"

#define CHARACTER_SPEED 2.0F

#define CHARACTER_MAX_TURN_SPEED 45.0f
#define CHARACTER_FLEE_DIST 1200.0f
#define CHARACTER_NEAR_OBJ_DIST 100.0f
#define CHARACTER_ITEM_NEAR_HOME_DIST 100.0f
#define CHARACTER_SIGHT_RANGE 800.0f
#define CHARACTER_PICKUP_COOLDOWN 120
#define CHARACTER_MIN_IDLE_TIME 120
#define CHARACTER_CONFUSION_TIME 120
#define CHARACTER_DEFAULT_ACTIVITY_TIME 300
#define CHARACTER_EYE_OFFSET_Y 120.0

static Vec3d characterItemOffset = {0.0F, 60.0F, 0.0F};

#ifndef __N64__
#include <stdio.h>
void Character_print(Character* self) {
  printf(
      "Character target=%s pos=",

      self->target ? ModelTypeStrings[self->target->obj->modelType] : "none");
}

void Character_printStateTransition(Character* self, CharacterState nextState) {
  printf("%s: %s -> %s\n", ModelTypeStrings[self->obj->modelType],
         CharacterStateStrings[self->state], CharacterStateStrings[nextState]);
}

void Character_toString(Character* self, char* buffer) {
  char pos[60];

  Vec3d_toString(&self->obj->position, pos);
  sprintf(
      buffer, "Character state=%s target=%s pos=%s",
      CharacterStateStrings[self->state],
      self->target ? ModelTypeStrings[self->target->obj->modelType] : "none",
      pos);
}
#endif

void Character_init(Character* self,
                    GameObject* obj,
                    Item* defaultActivityItem,
                    Game* game) {
  ItemHolder_init(&self->itemHolder, CharacterItemHolder, (void*)self);
  self->obj = obj;
  self->target = NULL;
  self->defaultActivityItem = defaultActivityItem;

  self->defaultActivityLocation = obj->position;
  self->state = IdleState;

  self->enteredStateTick = 0;
  self->startedActivityTick = 0;
}

int Character_canSeeItem(Character* self, Item* item, Game* game) {
  GameObject visibilityCheckObjects[MAX_WORLD_OBJECTS];
  int visibilityCheckObjectsCount = 0;
  int i;
  Vec3d vecToObject;
  GameObject* obj;

  for (obj = game->worldObjects, i = 0; i < game->worldObjectsCount;
       obj++, i++) {
    // these object types don't occlude
    // TODO: move this somewhere else
    switch (obj->modelType) {
      case UniFloorModel:
      case UniBldgModel:
      case FlagpoleModel:
      case GooseModel:
        continue;
      default:
        break;
    }

    Vec3d_directionTo(&self->obj->position, &obj->position, &vecToObject);

    visibilityCheckObjects[visibilityCheckObjectsCount] = *obj;
    visibilityCheckObjectsCount++;
  }

  return Game_canSeeOtherObject(
      self->obj, item->obj,
      /*viewer eye pos y offset*/ CHARACTER_EYE_OFFSET_Y,
      visibilityCheckObjects, visibilityCheckObjectsCount);
}

int Character_canSeePlayer(Character* self, Game* game) {
  return Game_canSeeOtherObject(
      self->obj, game->player.goose,
      /*viewer eye pos y offset*/ CHARACTER_EYE_OFFSET_Y, game->worldObjects,
      game->worldObjectsCount);
}

void Character_moveTowards(Character* self, Vec3d target) {
  Vec3d movement;
  Vec2d movement2d;
  float destAngle;
  Vec3d_directionTo(&self->obj->position, &target, &movement);

  Vec3d_multiplyScalar(&movement, CHARACTER_SPEED);

  Vec3d_add(&self->obj->position, &movement);

  // rotate towards target, but with a speed limit
  Vec2d_init(&movement2d, movement.x, movement.z);
  destAngle = 360.0 - radToDeg(Vec2d_angle(&movement2d));
  self->obj->rotation.y = GameUtils_rotateTowardsClamped(
      self->obj->rotation.y, destAngle, CHARACTER_MAX_TURN_SPEED);
}

void Character_update(Character* self, Game* game) {
  if (self->itemHolder.heldItem) {
    // bring item with you
    self->itemHolder.heldItem->obj->position = self->obj->position;
    Vec3d_add(&self->itemHolder.heldItem->obj->position, &characterItemOffset);
  }
  Character_updateState(self, game);
}

void Character_transitionToState(Character* self, CharacterState nextState) {
#ifndef __N64__
  Character_printStateTransition(self, nextState);
#endif
  self->enteredStateTick = Game_get()->tick;
  self->state = nextState;
}

void Character_maybeTransitionToHigherPriorityState(Character* self,
                                                    Game* game) {
  Item* possibleTarget = self->defaultActivityItem;
  if (self->state == ConfusionState) {
    // the only way out of this state is for it to time out
    return;
  }
  // TODO: start fleeing
  // TODO: heard sound
  if (self->state < SeekingItemState) {
    if (Vec3d_distanceTo(&possibleTarget->obj->position,
                         &possibleTarget->initialLocation) >
        CHARACTER_ITEM_NEAR_HOME_DIST) {
      // item has been stolen
      if (Vec3d_distanceTo(&possibleTarget->obj->position,
                           &self->obj->position) < CHARACTER_SIGHT_RANGE &&
          // can actually see it
          Character_canSeeItem(self, possibleTarget, game)) {
        // and we can see it
        Character_transitionToState(self, SeekingItemState);
        return;
      }
    }
  }
}

void Character_updateIdleState(Character* self, Game* game) {
  Character_transitionToState(self, DefaultActivityState);
}

void Character_updateConfusionState(Character* self, Game* game) {
  if (game->tick < self->enteredStateTick + CHARACTER_CONFUSION_TIME) {
    return;
  }
  Character_transitionToState(self, IdleState);
}

void Character_updateDefaultActivityState(Character* self, Game* game) {
  if (Vec3d_distanceTo(&self->obj->position, &self->defaultActivityLocation) >
      CHARACTER_NEAR_OBJ_DIST) {
    Character_moveTowards(self, self->defaultActivityLocation);
  } else {
    // do default activity
    if (self->startedActivityTick) {
      if (game->tick >
          self->startedActivityTick + CHARACTER_DEFAULT_ACTIVITY_TIME) {
        self->startedActivityTick = 0;
        Character_transitionToState(self, IdleState);
      } else {
        // continue doing
        return;
      }

    } else {
      self->startedActivityTick = game->tick;

#ifndef __N64__
      debugPrintf("started default activity");
#endif
    }
  }
}

void Character_haveItemTaken(Character* self) {
  // let nature take its course
  self->state = ConfusionState;
}

void Character_updateSeekingItemState(Character* self, Game* game) {
  self->target = self->defaultActivityItem;

  if (self->itemHolder.heldItem) {
    // are we near enough to drop item?
    if (Vec3d_distanceTo(&self->obj->position, &self->target->initialLocation) <
        CHARACTER_NEAR_OBJ_DIST) {
      // close enough to return item
      Item_drop(self->itemHolder.heldItem);
      self->target = NULL;
      Character_transitionToState(self, IdleState);
    } else {
      // bringing item back to initial location
      Character_moveTowards(self, self->target->initialLocation);
    }
  } else {
    // can we still see the item?

    if (!Character_canSeeItem(self, self->target, game)) {
      // nope, give up
      self->target = NULL;
#ifndef __N64__
      debugPrintf("can't see the item anymore, giving up\n");
#endif
      Character_transitionToState(self, IdleState);
      return;
    }

    // are we near enough to pick up item?
    if (Vec3d_distanceTo(&self->obj->position, &self->target->obj->position) <
        CHARACTER_NEAR_OBJ_DIST) {
      if (self->target->holder && self->target->holder != &self->itemHolder) {
#ifndef __N64__
        debugPrintf("stealing item back\n");
#endif
      }
      Item_take(self->target, &self->itemHolder);

    } else {
      // no, move towards
      Character_moveTowards(self, self->target->obj->position);
    }
  }
}
void Character_updateSeekingSoundSourceState(Character* self, Game* game) {
  // TODO
}
void Character_updateFleeingState(Character* self, Game* game) {
  Vec3d movement;
  Vec3d_directionTo(&game->player.goose->position, &self->obj->position,
                    &movement);
  Vec3d_add(&self->obj->position, &movement);

  if (Vec3d_distanceTo(&game->player.goose->position, &self->obj->position) <
      CHARACTER_FLEE_DIST) {
    Character_transitionToState(self, IdleState);
  }
}

void Character_updateState(Character* self, Game* game) {
  Character_maybeTransitionToHigherPriorityState(self, game);

  switch (self->state) {
    case IdleState:
      Character_updateIdleState(self, game);
      break;
    case ConfusionState:
      Character_updateConfusionState(self, game);
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

void Character_setTarget(Character* self, Item* target) {
  self->target = target;
}
