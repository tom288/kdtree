#define GLEW_STATIC
#include <GL/glew.h>
#include <stdio.h>
#include <stdlib.h> // For calloc, free

typedef struct Shader {
    GLuint id;
    GLint ok;
} Shader;

// Print any shader compilation or linking errors, return whether errors exist
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

// Compile the shader file and return its ID, or 0 in the event of an error
GLuint compile(char* path, GLenum type)
{
    // Read file at path into buffer (https://stackoverflow.com/a/3747128)
    FILE* file = fopen(path, "rb");
    if (!file)
    {
        fprintf(stderr, "Failed to fopen ");
        perror(path);
        return 0;
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
        return 0;
    }

    // Copy the file content into the buffer
    if (fread(buffer, file_size, 1, file) != 1) {
        fclose(file);
        free(buffer);
        fprintf(stderr, "Failed to fread for %s\n", path);
        return 0;
    }

    fclose(file);

    GLuint id = glCreateShader(type);
    glShaderSource(id, 1, (const GLchar**)&buffer, NULL);
    glCompileShader(id);
    if (compile_error(id, 0, path)) {
        id = 0;
    }

    // Free the memory containing the file content
    free(buffer);

    // Return the shader ID so that we may use it in a shader program
    return id;
}

void shader_kill(Shader* shader)
{
    // glDeleteProgram(shader.id);
    shader->id = 0;
    shader->ok = 0;
}

// Create a shader program from 2 or 3 input files
Shader shader_init(char* vertex, char* fragment, char* geometry)
{
    GLuint vert = compile(vertex, GL_VERTEX_SHADER);
    GLuint frag = compile(fragment, GL_FRAGMENT_SHADER);
    GLuint geom = geometry && *geometry
        ? compile(geometry, GL_GEOMETRY_SHADER)
        : 0;

    Shader shader = {
        .id = 0,
        .ok = vert && frag && (!geometry || !*geometry || geom),
    };

    if (shader.ok) {
        // Create shader program
        shader.id = glCreateProgram();
        glAttachShader(shader.id, vert);
        glAttachShader(shader.id, frag);
        if (geom) glAttachShader(shader.id, geom);
        glLinkProgram(shader.id);
    }

    // The shaders are linked to the program and so can now be deleted
    // Values of 0 are silently ignored, so there's no need to check
    glDeleteShader(vert);
    glDeleteShader(frag);
    glDeleteShader(geom);

    if (shader.ok && compile_error(shader.id, 1, vertex)) {
        shader_kill(&shader);
    }

    return shader;
}

void shader_use(Shader shader)
{
    glUseProgram(shader.id);
}

void shader_set_1i(Shader shader, char* name, GLint n)
{
    glUniform1i(glGetUniformLocation(shader.id, name), n);
}

void shader_set_1ui(Shader shader, char* name, GLuint n)
{
    glUniform1ui(glGetUniformLocation(shader.id, name), n);
}

void shader_set_1f(Shader shader, char* name, GLfloat n)
{
    glUniform1f(glGetUniformLocation(shader.id, name), n);
}

void shader_set_2i(Shader shader, char* name, GLint x, GLint y)
{
    glUniform2i(glGetUniformLocation(shader.id, name), x, y);
}

void shader_set_2ui(Shader shader, char* name, GLuint x, GLuint y)
{
    glUniform2ui(glGetUniformLocation(shader.id, name), x, y);
}

void shader_set_2f(Shader shader, char* name, GLfloat x, GLfloat y)
{
    glUniform2f(glGetUniformLocation(shader.id, name), x, y);
}

void shader_set_3i(Shader shader, char* name, GLint x, GLint y, GLint z)
{
    glUniform3i(glGetUniformLocation(shader.id, name), x, y, z);
}

void shader_set_3ui(Shader shader, char* name, GLuint x, GLuint y, GLuint z)
{
    glUniform3ui(glGetUniformLocation(shader.id, name), x, y, z);
}

void shader_set_3f(Shader shader, char* name, GLfloat x, GLfloat y, GLfloat z)
{
    glUniform3f(glGetUniformLocation(shader.id, name), x, y, z);
}

void shader_set_4i(Shader shader, char* name, GLint x, GLint y, GLint z, GLint w)
{
    glUniform4i(glGetUniformLocation(shader.id, name), x, y, z, w);
}

void shader_set_4ui(Shader shader, char* name, GLuint x, GLuint y, GLuint z, GLuint w)
{
    glUniform4ui(glGetUniformLocation(shader.id, name), x, y, z, w);
}

void shader_set_4f(Shader shader, char* name, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
    glUniform4f(glGetUniformLocation(shader.id, name), x, y, z, w);
}

void shader_set_1iv(Shader shader, char* name, GLsizei count, GLint* v)
{
    glUniform1iv(glGetUniformLocation(shader.id, name), count, v);
}

void shader_set_1uiv(Shader shader, char* name, GLsizei count, GLuint* v)
{
    glUniform1uiv(glGetUniformLocation(shader.id, name), count, v);
}

void shader_set_1fv(Shader shader, char* name, GLsizei count, GLfloat* v)
{
    glUniform1fv(glGetUniformLocation(shader.id, name), count, v);
}

void shader_set_2iv(Shader shader, char* name, GLsizei count, GLint* v)
{
    glUniform2iv(glGetUniformLocation(shader.id, name), count, v);
}

void shader_set_2uiv(Shader shader, char* name, GLsizei count, GLuint* v)
{
    glUniform2uiv(glGetUniformLocation(shader.id, name), count, v);
}

void shader_set_2fv(Shader shader, char* name, GLsizei count, GLfloat* v)
{
    glUniform2fv(glGetUniformLocation(shader.id, name), count, v);
}

void shader_set_3iv(Shader shader, char* name, GLsizei count, GLint* v)
{
    glUniform3iv(glGetUniformLocation(shader.id, name), count, v);
}

void shader_set_3uiv(Shader shader, char* name, GLsizei count, GLuint* v)
{
    glUniform3uiv(glGetUniformLocation(shader.id, name), count, v);
}

void shader_set_3fv(Shader shader, char* name, GLsizei count, GLfloat* v)
{
    glUniform3fv(glGetUniformLocation(shader.id, name), count, v);
}

void shader_set_4iv(Shader shader, char* name, GLsizei count, GLint* v)
{
    glUniform4iv(glGetUniformLocation(shader.id, name), count, v);
}

void shader_set_4uiv(Shader shader, char* name, GLsizei count, GLuint* v)
{
    glUniform4uiv(glGetUniformLocation(shader.id, name), count, v);
}

void shader_set_4fv(Shader shader, char* name, GLsizei count, GLfloat* v)
{
    glUniform4fv(glGetUniformLocation(shader.id, name), count, v);
}

void shader_set_m2(Shader shader, char* name, GLsizei count, GLfloat* m)
{
    glUniformMatrix2fv(glGetUniformLocation(shader.id, name), count, 0, m);
}

void shader_set_m3(Shader shader, char* name, GLsizei count, GLfloat* m)
{
    glUniformMatrix3fv(glGetUniformLocation(shader.id, name), count, 0, m);
}

void shader_set_m4(Shader shader, char* name, GLsizei count, GLfloat* m)
{
    glUniformMatrix4fv(glGetUniformLocation(shader.id, name), count, 0, m);
}

void shader_set_m2x3(Shader shader, char* name, GLsizei count, GLfloat* m)
{
    glUniformMatrix2x3fv(glGetUniformLocation(shader.id, name), count, 0, m);
}

void shader_set_m3x2(Shader shader, char* name, GLsizei count, GLfloat* m)
{
    glUniformMatrix3x2fv(glGetUniformLocation(shader.id, name), count, 0, m);
}

void shader_set_m2x4(Shader shader, char* name, GLsizei count, GLfloat* m)
{
    glUniformMatrix2x4fv(glGetUniformLocation(shader.id, name), count, 0, m);
}

void shader_set_m4x2(Shader shader, char* name, GLsizei count, GLfloat* m)
{
    glUniformMatrix4x2fv(glGetUniformLocation(shader.id, name), count, 0, m);
}

void shader_set_m3x4(Shader shader, char* name, GLsizei count, GLfloat* m)
{
    glUniformMatrix3x4fv(glGetUniformLocation(shader.id, name), count, 0, m);
}

void shader_set_m4x3(Shader shader, char* name, GLsizei count, GLfloat* m)
{
    glUniformMatrix4x3fv(glGetUniformLocation(shader.id, name), count, 0, m);
}
