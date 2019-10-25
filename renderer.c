
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
    case UniFloorModel:
    case UniBldgModel:
      return 0;
    default:
      return 1;
  }
}

float Renderer_gameobjectSortDist(GameObject* obj) {
  if (!Renderer_isZBufferedGameObject(obj)) {
    // always consider this far away
    return 10000.0F + obj->id;  // add object id to achieve stable sorting
  }

  return Vec3d_distanceTo(&obj->position, &Game_get()->viewPos);
}

int Renderer_gameobjectDistComparatorFn(const void* a, const void* b) {
  float distA, distB;
  GameObject *objA, *objB;
  objA = *((GameObject**)a);
  objB = *((GameObject**)b);
  distA = Renderer_gameobjectSortDist(objA);
  distB = Renderer_gameobjectSortDist(objB);
  return distB - distA;  // sort far to near
}

// note the GameObject** (it's an array of pointers)
void Renderer_sortWorldObjects(GameObject** sortedObjects,
                               int sortedObjectsSize) {
  qsort(sortedObjects, sortedObjectsSize, sizeof(GameObject*),
        Renderer_gameobjectDistComparatorFn);
}
