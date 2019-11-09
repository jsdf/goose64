
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
    {/* radius */ 50.0,
     /* centroidOffset */ {0.0, 0.0, 0.0}},
    /* GooseModel */
    {/* radius */ 50.0,
     /* centroidOffset */ {0.0, 20.0, 0.0}},
    /* BookItemModel */
    {/* radius */ 20.0,
     /* centroidOffset */ {0.0, 0.0, 0.0}},
    /* HomeworkItemModel */
    {/* radius */ 50.0,
     /* centroidOffset */ {0.0, 0.0, 0.0}},
    /* CakeItemModel */
    {/* radius */ 50.0,
     /* centroidOffset */ {0.0, 0.0, 0.0}},
    /* UniBldgModel */
    {/* radius */ 50.0,
     /* centroidOffset */ {0.0, 0.0, 0.0}},
    /* UniFloorModel */
    {/* radius */ 50.0,
     /* centroidOffset */ {0.0, 0.0, 0.0}},
    /* BushModel */
    {/* radius */ 55.0,
     /* centroidOffset */ {0.0, 15.0, 0.0}},
    /* FlagpoleModel */
    {/* radius */ 50.0,
     /* centroidOffset */ {0.0, 0.0, 0.0}},
    /* GroundskeeperCharacterModel */
    {/* radius */ 50.0,
     /* centroidOffset */ {0.0, 70.0, 0.0}},
    /* MAX_MODEL_TYPE */
    {/* radius */ 50.0,
     /* centroidOffset */ {0.0, 0.0, 0.0}},
};
