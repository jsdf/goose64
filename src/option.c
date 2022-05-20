
#include "option.h"
#include "constants.h"

char* OptionStrings[] = {
    "OptDebug",               //
    "OptResolution",          //
    "OptAntialias",           //
    "OptSfx",                 //
    "OptMusic",               //
    "OptMeshSubdiv",          //
    "OptDeflickerInterlace",  //
    "MAX_OPTION"              //
};

int options[MAX_OPTION];

void Option_initAll() {
  int i;
  for (i = 0; i < MAX_OPTION; ++i) {
    options[i] = 0;
  }
  options[OptDeflickerInterlace] = 1;
  options[OptAntialias] = !(HIGH_RESOLUTION && !HIGH_RESOLUTION_HALF_Y);
  for (i = 0; i < MAX_OPTION; ++i) {
    if (options[i])
      debugPrintf("%s=%d\n", OptionStrings[i], options[i]);
  }
}
