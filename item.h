
#ifndef ITEM_H
#define ITEM_H

#include "gameobject.h"
#include "gametypes.h"

void Item_init(Item* self, GameObject* obj, Game* game);

void Item_take(Item* self, ItemHolder* newHolder);
void Item_drop(Item* self);

void ItemHolder_init(ItemHolder* self,
                     ItemHolderType itemHolderType,
                     void* owner);

#ifndef __N64__
void Item_print(Item* self);
#endif

#endif /* !ITEM_H */
