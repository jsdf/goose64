
#ifndef ITEM_H
#define ITEM_H

#include "gameobject.h"
#include "gametypes.h"

void Item_init(Item* self, GameObject* obj, Game* game);

#ifndef __N64__
void Item_print(Item* self);
#endif

#endif /* !ITEM_H */
