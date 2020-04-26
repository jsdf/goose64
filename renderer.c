
#include "renderer.h"
#include <math.h>
#ifndef __N64__
#include <stdio.h>
#endif
#include <assert.h>
#include <stdlib.h>

#include <stdlib.h>
#include "ed64io_usb.h"
#include "frustum.h"
#include "game.h"
#include "gameobject.h"
#include "vec3d.h"

#define RENDERER_FRUSTUM_CULLING 1

inline int Renderer_isDynamicObject(GameObject* obj) {
  return obj->physBody != NULL;
}

int Renderer_isZBufferedGameObject(GameObject* obj) {
  if (Renderer_isDynamicObject(obj))
    return TRUE;

  switch (obj->modelType) {
    case BushModel:
      // case WatergrassModel:
      // case ReedModel:
      return TRUE;
    default:
      return FALSE;
  }
}
int Renderer_isZWriteGameObject(GameObject* obj) {
  switch (obj->modelType) {
    // case GroundModel:
    case WaterModel:
      return TRUE;
    default:
      return FALSE;
  }
}

int Renderer_isBackgroundGameObject(GameObject* obj) {
  switch (obj->modelType) {
    case GroundModel:
    case WaterModel:
      return TRUE;
    default:
      return FALSE;
  }
}

int Renderer_isLitGameObject(GameObject* obj) {
  switch (obj->modelType) {
    case GroundModel:
    case WaterModel:
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
    case GardenerCharacterModel:
      return TRUE;
    default:
      return FALSE;
  }
}

float Renderer_gameobjectSortDist(GameObject* obj) {
  if (Renderer_isBackgroundGameObject(obj)) {
    // always consider this far away
    return 10000.0F - obj->id;  // add object id to achieve stable sorting
  }

  return Vec3d_distanceTo(&obj->position, &Game_get()->viewPos);
}

int Renderer_sortWorldComparatorFn(const void* a, const void* b) {
#if RENDERER_PAINTERS_ALGORITHM
  // sort far to near for painters algorithm
  return ((RendererSortDistance*)b)->distance -
         ((RendererSortDistance*)a)->distance;

#else
  // sort near to far, so we benefit from zbuffer fast bailout
  return ((RendererSortDistance*)a)->distance -
         ((RendererSortDistance*)b)->distance;
#endif
}

AABB Renderer_getWorldAABB(AABB* localAABBs, GameObject* obj) {
  AABB* localAABB = localAABBs + obj->id;
  AABB worldAABB = *localAABB;

  Vec3d_add(&worldAABB.min, &obj->position);
  Vec3d_add(&worldAABB.max, &obj->position);
  return worldAABB;
}

typedef struct GameObjectAABB {
  int index;
  AABB aabb;
} GameObjectAABB;

void Renderer_calcIntersecting(
    int* objectsIntersecting,  // the result, keyed by index in sorted objects
    int objectsCount,
    RendererSortDistance* sortedObjects,
    AABB* localAABBs) {
  int i, k;
  int zWriteObjectsCount, zBufferedObjectsCount;
  GameObjectAABB* zWriteObjects;
  GameObjectAABB* zBufferedObjects;
  GameObject* obj;
  GameObjectAABB* zWriteAABB;
  GameObjectAABB* zBufferedAABB;
  GameObjectAABB* otherZBufferedAABB;

  zWriteObjectsCount = 0;
  zBufferedObjectsCount = 0;
  zWriteObjects =
      (GameObjectAABB*)malloc((objectsCount) * sizeof(GameObjectAABB));
  zBufferedObjects =
      (GameObjectAABB*)malloc((objectsCount) * sizeof(GameObjectAABB));
  invariant(zWriteObjects);
  invariant(zBufferedObjects);

  for (i = 0; i < objectsCount; ++i) {
    obj = (sortedObjects + i)->obj;
    objectsIntersecting[i] = FALSE;
    if (Renderer_isZWriteGameObject(obj)) {
      zWriteObjects[zWriteObjectsCount] =
          (GameObjectAABB){i, Renderer_getWorldAABB(localAABBs, obj)};
      zWriteObjectsCount++;
    } else if (Renderer_isZBufferedGameObject(obj)) {
      zBufferedObjects[zBufferedObjectsCount] =
          (GameObjectAABB){i, Renderer_getWorldAABB(localAABBs, obj)};
      zBufferedObjectsCount++;
    }
  }
  for (i = 0; i < zBufferedObjectsCount; ++i) {
    zBufferedAABB = &zBufferedObjects[i];

    for (k = 0; k < zWriteObjectsCount; ++k) {
      zWriteAABB = &zWriteObjects[k];
      if (Collision_intersectAABBAABB(&zBufferedAABB->aabb,
                                      &zWriteAABB->aabb)) {
        objectsIntersecting[zBufferedAABB->index] = TRUE;
        objectsIntersecting[zWriteAABB->index] = TRUE;
      }
    }
    for (k = 0; k < zBufferedObjectsCount; ++k) {
      if (i == k)
        continue;
      otherZBufferedAABB = &zBufferedObjects[k];
      if (Collision_intersectAABBAABB(&zBufferedAABB->aabb,
                                      &otherZBufferedAABB->aabb)) {
        objectsIntersecting[zBufferedAABB->index] = TRUE;
        objectsIntersecting[otherZBufferedAABB->index] = TRUE;
      }
    }
  }

  free(zWriteObjects);
  free(zBufferedObjects);
}

int Renderer_cullVisibility(GameObject* worldObjects,
                            int worldObjectsCount,
                            int* worldObjectsVisibility,
                            Frustum* frustum,
                            AABB* localAABBs) {
  GameObject* obj;
  int i;
  int visibilityCulled = 0;
  for (i = 0; i < worldObjectsCount; i++) {
    obj = worldObjects + i;
    if (obj->modelType == NoneModel || !obj->visible) {
      worldObjectsVisibility[i] = FALSE;
      visibilityCulled++;
      continue;
    }

#if RENDERER_FRUSTUM_CULLING
    {
      FrustumTestResult frustumTestResult;
      AABB* localAABB = localAABBs + i;
      AABB worldAABB = *localAABB;

      Vec3d_add(&worldAABB.min, &obj->position);
      Vec3d_add(&worldAABB.max, &obj->position);

      frustumTestResult = Frustum_boxInFrustum(frustum, &worldAABB);
      // printf("%d: %s", i, FrustumTestResultStrings[frustumTestResult]);
      if (frustumTestResult == OutsideFrustum) {
        // cull this object
        visibilityCulled++;
        worldObjectsVisibility[i] = FALSE;
        continue;
      }
    }
#endif

    worldObjectsVisibility[i] = TRUE;
  }

  return visibilityCulled;
}

void Renderer_sortVisibleObjects(GameObject* worldObjects,
                                 int worldObjectsCount,
                                 int* worldObjectsVisibility,
                                 int visibleObjectsCount,
                                 RendererSortDistance* result) {
  int i;
  RendererSortDistance* sortDist;
  int visibleObjectIndex = 0;
  for (i = 0; i < worldObjectsCount; ++i) {
    // only add visible objects, compacting results array
    if (worldObjectsVisibility[i]) {
      // results array is only as long as num visible objects
      invariant(visibleObjectIndex < visibleObjectsCount);
      sortDist = result + visibleObjectIndex;
      sortDist->obj = worldObjects + i;
      sortDist->distance = Renderer_gameobjectSortDist(sortDist->obj);

      visibleObjectIndex++;
    }
  }

  qsort(result, visibleObjectsCount, sizeof(RendererSortDistance),
        Renderer_sortWorldComparatorFn);
}
