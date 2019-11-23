
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

#endif /* !PHYSICS_H */
