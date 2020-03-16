#include <math.h>

#include "character.h"
#include "game.h"
#include "item.h"
#include "modeltype.h"
#include "trace.h"
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
#define CHARACTER_FOLLOW_PLAYER 0
#define DEBUG_CHARACTER 0
#define CHARACTER_MAX_TURN_SPEED 10.0f
#define CHARACTER_FLEE_DIST 1200.0f
#define CHARACTER_NEAR_TARGET_DIST 60.0f
#define CHARACTER_NEAR_OBJ_DROP_DIST 60.0f
#define CHARACTER_NEAR_OBJ_PICKUP_DIST 40.0f
#define CHARACTER_NEAR_OBJ_STEAL_DIST 60.0f
#define CHARACTER_ITEM_NEAR_HOME_DIST 60.0f
#define CHARACTER_SIGHT_RANGE 800.0f
#define CHARACTER_PICKUP_COOLDOWN 120
#define CHARACTER_MIN_IDLE_TIME 120
#define CHARACTER_CONFUSION_TIME 120
#define CHARACTER_DEFAULT_ACTIVITY_TIME 300
#define CHARACTER_EYE_OFFSET_Y 120.0
#define CHARACTER_VIEW_ANGLE_HALF 90.0
#define CHARACTER_FACING_OBJECT_ANGLE 30.0
#define CHARACTER_FACING_MOVEMENT_TARGET_ANGLE 30.0
#define CHARACTER_DEBUG_ANIMATION 0
#define CHARACTER_WALK_ANIM_MOVEMENT_DIVISOR 1.6
#define CHARACTER_SPEED_MULTIPLIER_RUN 1.0
#define CHARACTER_SPEED_MULTIPLIER_WALK 0.5
#define CHARACTER_TARGET_PATH_PARAM 0
#define DEBUG_CHARACTER_PATH_SMOOTHING 0

static Vec3d characterItemOffset = {0.0F, 60.0F, 0.0F};

#ifndef __N64__
#include <stdio.h>
void Character_print(Character* self) {
  printf("Character target=%s pos=",

         self->targetItem ? ModelTypeStrings[self->targetItem->obj->modelType]
                          : "none");
}

void Character_toString(Character* self, char* buffer) {
  char pos[60];
  char rot[60];
  float angleToPlayer;

  angleToPlayer =
      Character_topDownAngleMagToObj(self, Game_get()->player.goose);

  Vec3d_toString(&self->obj->position, pos);
  Vec3d_toString((Vec3d*)&self->obj->rotation, rot);
  sprintf(buffer, "Character state=%s target=%s pos=%s rot=%s angleToPlayer=%f",
          CharacterStateStrings[self->state],
          self->targetItem ? ModelTypeStrings[self->targetItem->obj->modelType]
                           : "none",
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

  self->targetItem = NULL;
  self->defaultActivityItem = defaultActivityItem;

  self->defaultActivityLocation = obj->position;
  self->state = IdleState;

  self->enteredStateTick = 0;
  self->startedActivityTick = 0;

  self->pathfindingResult = NULL;

  self->pathProgress = 0;
  self->pathSegmentProgress = 0.0f;
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
  result->y = 0;
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
  float speedScaleForHeading;

  Vec3d_directionTo(&self->obj->position, &target, &targetDirection);

  targetDirection.y =
      0;  // remove vertical component to stop character trying to fly
  Vec3d_normalise(&targetDirection);  // renormalize with y=0

  // rotate towards target, but with a speed limit
  Vec2d_init(&targetDirection2d, targetDirection.x, targetDirection.z);
  targetAngle = 360.0 - radToDeg(Vec2d_angle(&targetDirection2d));
  self->obj->rotation.y = GameUtils_rotateTowardsClamped(
      self->obj->rotation.y, targetAngle,
      CHARACTER_MAX_TURN_SPEED * speedMultiplier);

  // resulting heading
  speedScaleForHeading = MIN(
      1.0, 1.0 - MAX(0.0f, (Character_topDownAngleDeltaToPos(self, &target) -
                            CHARACTER_FACING_MOVEMENT_TARGET_ANGLE)) /
                     90.0f);
  self->speedScaleForHeading = speedScaleForHeading;
  Character_directionFromTopDownAngle(degToRad(self->obj->rotation.y),
                                      &headingDirection);

  Vec3d_copyFrom(&movement, &headingDirection);
  Vec3d_mulScalar(&movement,
                  CHARACTER_SPEED * speedMultiplier * speedScaleForHeading);
  Vec3d_add(&self->obj->position, &movement);
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

float Character_getDistanceTopDown(Vec3d* from, Vec3d* to) {
  Vec2d from2d, to2d;
  Vec2d_init(&from2d, from->x, -from->z);
  Vec2d_init(&to2d, to->x, -to->z);
  return Vec2d_distanceTo(&from2d, &to2d);
}

void Character_goToTarget(Character* self,
                          Game* game,
                          Vec3d* target,
                          float speedMultiplier) {
  int from;
  int to;
  float profStartPathfinding;
  Graph* pathfindingGraph;
  PathfindingState* pathfindingState;
  Vec3d* nextNodePos;
#if CHARACTER_TARGET_PATH_PARAM
  Vec3d* pathSegmentP0;
  Vec3d* pathSegmentP1;
#endif
  Vec3d movementTarget;
  Vec3d objCenter;
  float objRadius;

  int spatialHashResults[100];
  int spatialHashResultsCount;
  int spatialHashMaxResults;
  int pathToNextNodeUnobstructed;
  int i;
  AABB triangleAABB;

  spatialHashMaxResults = 100;

  Game_getObjCenter(self->obj, &objCenter);
  objRadius = Game_getObjRadius(self->obj);

  pathfindingGraph = game->pathfindingGraph;
  pathfindingState = game->pathfindingState;

  to = Path_quantizePosition(pathfindingGraph, target);

  // check that the goal is still the closest node to the destination
  if (self->pathfindingResult && self->pathfindingResult->end->id != to) {
    // needs new pathfinding result
    self->pathfindingResult = NULL;
  }

  // find a path if we need one
  if (!self->pathfindingResult) {
    profStartPathfinding = CUR_TIME_MS();

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
    if (game->traceEventStarts[PathfindingTraceEvent] == 0) {
      game->traceEventStarts[PathfindingTraceEvent] = profStartPathfinding;
    }
    game->trace[PathfindingTraceEvent] +=
        (CUR_TIME_MS() - profStartPathfinding);
  }

  if (self->pathfindingResult) {
    // path smoothing
    // raycasts to find if there is any obstacle to going straight to next node
    // in path, and if not, skips ahead
    while (TRUE) {
#ifndef __N64__
      assert(self->pathProgress <= self->pathfindingResult->resultSize);
#endif
      if (self->pathProgress == self->pathfindingResult->resultSize)
        break;
      pathToNextNodeUnobstructed = TRUE;

      // is cast ray unobstructed from where we are to the n+1 node?

      nextNodePos =
          self->pathProgress < self->pathfindingResult->resultSize - 1
              ? &Character_getPathNode(self, game, self->pathProgress + 1)
                     ->position  // path node
              : target;          // final target

      // find triangles to raycast
      spatialHashResultsCount = SpatialHash_getTrianglesForRaycast(
          &objCenter, nextNodePos,
          game->physicsState.worldData->worldMeshSpatialHash,
          spatialHashResults, spatialHashMaxResults);

      // actually do raycast
      for (i = 0; i < spatialHashResultsCount; i++) {
        AABB_fromTriangle(
            /*tri*/ game->physicsState.worldData->worldMeshTris +
                spatialHashResults[i],
            &triangleAABB);

        triangleAABB.min.x -= objRadius;
        triangleAABB.min.z -= objRadius;

        triangleAABB.max.x += objRadius;
        triangleAABB.max.z += objRadius;
        if (Collision_testSegmentAABBCollision(&objCenter, nextNodePos,
                                               &triangleAABB)) {
          pathToNextNodeUnobstructed = FALSE;
#if DEBUG_CHARACTER_PATH_SMOOTHING
          printf("hit tri %d\n", spatialHashResults[i]);
#endif
          break;
        }
      }

      if (pathToNextNodeUnobstructed) {
#if DEBUG_CHARACTER_PATH_SMOOTHING
        printf("skipping path node %d: %d (pathLength=%d)\n",
               self->pathProgress,
               Character_getPathNode(self, game, self->pathProgress)->id,
               self->pathfindingResult->resultSize);

        printf("no collision with tris: ");
        for (i = 0; i < spatialHashResultsCount; i++) {
          printf("%d, ", spatialHashResults[i]);
        }
        printf("\n");

        printf(
            "advancing due to no collisions with this segment "
            "pathProgress=%d\n",
            self->pathProgress);
#endif

        // no obstacles between character and next node
        // we can go straight there
        self->pathProgress++;
#ifndef __N64__
        assert(self->pathProgress <= self->pathfindingResult->resultSize);
#endif
      } else {
        break;
      }
    }

    // this combined with path smoothing results in invalid corner cutting
#if CHARACTER_TARGET_PATH_PARAM
    pathSegmentP0 =
        &Character_getPathNode(self, game, self->pathProgress)->position;
    pathSegmentP1 =
        &Character_getPathNode(self, game,
                               MIN(self->pathfindingResult->resultSize - 1,
                                   self->pathProgress + 1))
             ->position;

    self->pathSegmentProgress = Path_getClosestPointParameter(
        pathSegmentP0, pathSegmentP1, &self->obj->position);
    // get position along line to next point
    Vec3d_lerp(&movementTarget, pathSegmentP1,
               // lead target point a little bit
               MIN(1.0f, self->pathSegmentProgress + 0.1));
    movementTarget = *pathSegmentP0;
#else

    movementTarget =
        *(nextNodePos =
              self->pathProgress < self->pathfindingResult->resultSize - 1
                  ? &Character_getPathNode(self, game, self->pathProgress)
                         ->position  // path node
                  : target           // final target
        );
#endif

    self->movementTarget = movementTarget;

    // head towards waypoint
    Character_moveTowards(self, movementTarget, speedMultiplier);

#if CHARACTER_TARGET_PATH_PARAM
    nextNodePos = pathSegmentP1;
#else
    nextNodePos = &movementTarget;
#endif

    if (self->pathProgress < self->pathfindingResult->resultSize - 1 &&
        self->pathSegmentProgress > 0.999) {
      // printf("advancing due to self->pathSegmentProgress pathProgress=%d\n",
      //        self->pathProgress);
      self->pathProgress++;

    } else if (self->pathProgress < self->pathfindingResult->resultSize - 1 &&
               Character_getDistanceTopDown(&self->obj->position, nextNodePos) <
                   40) {
      // printf("advancing due to near enough to waypoint pathProgress=%d\n",
      //        self->pathProgress);
      // near enough to waypoint
      self->pathProgress++;
    }
  } else {
    // no path or at end of path, just head towards target
    Character_moveTowards(self, *target, speedMultiplier);
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
  self->targetLocation = game->player.goose->position;
  Character_goToTarget(self, game, &self->targetLocation,
                       CHARACTER_SPEED_MULTIPLIER_WALK);
#elif CHARACTER_ENABLED
  Character_updateState(self, game);
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
#ifndef DEBUG_CHARACTER
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
#ifndef DEBUG_CHARACTER
      printf(
          "transitioning to higher priority state: SeekingSoundSourceState\n");
#endif
      Character_transitionToState(self, SeekingSoundSourceState);
      return;
    }
  }
  if (self->state < SeekingItemState) {
    // has item been stolen?
    if (Character_getDistanceTopDown(&possibleTarget->obj->position,
                                     &possibleTarget->initialLocation) >
        CHARACTER_ITEM_NEAR_HOME_DIST) {
      // and can we see it?
      if (Character_canSeeItem(self, possibleTarget, game)) {
#ifndef DEBUG_CHARACTER
        printf(
            "saw stolen item. transitioning to higher priority state: "
            "SeekingItemState\n");
#endif
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

int Character_isCloseToAndFacing(Character* self,
                                 Vec3d* target,
                                 float targetDist) {
  return !(
      // not close enough
      Character_getDistanceTopDown(&self->obj->position, target) > targetDist ||
      // not facing towards enough
      Character_topDownAngleDeltaToPos(self, target) >
          CHARACTER_FACING_OBJECT_ANGLE);
}

// in this state the charcter is trying to go to a 'default activity' and do it
void Character_updateDefaultActivityState(Character* self, Game* game) {
  if (!Character_isCloseToAndFacing(self, &self->defaultActivityLocation,
                                    CHARACTER_NEAR_OBJ_DROP_DIST)) {
    // go there
    Character_goToTarget(self, game, &self->defaultActivityLocation,
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

#ifndef DEBUG_CHARACTER
      printf("started default activity\n");
#endif
    }
  }
}

void Character_haveItemTaken(Character* self, Item* item) {
  Game* game;
  // let nature take its course
  self->state = ConfusionState;

  // make sure that the character can find the player after having item stolen
  self->targetType = ItemCharacterTarget;
  self->targetItem = item;
  game = Game_get();
  self->targetLocation = game->player.goose->position;

  Character_transitionToState(self, SeekingItemState);
}

int Character_isSomeoneElseIsHoldingItem(Character* self) {
  return self->targetItem->holder &&
         self->targetItem->holder != &self->itemHolder;
}

void Character_updateSeekingItemState(Character* self, Game* game) {
  self->targetItem = self->defaultActivityItem;

  if (self->itemHolder.heldItem) {
    // are we near enough to drop item?
    if (Vec3d_distanceTo(&self->obj->position,
                         &self->targetItem->initialLocation) <
        CHARACTER_NEAR_OBJ_DROP_DIST) {
      // close enough to return item

      Vec3d_copyFrom(&self->itemHolder.heldItem->obj->position,
                     &self->targetItem->initialLocation);

      Item_drop(self->itemHolder.heldItem);
      self->targetItem = NULL;
      Character_transitionToState(self, IdleState);
    } else {
      // bringing item back to initial location
      Character_goToTarget(self, game, &self->targetItem->initialLocation,
                           CHARACTER_SPEED_MULTIPLIER_WALK);
    }
  } else {
    // can we still see the item?

    if (!Character_canSeeItem(self, self->targetItem, game)) {
#ifndef DEBUG_CHARACTER
      debugPrintf(
          "can't see the item anymore, looking in last seen location\n");
#endif
      Character_transitionToState(self, SeekingLastSeenState);
      return;
    }

    // keep last seen location in case it goes out of view
    self->targetType = ItemCharacterTarget;
    self->targetLocation = self->targetItem->obj->position;

    // are we near enough to pick up item?
    if (Character_getDistanceTopDown(&self->obj->position,
                                     &self->targetItem->obj->position) <
        (Character_isSomeoneElseIsHoldingItem(self)
             ? CHARACTER_NEAR_OBJ_STEAL_DIST
             : CHARACTER_NEAR_OBJ_PICKUP_DIST)) {
      if (Character_isSomeoneElseIsHoldingItem(self)) {
#ifndef DEBUG_CHARACTER
        debugPrintf("stealing item back\n");
#endif
      }
      Item_take(self->targetItem, &self->itemHolder);
      self->targetType = NoneCharacterTarget;

    } else {
      // no, move towards
      Character_goToTarget(self, game, &self->targetItem->obj->position,
                           Character_isSomeoneElseIsHoldingItem(self)
                               ? CHARACTER_SPEED_MULTIPLIER_WALK
                               : CHARACTER_SPEED_MULTIPLIER_WALK);
    }
  }
}
void Character_updateSeekingTargetState(Character* self, Game* game) {
#ifndef __N64__
  if (self->targetType == ItemCharacterTarget) {
    assert(self->targetType);
  }
#endif
  // if character is close enough from target, give up
  if (Character_isCloseToAndFacing(self, &self->targetLocation,
                                   CHARACTER_NEAR_TARGET_DIST)) {
    // maybe we should check line of sight too
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
    // TODO: this is pretty sketchy, instead perhaps this should be incorporated
    // into SeekingItemState
    // continue advancing towards target
    Character_goToTarget(self, game, &self->targetLocation,
                         Character_isSomeoneElseIsHoldingItem(self)
                             ? CHARACTER_SPEED_MULTIPLIER_WALK
                             : CHARACTER_SPEED_MULTIPLIER_WALK);
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
