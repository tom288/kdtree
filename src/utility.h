#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

size_t gl_sizeof(GLenum type);
GLenum _gl_error(char* file, int line);
#define gl_error() _gl_error(__FILE__, __LINE__)
