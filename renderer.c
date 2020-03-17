
#include "renderer.h"
#include <math.h>
#ifndef __N64__
#include <stdio.h>
#endif
#include <stdlib.h>
#include "game.h"
#include "gameobject.h"
#include "vec3d.h"

int Renderer_isZBufferedGameObject(GameObject* obj) {
  switch (obj->modelType) {
    // case UniFloorModel:
    // case UniBldgModel:
    //   return FALSE;
    default:
      return TRUE;
  }
}

int Renderer_isLitGameObject(GameObject* obj) {
  switch (obj->modelType) {
    case UniFloorModel:
    case UniBldgModel:
    case WallModel:
    case PlanterModel:
      return TRUE;
    default:
      return FALSE;
  }
}

int Renderer_isAnimatedGameObject(GameObject* obj) {
  switch (obj->modelType) {
    case GooseModel:
    case GroundskeeperCharacterModel:
      return TRUE;
    default:
      return FALSE;
  }
}

float Renderer_gameobjectSortDist(GameObject* obj) {
  if (!Renderer_isZBufferedGameObject(obj)) {
    // always consider this far away
    return 10000.0F + obj->id;  // add object id to achieve stable sorting
  }

  return Vec3d_distanceTo(&obj->position, &Game_get()->viewPos);
}

int Renderer_sortWorldComparatorFn(const void* a, const void* b) {
  return ((RendererSortDistance*)a)->distance -
         ((RendererSortDistance*)b)->distance;  // sort far to near
}

void Renderer_sortWorldObjects(GameObject* objects,
                               RendererSortDistance* result,
                               int objectsCount) {
  int i;
  RendererSortDistance* sortDist;
  for (i = 0; i < objectsCount; ++i) {
    sortDist = result + i;
    sortDist->obj = objects + i;
    sortDist->distance = Renderer_gameobjectSortDist(sortDist->obj);
  }

  qsort(result, objectsCount, sizeof(RendererSortDistance),
        Renderer_sortWorldComparatorFn);
}
