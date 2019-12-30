
#include "modeltype.h"

#ifndef __N64__

char* ModelTypeStrings[] = {
    "NoneModel",                    //
    "GooseModel",                   //
    "BookItemModel",                //
    "HomeworkItemModel",            //
    "CakeItemModel",                //
    "UniBldgModel",                 //
    "UniFloorModel",                //
    "BushModel",                    //
    "FlagpoleModel",                //
    "GroundskeeperCharacterModel",  //
    "MAX_MODEL_TYPE",               //
};
#endif

ModelProperties modelTypesProperties[] = {
    /* NoneModel */
    {/* mass */ 100, /* radius */ 50.0,
     /* centroidOffset */ {0.0, 0.0, 0.0}},
    /* GooseModel */
    {/* mass */ 200, /* radius */ 25.0,
     /* centroidOffset */ {0.0, 15.0, 0.0}},
    /* BookItemModel */
    {/* mass */ 10, /* radius */ 9.0,
     /* centroidOffset */ {0.0, 0.0, 0.0}},
    /* HomeworkItemModel */
    {/* mass */ 100, /* radius */ 50.0,
     /* centroidOffset */ {0.0, 0.0, 0.0}},
    /* CakeItemModel */
    {/* mass */ 100, /* radius */ 50.0,
     /* centroidOffset */ {0.0, 0.0, 0.0}},
    /* UniBldgModel */
    {/* mass */ 100, /* radius */ 50.0,
     /* centroidOffset */ {0.0, 0.0, 0.0}},
    /* UniFloorModel */
    {/* mass */ 100, /* radius */ 50.0,
     /* centroidOffset */ {0.0, 0.0, 0.0}},
    /* BushModel */
    {/* mass */ 100, /* radius */ 55.0,
     /* centroidOffset */ {0.0, 15.0, 0.0}},
    /* FlagpoleModel */
    {/* mass */ 100, /* radius */ 50.0,
     /* centroidOffset */ {0.0, 0.0, 0.0}},
    /* GroundskeeperCharacterModel */
    {/* mass */ 2000, /* radius */ 25.0,
     /* centroidOffset */ {0.0, 22.0, 0.0}},
    /* MAX_MODEL_TYPE */
    {/* mass */ 100, /* radius */ 50.0,
     /* centroidOffset */ {0.0, 0.0, 0.0}},
};
