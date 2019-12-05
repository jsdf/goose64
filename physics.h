
#ifndef PHYSICS_H
#define PHYSICS_H

#include "constants.h"
#include "vec3d.h"

#define PHYS_MAX_STEPS 4

#define PHYS_TIMESTEP 1.0 / 60.0

typedef struct PhysState {
  float accumulatedTime;
  float clock;
  float viscosity;
  float simulationRate;
  float timeScale;
  int dynamicTimestep;  // boolean
} PhysState;

typedef struct PhysBody {
  int id;
  float mass;
  float massInverse;
  float radius;
  float radiusSquared;
  float restitution;
  int enabled;
  Vec3d prevPosition;
  Vec3d position;
  Vec3d velocity;
  Vec3d nonIntegralVelocity;
  Vec3d acceleration;
  Vec3d prevAcceleration;
} PhysBody;

void PhysState_init(PhysState* self, float viscosity);

void PhysState_step(PhysState* physics,
                    PhysBody* bodies,
                    int numBodies,
                    float now);

void PhysBody_init(PhysBody* self,
                   float mass,
                   float radius,
                   Vec3d* position,
                   int id);

void PhysBody_applyForce(PhysBody* body, Vec3d* force);
void PhysBody_translateWithoutForce(PhysBody* body, Vec3d* translation);
void PhysBody_setEnabled(PhysBody* body, int enabled);

#ifndef __N64__
#include <stdio.h>

void PhysBody_toString(PhysBody* self, char* buffer);
#endif

#endif /* !PHYSICS_H */
