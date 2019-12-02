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

#define NUM_CHARACTERS 1
Character characters[NUM_CHARACTERS];
#define NUM_ITEMS 2
Item items[NUM_ITEMS];
#define NUM_PHYS_BODIES 10
PhysBody physicsBodies[NUM_PHYS_BODIES];

void Game_init(GameObject* worldObjects, int worldObjectsCount) {
  int i;
  Vec3d pos;
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
  PhysState_init(&game.physicsState, 0.02);

  // setup camera
  Vec3d_copyFrom(&game.viewTarget, &game.player.goose->position);

  Item_init(&items[0], Game_findObjectByType(BookItemModel), &game);
  Item_init(&items[1], Game_findObjectByType(HomeworkItemModel), &game);
  Character_init(&characters[0],
                 Game_findObjectByType(GroundskeeperCharacterModel),
                 /*book*/ &items[0], &game);

  for (i = 0; i < NUM_PHYS_BODIES; ++i) {
    Vec3d_init(&pos, RAND(200), RAND(10) + 10, RAND(200));
    PhysBody_init(&physicsBodies[i],
                  /* mass */ 10.0,
                  /* radius */ 20.0, &pos, i);
  }

  game.items = items;
  game.itemsCount = NUM_ITEMS;
  game.characters = characters;
  game.charactersCount = NUM_CHARACTERS;
  game.physicsBodies = physicsBodies;
  game.physicsBodiesCount = NUM_PHYS_BODIES;
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
GameObject* Game_findObjectNByType(ModelType modelType, int n) {
  int i, found;
  GameObject* obj;

  found = 0;

  for (i = 0, obj = game.worldObjects; i < game.worldObjectsCount; i++, obj++) {
    if (obj->modelType == modelType) {
      if (found < n) {
        found++;
      } else {
        return obj;
      }
    }
  }
  return NULL;
}

void Game_updateCamera(Game* game) {
  float cameraDist;
  Vec3d cameraOffset;
  float zoom;
  Vec3d_set(&cameraOffset, 0.0F, 0.65F, -0.8F);
  // Vec3d_set(&cameraOffset, 0.0F, 0, -0.8F); // side view

  zoom = 2.0f;
  cameraDist = 1000.0f / zoom;
  Vec3d_multiplyScalar(&cameraOffset, cameraDist);

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

    physicsBodies[0].position = game->player.goose->position;
    physicsBodies[1].position = characters[0].obj->position;
    physicsBodies[2].position = items[0].obj->position;
    PhysBody_init(&physicsBodies[0],
                  /* mass */ 20.0,
                  /* radius */ 30.0, &game->player.goose->position, 0);

    PhysBody_init(&physicsBodies[1],
                  /* mass */ 200.0,
                  /* radius */ 30.0, &characters[0].obj->position, 1);

    PhysBody_init(&physicsBodies[2],
                  /* mass */ 100.0,
                  /* radius */ 30.0, &items[0].obj->position, 2);

    PhysState_step(&game->physicsState, game->physicsBodies,
                   game->physicsBodiesCount,
                   (float)game->tick / 60.0f * 1000.0f);

    game->player.goose->position = physicsBodies[0].position;
    characters[0].obj->position = physicsBodies[1].position;
    items[0].obj->position = physicsBodies[2].position;
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
  float tca, d2, radius2, thc, t0, t1;
  // l = objCenter - origin;
  Vec3d_copyFrom(&l, objCenter);
  Vec3d_sub(&l, origin);

  tca = Vec3d_dot(&l, rayDirection);
  d2 = Vec3d_dot(&l, &l) - tca * tca;
  radius2 = objRadius * objRadius;

  if (d2 > radius2)
    return FALSE;

  thc = sqrtf(radius2 - d2);

  // t0 = first intersect point - entrance on front of sphere
  t0 = tca - thc;

  // t1 = second intersect point - exit point on back of sphere
  t1 = tca + thc;

  // test to see if both t0 and t1 are behind the ray - if so, return null
  if (t0 < 0 && t1 < 0)
    return FALSE;

  return TRUE;
}

void Game_getObjCenter(GameObject* obj, Vec3d* result) {
  Vec3d_copyFrom(result, &obj->position);
  Vec3d_add(result, &modelTypesProperties[obj->modelType].centroidOffset);
}

float Game_distanceToGameObject(Vec3d* from, GameObject* to) {
  Vec3d toObjCenter;

  Game_getObjCenter(to, &toObjCenter);

  return Vec3d_distanceTo(from, &toObjCenter) -
         modelTypesProperties[to->modelType].radius;
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
  int i, canSee;
  float targetDistance, objDistance;
  GameObject* obj;

  canSee = TRUE;
  eye = viewer->position;
  eye.y += viewerEyeOffset;  // eye offset
  Vec3d_directionTo(&eye, &target->position, &rayDirection);

  targetDistance = Game_distanceToGameObject(&eye, target);

  for (obj = occuludingObjects, i = 0; i < occuludingObjectsCount; obj++, i++) {
    if (obj->id == target->id || obj->id == viewer->id) {
      // the ray will definitely intersect these, but we care about
      // intersections with other things
      continue;
    }

    objDistance = Game_distanceToGameObject(&eye, obj);

    if (objDistance > targetDistance) {
      // ignore objects further than target
      continue;
    }

    Game_getObjCenter(obj, &objCenter);

    if (Game_rayIntersectsSphere(&eye, &rayDirection, &objCenter,
                                 modelTypesProperties[obj->modelType].radius)) {
      canSee = FALSE;
      break;
    }
  }

#ifdef __cplusplus

  Game_traceRaycast({/*int result;*/ canSee,
                     /*Vec3d origin;*/ eye,
                     /*Vec3d direction;*/ rayDirection,
                     /*GameObject* hit;*/ canSee ? NULL : obj});
#endif
  return canSee;
}
