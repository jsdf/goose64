
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
    "MAX_MODEL_TYPE",          //
};

ModelProperties modelTypesProperties[] = {
    /* NoneModel */
    {/* mass */ 100, /* radius */ 50.0,
     /* centroidOffset */ {0.0, 0.0, 0.0}, /*scale*/ 1.0, GenericModelType},
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
    {/* mass */ 100, /* radius */ 50.0,
     /* centroidOffset */ {0.0, 0.0, 0.0}, /*scale*/ 1.0, GenericModelType},
    /* UniFloorModel */
    {/* mass */ 100, /* radius */ 50.0,
     /* centroidOffset */ {0.0, 0.0, 0.0}, /*scale*/ 1.0, GenericModelType},
    /* BushModel */
    {/* mass */ 100, /* radius */ 55.0,
     /* centroidOffset */ {0.0, 15.0, 0.0}, /*scale*/ 1.0, GenericModelType},
    /* FlagpoleModel */
    {/* mass */ 100, /* radius */ 50.0,
     /* centroidOffset */ {0.0, 0.0, 0.0}, /*scale*/ 1.0, GenericModelType},
    /* GardenerCharacterModel */
    {/* mass */ 2000, /* radius */ 25.0,
     /* centroidOffset */ {0.0, 22.0, 0.0}, /*scale*/ 1.0, CharacterModelType},
    /* WallModel */
    {/* mass */ 100, /* radius */ 50.0,
     /* centroidOffset */ {0.0, 0.0, 0.0}, /*scale*/ 1.0, GenericModelType},
    /* PlanterModel */
    {/* mass */ 100, /* radius */ 50.0,
     /* centroidOffset */ {0.0, 0.0, 0.0}, /*scale*/ 1.0, GenericModelType},
    /* GroundModel */
    {/* mass */ 100, /* radius */ 50.0,
     /* centroidOffset */ {0.0, 0.0, 0.0}, /*scale*/ 1.0, GenericModelType},
    /* MAX_MODEL_TYPE */
    {/* mass */ 100, /* radius */ 50.0,
     /* centroidOffset */ {0.0, 0.0, 0.0}, /*scale*/ 1.0, GenericModelType},
};
