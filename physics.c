
#include <math.h>
#include <stdlib.h>

#include "collision.h"
#include "constants.h"
#include "physics.h"
#include "trace.h"

#define PHYS_DEBUG 0
#define PHYS_MIN_MOVEMENT 0.5
#define PHYS_COLLISION_MIN_SEPARATION 0.001
#define PHYSICS_MOTION_DAMPENING 0
#define PHYSICS_USE_VERLET_INTEGRATION 0
#define PHYS_MAX_COLLISION_ITERATIONS 10
#define PHYS_DEBUG_PRINT_COLLISIONS 0

void PhysState_init(PhysState* self, PhysWorldData* worldData) {
  self->accumulatedTime = 0.0;
  self->clock = 0.0;
  self->simulationRate = 1.0;
  self->timeScale = 1.0;
  self->dynamicTimestep = TRUE;
  self->worldData = worldData;
}

void PhysBody_init(PhysBody* self,
                   float mass,
                   float radius,
                   Vec3d* position,
                   int id) {
  self->id = id;
  self->mass = mass;
  self->massInverse = 1.0 / mass;
  self->radius = radius;
  self->radiusSquared = radius * radius;
  self->restitution = 1.0;
  self->enabled = TRUE;
  self->position = *position;
  self->prevPosition = *position;
  Vec3d_origin(&self->velocity);
  Vec3d_origin(&self->nonIntegralVelocity);
  Vec3d_origin(&self->acceleration);
  Vec3d_origin(&self->nonIntegralAcceleration);
  Vec3d_origin(&self->prevAcceleration);
}

void PhysBehavior_floorBounce(PhysBody* body, float floorHeight) {
  float opposite;
  Vec3d response;

  opposite = (-1.0) * body->mass;
  if (body->position.y - body->radius < floorHeight) {
    body->position.y = floorHeight + body->radius;
    Vec3d_init(&response, 0.0, body->nonIntegralVelocity.y * opposite, 0.0);
    Vec3d_add(&body->acceleration, &response);
  }
}

void PhysBehavior_floorClamp(PhysBody* body, float floorHeight) {
  if (body->position.y - body->radius < floorHeight) {
    body->position.y = floorHeight + body->radius;
  }
}

int PhysBehavior_worldCollisionResponseStep(PhysBody* body,
                                            PhysWorldData* world) {
  int hasCollision;
  float distanceToIntersect, responseDistance, bodyInFrontOfTriangle;
  SphereTriangleCollision collision;
  Vec3d response, beforePos;

  hasCollision = Collision_testMeshSphereCollision(
      world->worldMeshTris, world->worldMeshTrisLength, &body->position,
      body->radius, world->worldMeshSpatialHash, &collision);

  if (!hasCollision) {
    return FALSE;
  }

#ifndef PHYS_DEBUG
  // if (body->id == 2) {
  //   printf("player collided\n");
  // }
  debugPrintf("body %d collided\n", body->id);
#endif
  distanceToIntersect = collision.distance;

  bodyInFrontOfTriangle =
      Triangle_comparePoint(collision.triangle, &body->position);

  // move away by radius
  Triangle_getNormal(collision.triangle, &response);

  responseDistance = 0;

  if (bodyInFrontOfTriangle >= 0) {
    // center of body is in front of or on face
    responseDistance =
        body->radius + PHYS_COLLISION_MIN_SEPARATION - distanceToIntersect;
  } else {
    // center of body is behind face
    responseDistance =
        body->radius + PHYS_COLLISION_MIN_SEPARATION + distanceToIntersect;
  }

  beforePos = body->position;
#if PHYSICS_USE_VERLET_INTEGRATION
  Vec3d_mulScalar(&response, responseDistance);
  PhysBody_translateWithoutForce(body, &response);
#else
  Vec3d_mulScalar(&response, responseDistance);
  PhysBody_translateWithoutForce(body, &response);
  Vec3d_origin(&body->nonIntegralVelocity);
  // Vec3d_mulScalar(&response, responseDistance * body->mass);
  // PhysBody_applyForce(body, &response);
#endif

#if PHYS_DEBUG_PRINT_COLLISIONS
  if (body->id == 2) {
    printf("player collided with world\n");
  }
  if (body->id == 22) {
    printf("groundskeeper collided with world\n");
  }
#ifdef __cplusplus
  printf(
      "PhysBody id=%d hasCollision tri=%d distanceToIntersect=%f beforePos=%s "
      "afterPos=%s "
      "posInTriangle=%s response=%s "
      "responseDistance=%f bodyInFrontOfTriangle=%f",
      body->id, collision.index, distanceToIntersect,
      Vec3d_toStdString(&beforePos).c_str(),
      Vec3d_toStdString(&body->position).c_str(),
      Vec3d_toStdString(&collision.posInTriangle).c_str(),
      Vec3d_toStdString(&response).c_str(), responseDistance,
      bodyInFrontOfTriangle

  );
#endif
#endif

  return TRUE;
}

void PhysBehavior_collisionSeparationOffset(Vec3d* result,
                                            Vec3d* pos,
                                            float overlap,
                                            float separationForce) {
  Vec3d_copyFrom(result, pos);
  Vec3d_normalise(result);
  Vec3d_mulScalar(result, overlap * separationForce);
}

int PhysBehavior_bodyBodyCollisionResponse(PhysBody* body,
                                           PhysBody* pool,
                                           int numInPool) {
  Vec3d delta, direction, collisionSeparationOffset;
  int i, hasCollision;
  float distanceSquared, radii, distance, overlap, mt, bodySeparationForce,
      otherBodySeparationForce;
  PhysBody* otherBody;

  hasCollision = FALSE;

  Vec3d_origin(&delta);
  Vec3d_origin(&direction);

  for (i = 0, otherBody = pool; i < numInPool; i++, otherBody++) {
    if (body != otherBody && otherBody->enabled) {
      Vec3d_copyFrom(&delta, &otherBody->position);
      Vec3d_sub(&delta, &body->position);
      distanceSquared = Vec3d_magSq(&delta);
      Vec3d_copyFrom(&direction, &delta);
      Vec3d_normalise(&direction);
      radii = body->radius + otherBody->radius;
      if (distanceSquared <= radii * radii) {  // collision
        hasCollision = TRUE;

        distance = sqrtf(distanceSquared);
        overlap = radii - distance - 0.5;
        /* Total mass. */
        mt = body->mass + otherBody->mass;
        /* Distribute collision responses. */
        bodySeparationForce = otherBody->mass / mt;
        otherBodySeparationForce = body->mass / mt;

        /* Move particles so they no longer overlap.*/
        PhysBehavior_collisionSeparationOffset(
            &collisionSeparationOffset, &delta, overlap, -bodySeparationForce);
        Vec3d_add(&body->position, &collisionSeparationOffset);

        PhysBehavior_collisionSeparationOffset(&collisionSeparationOffset,
                                               &delta, overlap,
                                               otherBodySeparationForce);
        Vec3d_add(&otherBody->position, &collisionSeparationOffset);
      }
    }
  }
  return hasCollision;
}

int PhysBehavior_collisionResponseStep(PhysBody* body,
                                       PhysWorldData* world,
                                       PhysBody* pool,
                                       int numInPool) {
  int hasCollision;
  hasCollision = FALSE;

  hasCollision =
      hasCollision || PhysBehavior_worldCollisionResponseStep(body, world);
  return hasCollision;
}

void PhysBehavior_collisionResponse(PhysWorldData* world,
                                    PhysBody* bodies,
                                    int numBodies) {
  int i, k, hasAnyCollision;
  PhysBody* body;

  // int floorHeight;
  // floorHeight = 0.0;
  for (k = 0, body = bodies; k < numBodies; k++, body++) {
    if (body->enabled) {
      // PhysBehavior_floorBounce(body, floorHeight);
      // PhysBehavior_floorClamp(body, floorHeight);
      PhysBehavior_bodyBodyCollisionResponse(body, bodies, numBodies);
    }
  }
  // run multiple iterations, because the response to a collision can create
  // another collision
  for (i = 0; i < PHYS_MAX_COLLISION_ITERATIONS; ++i) {
    hasAnyCollision = FALSE;
    for (k = 0, body = bodies; k < numBodies; k++, body++) {
      if (body->enabled) {
        hasAnyCollision =
            hasAnyCollision ||
            PhysBehavior_collisionResponseStep(body, world, bodies, numBodies);
      }
    }
    if (!hasAnyCollision) {
      break;
    }
  }
#ifndef PHYS_DEBUG
  if (i > 0) {
    debugPrintf("collision response took %d iters\n", i);
  }
  if (hasAnyCollision) {
    debugPrintf(
        "hit PHYS_MAX_COLLISION_ITERATIONS and ended collision response with "
        "collisions remaining\n");
  }
#endif
}

void PhysBody_setEnabled(PhysBody* body, int enabled) {
  if (enabled) {
    body->enabled = TRUE;
    // prevent velocity from movement while disabled
    Vec3d_copyFrom(&body->prevPosition, &body->position);
  } else {
    body->enabled = FALSE;
  }
}

void PhysBehavior_constantForce(PhysBody* body, Vec3d force) {
  Vec3d_add(&body->acceleration, &force);
}

void PhysBody_applyForce(PhysBody* body, Vec3d* force) {
  Vec3d_add(&body->acceleration, force);
}

// move but don't affect velocity
void PhysBody_translateWithoutForce(PhysBody* body, Vec3d* translation) {
  Vec3d_add(&body->position, translation);
  Vec3d_add(&body->prevPosition, translation);
}

void PhysBody_update(PhysBody* self,
                     float dt,
                     float drag,
                     PhysBody* pool,
                     int numInPool,
                     PhysState* physics) {
  Vec3d gravity;
  Vec3d_init(&gravity, 0, physics->worldData->gravity * self->mass, 0);
  // do behaviours
  PhysBehavior_constantForce(self, gravity);  // apply gravity
}

void PhysBody_dampenSmallMovements(PhysBody* body) {
  // dampen small movements
  if (Vec3d_distanceTo(&body->position, &body->prevPosition) <
      PHYS_MIN_MOVEMENT) {
    body->position = body->prevPosition;
    Vec3d_origin(&body->velocity);
    Vec3d_origin(&body->nonIntegralVelocity);
    Vec3d_origin(&body->acceleration);
    Vec3d_origin(&body->prevAcceleration);
  }
}

void PhysBody_integrateMotionVerlet(PhysBody* body, float dt, float drag) {
  Vec3d newPosition;
  Vec3d_origin(&newPosition);
  /* Scale force to mass. */
  Vec3d_mulScalar(&body->acceleration, body->massInverse);
  /* Derive velocity. */
  Vec3d_copyFrom(&body->velocity, &body->position);
  Vec3d_sub(&body->velocity, &body->prevPosition);
  /* Apply friction. */
  Vec3d_mulScalar(&body->velocity, drag);
  /* Apply acceleration force to new position. */
  /* Get integral acceleration, apply to velocity, then apply updated
     velocity to position */
  Vec3d_copyFrom(&newPosition, &body->position);
  Vec3d_mulScalar(&body->acceleration, dt);
  Vec3d_add(&body->velocity, &body->acceleration);
  Vec3d_add(&newPosition, &body->velocity);

  /* Store old position, update position to new position. */
  Vec3d_copyFrom(&body->prevPosition, &body->position);
  Vec3d_copyFrom(&body->position, &newPosition);

#if PHYSICS_MOTION_DAMPENING
  PhysBody_dampenSmallMovements(&body);
#endif

  /* Reset acceleration force. */
  Vec3d_copyFrom(&body->prevAcceleration, &body->acceleration);
  Vec3d_origin(&body->acceleration);
  /* store velocity for use in acc calculations by user code */
  Vec3d_copyFrom(&body->nonIntegralVelocity, &body->velocity);
  Vec3d_mulScalar(&body->nonIntegralVelocity, 1.0 / dt);
}
void PhysBody_integrateMotionSemiImplicitEuler(PhysBody* body,
                                               float dt,
                                               float drag) {
  // acceleration = force / mass
  // accelerationForDT = acceleration * dt
  // velocity = velocity + accelerationForDT
  // velocityForDT = velocity * dt
  // position = position + velocityForDT

  Vec3d newPosition;
  /* Scale force by mass to calculate actual acceleration */
  // acceleration = ( force / mass )
  Vec3d_mulScalar(&body->acceleration, body->massInverse);
  body->nonIntegralAcceleration = body->acceleration;  // for debugging
  // accelerationForDT = acceleration * dt
  Vec3d_mulScalar(&body->acceleration, dt);

  // velocity = velocity + accelerationForDT
  Vec3d_add(&body->nonIntegralVelocity, &body->acceleration);

  // velocityForDT = velocity * dt
  body->velocity = body->nonIntegralVelocity;
  Vec3d_mulScalar(&body->velocity, dt);

  /* Apply friction. */
  Vec3d_mulScalar(&body->velocity, drag);

  // position = position + velocityForDT;
  newPosition = body->position;
  Vec3d_add(&newPosition, &body->velocity);

  /* Store old position, update position to new position. */
  body->prevPosition = body->position;
  body->position = newPosition;

#if PHYSICS_MOTION_DAMPENING
  PhysBody_dampenSmallMovements(body);
#endif

  /* Reset acceleration force. */
  body->prevAcceleration = body->acceleration;
  Vec3d_origin(&body->acceleration);
}

void PhysBody_integrateBodies(PhysBody* bodies,
                              int numBodies,
                              float dt,
                              float drag,
                              PhysState* physics) {
  PhysBody* body;
  int i;
  float profStartObjCollision;
  float profStartWorldCollision;

  profStartObjCollision = CUR_TIME_MS();
  for (i = 0, body = bodies; i < numBodies; i++, body++) {
    if (body->enabled) {
      PhysBody_update(body, dt, drag, bodies, numBodies, physics);
    }
  }
  if (traceEventStarts[PhysObjCollisionTraceEvent] == 0) {
    traceEventStarts[PhysObjCollisionTraceEvent] = profStartObjCollision;
  }
  traceEventDurations[PhysObjCollisionTraceEvent] +=
      (CUR_TIME_MS() - profStartObjCollision);

  for (i = 0, body = bodies; i < numBodies; i++, body++) {
    if (body->enabled /*&& !body->controlled*/) {
#if PHYSICS_USE_VERLET_INTEGRATION
      PhysBody_integrateMotionVerlet(body, dt, drag);
#else
      PhysBody_integrateMotionSemiImplicitEuler(body, dt, drag);
#endif
    }
  }

  profStartWorldCollision = CUR_TIME_MS();
  // do this after so we can fix any world penetration resulting from motion
  // integration
  PhysBehavior_collisionResponse(physics->worldData, bodies, numBodies);
  if (traceEventStarts[PhysWorldCollisionTraceEvent] == 0) {
    traceEventStarts[PhysWorldCollisionTraceEvent] = profStartWorldCollision;
  }
  traceEventDurations[PhysWorldCollisionTraceEvent] +=
      (CUR_TIME_MS() - profStartWorldCollision);
}

void PhysState_step(PhysState* physics,
                    PhysBody* bodies,
                    int numBodies,
                    float now) {
  float time;
  int i;
  float timestep;
  float delta;
  float drag;
  /* Initialise the clock on first step. */
  if (physics->clock == 0.0) {
    physics->clock = now;
  };
  /* Compute delta time since last step. */
  time = now;
  /* fixed delta for debugging */
  time = physics->dynamicTimestep
             ? time
             : physics->clock +
                   16.667 * physics->timeScale * physics->simulationRate;
  delta = time - physics->clock;
  /* sufficient change. */
  if (delta > 0.0) {
    /* Convert time to seconds. */
    delta = delta * 0.001;
    /* Drag is inversely proportional to viscosity. */
    drag = 1.0 - physics->worldData->viscosity;
    /* Update the clock. */
    physics->clock = time;
    /* Increment time accumulatedTime.
       Don't accumulate any additional time if we're already more than 1 second
       behind. This happens when the tab is backgrounded, and if this grows
       large enough we won't be able to ever catch up.
       */
    if (physics->accumulatedTime < 1.0) {
      physics->accumulatedTime = physics->accumulatedTime + delta;
    } else {
#ifndef PHYS_DEBUG
      debugPrintf(
          "Physics: accumulated too much time, not accumulating any more\n");
#endif
    };
    /* Integrate until the accumulatedTime is empty or until the */
    /* maximum amount of iterations per step is reached. */
    i = 0;
    timestep = PHYS_TIMESTEP * physics->timeScale;
    while (physics->accumulatedTime >= timestep && i < PHYS_MAX_STEPS) {
      /* Integrate bodies by fixed timestep. */
      PhysBody_integrateBodies(bodies, numBodies, timestep, drag, physics);
      /* Reduce accumulatedTime by one timestep. */
      physics->accumulatedTime = physics->accumulatedTime - timestep;
      i++;

      // TODO: updating physics step multiple times without running game update
      // step is probably wrong, as forces are reset after each step
      break;
    }
#ifndef PHYS_DEBUG
    debugPrintf("Physics: ran %d timesteps\n", i + 1);
#endif
  }
}

#ifndef __N64__
#include <stdio.h>

void PhysBody_toString(PhysBody* self, char* buffer) {
  char pos[60];
  char vel[60];
  Vec3d_toString(&self->position, pos);
  Vec3d_toString(&self->nonIntegralVelocity, vel);
  sprintf(buffer, "PhysBody id=%d pos=%s vel=%s", self->id, pos, vel);
}
#endif
