
#ifndef PLAYER_H
#define PLAYER_H

#include "gameobject.h"
#include "gametypes.h"
#include "input.h"

#define GOOSE_SPEED 5.0F

#define GOOSE_MAX_TURN_SPEED 15.0f

void Player_init(Player* self, GameObject* obj);
void Player_update(Player* self, Input* input, Game* game);

void Player_haveItemTaken(Player* self);

#ifndef __N64__
void Player_print(Player* self);
#endif

#endif /* !PLAYER_H */
