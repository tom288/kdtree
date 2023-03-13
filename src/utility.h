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

/// @brief Generate a random boolean
/// @return Random boolean
GLboolean rand_bool(void);

/// @brief Generate a random float 0 <= f <= 1
/// @return Random float
float rand_float(void);

/// @brief Assign independent random vector components 0 <= f <= 1
/// @param v Vector to randomise
void rand_vec3(vec3 v);

/// @brief Bias a float towards some value with some strength
/// @param f Value to apply a bias to
/// @param towards Value to bias towards
/// @param strength Strength of bias (>= 1) or 'anti-bias' (>0, <= 1)
/// @return Value with bias applied
float bias_float(float f, float towards, float strength);

/// @brief Take the minimum of two doubles
/// @param x 
/// @param y 
/// @return Minimum double
double min_d(double x, double y);

/// @brief Roll a random dice to get a number between 0 and sides - 1
/// @param sides Number of sides
/// @param fair Whether to ensure the roll is fair for all values of sides
/// @return A random number between 0 and sides - 1
size_t rand_int(size_t sides, GLboolean fair);
