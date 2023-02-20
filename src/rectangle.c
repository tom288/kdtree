#define GLEW_STATIC
#include "rectangle.h"
#include "utility.h"
#include <GL/glew.h>
#include <stdio.h>

typedef struct Component {
    char* name;
    GLint size;
    GLenum type;
    GLboolean normalised;
} Component;

float vertices[] = {
    -0.5f, -0.5f,  0.0f,  0.0f,  1.0f,
     0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
    -0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
};

void rectangle_kill(Rectangle* rect)
{
    glDeleteVertexArrays(1, &rect->vao);
    glDeleteBuffers(1, &rect->vbo);
    rect->vao = 0;
    rect->vbo = 0;
    rect->numbers_per_vertex = 0;
}

Rectangle rectangle_init(Shader shader)
{
    Rectangle rect;
    glGenVertexArrays(1, &rect.vao);
    glGenBuffers(1, &rect.vbo);
    glBindVertexArray(rect.vao);

    glBindBuffer(GL_ARRAY_BUFFER, rect.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    Component components[] = {
        {
            .name = "position",
            .size = 2,
            .type = GL_FLOAT,
        },
        {
            .name = "colour",
            .size = 3,
            .type = GL_FLOAT,
        },
    };

    size_t component_count = sizeof(components) / sizeof(Component);

    rect.numbers_per_vertex = 0;
    for (size_t i = 0; i < component_count; i++)
    {
        rect.numbers_per_vertex += components[i].size;
    }

    const GLsizei stride = rect.numbers_per_vertex * sizeof(vertices[0]);
    void* first = 0;
    GLboolean error = GL_FALSE;

    for (size_t i = 0; i < component_count; i++)
    {
        Component c = components[i];
        GLint index = glGetAttribLocation(shader.id, c.name);
        if (index == -1)
        {
            fprintf(stderr, "Failed to find %s in shader\n", c.name);
            error = GL_TRUE;
        }
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(index, c.size, c.type, GL_FALSE, stride, first);
        first += c.size * sizeof(vertices[0]);
    }

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    if (error || gl_error())
    {
        rectangle_kill(&rect);
    }

    return rect;
}

void rectangle_draw(Rectangle rect)
{
    if (!rect.vao) return;
    glBindVertexArray(rect.vao);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / sizeof(vertices[0]) / rect.numbers_per_vertex);
    glBindVertexArray(0);
}
