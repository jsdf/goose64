
#ifndef _GAME_H_
#define _GAME_H_

#include "gameobject.h"
#include "gametypes.h"
#include "input.h"

// TODO: move to modeltype.h
typedef struct ModelProperties {
  float radius;
  Vec3d centroidOffset;
} ModelProperties;

extern ModelProperties modelTypesProperties[];

void Game_init(GameObject* worldObjects, int worldObjectsCount);
Game* Game_get();

GameObject* Game_findObjectByType(ModelType modelType);

int Game_rayIntersectsSphere(Vec3d* origin,
                             Vec3d* rayDirection,
                             Vec3d* objCenter,
                             float objRadius);

int Game_canSeeOtherObject(GameObject* viewer,
                           GameObject* target,
                           float viewerEyeOffset,
                           GameObject* occuludingObjects,
                           int occuludingObjectsCount);

void Game_update(Input* input);

#ifndef __N64__
#ifdef __cplusplus

#include <vector>

typedef struct RaycastTraceEvent {
  int result;
  Vec3d origin;
  Vec3d direction;
  GameObject* hit;
} RaycastTraceEvent;

extern std::vector<RaycastTraceEvent> gameRaycastTrace;
void Game_traceRaycast(RaycastTraceEvent event);
#endif
#endif

#endif /* !_GAME_H_ */
