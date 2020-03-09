#include <math.h>

#include "character.h"
#include "game.h"
#include "item.h"
#include "modeltype.h"
#include "vec2d.h"
#include "vec3d.h"

#include "gameutils.h"

#include "constants.h"

#include "characteranimtypes.h"

#if USE_PHYSICS_MOVEMENT
#define CHARACTER_SPEED 0.1F
#else
#define CHARACTER_SPEED 4.1F
#endif

#define CHARACTER_ENABLED 1
#define CHARACTER_FOLLOW_PLAYER 1
#define DEBUG_CHARACTER 1
#define CHARACTER_MAX_TURN_SPEED 4.0f
#define CHARACTER_FLEE_DIST 1200.0f
#define CHARACTER_NEAR_TARGET_DIST 100.0f
#define CHARACTER_NEAR_OBJ_DROP_DIST 100.0f
#define CHARACTER_NEAR_OBJ_TAKE_DIST 100.0f
#define CHARACTER_ITEM_NEAR_HOME_DIST 100.0f
#define CHARACTER_SIGHT_RANGE 800.0f
#define CHARACTER_PICKUP_COOLDOWN 120
#define CHARACTER_MIN_IDLE_TIME 120
#define CHARACTER_CONFUSION_TIME 120
#define CHARACTER_DEFAULT_ACTIVITY_TIME 300
#define CHARACTER_EYE_OFFSET_Y 120.0
#define CHARACTER_VIEW_ANGLE_HALF 90.0
#define CHARACTER_FACING_OBJECT_ANGLE 30.0
#define CHARACTER_FACING_MOVEMENT_TARGET_ANGLE 75.0
#define CHARACTER_DEBUG_ANIMATION 0
#define CHARACTER_WALK_ANIM_MOVEMENT_DIVISOR 1.6
#define CHARACTER_SPEED_MULTIPLIER_RUN 1.0
#define CHARACTER_SPEED_MULTIPLIER_WALK 0.5

static Vec3d characterItemOffset = {0.0F, 60.0F, 0.0F};

#ifndef __N64__
#include <stdio.h>
void Character_print(Character* self) {
  printf(
      "Character target=%s pos=",

      self->target ? ModelTypeStrings[self->target->obj->modelType] : "none");
}

void Character_toString(Character* self, char* buffer) {
  char pos[60];
  char rot[60];
  float angleToPlayer;

  angleToPlayer =
      Character_topDownAngleMagToObj(self, Game_get()->player.goose);

  Vec3d_toString(&self->obj->position, pos);
  Vec3d_toString((Vec3d*)&self->obj->rotation, rot);
  sprintf(
      buffer, "Character state=%s target=%s pos=%s rot=%s angleToPlayer=%f",
      CharacterStateStrings[self->state],
      self->target ? ModelTypeStrings[self->target->obj->modelType] : "none",
      pos, rot, angleToPlayer);
}
#endif

void Character_printStateTransition(Character* self, CharacterState nextState) {
  debugPrintf("%s: %s -> %s\n", ModelTypeStrings[self->obj->modelType],
              CharacterStateStrings[self->state],
              CharacterStateStrings[nextState]);
}

void Character_init(Character* self,
                    GameObject* obj,
                    Item* defaultActivityItem,
                    Game* game) {
  ItemHolder_init(&self->itemHolder, CharacterItemHolder, (void*)self);
  AnimationState_init(&self->animState);
  self->obj = obj;
  obj->animState = &self->animState;
  // setup picked-up object attachment point
  self->animState.attachment.boneIndex =
      (int)characterforearm_r_characterforearm_rmesh;
  self->animState.attachment.modelType = BookItemModel;
  self->animState.attachment.offset.z = -32;  // down along arm
  self->animState.attachment.offset.y = -10;  // back
  self->animState.attachment.rotation.x = 90;
  self->animState.attachment.rotation.z = 90;

  Vec3d_origin(&self->targetLocation);
  self->targetType = NoneCharacterTarget;

  self->target = NULL;
  self->defaultActivityItem = defaultActivityItem;

  self->defaultActivityLocation = obj->position;
  self->state = IdleState;

  self->enteredStateTick = 0;
  self->startedActivityTick = 0;

  self->pathfindingResult = NULL;
}

// find smallest angle delta, discarding sign
float Character_angleDeltaMag(float a1, float a2) {
  return fabsf(180.0F - fabsf(fabsf(fmodf(a1 - a2, 360.0f)) - 180.0F));
}

float Character_topDownAngleDeltaToPos(Character* self, Vec3d* position) {
  Vec3d toPos;
  Vec2d toPos2d;
  float angleToPos;

  Vec3d_directionTo(&self->obj->position, position, &toPos);
  toPos2d.x = toPos.x;
  toPos2d.y = -toPos.z;
  angleToPos = radToDeg(Vec2d_angle(&toPos2d));

  return Character_angleDeltaMag(self->obj->rotation.y, angleToPos);
}

void Character_directionFromTopDownAngle(float angle, Vec3d* result) {
  Vec2d direction2d;

  Vec2d_fromAngle(&direction2d, angle);

  result->x = direction2d.x;
  result->z = -direction2d.y;
}

float Character_topDownAngleMagToObj(Character* self, GameObject* obj) {
  float angleFromHeadingToPos;
  angleFromHeadingToPos =
      Character_topDownAngleDeltaToPos(self, &obj->position);
  return fabsf(fmodf(angleFromHeadingToPos, 360.0f));
}

int Character_posIsInViewArc(Character* self, Vec3d* position) {
  float angleFromHeadingToPos;

  // within range
  if (Vec3d_distanceTo(position, &self->obj->position) >
      CHARACTER_SIGHT_RANGE) {
    return FALSE;
  }

  // within arc
  angleFromHeadingToPos = Character_topDownAngleDeltaToPos(self, position);
  if (fabsf(fmodf(angleFromHeadingToPos, 360.0f)) > CHARACTER_VIEW_ANGLE_HALF) {
    return FALSE;
  }

  return TRUE;
}

int Character_canSeeItem(Character* self, Item* item, Game* game) {
  GameObject visibilityCheckObjects[MAX_WORLD_OBJECTS];
  int visibilityCheckObjectsCount = 0;
  int i;
  Vec3d vecToObject;
  GameObject* obj;

  if (!Character_posIsInViewArc(self, &item->obj->position)) {
    return FALSE;
  }

  // build list of occluding objects for line of sight raycasting
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

  // check line of sight by raycasting
  return Game_canSeeOtherObject(
      self->obj, item->obj,
      /*viewer eye pos y offset*/ CHARACTER_EYE_OFFSET_Y,
      visibilityCheckObjects, visibilityCheckObjectsCount);
}

int Character_canSeePlayer(Character* self, Game* game) {
  // check whether player is in view arc of character
  if (!Character_posIsInViewArc(self, &game->player.goose->position)) {
    return FALSE;
  }
  // check line of sight by raycasting
  return Game_canSeeOtherObject(
      self->obj, game->player.goose,
      /*viewer eye pos y offset*/ CHARACTER_EYE_OFFSET_Y, game->worldObjects,
      game->worldObjectsCount);
}

void Character_moveTowards(Character* self,
                           Vec3d target,
                           float speedMultiplier) {
  Vec3d targetDirection;
  Vec2d targetDirection2d;
  Vec3d headingDirection;
  Vec3d movement;
  float targetAngle;
  Vec3d_directionTo(&self->obj->position, &target, &targetDirection);

  targetDirection.y =
      0;  // remove vertical component to stop character trying to fly
  Vec3d_normalise(&targetDirection);  // renormalize with y=0

  // rotate towards target, but with a speed limit
  Vec2d_init(&targetDirection2d, targetDirection.x, targetDirection.z);
  targetAngle = 360.0 - radToDeg(Vec2d_angle(&targetDirection2d));
  self->obj->rotation.y = GameUtils_rotateTowardsClamped(
      self->obj->rotation.y, targetAngle, CHARACTER_MAX_TURN_SPEED);

  // resulting heading

  if (
      // is facing towards target enough to move forward?
      Character_topDownAngleDeltaToPos(self, &target) <=
      CHARACTER_FACING_MOVEMENT_TARGET_ANGLE) {
    Character_directionFromTopDownAngle(degToRad(self->obj->rotation.y),
                                        &headingDirection);

    Vec3d_copyFrom(&movement, &headingDirection);
    Vec3d_mulScalar(&movement, CHARACTER_SPEED * speedMultiplier);
    Vec3d_add(&self->obj->position, &movement);
  }
}

void Character_setVisibleItemAttachment(Character* self, ModelType modelType) {
  self->animState.attachment.modelType = modelType;
}

int Character_isNextPathNodeDestination(Character* self) {
  return self->pathfindingResult->resultSize - 1 == self->pathProgress;
}

Node* Character_getPathNode(Character* self, Game* game, int pathNodeIndex) {
  int* pathNodeID;
  pathNodeID = self->pathfindingResult->result + pathNodeIndex;
  return Path_getNodeByID(game->pathfindingGraph, *pathNodeID);
}

void Character_followPlayer(Character* self, Game* game) {
  int from;
  int to;
  float profStartPath;
  Graph* pathfindingGraph;
  PathfindingState* pathfindingState;
  int* pathNodeID;
  Node* nextNode;
  Vec3d target;
  Vec3d* pathSegmentP0;
  Vec3d* pathSegmentP1;
  Vec3d movementTarget;
  float distanceToTarget;

  distanceToTarget =
      Vec3d_distanceTo(&self->obj->position, &self->obj->position);
  target = game->player.goose->position;
  pathfindingGraph = game->pathfindingGraph;
  pathfindingState = game->pathfindingState;

  to = Path_quantizePosition(pathfindingGraph, &target);

  // check that the goal is still the closest node to the destination
  if (self->pathfindingResult && self->pathfindingResult->end->id != to) {
    // needs new pathfinding result
    self->pathfindingResult = NULL;
  }

  // find a path if we need one
  if (!self->pathfindingResult) {
    profStartPath = CUR_TIME_MS();

    from = Path_quantizePosition(pathfindingGraph, &self->obj->position);

    Path_initState(pathfindingGraph,                          // graph
                   pathfindingState,                          // state
                   Path_getNodeByID(pathfindingGraph, from),  // start
                   Path_getNodeByID(pathfindingGraph, to),    // end
                   pathfindingState->nodeStates,     // nodeStates array
                   pathfindingState->nodeStateSize,  // nodeStateSize
                   pathfindingState->result          // results array
    );

    if (Path_findAStar(pathfindingGraph, pathfindingState)) {
      // pathfinding complete
      self->pathfindingResult = pathfindingState;
      self->pathProgress = 0;
      self->pathSegmentProgress = 0;
    } else {
      debugPrintf("character: pathfinding failed\n");
    }

    game->profTimePath += (CUR_TIME_MS() - profStartPath);
  }

  // TODO: implement path smoothing (raycast to see if we can cut corners)

  if (self->pathfindingResult &&
      // TODO: use path smoothing instead of just cutting off last waypoint
      !Character_isNextPathNodeDestination(self)) {
    nextNode = Character_getPathNode(self, game, self->pathProgress);

    // near enough to waypoint
    if (Vec3d_distanceTo(&self->obj->position, &nextNode->position) < 40) {
      self->pathProgress++;
    } else {
      pathSegmentP0 =
          &Character_getPathNode(self, game, self->pathProgress)->position;
      pathSegmentP1 =
          &Character_getPathNode(self, game,
                                 MIN(self->pathfindingResult->resultSize - 1,
                                     self->pathProgress + 1))
               ->position;
      movementTarget = *pathSegmentP0;

      self->pathSegmentProgress = Path_getClosestPointParameter(
          pathSegmentP0, pathSegmentP1, &self->obj->position);
      // get position along line to next point
      Vec3d_lerp(&movementTarget, pathSegmentP1,
                 // lead target point a little bit
                 MIN(1.0f, self->pathSegmentProgress + 0.1));

      self->targetLocation = movementTarget;

      // head towards waypoint
      Character_moveTowards(self, movementTarget,
                            CHARACTER_SPEED_MULTIPLIER_WALK);

      if (self->pathSegmentProgress > 0.999) {
        self->pathProgress++;
      }
    }
  } else {
    // no path or at end of path, just head towards target
    Character_moveTowards(self, target, CHARACTER_SPEED_MULTIPLIER_WALK);
  }
}

void Character_update(Character* self, Game* game) {
  Vec3d startPos;
  float startRot;
  float animationMovementSpeed;
  int isTurning;
  int isWalking;

  Vec3d_copyFrom(&startPos, &self->obj->position);
  startRot = self->obj->rotation.y;
  if (self->itemHolder.heldItem) {
    // bring item with you
    self->itemHolder.heldItem->obj->position = self->obj->position;
    Vec3d_add(&self->itemHolder.heldItem->obj->position, &characterItemOffset);
  }

  // update held item visual attachment
  if (self->itemHolder.heldItem != NULL) {
    Character_setVisibleItemAttachment(
        self, self->itemHolder.heldItem->obj->modelType);
  } else {
    Character_setVisibleItemAttachment(self, NoneModel);
  }

#if CHARACTER_FOLLOW_PLAYER
  Character_followPlayer(self, game);
#else
#if CHARACTER_ENABLED
  Character_updateState(self, game);
#endif
#endif

  isTurning = fabsf(self->obj->rotation.y - startRot) > 0.001;
  animationMovementSpeed =
      Vec3d_distanceTo(&startPos, &self->obj->position) / 100.0f;
  isWalking = animationMovementSpeed > 0.0001;
  if (!isWalking && isTurning) {
    // for now when turning, just play back walk anim at constant speed
    animationMovementSpeed = 0.03;
  }

#if CHARACTER_DEBUG_ANIMATION
  animationMovementSpeed = 0.001;
#endif

  // update animation
  if (isWalking || isTurning  // TODO: turn in place anim
  ) {
    if (self->animState.state != character_walk_anim) {
      // enter walk anim
      self->animState.progress = 0.0;
    } else {
      // advance walk anim
      self->animState.progress = fmodf(
          self->animState.progress +
              (animationMovementSpeed / CHARACTER_WALK_ANIM_MOVEMENT_DIVISOR),
          1.0);
    }
    self->animState.state = character_walk_anim;

  } else {
    self->animState.state = character_idle_anim;
    self->animState.progress = 0.0;
  }
}

void Character_transitionToState(Character* self, CharacterState nextState) {
#ifndef __N64__
  if (nextState == SeekingItemState) {
    printf("starting SeekingItemState\n");
  }
#endif
  Character_printStateTransition(self, nextState);
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
  if (self->state < SeekingSoundSourceState) {
    if (self->targetType == HonkCharacterTarget) {
      Character_transitionToState(self, SeekingSoundSourceState);
      return;
    }
  }
  if (self->state < SeekingItemState) {
    // has item been stolen?
    if (Vec3d_distanceTo(&possibleTarget->obj->position,
                         &possibleTarget->initialLocation) >
        CHARACTER_ITEM_NEAR_HOME_DIST) {
      // and can we see it?
      if (Character_canSeeItem(self, possibleTarget, game)) {
        Character_transitionToState(self, SeekingItemState);
        Character_canSeeItem(self, possibleTarget, game);
        return;
      }
    }
  }
  if (self->state < SeekingLastSeenState) {
    if (self->targetType == ItemCharacterTarget) {
      Character_transitionToState(self, SeekingLastSeenState);
      return;
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

int Character_isCloseToAndFacing(Character* self,
                                 Vec3d* target,
                                 float targetDist) {
  return !(
      // not close enough
      Vec3d_distanceTo(&self->obj->position, target) > targetDist ||
      // not facing towards enough
      Character_topDownAngleDeltaToPos(self, target) >
          CHARACTER_FACING_OBJECT_ANGLE);
}

// in this state the charcter is trying to go to a 'default activity' and do it
void Character_updateDefaultActivityState(Character* self, Game* game) {
  if (!Character_isCloseToAndFacing(self, &self->defaultActivityLocation,
                                    CHARACTER_NEAR_OBJ_DROP_DIST)) {
    // go there
    Character_moveTowards(self, self->defaultActivityLocation,
                          CHARACTER_SPEED_MULTIPLIER_WALK);
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
      printf("started default activity\n");
#endif
    }
  }
}

void Character_haveItemTaken(Character* self) {
  Game* game;
  // let nature take its course
  self->state = ConfusionState;

  // make sure that the character can find the player after having item stolen
  self->targetType = ItemCharacterTarget;
  game = Game_get();
  self->targetLocation = game->player.goose->position;
}

void Character_updateSeekingItemState(Character* self, Game* game) {
  int someoneElseIsHoldingItem;
  self->target = self->defaultActivityItem;

  if (self->itemHolder.heldItem) {
    // are we near enough to drop item?
    if (Vec3d_distanceTo(&self->obj->position, &self->target->initialLocation) <
        CHARACTER_NEAR_OBJ_DROP_DIST) {
      // close enough to return item

      Vec3d_copyFrom(&self->itemHolder.heldItem->obj->position,
                     &self->target->initialLocation);

      Item_drop(self->itemHolder.heldItem);
      self->target = NULL;
      Character_transitionToState(self, IdleState);
    } else {
      // bringing item back to initial location
      Character_moveTowards(self, self->target->initialLocation,
                            CHARACTER_SPEED_MULTIPLIER_WALK);
    }
  } else {
    // can we still see the item?

    if (!Character_canSeeItem(self, self->target, game)) {
      // nope, give up
      self->target = NULL;
#ifndef DEBUG_CHARACTER
      debugPrintf("can't see the item anymore, giving up\n");
#endif
      Character_transitionToState(self, IdleState);
      return;
    }

    // keep last seen location in case it goes out of view
    self->targetType = ItemCharacterTarget;
    self->targetLocation = self->target->obj->position;

    someoneElseIsHoldingItem =
        self->target->holder && self->target->holder != &self->itemHolder;

    // are we near enough to pick up item?
    if (Vec3d_distanceTo(&self->obj->position, &self->target->obj->position) <
        CHARACTER_NEAR_OBJ_TAKE_DIST) {
      if (someoneElseIsHoldingItem) {
#ifndef DEBUG_CHARACTER
        debugPrintf("stealing item back\n");
#endif
      }
      Item_take(self->target, &self->itemHolder);
      self->targetType = NoneCharacterTarget;

    } else {
      // no, move towards
      Character_moveTowards(self, self->target->obj->position,
                            someoneElseIsHoldingItem
                                ? CHARACTER_SPEED_MULTIPLIER_RUN
                                : CHARACTER_SPEED_MULTIPLIER_WALK);
    }
  }
}
void Character_updateSeekingTargetState(Character* self, Game* game) {
  // if character is close enough from target, give up
  if (Character_isCloseToAndFacing(self, &self->targetLocation,
                                   CHARACTER_NEAR_TARGET_DIST)) {
#ifndef DEBUG_CHARACTER
    debugPrintf("close enough to target, giving up\n");
#endif

    self->targetType = NoneCharacterTarget;
    Character_transitionToState(self, IdleState);
    return;
  } else if (self->targetType == HonkCharacterTarget) {
    if (Character_canSeePlayer(self, game)) {
#ifndef DEBUG_CHARACTER
      debugPrintf("can see player that honked, giving up\n");
#endif
      self->targetType = NoneCharacterTarget;
      Character_transitionToState(self, IdleState);
      return;
    }
  } else {
    // continue advancing towards target
    Character_moveTowards(self, self->targetLocation,
                          CHARACTER_SPEED_MULTIPLIER_WALK);
  }
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
    case SeekingLastSeenState:
      Character_updateSeekingTargetState(self, game);
      break;
    case SeekingItemState:
      Character_updateSeekingItemState(self, game);
      break;
    case SeekingSoundSourceState:
      Character_updateSeekingTargetState(self, game);
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
