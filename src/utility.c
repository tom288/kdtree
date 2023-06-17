#define __USE_MINGW_ANSI_STDIO 1 // Make MinGW printf support size_t with %zu
#include "utility.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h> // RAND_MAX

// In the event where type is not a recognised case we print its hexadecimal
// value as defined in glew.h, rather than the macro string itself.
size_t gl_sizeof(GLenum type)
{
    switch (type)
    {
        case GL_BYTE:
        case GL_UNSIGNED_BYTE:
            return sizeof(GLbyte);
        case GL_SHORT:
        case GL_UNSIGNED_SHORT:
            return sizeof(GLshort);
        case GL_INT_2_10_10_10_REV:
        case GL_INT:
        case GL_UNSIGNED_INT_2_10_10_10_REV:
        case GL_UNSIGNED_INT:
            return sizeof(GLint);
        case GL_FLOAT:
            return sizeof(GLfloat);
        case GL_DOUBLE:
            return sizeof(GLdouble);
        case GL_FIXED:
            return sizeof(GLfixed);
        case GL_HALF_FLOAT:
            return sizeof(GLhalf);
        default:
            fprintf(stderr, "gl_sizeof unknown type with value 0x%X\n", type);
    }

    // Zero is likely to make error cases fatal, but by returning a safer value
    // like 4 we'd be making these cases impossible to programmatically detect.
    return 0;
}

GLboolean _gl_error(char* file, int line)
{
    GLenum error_code;
    while ((error_code = glGetError()) != GL_NO_ERROR)
    {
        const char* error;
        switch (error_code)
        {
            case GL_INVALID_ENUM:
                error = "INVALID_ENUM";
                break;
            case GL_INVALID_VALUE:
                error = "INVALID_VALUE";
                break;
            case GL_INVALID_OPERATION:
                error = "INVALID_OPERATION";
                break;
            case GL_STACK_OVERFLOW:
                error = "STACK_OVERFLOW";
                break;
            case GL_STACK_UNDERFLOW:
                error = "STACK_UNDERFLOW";
                break;
            case GL_OUT_OF_MEMORY:
                error = "OUT_OF_MEMORY";
                break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:
                error = "INVALID_FRAMEBUFFER_OPERATION";
                break;
            default:
                fprintf (
                    stderr,
                    "OpenGL unrecognised error 0x%X @ %s (%d)\n",
                    error_code,
                    file,
                    line
                );
                return GL_TRUE;
        }
        fprintf(stderr, "OpenGL %s @ %s (%d)\n", error, file, line);
    }
    return !!error_code;
}

void _error(const char* file, const char* func, const size_t line, const char* description) {
    fprintf(stderr, "%s, %s, %zu --> %s\n", file, func, line, description);
}

// Be careful if refactoring this to avoid overflow on unix / macOS
GLboolean rand_bool()
{
    return rand() > RAND_MAX / 2;
}

size_t rand_int(size_t sides, GLboolean fair)
{
    if (sides <= 1) return 0;

    size_t random = rand();
    size_t options = RAND_MAX;
    while (sides > options)
    {
        options *= RAND_MAX;
        random *= RAND_MAX;
        random += rand();
    }

    if (fair && random - random % sides + sides > options)
    {
        return rand_int(sides, fair);
    }

    return random % sides;
}

float rand_float()
{
    return (float)rand() / (float)RAND_MAX;
}

void rand_vec3(vec3 v)
{
    for (size_t i = 0; i < 3; ++i)
    {
        v[i] = rand_float();
    }
}

float bias_float(float f, float towards, float strength)
{
    const float signum = (f -= towards) < 0 ? -1 : 1;
    return powf(f * signum, strength) * signum + towards;
}

double min_d(double x, double y)
{
    return x < y ? x : y;
}

size_t rotate_left(size_t n, size_t dist)
{
    return (n << dist) | (n >> (8 * sizeof(size_t) - dist));
}

size_t rotate_left_quarter(size_t n, size_t count)
{
    return rotate_left(n, count * 2 * sizeof(size_t));
}

size_t sampleRandom(Node sample)
{
    typedef union {
        float from;
        size_t to;
    } float_to_size;

    float_to_size x = { .from = sample.min_corner[0] };
    float_to_size y = { .from = sample.min_corner[1] };
    float_to_size width = { .from = sample.size[0] };
    float_to_size height = { .from = sample.size[1] };

    return x.to
        ^ rotate_left_quarter(y.to     , 1)
        ^ rotate_left_quarter(width.to , 2)
        ^ rotate_left_quarter(height.to, 3);
}

// ---

bool for_helper(void* array, size_t array_size, void* value, size_t value_size, Compare cmp, bool any) {
    if (value_size > array_size) error("value length exceeds array length");
    if (array_size % value_size != 0) error("items in the array should be value_size bytes");
    for (size_t index_byte = 0; index_byte + value_size <= array_size; index_byte += value_size) {
        int result = memcmp((char*)array + index_byte, value, value_size);
        if (cmp == COMPARE_EQ && (result == 0) == any) return any;
        if (cmp == COMPARE_LESS && (result < 0) == any) return any;
        if (cmp == COMPARE_MORE && (result > 0) == any) return any;
    }
    return !any;
}

bool for_any(void* array, size_t array_size, void* value, size_t value_size, Compare comparison) {
    return for_helper(array, array_size, value, value_size, comparison, true);
}

bool for_all(void* array, size_t array_size, void* value, size_t value_size, Compare comparison) {
    return for_helper(array, array_size, value, value_size, comparison, false);
}
