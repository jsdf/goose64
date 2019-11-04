#include <assert.h>
#include <math.h>
#ifndef __N64__
#include <stdio.h>
#endif
#include <stdlib.h>
// game
#include "character.h"
#include "game.h"
#include "gameobject.h"
#include "gameutils.h"
#include "item.h"
#include "modeltype.h"
#include "player.h"
#include "vec3d.h"

#include "constants.h"

static Game game;

// TODO: move to modeltype.c
ModelProperties modelTypesProperties[] = {
    /* NoneModel */
    {/* radius */ 50.0,
     /* centroidOffset */ {0.0, 0.0, 0.0}},
    /* GooseModel */
    {/* radius */ 50.0,
     /* centroidOffset */ {0.0, 0.0, 0.0}},
    /* BookItemModel */
    {/* radius */ 20.0,
     /* centroidOffset */ {0.0, 0.0, 0.0}},
    /* HomeworkItemModel */
    {/* radius */ 50.0,
     /* centroidOffset */ {0.0, 0.0, 0.0}},
    /* CakeItemModel */
    {/* radius */ 50.0,
     /* centroidOffset */ {0.0, 0.0, 0.0}},
    /* UniBldgModel */
    {/* radius */ 50.0,
     /* centroidOffset */ {0.0, 0.0, 0.0}},
    /* UniFloorModel */
    {/* radius */ 50.0,
     /* centroidOffset */ {0.0, 0.0, 0.0}},
    /* BushModel */
    {/* radius */ 50.0,
     /* centroidOffset */ {0.0, 15.0, 0.0}},
    /* FlagpoleModel */
    {/* radius */ 50.0,
     /* centroidOffset */ {0.0, 0.0, 0.0}},
    /* GroundskeeperCharacterModel */
    {/* radius */ 50.0,
     /* centroidOffset */ {0.0, 0.0, 0.0}},
    /* MAX_MODEL_TYPE */
    {/* radius */ 50.0,
     /* centroidOffset */ {0.0, 0.0, 0.0}},
};

#define NUM_CHARACTERS 1
Character characters[NUM_CHARACTERS];
#define NUM_ITEMS 2
Item items[NUM_ITEMS];

void Game_init(GameObject* worldObjects, int worldObjectsCount) {
  GameObject* goose;

  game.paused = FALSE;
  game.worldObjects = worldObjects;
  game.worldObjectsCount = worldObjectsCount;

  // camera
  Vec3d_init(&game.viewPos, 0.0F, 0.0F, -400.0F);
  Vec3d_init(&game.viewRot, 0.0F, 0.0F, 0.0F);
  game.freeView = 0;

  goose = Game_findObjectByType(GooseModel);
  assert(goose != NULL);

  Player_init(&game.player, goose);

  // setup camera
  Vec3d_copyFrom(&game.viewTarget, &game.player.goose->position);

  Item_init(&items[0], Game_findObjectByType(BookItemModel), &game);
  Item_init(&items[1], Game_findObjectByType(HomeworkItemModel), &game);
  Character_init(&characters[0],
                 Game_findObjectByType(GroundskeeperCharacterModel),
                 /*book*/ &items[0], &game);

  game.items = items;
  game.itemsCount = NUM_ITEMS;
  game.characters = characters;
  game.charactersCount = NUM_CHARACTERS;
}

Game* Game_get() {
  return &game;
}

// finds the first object with a particular modeltype
GameObject* Game_findObjectByType(ModelType modelType) {
  int i;
  GameObject* obj;

  for (i = 0, obj = game.worldObjects; i < game.worldObjectsCount; i++, obj++) {
    if (obj->modelType == modelType) {
      return obj;
    }
  }
  return NULL;
}

void Game_updateCamera(Game* game) {
  Vec3d cameraOffset;
  Vec3d_set(&cameraOffset, 0.0F, 650.0F, -800.0F);

  Vec3d_copyFrom(&game->viewPos, &game->player.goose->position);
  Vec3d_add(&game->viewPos, &cameraOffset);

  // look at goose
  Vec3d_copyFrom(&game->viewTarget, &game->player.goose->position);
}

void Game_update(Input* input) {
  Game* game;
  int i;

  game = Game_get();

  if (!game->paused) {
    game->tick++;

    for (i = 0; i < NUM_CHARACTERS; ++i) {
      Character_update(&characters[i], game);
    }

    Player_update(&game->player, input, game);

    Game_updateCamera(game);
  }

  // reset inputs
  Input_init(input);
}

#ifndef __N64__
#ifdef __cplusplus

std::vector<RaycastTraceEvent> gameRaycastTrace;

void Game_traceRaycast(RaycastTraceEvent event) {
  if (gameRaycastTrace.size() > 1000) {
    printf("!!! gameRaycastTrace overflowed\n");
    gameRaycastTrace.clear();
  }
  gameRaycastTrace.push_back(event);
}
#endif
#endif

int Game_rayIntersectsSphere(Vec3d* origin,
                             Vec3d* rayDirection,
                             Vec3d* objCenter,
                             float objRadius) {
  Vec3d l;
  float tc, d, t1c, t1, t2;
  // l = objCenter - origin;
  Vec3d_copyFrom(&l, objCenter);
  Vec3d_sub(&l, origin);
  // solve for tc
  tc = Vec3d_dot(&l, rayDirection);
  if (tc < 0.0)
    return FALSE;

  d = sqrtf((tc * tc) - Vec3d_dot(&l, &l));
  if (d > objRadius)
    return FALSE;

  // solve for t1c
  t1c = sqrtf((objRadius * objRadius) - (d * d));

  // solve for intersection points
  t1 = tc - t1c;
  t2 = tc + t1c;

  return TRUE;
}

/*
method: check if ray from viewer to target intersects any other object's
bounding sphere at a distance less than the distance to the target's
bounding sphere
*/
int Game_canSeeOtherObject(GameObject* viewer,
                           GameObject* target,
                           float viewerEyeOffset,
                           GameObject* occuludingObjects,
                           int occuludingObjectsCount) {
  Vec3d eye, rayDirection, objCenter;
  int i, result;
  float targetDistance;
  GameObject* obj;

  result = TRUE;
  eye = viewer->position;
  eye.y += viewerEyeOffset;  // eye offset
  Vec3d_directionTo(&eye, &target->position, &rayDirection);

  targetDistance = Vec3d_distanceTo(&eye, &target->position) -
                   modelTypesProperties[target->modelType].radius;

  for (obj = occuludingObjects, i = 0; i < occuludingObjectsCount; obj++, i++) {
    if (obj == target || obj == viewer) {
      // the ray will definitely intersect these, but we care about
      // intersections with other things
      continue;
    }

    Vec3d_copyFrom(&objCenter,
                   &modelTypesProperties[obj->modelType].centroidOffset);

    if (Vec3d_distanceTo(&eye, &objCenter) -
            modelTypesProperties[obj->modelType].radius >
        targetDistance) {
      // ignore objects further than target
      continue;
    }

    if (Game_rayIntersectsSphere(&eye, &rayDirection, &objCenter,
                                 modelTypesProperties[obj->modelType].radius)) {
      result = FALSE;
      break;
    }
  }

#ifdef __cplusplus
  Game_traceRaycast({/*int result;*/ result,
                     /*Vec3d origin;*/ eye,
                     /*Vec3d direction;*/ rayDirection});
#endif
  return result;
}
