#ifndef SEGMENTS_H
#define SEGMENTS_H

#include "constants.h"

// define stuff like _modelsSegmentRomStart
EXTERN_SEGMENT(models);
EXTERN_SEGMENT(sprites);
EXTERN_SEGMENT(collision);

EXTERN_SEGMENT(wbank);
EXTERN_SEGMENT(pbank);
EXTERN_SEGMENT(sfx);

#define WBANK_START _wbankSegmentRomStart
#define PBANK_START _pbankSegmentRomStart
#define PBANK_END _pbankSegmentRomEnd
#define SFX_START _sfxSegmentRomStart
#define SFX_END _sfxSegmentRomEnd

#endif /* SEGMENTS_H */