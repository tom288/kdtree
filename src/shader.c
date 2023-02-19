#define GLEW_STATIC
#include <GL/glew.h>
#include <stdio.h>
#include <stdlib.h> // For exit, calloc, free

// Print any shader compilation or linking errors, return whether errors exist
GLint compile_errors(GLuint id, int is_program, char* path)
{
    const GLsizei max_length = 1024;
    GLint ok;
    GLchar log[max_length];

    if (is_program)
    {
        glGetProgramiv(id, GL_LINK_STATUS, &ok);
        if (!ok)
        {
            glGetProgramInfoLog(id, max_length, NULL, log);
            printf("Failed to link shader program (vertex shader %s)", path);
        }
    }
    else
    {
        glGetShaderiv(id, GL_COMPILE_STATUS, &ok);
        if (!ok)
        {
            glGetShaderInfoLog(id, max_length, NULL, log);
            printf("Failed to compile shader file %s", path);
        }
    }

    if (!ok)
    {
        printf("\n%s\n", log);
    }

    return !ok;
}

// Compile the shader file and return its ID
GLuint compile(char* path, GLenum type)
{
    // Read file at path into buffer (https://stackoverflow.com/a/3747128)
    FILE* file = fopen(path, "rb");
    if (!file)
    {
        fprintf(stderr, "Failed to fopen ");
        perror(path);
        exit(1);
    }

    // Record size of file content
    fseek(file, 0L, SEEK_END);
    long file_size = ftell(file);
    rewind(file);

    // Allocate memory for entire file content
    GLchar* buffer = calloc(1, file_size + 1);
    if (!buffer) {
        fclose(file);
        fprintf(stderr, "Failed to calloc for %s buffer\n", path);
        exit(1);
    }

    // Copy the file content into the buffer
    if (fread(buffer, file_size, 1, file) != 1) {
        fclose(file);
        free(buffer);
        fprintf(stderr, "Failed to fread for %s\n", path);
        exit(1);
    }

    fclose(file);

    GLuint id = glCreateShader(type);
    glShaderSource(id, 1, (const GLchar**)&buffer, NULL);
    glCompileShader(id);
    if (compile_errors(id, 0, path)) {
        exit(1);   
    }

    // Free the memory containing the file content
    free(buffer);
    
    // Return the shader ID so that we may use it in a shader program
    return id;
}

// Create a shader program from 2 or 3 input files
GLuint shader_init(char* vertex, char* fragment, char* geometry)
{
    GLuint vert = compile(vertex, GL_VERTEX_SHADER);
    GLuint frag = compile(fragment, GL_FRAGMENT_SHADER);
    GLuint geom = *geometry
        ? compile(geometry, GL_GEOMETRY_SHADER)
        : 0;

    // Create shader program
    GLuint id = glCreateProgram();
    glAttachShader(id, vert);
    glAttachShader(id, frag);
    if (geom) glAttachShader(id, geom);

    glLinkProgram(id);
    if (compile_errors(id, 1, vertex)) {
        exit(1);   
    }

    // The shaders are now linked to the program and so can be deleted
    glDeleteShader(vert);
    glDeleteShader(frag);
    if (geom) glDeleteShader(geom);

    return id;
}

void shader_use(GLuint id)
{
    glUseProgram(id);
}

void shader_kill(GLuint id)
{
    glDeleteProgram(id);
}

void shader_set_1i(GLuint id, char* name, GLint n)
{
    glUniform1i(glGetUniformLocation(id, name), n);
}

void shader_set_1ui(GLuint id, char* name, GLuint n)
{
    glUniform1ui(glGetUniformLocation(id, name), n);
}

void shader_set_1f(GLuint id, char* name, GLfloat n)
{
    glUniform1f(glGetUniformLocation(id, name), n);
}

void shader_set_2i(GLuint id, char* name, GLint x, GLint y)
{
    glUniform2i(glGetUniformLocation(id, name), x, y);
}

void shader_set_2ui(GLuint id, char* name, GLuint x, GLuint y)
{
    glUniform2ui(glGetUniformLocation(id, name), x, y);
}

void shader_set_2f(GLuint id, char* name, GLfloat x, GLfloat y)
{
    glUniform2f(glGetUniformLocation(id, name), x, y);
}

void shader_set_3i(GLuint id, char* name, GLint x, GLint y, GLint z)
{
    glUniform3i(glGetUniformLocation(id, name), x, y, z);
}

void shader_set_3ui(GLuint id, char* name, GLuint x, GLuint y, GLuint z)
{
    glUniform3ui(glGetUniformLocation(id, name), x, y, z);
}

void shader_set_3f(GLuint id, char* name, GLfloat x, GLfloat y, GLfloat z)
{
    glUniform3f(glGetUniformLocation(id, name), x, y, z);
}

void shader_set_4i(GLuint id, char* name, GLint x, GLint y, GLint z, GLint w)
{
    glUniform4i(glGetUniformLocation(id, name), x, y, z, w);
}

void shader_set_4ui(GLuint id, char* name, GLuint x, GLuint y, GLuint z, GLuint w)
{
    glUniform4ui(glGetUniformLocation(id, name), x, y, z, w);
}

void shader_set_4f(GLuint id, char* name, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
    glUniform4f(glGetUniformLocation(id, name), x, y, z, w);
}

// TODO support cglm vectors and matrices
