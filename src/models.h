#ifndef MODELS_H
#define MODELS_H

#include "modeltype.h"

extern Gfx* getMeshDisplayListForModelMeshPart(ModelType modelType,
                                               int meshPart);
extern Gfx* getModelDisplayList(ModelType modelType, int subtype);

#endif /* MODELS_H */