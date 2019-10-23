
#ifndef PLAYER_H
#define PLAYER_H

#include "gameobject.h"
#include "gametypes.h"
#include "input.h"

void Player_init(Player* self, GameObject* obj);
void Player_update(Player* self, Input* input);

#ifndef __N64__
void Player_print(Player* self);
#endif

#endif /* !PLAYER_H */
