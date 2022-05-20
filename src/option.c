
#include "option.h"

char* OptionStrings[] = {
    "OptDebug",       //
    "OptResolution",  //
    "OptAntialias",   //
    "OptSfx",         //
    "OptMusic",       //
    "OptMeshSubdiv",  //
    "MAX_OPTION"      //
};

int options[MAX_OPTION];

void Option_initAll() {
  int i;
  for (i = 0; i < MAX_OPTION; ++i) {
    options[i] = 0;
  }
  options[OptDeflickerInterlace] = 0;
  // options[OptAntialias] = ANTIALIASING;
}
