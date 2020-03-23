
#include "modeltype.h"

char* ModelTypeStrings[] = {
    "NoneModel",               //
    "GooseModel",              //
    "BookItemModel",           //
    "HomeworkItemModel",       //
    "CakeItemModel",           //
    "UniBldgModel",            //
    "UniFloorModel",           //
    "BushModel",               //
    "FlagpoleModel",           //
    "GardenerCharacterModel",  //
    "WallModel",               //
    "PlanterModel",            //
    "GroundModel",             //
    "WaterModel",              //
    "RockModel",               //
    "MAX_MODEL_TYPE",          //
};

#define DEFAULT_MODEL_PROPERTIES                                  \
  { /* mass */                                                    \
    100, /* radius */ 50.0, /* centroidOffset */ {0.0, 0.0, 0.0}, \
        /*scale*/ 1.0, GenericModelType                           \
  }

ModelProperties modelTypesProperties[] = {
    /* NoneModel */
    DEFAULT_MODEL_PROPERTIES,
    /* GooseModel */
    {/* mass */ 200, /* radius */ 25.0,
     /* centroidOffset */ {0.0, 15.0, 0.0}, /*scale*/ 0.7, PlayerModelType},
    /* BookItemModel */
    {/* mass */ 10, /* radius */ 9.0,
     /* centroidOffset */ {0.0, 0.0, 0.0}, /*scale*/ 1.0, ItemModelType},
    /* HomeworkItemModel */
    {/* mass */ 100, /* radius */ 50.0,
     /* centroidOffset */ {0.0, 0.0, 0.0}, /*scale*/ 1.0, ItemModelType},
    /* CakeItemModel */
    {/* mass */ 100, /* radius */ 50.0,
     /* centroidOffset */ {0.0, 0.0, 0.0}, /*scale*/ 1.0, ItemModelType},
    /* UniBldgModel */
    DEFAULT_MODEL_PROPERTIES,
    /* UniFloorModel */
    DEFAULT_MODEL_PROPERTIES,
    /* BushModel */
    {/* mass */ 100, /* radius */ 55.0,
     /* centroidOffset */ {0.0, 15.0, 0.0}, /*scale*/ 1.0, GenericModelType},
    /* FlagpoleModel */
    DEFAULT_MODEL_PROPERTIES,
    /* GardenerCharacterModel */
    {/* mass */ 2000, /* radius */ 25.0,
     /* centroidOffset */ {0.0, 22.0, 0.0}, /*scale*/ 1.0, CharacterModelType},
    /* WallModel */
    DEFAULT_MODEL_PROPERTIES,
    /* PlanterModel */
    DEFAULT_MODEL_PROPERTIES,
    /* GroundModel */
    DEFAULT_MODEL_PROPERTIES,
    /* WaterModel */
    DEFAULT_MODEL_PROPERTIES,
    /* RockModel */
    DEFAULT_MODEL_PROPERTIES,
    /* MAX_MODEL_TYPE */
    DEFAULT_MODEL_PROPERTIES,
};
