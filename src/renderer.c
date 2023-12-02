
#include "renderer.h"
#include <math.h>
#ifndef __N64__
#include <stdio.h>
#endif
#ifdef __N64__
#include <PR/os_internal.h>
#endif
#include <assert.h>

#include <stdlib.h>
#ifdef __N64__
#include <malloc.h>
#endif

#ifdef ED64
#include "ed64io_usb.h"
#endif
#include "frustum.h"
#include "game.h"
#include "gameobject.h"
#include "vec3d.h"

#define RENDERER_FRUSTUM_CULLING 1

int Renderer_isDynamicObject(GameObject* obj) {
  return obj->physBody != NULL;
}

int Renderer_isZBufferedGameObject(GameObject* obj) {
  if (Renderer_isDynamicObject(obj))
    return TRUE;
  if (Renderer_isAnimatedGameObject(obj))
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

float Renderer_gameobjectSortDist(GameObject* obj, Vec3d* viewPos) {
  if (Renderer_isBackgroundGameObject(obj)) {
    // always consider this far away
    return 10000.0F - obj->id;  // add object id to achieve stable sorting
  }

  return Vec3d_distanceTo(&obj->position, viewPos);
}

void Renderer_closestPointOnAABB(AABB* b,
                                 /* sourcePoint*/ Vec3d* p,
                                 /* result */ Vec3d* q) {
  float v;
  v = p->x;
  if (v < b->min.x)
    v = b->min.x;  // v = max(v, b->min.x)
  if (v > b->max.x)
    v = b->max.x;  // v = min(v, b->max.x)
  q->x = v;
  v = p->y;
  if (v < b->min.y)
    v = b->min.y;  // v = max(v, b->min.y)
  if (v > b->max.y)
    v = b->max.y;  // v = min(v, b->max.y)
  q->y = v;
  v = p->z;
  if (v < b->min.z)
    v = b->min.z;  // v = max(v, b->min.z)
  if (v > b->max.z)
    v = b->max.z;  // v = min(v, b->max.z)
  q->z = v;
}

void Renderer_getSeparatingPlane(Vec3d* a, Vec3d* b, Plane* separatingPlane) {
  Vec3d halfwayPoint, aToBDirection;
  halfwayPoint = *a;
  Vec3d_add(&halfwayPoint, b);
  Vec3d_divScalar(&halfwayPoint, 2);

  Vec3d_directionTo(a, b, &aToBDirection);
  aToBDirection.y = 0;  // only separate on x,z
  if (fabsf(aToBDirection.x) > fabsf(aToBDirection.z)) {
    aToBDirection.z = 0;
  } else {
    aToBDirection.x = 0;
  }

  Plane_setNormalAndPoint(separatingPlane, &aToBDirection, &halfwayPoint);
}

// currently this somehow causes the game to crash when touching a BushModel ??
int Renderer_isCloserBySeparatingPlane(RendererSortDistance* a,
                                       RendererSortDistance* b,
                                       Vec3d* viewPos) {
  Plane separatingPlane;
  Vec3d aCenter, bCenter, aClosestPoint, bClosestPoint, aReallyClosestPoint,
      bReallyClosestPoint;
  float planeToADist, planeToBDist, planeToViewDist;
  invariant(a->obj != NULL);
  invariant(b->obj != NULL);

  Game_getObjCenter(a->obj, &aCenter);
  Game_getObjCenter(b->obj, &bCenter);

  // dumb heuristic
  if (Renderer_isDynamicObject(a->obj)) {
    aClosestPoint = aCenter;
  } else {
    Renderer_closestPointOnAABB(&a->worldAABB, &bCenter, &aClosestPoint);
  }
  if (Renderer_isDynamicObject(b->obj)) {
    bClosestPoint = bCenter;
  } else {
    Renderer_closestPointOnAABB(&b->worldAABB, &aCenter, &bClosestPoint);
  }

  if (Renderer_isDynamicObject(a->obj)) {
    aReallyClosestPoint = aCenter;
  } else {
    Renderer_closestPointOnAABB(&a->worldAABB, &bClosestPoint,
                                &aReallyClosestPoint);
  }
  if (Renderer_isDynamicObject(b->obj)) {
    bReallyClosestPoint = bCenter;
  } else {
    Renderer_closestPointOnAABB(&b->worldAABB, &aClosestPoint,
                                &bReallyClosestPoint);
  }

  Renderer_getSeparatingPlane(&aReallyClosestPoint, &bReallyClosestPoint,
                              &separatingPlane);

  planeToADist = Plane_distance(&separatingPlane, &aCenter);
  planeToBDist = Plane_distance(&separatingPlane, &bCenter);
  planeToViewDist = Plane_distance(&separatingPlane, viewPos);

  if ((planeToADist < 0.0) == (planeToBDist < 0.0)) {
    // if A is on the same side of plane as B, probably intersecting
    return 0;
  } else {
    if ((planeToADist < 0.0) == (planeToViewDist < 0.0)) {
      // if A is on the same side of plane as the view, A closer than B
      return -1;
    } else {
      // B closer than A
      return 1;
    }
  }
}

// global variable because qsort's API sucks lol
Vec3d sortWorldComparatorFn_viewPos;
int sortIterations = 0;
int Renderer_sortWorldComparatorFnPaintersSeparatingPlane(const void* a,
                                                          const void* b) {
  RendererSortDistance* sortA = (RendererSortDistance*)a;
  RendererSortDistance* sortB = (RendererSortDistance*)b;
#ifdef DEBUG
  sortIterations++;
  invariant(sortIterations < 1000);
#endif
  // sort far to near for painters algorithm
  if (Renderer_isBackgroundGameObject(sortA->obj) ||
      Renderer_isBackgroundGameObject(sortB->obj)) {
    return sortB->distance - sortA->distance;
  }

  return -Renderer_isCloserBySeparatingPlane(sortA, sortB,
                                             &sortWorldComparatorFn_viewPos);
}

int Renderer_sortWorldComparatorFnPaintersSimple(const void* a, const void* b) {
  RendererSortDistance* sortA = (RendererSortDistance*)a;
  RendererSortDistance* sortB = (RendererSortDistance*)b;

  // sort far to near for painters algorithm
  return sortB->distance - sortA->distance;
}

int Renderer_sortWorldComparatorFnZBuffer(const void* a, const void* b) {
  RendererSortDistance* sortA = (RendererSortDistance*)a;
  RendererSortDistance* sortB = (RendererSortDistance*)b;
  // sort near to far, so we benefit from zbuffer fast bailout
  return sortA->distance - sortB->distance;
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
#if RENDERER_PAINTERS_ALGORITHM
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
#else
  int i;
  // no-op impl which just marks all objects as potentially intersecting
  for (i = 0; i < objectsCount; ++i) {
    objectsIntersecting[i] = TRUE;
  }
#endif
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
    if (obj->modelType == NoneModel || !obj->visible
#if FAKE_GROUND
        || obj->modelType == GroundModel
#endif

    ) {
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
                                 RendererSortDistance* result,
                                 Vec3d* viewPos,
                                 AABB* localAABBs) {
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
      sortDist->distance = Renderer_gameobjectSortDist(sortDist->obj, viewPos);

#if RENDERER_PAINTERS_ALGORITHM
      sortDist->worldAABB = Renderer_getWorldAABB(localAABBs, sortDist->obj);
#endif

      visibleObjectIndex++;
    }
  }

  invariant(visibleObjectIndex == visibleObjectsCount);

  // used for painters algo
  sortWorldComparatorFn_viewPos = *viewPos;
#if RENDERER_PAINTERS_ALGORITHM
#if 1
  sortIterations = 0;
  qsort(result, visibleObjectsCount, sizeof(RendererSortDistance),
        Renderer_sortWorldComparatorFnPaintersSeparatingPlane);
#else
  qsort(result, visibleObjectsCount, sizeof(RendererSortDistance),
        Renderer_sortWorldComparatorFnPaintersSimple);
#endif
#else
  qsort(result, visibleObjectsCount, sizeof(RendererSortDistance),
        Renderer_sortWorldComparatorFnZBuffer);
#endif
}

void RendererZSortList_insertMeshTri(RendererZSortList* list,
                                     GameObject* obj,
                                     MeshTri* meshtri,
                                     Vec3d* viewPos,
                                     Renderer_meshTriDistCallback cb) {
  // - calculate centroid
  // - transform centroid to world (using transform matrix? directly applying
  // components?)
  // - determine bucket for tri
  // - resize bucket if necessary
  // - insert tri

  RendererZSortBucket* bucket;
  float dist = cb(obj, meshtri, viewPos);
  float clampedDist = CLAMP(dist, list->near, (list->far - 0.001));
  int bucketIdx = (clampedDist - list->near) / list->bucketSize;
  invariant(bucketIdx >= 0 && bucketIdx < list->count);

  bucket = &list->buckets[bucketIdx];

  if (bucket->count == bucket->capacity) {
    RendererZSortBucket_expand(bucket);
  }
  invariant(bucket->count < bucket->capacity);

  RendererZSortItem_init(&bucket->items[bucket->count], obj, meshtri);
  bucket->count++;
}

void RendererZSortItem_init(RendererZSortItem* self,
                            GameObject* obj,
                            MeshTri* meshtri) {
  self->obj = obj;
  self->meshtri = meshtri;
}

void RendererZSortBucket_init(RendererZSortBucket* self) {
  self->capacity = 0;
  self->count = 0;
  self->items = NULL;
}

void RendererZSortList_init(RendererZSortList* self, float near, float far) {
  int i;
  // invariant(self->initialized == FALSE);
  self->count = RENDERER_ZSORT_BUCKET_COUNT;

  self->near = near;
  self->far = far;
  self->bucketSize = ((self->far - self->near) / self->count);
  for (i = 0; i < self->count; ++i) {
    RendererZSortBucket_init(&self->buckets[i]);
  }
  self->initialized = TRUE;
}

void RendererZSortList_destroy(RendererZSortList* self) {
  int i;
  invariant(self->initialized == TRUE);

  for (i = 0; i < self->count; ++i) {
    RendererZSortBucket_destroy(&self->buckets[i]);
  }

  self->initialized = FALSE;
}

void RendererZSortBucket_expand(RendererZSortBucket* self) {
  if (self->capacity == 0) {
    invariant(self->items == NULL);
    self->capacity = RENDERER_ZSORT_BUCKET_INIT_CAPACITY;
    self->items = (RendererZSortItem*)malloc((self->capacity) *
                                             sizeof(RendererZSortItem));
    invariant(self->items);
  } else {
    int newCapacity = self->capacity * 2;
    RendererZSortItem* newItems =
        (RendererZSortItem*)malloc((newCapacity) * sizeof(RendererZSortItem));
    memcpy(newItems, self->items, self->count * sizeof(RendererZSortItem));
    free(self->items);
    self->capacity = newCapacity;
    self->items = newItems;
    // self->count is still correct
  }
}

void RendererZSortBucket_destroy(RendererZSortBucket* self) {
  if (self->capacity == 0) {
    invariant(self->items == NULL);
    // nothing to do here
  } else {
    // clean up items
    invariant(self->items);
    free(self->items);
    self->items = NULL;
    self->capacity = 0;
    self->count = 0;
  }
}
