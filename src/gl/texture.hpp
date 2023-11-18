#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#ifdef __APPLE__
#include <OpenGL/gl.h> 
#else
#include <GL/gl.h> 
#endif
 

// Load a .BMP file using our custom loader
GLuint loadBMP_custom(const char* imagepath);

#endif /*TEXTURE_HPP*/
