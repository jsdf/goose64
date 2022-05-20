
#ifndef _OPTION_H_
#define _OPTION_H_

#define HIGH_RESOLUTION 1
#define HIGH_RESOLUTION_HALF_Y 0
// TODO: switch based on region
#if HIGH_RESOLUTION
#if HIGH_RESOLUTION_HALF_Y
#define VSYNC_FPS 30
#define FRAME_SKIP 2
#else

#define VSYNC_FPS 30
#define FRAME_SKIP 2
#endif
#else  // low resolution
#define VSYNC_FPS 60
#define FRAME_SKIP 1
#endif

// when updating this, also update OptionStrings[]
typedef enum Option {
  OptDebug,
  OptResolution,
  OptAntialias,
  OptSfx,
  OptMusic,
  OptMeshSubdiv,
  OptDeflickerInterlace,
  MAX_OPTION
} Option;

extern int options[MAX_OPTION];
extern char* OptionStrings[];
#endif /* !_OPTION_H_ */