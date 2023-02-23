#include "shader.h"
#include <stdio.h>
#include <stdlib.h> // calloc, free

// The term 'shader' is ambiguous. It could refer to a single shader file such
// as a vertex shader named *.vert or a fragment shader named *.frag, but it
// could also refer to the whole shader program which is formed by compiling
// and linking these shader source files. This file encapsulates all the work
// done using shader source files, and exposes shader program functionality.

/// @brief Print any shader compilation or linking errors
/// @param id Shader id or shader program id
/// @param is_program Whether the id is of a shader program
/// @param path Path from project root to shader file (.frag for programs)
/// @return Whether an error occurred
GLint compile_error(GLuint id, int is_program, char* path)
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
            fprintf(stderr, "Failed to link shader program (vertex shader %s)", path);
        }
    }
    else
    {
        glGetShaderiv(id, GL_COMPILE_STATUS, &ok);
        if (!ok)
        {
            glGetShaderInfoLog(id, max_length, NULL, log);
            fprintf(stderr, "Failed to compile shader file %s", path);
        }
    }

    if (!ok)
    {
        fprintf(stderr, "\n%s\n", log);
    }

    return !ok;
}

/// @brief Compile a shader file
/// @param path Path from project root to shader file
/// @param type Shader type
/// @return ID of shader created, or 0 in the event of an error
GLuint compile(char* path, GLenum type)
{
    // Read file at path into buffer (https://stackoverflow.com/a/3747128)
    FILE* const file = fopen(path, "rb");
    if (!file)
    {
        fprintf(stderr, "Failed to fopen ");
        perror(path);
        return 0;
    }

    // Record size of file content
    fseek(file, 0L, SEEK_END);
    const long file_size = ftell(file);
    rewind(file);

    // Allocate memory for entire file content
    GLchar* const buffer = calloc(1, file_size + 1);
    if (!buffer)
    {
        fclose(file);
        fprintf(stderr, "Failed to calloc for %s buffer\n", path);
        return 0;
    }

    // Copy the file content into the buffer
    if (fread(buffer, file_size, 1, file) != 1)
    {
        fclose(file);
        free(buffer);
        fprintf(stderr, "Failed to fread for %s\n", path);
        return 0;
    }

    fclose(file);

    GLuint id = glCreateShader(type);
    glShaderSource(id, 1, (const GLchar**)&buffer, NULL);
    glCompileShader(id);
    if (compile_error(id, 0, path))
    {
        id = 0;
    }

    // Free the memory containing the file content
    free(buffer);

    // Return the shader ID so that we may use it in a shader program
    return id;
}

// A shader or shader program ID of 0 is silently ignored by OpenGL
void shader_kill(Shader* shader)
{
    glDeleteProgram(shader->id);
    shader->id = 0;
    shader->ok = GL_FALSE;
}

// The geometry path is made optional by having checks for values NULL and ""
Shader shader_init(char* vertex, char* fragment, char* geometry)
{
    const GLuint vert = compile(vertex, GL_VERTEX_SHADER);
    const GLuint frag = compile(fragment, GL_FRAGMENT_SHADER);
    const GLuint geom = geometry && *geometry
        ? compile(geometry, GL_GEOMETRY_SHADER)
        : 0;

    Shader shader = {
        .id = 0,
        .ok = vert && frag && (!geometry || !*geometry || geom),
    };

    if (shader.ok)
    {
        // Create shader program
        shader.id = glCreateProgram();
        glAttachShader(shader.id, vert);
        glAttachShader(shader.id, frag);
        if (geom) glAttachShader(shader.id, geom);
        glLinkProgram(shader.id);
    }

    // The shaders are linked to the program and so can now be deleted.
    // Values of 0 are silently ignored, so there's no need to check.
    glDeleteShader(vert);
    glDeleteShader(frag);
    glDeleteShader(geom);

    // For program linking errors lets print the fragment shader path,
    // as this shader is likely to have the highest specificity
    if (shader.ok && compile_error(shader.id, 1, fragment))
    {
        shader_kill(&shader);
    }

    return shader;
}

// See https://docs.gl/gl3/glUseProgram for details on errors and mutation
void shader_use(Shader shader)
{
    glUseProgram(shader.id);
}

// These probably ought to be generated using a macro, but that would involve
// generating the header declarations too, and in the meantime it's not too hard
// to use an editor that supports multiple cursors to manage all of this.

GLboolean shader_set_1i(Shader shader, char* name, GLint n)
{
    const GLint location = glGetUniformLocation(shader.id, name);
    if (location == -1)
    {
        fprintf(stderr, "Failed to find uniform %s in shader_set_1ui\n", name);
    }
    else
    {
        glUniform1i(location, n);
    }
    return location != -1;
}

GLboolean shader_set_1ui(Shader shader, char* name, GLuint n)
{
    const GLint location = glGetUniformLocation(shader.id, name);
    if (location == -1)
    {
        fprintf(stderr, "Failed to find uniform %s in shader_set_1ui\n", name);
    }
    else
    {
        glUniform1ui(location, n);
    }
    return location != -1;
}

GLboolean shader_set_1f(Shader shader, char* name, GLfloat n)
{
    const GLint location = glGetUniformLocation(shader.id, name);
    if (location == -1)
    {
        fprintf(stderr, "Failed to find uniform %s in shader_set_1f\n", name);
    }
    else
    {
        glUniform1f(location, n);
    }
    return location != -1;
}

GLboolean shader_set_2i(Shader shader, char* name, GLint x, GLint y)
{
    const GLint location = glGetUniformLocation(shader.id, name);
    if (location == -1)
    {
        fprintf(stderr, "Failed to find uniform %s in shader_set_2i\n", name);
    }
    else
    {
        glUniform2i(location, x, y);
    }
    return location != -1;
}

GLboolean shader_set_2ui(Shader shader, char* name, GLuint x, GLuint y)
{
    const GLint location = glGetUniformLocation(shader.id, name);
    if (location == -1)
    {
        fprintf(stderr, "Failed to find uniform %s in shader_set_2ui\n", name);
    }
    else
    {
        glUniform2ui(location, x, y);
    }
    return location != -1;
}

GLboolean shader_set_2f(Shader shader, char* name, GLfloat x, GLfloat y)
{
    const GLint location = glGetUniformLocation(shader.id, name);
    if (location == -1)
    {
        fprintf(stderr, "Failed to find uniform %s in shader_set_2f\n", name);
    }
    else
    {
        glUniform2f(location, x, y);
    }
    return location != -1;
}

GLboolean shader_set_3i(Shader shader, char* name, GLint x, GLint y, GLint z)
{
    const GLint location = glGetUniformLocation(shader.id, name);
    if (location == -1)
    {
        fprintf(stderr, "Failed to find uniform %s in shader_set_3i\n", name);
    }
    else
    {
        glUniform3i(location, x, y, z);
    }
    return location != -1;
}

GLboolean shader_set_3ui(Shader shader, char* name, GLuint x, GLuint y, GLuint z)
{
    const GLint location = glGetUniformLocation(shader.id, name);
    if (location == -1)
    {
        fprintf(stderr, "Failed to find uniform %s in shader_set_3ui\n", name);
    }
    else
    {
        glUniform3ui(location, x, y, z);
    }
    return location != -1;
}

GLboolean shader_set_3f(Shader shader, char* name, GLfloat x, GLfloat y, GLfloat z)
{
    const GLint location = glGetUniformLocation(shader.id, name);
    if (location == -1)
    {
        fprintf(stderr, "Failed to find uniform %s in shader_set_3f\n", name);
    }
    else
    {
        glUniform3f(location, x, y, z);
    }
    return location != -1;
}

GLboolean shader_set_4i(Shader shader, char* name, GLint x, GLint y, GLint z, GLint w)
{
    const GLint location = glGetUniformLocation(shader.id, name);
    if (location == -1)
    {
        fprintf(stderr, "Failed to find uniform %s in shader_set_4i\n", name);
    }
    else
    {
        glUniform4i(location, x, y, z, w);
    }
    return location != -1;
}

GLboolean shader_set_4ui(Shader shader, char* name, GLuint x, GLuint y, GLuint z, GLuint w)
{
    const GLint location = glGetUniformLocation(shader.id, name);
    if (location == -1)
    {
        fprintf(stderr, "Failed to find uniform %s in shader_set_4ui\n", name);
    }
    else
    {
        glUniform4ui(location, x, y, z, w);
    }
    return location != -1;
}

GLboolean shader_set_4f(Shader shader, char* name, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
    const GLint location = glGetUniformLocation(shader.id, name);
    if (location == -1)
    {
        fprintf(stderr, "Failed to find uniform %s in shader_set_4f\n", name);
    }
    else
    {
        glUniform4f(location, x, y, z, w);
    }
    return location != -1;
}

GLboolean shader_set_1iv(Shader shader, char* name, GLsizei count, GLint* v)
{
    const GLint location = glGetUniformLocation(shader.id, name);
    if (location == -1)
    {
        fprintf(stderr, "Failed to find uniform %s in shader_set_1iv\n", name);
    }
    else
    {
        glUniform1iv(location, count, v);
    }
    return location != -1;
}

GLboolean shader_set_1uiv(Shader shader, char* name, GLsizei count, GLuint* v)
{
    const GLint location = glGetUniformLocation(shader.id, name);
    if (location == -1)
    {
        fprintf(stderr, "Failed to find uniform %s in shader_set_1uiv\n", name);
    }
    else
    {
        glUniform1uiv(location, count, v);
    }
    return location != -1;
}

GLboolean shader_set_1fv(Shader shader, char* name, GLsizei count, GLfloat* v)
{
    const GLint location = glGetUniformLocation(shader.id, name);
    if (location == -1)
    {
        fprintf(stderr, "Failed to find uniform %s in shader_set_1fv\n", name);
    }
    else
    {
        glUniform1fv(location, count, v);
    }
    return location != -1;
}

GLboolean shader_set_2iv(Shader shader, char* name, GLsizei count, GLint* v)
{
    const GLint location = glGetUniformLocation(shader.id, name);
    if (location == -1)
    {
        fprintf(stderr, "Failed to find uniform %s in shader_set_2iv\n", name);
    }
    else
    {
        glUniform2iv(location, count, v);
    }
    return location != -1;
}

GLboolean shader_set_2uiv(Shader shader, char* name, GLsizei count, GLuint* v)
{
    const GLint location = glGetUniformLocation(shader.id, name);
    if (location == -1)
    {
        fprintf(stderr, "Failed to find uniform %s in shader_set_2uiv\n", name);
    }
    else
    {
        glUniform2uiv(location, count, v);
    }
    return location != -1;
}

GLboolean shader_set_2fv(Shader shader, char* name, GLsizei count, GLfloat* v)
{
    const GLint location = glGetUniformLocation(shader.id, name);
    if (location == -1)
    {
        fprintf(stderr, "Failed to find uniform %s in shader_set_2fv\n", name);
    }
    else
    {
        glUniform2fv(location, count, v);
    }
    return location != -1;
}

GLboolean shader_set_3iv(Shader shader, char* name, GLsizei count, GLint* v)
{
    const GLint location = glGetUniformLocation(shader.id, name);
    if (location == -1)
    {
        fprintf(stderr, "Failed to find uniform %s in shader_set_3iv\n", name);
    }
    else
    {
        glUniform3iv(location, count, v);
    }
    return location != -1;
}

GLboolean shader_set_3uiv(Shader shader, char* name, GLsizei count, GLuint* v)
{
    const GLint location = glGetUniformLocation(shader.id, name);
    if (location == -1)
    {
        fprintf(stderr, "Failed to find uniform %s in shader_set_3uiv\n", name);
    }
    else
    {
        glUniform3uiv(location, count, v);
    }
    return location != -1;
}

GLboolean shader_set_3fv(Shader shader, char* name, GLsizei count, GLfloat* v)
{
    const GLint location = glGetUniformLocation(shader.id, name);
    if (location == -1)
    {
        fprintf(stderr, "Failed to find uniform %s in shader_set_3fv\n", name);
    }
    else
    {
        glUniform3fv(location, count, v);
    }
    return location != -1;
}

GLboolean shader_set_4iv(Shader shader, char* name, GLsizei count, GLint* v)
{
    const GLint location = glGetUniformLocation(shader.id, name);
    if (location == -1)
    {
        fprintf(stderr, "Failed to find uniform %s in shader_set_4iv\n", name);
    }
    else
    {
        glUniform4iv(location, count, v);
    }
    return location != -1;
}

GLboolean shader_set_4uiv(Shader shader, char* name, GLsizei count, GLuint* v)
{
    const GLint location = glGetUniformLocation(shader.id, name);
    if (location == -1)
    {
        fprintf(stderr, "Failed to find uniform %s in shader_set_4uiv\n", name);
    }
    else
    {
        glUniform4uiv(location, count, v);
    }
    return location != -1;
}

GLboolean shader_set_4fv(Shader shader, char* name, GLsizei count, GLfloat* v)
{
    const GLint location = glGetUniformLocation(shader.id, name);
    if (location == -1)
    {
        fprintf(stderr, "Failed to find uniform %s in shader_set_4fv\n", name);
    }
    else
    {
        glUniform4fv(location, count, v);
    }
    return location != -1;
}

GLboolean shader_set_m2(Shader shader, char* name, GLsizei count, GLfloat* m)
{
    const GLint location = glGetUniformLocation(shader.id, name);
    if (location == -1)
    {
        fprintf(stderr, "Failed to find uniform %s in shader_set_m2\n", name);
    }
    else
    {
        glUniformMatrix2fv(location, count, 0, m);
    }
    return location != -1;
}

GLboolean shader_set_m3(Shader shader, char* name, GLsizei count, GLfloat* m)
{
    const GLint location = glGetUniformLocation(shader.id, name);
    if (location == -1)
    {
        fprintf(stderr, "Failed to find uniform %s in shader_set_m3\n", name);
    }
    else
    {
        glUniformMatrix3fv(location, count, 0, m);
    }
    return location != -1;
}

GLboolean shader_set_m4(Shader shader, char* name, GLsizei count, GLfloat* m)
{
    const GLint location = glGetUniformLocation(shader.id, name);
    if (location == -1)
    {
        fprintf(stderr, "Failed to find uniform %s in shader_set_m4\n", name);
    }
    else
    {
        glUniformMatrix4fv(location, count, 0, m);
    }
    return location != -1;
}

GLboolean shader_set_m2x3(Shader shader, char* name, GLsizei count, GLfloat* m)
{
    const GLint location = glGetUniformLocation(shader.id, name);
    if (location == -1)
    {
        fprintf(stderr, "Failed to find uniform %s in shader_set_m2x3\n", name);
    }
    else
    {
        glUniformMatrix2x3fv(location, count, 0, m);
    }
    return location != -1;
}

GLboolean shader_set_m3x2(Shader shader, char* name, GLsizei count, GLfloat* m)
{
    const GLint location = glGetUniformLocation(shader.id, name);
    if (location == -1)
    {
        fprintf(stderr, "Failed to find uniform %s in shader_set_m3x2\n", name);
    }
    else
    {
        glUniformMatrix3x2fv(location, count, 0, m);
    }
    return location != -1;
}

GLboolean shader_set_m2x4(Shader shader, char* name, GLsizei count, GLfloat* m)
{
    const GLint location = glGetUniformLocation(shader.id, name);
    if (location == -1)
    {
        fprintf(stderr, "Failed to find uniform %s in shader_set_m2x4\n", name);
    }
    else
    {
        glUniformMatrix2x4fv(location, count, 0, m);
    }
    return location != -1;
}

GLboolean shader_set_m4x2(Shader shader, char* name, GLsizei count, GLfloat* m)
{
    const GLint location = glGetUniformLocation(shader.id, name);
    if (location == -1)
    {
        fprintf(stderr, "Failed to find uniform %s in shader_set_m4x2\n", name);
    }
    else
    {
        glUniformMatrix4x2fv(location, count, 0, m);
    }
    return location != -1;
}

GLboolean shader_set_m3x4(Shader shader, char* name, GLsizei count, GLfloat* m)
{
    const GLint location = glGetUniformLocation(shader.id, name);
    if (location == -1)
    {
        fprintf(stderr, "Failed to find uniform %s in shader_set_m3x4\n", name);
    }
    else
    {
        glUniformMatrix3x4fv(location, count, 0, m);
    }
    return location != -1;
}

GLboolean shader_set_m4x3(Shader shader, char* name, GLsizei count, GLfloat* m)
{
    const GLint location = glGetUniformLocation(shader.id, name);
    if (location == -1)
    {
        fprintf(stderr, "Failed to find uniform %s in shader_set_m4x3\n", name);
    }
    else
    {
        glUniformMatrix4x3fv(location, count, 0, m);
    }
    return location != -1;
}
