#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include <cglm/cglm.h>

// See gl_error for the single intended use of this function
GLboolean _gl_error(char* file, int line);

/// @brief Check for OpenGL runtime errors. Print the first error found,
/// along with filename & line number. Other errors are left unread.
/// @return Whether an error was found
#define gl_error() _gl_error(__FILE__, __LINE__)

/// @brief Calculate size of type described by OpenGL enum
/// @param type
/// @return Size in bytes
size_t gl_sizeof(GLenum type);

GLboolean rand_bool();

float rand_float();

void rand_vec3(vec3* v);

float bias_float(float f, float towards, float strength);
