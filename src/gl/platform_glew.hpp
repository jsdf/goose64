
#ifndef PLATFORM_GLEW_H_
#define PLATFORM_GLEW_H_

// emscripten doesn't need glew
#ifdef __EMSCRIPTEN__
#include "platform_gl.hpp"
#else
#include <GL/glew.h>
#include "platform_gl.hpp"
#endif

#endif // PLATFORM_GLEW_H_