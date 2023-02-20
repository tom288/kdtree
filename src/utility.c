#define GLEW_STATIC
#include <GL/glew.h>
#include <stdio.h>

// Check for an OpenGL runtime error
GLenum _gl_error(char* file, int line)
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
                error = "UNKNOWN ERROR";
                break;
        }
        fprintf(stderr, "OpenGL %s @ %s (%d)\n", error, file, line);
    }
    return !!error_code;
}
