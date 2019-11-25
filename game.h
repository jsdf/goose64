
#ifndef _GAME_H_
#define _GAME_H_

#include "gameobject.h"
#include "gametypes.h"
#include "input.h"

void Game_init(GameObject* worldObjects, int worldObjectsCount);
Game* Game_get();

GameObject* Game_findObjectByType(ModelType modelType);
GameObject* Game_findObjectNByType(ModelType modelType, int n);

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
