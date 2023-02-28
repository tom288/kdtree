#include "utility.h"
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

// Be careful if refactoring this to avoid overflow on unix / macOS
GLboolean rand_bool()
{
    return rand() > RAND_MAX / 2;
}

float rand_float()
{
    return (float)rand() / (float)RAND_MAX;
}

void rand_vec3(vec3 *v)
{
    for (size_t i = 0; i < 3; i++)
    {
        (*v)[i] = rand_float();
    }
}

float bias_float(float f, float towards, float strength)
{
    const float signum = (f -= towards) < 0 ? -1 : 1;
    return powf(f * signum, strength) * signum + towards;
}
