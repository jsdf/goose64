
#ifndef RENDERER_H
#define RENDERER_H

#include "frustum.h"
#include "gameobject.h"

typedef struct RendererSortDistance {
  GameObject* obj;
  float distance;
} RendererSortDistance;

int Renderer_isZBufferedGameObject(GameObject* obj);
int Renderer_isLitGameObject(GameObject* obj);
int Renderer_isAnimatedGameObject(GameObject* obj);

void Renderer_sortVisibleObjects(GameObject* worldObjects,
                                 int worldObjectsCount,
                                 int* worldObjectsVisibility,
                                 int visibleObjectsCount,
                                 RendererSortDistance* result);

int Renderer_cullVisibility(GameObject* worldObjects,
                            int worldObjectsCount,
                            int* worldObjectsVisibility,
                            Frustum* frustum,
                            AABB* localAABBs);
#endif /* !RENDERER_H_ */
