
#ifndef GAMEUTILS_H
#define GAMEUTILS_H
#include "constants.h"
#include "gameobject.h"
#include "vec3d.h"

float GameUtils_lerpDegrees(float start, float end, float amount);
float GameUtils_fclamp(float x, float lower, float upper);
float GameUtils_rotateTowardsClamped(float from,
                                     float to,
                                     float maxSpeed  // must be positive
);

void GameUtils_directionFromTopDownAngle(float angle, Vec3d* result);

int GameUtils_inWater(GameObject* obj);

#endif /* !GAMEUTILS_H */
