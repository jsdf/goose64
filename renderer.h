
#ifndef RENDERER_H
#define RENDERER_H

#include "gameobject.h"

int Renderer_isZBufferedGameObject(GameObject* obj);
int Renderer_isLitGameObject(GameObject* obj);
float Renderer_gameobjectSortDist(GameObject* obj);
int Renderer_gameobjectDistComparatorFn(const void* a, const void* b);
void Renderer_sortWorldObjects(GameObject** sortedObjects,
                               int sortedObjectsSize);
#endif /* !RENDERER_H_ */
