
#ifndef CHARACTER_H
#define CHARACTER_H

#include "gameobject.h"
#include "gametypes.h"

void Character_init(Character* self,
                    GameObject* obj,
                    Item* defaultActivityItem,
                    Game* game);
void Character_update(Character* self, Game* game);
void Character_updateState(Character* self, Game* game);

void Character_haveItemTaken(Character* self, Item* item);

float Character_topDownAngleMagToObj(Character* self, GameObject* obj);

#ifndef __N64__
void Character_print(Character* self);
void Character_toString(Character* self, char* buffer);
#endif

#endif /* !CHARACTER_H */
