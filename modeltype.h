
#ifndef MODELTYPE_H
#define MODELTYPE_H

#include "vec3d.h"

// when updating this, also update ModelTypeStrings[]
typedef enum ModelType {
  NoneModel,
  GooseModel,
  BookItemModel,
  HomeworkItemModel,
  CakeItemModel,
  UniBldgModel,
  UniFloorModel,
  BushModel,
  FlagpoleModel,
  GroundskeeperCharacterModel,
  WallModel,
  PlanterModel,
  MAX_MODEL_TYPE
} ModelType;

typedef struct ModelProperties {
  float mass;
  float radius;
  Vec3d centroidOffset;
} ModelProperties;

extern ModelProperties modelTypesProperties[];

extern char* ModelTypeStrings[];

#endif /* !MODELTYPE_H */
