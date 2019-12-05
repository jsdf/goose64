
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
  MAX_MODEL_TYPE
} ModelType;

typedef struct ModelProperties {
  float mass;
  float radius;
  Vec3d centroidOffset;
} ModelProperties;

extern ModelProperties modelTypesProperties[];

#ifndef __N64__

extern char* ModelTypeStrings[];
#endif

#endif /* !MODELTYPE_H */
