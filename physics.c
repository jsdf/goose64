
#include <math.h>
#include <stdlib.h>

#include "physics.h"

void PhysState_init(PhysState* self, float viscosity) {
  self->accumulatedTime = 0.0;
  self->clock = 0.0;
  self->viscosity = viscosity;
  self->simulationRate = 1.0;
  self->timeScale = 1.0;
  self->dynamicTimestep = TRUE;
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

void PhysBehavior_collisionSeparationOffset(Vec3d* result,
                                            Vec3d* pos,
                                            float overlap,
                                            float separationForce) {
  Vec3d_copyFrom(result, pos);
  Vec3d_normalise(result);
  Vec3d_mulScalar(result, overlap * separationForce);
}

void PhysBehavior_collision(PhysBody* body, PhysBody* pool, int numInPool) {
  Vec3d delta, direction, collisionSeparationOffset;
  int i;
  float distanceSquared, radii, distance, overlap, mt, bodySeparationForce,
      otherBodySeparationForce;
  PhysBody* otherBody;

  Vec3d_origin(&delta);
  Vec3d_origin(&direction);

  for (i = 0, otherBody = pool; i < numInPool; i++, otherBody++) {
    if (body != otherBody) {
      Vec3d_copyFrom(&delta, &otherBody->position);
      Vec3d_sub(&delta, &body->position);
      distanceSquared = Vec3d_magSq(&delta);
      Vec3d_copyFrom(&direction, &delta);
      Vec3d_normalise(&direction);
      radii = body->radius + otherBody->radius;
      if (distanceSquared <= radii * radii) {
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
                     int numInPool) {
  int floorHeight;
  Vec3d gravity;
  Vec3d_init(&gravity, 0, -98.0 * self->mass, 0);
  // do behaviours
  PhysBehavior_constantForce(self, gravity);
  PhysBehavior_collision(self, pool, numInPool);
  floorHeight = 0.0;
  // PhysBehavior_floorBounce(self,floorHeight);
  PhysBehavior_floorClamp(self, floorHeight);
}

void PhysBody_integrateMotion(PhysBody* body, float dt, float drag) {
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
  /* Reset acceleration force. */
  Vec3d_copyFrom(&body->prevAcceleration, &body->acceleration);
  Vec3d_origin(&body->acceleration);
  /* store velocity for use in acc calculations by user code */
  Vec3d_copyFrom(&body->nonIntegralVelocity, &body->velocity);
  Vec3d_mulScalar(&body->nonIntegralVelocity, 1.0 / dt);
}

void PhysBody_integrateBodies(PhysBody* bodies,
                              int numBodies,
                              float dt,
                              float drag) {
  PhysBody* body;
  int i;

  for (i = 0, body = bodies; i < numBodies; i++, body++) {
    if (body->enabled) {
      PhysBody_update(body, dt, drag, bodies, numBodies);
    }
  }

  for (i = 0, body = bodies; i < numBodies; i++, body++) {
    if (body->enabled) {
      PhysBody_integrateMotion(body, dt, drag);
    }
  }
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
    drag = 1.0 - physics->viscosity;
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
#ifndef __N64__
      printf("Physics: accumulated too much time, not accumulating any more\n");
#endif
    };
    /* Integrate until the accumulatedTime is empty or until the */
    /* maximum amount of iterations per step is reached. */
    i = 0;
    timestep = PHYS_TIMESTEP * physics->timeScale;
    while (physics->accumulatedTime >= timestep && i < PHYS_MAX_STEPS) {
      /* Integrate bodies by fixed timestep. */
      PhysBody_integrateBodies(bodies, numBodies, timestep, drag);
      /* Reduce accumulatedTime by one timestep. */
      physics->accumulatedTime = physics->accumulatedTime - timestep;
      i++;
    }
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
