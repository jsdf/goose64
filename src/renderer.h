
#ifndef RENDERER_H
#define RENDERER_H

#include "frustum.h"
#include "gameobject.h"
#include "mesh.h"
#include "n64compat.h"

// when painter's alg enabled we change render order and disable z buffer for
// some objects that don't need it
#define RENDERER_PAINTERS_ALGORITHM 1
// just use solid color fill instead of rendering ground
#define RENDERER_FAKE_GROUND 1

#define RENDERER_ZSORT_BUCKET_COUNT 50
#define RENDERER_ZSORT_BUCKET_INIT_CAPACITY 100

typedef struct RendererSortDistance {
  GameObject* obj;
  float distance;
  AABB worldAABB;
} RendererSortDistance;

typedef struct RendererZSortItem {
  GameObject* obj;  // object this item should be transformed relative to
  MeshTri* meshtri;
  // Vtx_tn* vertTable;
  // int vertIdx[3]; // 3 verts of tri
} RendererZSortItem;

typedef struct RendererZSortBucket {
  int capacity;
  int count;
  RendererZSortItem* items;
} RendererZSortBucket;

typedef struct RendererZSortList {
  int count;
  int initialized;
  float near;
  float far;
  float bucketSize;
  RendererZSortBucket buckets[RENDERER_ZSORT_BUCKET_COUNT];
} RendererZSortList;

typedef float (*Renderer_meshTriDistCallback)(GameObject* obj,
                                              MeshTri* meshtri,
                                              Vec3d* viewPos);
void RendererZSortList_insertMeshTri(RendererZSortList* list,
                                     GameObject* obj,
                                     MeshTri* meshtri,
                                     Vec3d* viewPos,
                                     Renderer_meshTriDistCallback cb);
// get vert tables & tris for GameObject
// for each tri, for each calculate centroid
// transform centroid to world (using transform matrix? directly applying
// components?) determine bucket for tri resize bucket if necessary insert tri

void RendererZSortItem_init(RendererZSortItem* self,
                            GameObject* obj,
                            MeshTri* meshtri);
void RendererZSortBucket_init(RendererZSortBucket* self);
void RendererZSortBucket_expand(RendererZSortBucket* self);
void RendererZSortList_init(RendererZSortList* self, float near, float far);
void RendererZSortList_destroy(RendererZSortList* self);
void RendererZSortBucket_destroy(RendererZSortBucket* self);

int Renderer_isDynamicObject(GameObject* obj);
int Renderer_isZBufferedGameObject(GameObject* obj);
int Renderer_isZWriteGameObject(GameObject* obj);
int Renderer_isBackgroundGameObject(GameObject* obj);

int Renderer_isLitGameObject(GameObject* obj);
int Renderer_isAnimatedGameObject(GameObject* obj);

void Renderer_sortVisibleObjects(GameObject* worldObjects,
                                 int worldObjectsCount,
                                 int* worldObjectsVisibility,
                                 int visibleObjectsCount,
                                 RendererSortDistance* result,
                                 Vec3d* viewPos,
                                 AABB* localAABBs);

int Renderer_cullVisibility(GameObject* worldObjects,
                            int worldObjectsCount,
                            int* worldObjectsVisibility,
                            Frustum* frustum,
                            AABB* localAABBs);

void Renderer_calcIntersecting(int* objectsIntersecting,
                               int objectsCount,
                               RendererSortDistance* sortedObjects,
                               AABB* localAABBs);

void Renderer_getSeparatingPlane(Vec3d* a, Vec3d* b, Plane* separatingPlane);

int Renderer_isCloserBySeparatingPlane(RendererSortDistance* a,
                                       RendererSortDistance* b,
                                       Vec3d* viewPos);

void Renderer_closestPointOnAABB(AABB* b,
                                 /* sourcePoint*/ Vec3d* p,
                                 /* result */ Vec3d* q);

AABB Renderer_getWorldAABB(AABB* localAABBs, GameObject* obj);
#endif /* !RENDERER_H_ */
