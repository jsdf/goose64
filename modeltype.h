
#ifndef MODELTYPE_H
#define MODELTYPE_H

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

#ifndef __N64__

extern char *ModelTypeStrings[];
#endif

#endif /* !MODELTYPE_H */
