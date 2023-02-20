#define GLEW_STATIC
#include <GL/glew.h>
#include "shader.h"
#include "utility.h"

typedef struct Rectangle {
    GLuint vao;
    GLuint vbo;
    GLint total_components;
} Rectangle;

float vertices[] = {
    -0.5f, -0.5f,  0.0f,  0.0f,  1.0f,
     0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
    -0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
};

Rectangle rectangle_init(Shader shader)
{
    Rectangle rect;
    glGenVertexArrays(1, &rect.vao);
    glGenBuffers(1, &rect.vbo);
    glBindVertexArray(rect.vao);

    glBindBuffer(GL_ARRAY_BUFFER, rect.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Attribute properties for the vertex positions
    rect.total_components = 5;
    const char* name = "position";
    GLint index = glGetAttribLocation(shader.id, name);
    GLint components = 2;
    const GLenum type = GL_FLOAT;
    const GLboolean normalise = GL_FALSE;
    const GLsizei stride = rect.total_components * sizeof(vertices[0]);
    void* first = 0;

    if (index != -1)
    {
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(index, components, type, normalise, stride, first);
    }
    else
    {
        fprintf(stderr, "Failed to find %s in shader\n", name);
    }

    name = "colour";
    index = glGetAttribLocation(shader.id, name);
    first += components * sizeof(vertices[0]);
    components = 3;

    if (index != -1) 
    {
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(index, components, type, normalise, stride, first);
    }
    else
    {
        fprintf(stderr, "Failed to find %s in shader\n", name);
    }

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    check_gl_error();

    return rect;
}

void rectangle_draw(Rectangle rect)
{
    glBindVertexArray(rect.vao);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / sizeof(vertices[0]) / rect.total_components);
    glBindVertexArray(0);
}

void rectangle_kill(Rectangle* rect)
{
    glDeleteVertexArrays(1, &rect->vao);
    glDeleteBuffers(1, &rect->vbo);
    rect->vao = 0;
    rect->vbo = 0;
    rect->total_components = 0;
}
