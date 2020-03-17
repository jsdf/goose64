
#ifndef RENDERER_H
#define RENDERER_H

#include "gameobject.h"

typedef struct RendererSortDistance {
  GameObject* obj;
  float distance;
} RendererSortDistance;

int Renderer_isZBufferedGameObject(GameObject* obj);
int Renderer_isLitGameObject(GameObject* obj);
int Renderer_isAnimatedGameObject(GameObject* obj);
void Renderer_sortWorldObjects(GameObject* objects,
                               RendererSortDistance* result,
                               int objectsCount);
#endif /* !RENDERER_H_ */
