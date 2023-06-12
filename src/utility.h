#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include <cglm/cglm.h>
#include "kdtree.h"

// See gl_error for the single intended use of this function
GLboolean _gl_error(char* file, int line);

/// @brief Check for OpenGL runtime errors. Print the first error found,
/// along with filename & line number. Other errors are left unread.
/// @return Whether an error was found
#define gl_error() _gl_error(__FILE__, __LINE__)

void _error(const char* file, const char* func, const size_t line, const char* description);

#define error(description) _error(__FILE__, __func__, __LINE__, description)

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

/// @brief Roll a random dice to get a number between 0 and sides - 1
/// @param sides Number of sides
/// @param fair Whether to ensure the roll is fair for all values of sides
/// @return A random number between 0 and sides - 1
size_t rand_int(size_t sides, GLboolean fair);

/// @brief Rotate n left by dist bytes
/// @param n Value to rotate
/// @param dist Number of bytes to rotate by
/// @return n rotated by dist bytes
size_t rotate_left(size_t n, size_t dist);

/// @brief Rotate n left by n dist quarters
/// @param n Value to rotate
/// @param count Number of quarters to rotate by
/// @return n rotated by dist quarters
size_t rotate_left_quarter(size_t n, size_t count);

/// @brief Sample some randomness for a given node, by treating it as a vec4
/// @param sample Node to sample from
/// @return The random value at that node
size_t sampleRandom(Node sample);

// ---

/// @brief Take the smaller of two values of any equal type
#define min(a, b) \
    __extension__({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
            _a < _b ? _a : _b; })

/// @brief Take the larger of two values of any equal type
#define max(a, b) \
    __extension__({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
            _a > _b ? _a : _b; })

typedef enum {
    COMPARE_EQ,
    COMPARE_LESS,
    COMPARE_MORE
} Compare;

/// @brief true if any element in an array matches the provided value with the given comparison
/// @param array an array of any type, all items are of the same type
/// @param array_size sizeof(array)
/// @param value a value of the same type as the array elements
/// @param value_size sizeof(value)
/// @param comparison use COMPARE_... to specify the type of comparison
/// @return
bool for_any(void* array, size_t array_size, void* value, size_t value_size, Compare comparison);

/// @brief true if all elements in an array matches the provided value with the given comparison
/// @param array an array of any type, all items are of the same type
/// @param array_size sizeof(array)
/// @param value a value of the same type as the array elements
/// @param value_size sizeof(value)
/// @param comparison use COMPARE_... to specify the type of comparison
/// @return
bool for_all(void* array, size_t array_size, void* value, size_t value_size, Compare comparison);
