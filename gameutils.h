
#ifndef GAMEUTILS_H
#define GAMEUTILS_H

float GameUtils_lerpDegrees(float start, float end, float amount);
float GameUtils_fclamp(float x, float lower, float upper);
float GameUtils_rotateTowardsClamped(float from,
                                     float to,
                                     float maxSpeed  // must be positive
);

#endif /* !GAMEUTILS_H */
