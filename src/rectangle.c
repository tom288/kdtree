#define GLEW_STATIC
#include "rectangle.h"
#include "utility.h"
#include <GL/glew.h>
#include <stdio.h>
#include <stdlib.h> // malloc, free
#include <string.h> // memcpy

typedef struct Component {
    char* name;
    size_t size;
    GLenum type;
    GLboolean normalised;
} Component;

void rectangle_kill(Rectangle* rect)
{
    glDeleteVertexArrays(1, &rect->vao);
    rect->vao = 0;
    glDeleteBuffers(1, &rect->vbo);
    rect->vbo = 0;
    rect->vertices_size = 0;
    free(rect->vertices);
    rect->vertices = NULL;
    rect->stride = 0;
}

Rectangle rectangle_init(Shader shader)
{
    Rectangle rect;

    const float vertices[] = {
        -0.5f, -0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
    };

    rect.vertices_size = sizeof(vertices);
    rect.vertices = malloc(rect.vertices_size);
    memcpy(rect.vertices, vertices, sizeof(vertices));

    glGenVertexArrays(1, &rect.vao);
    glGenBuffers(1, &rect.vbo);
    glBindVertexArray(rect.vao);

    const GLenum usage = GL_STATIC_DRAW;

    glBindBuffer(GL_ARRAY_BUFFER, rect.vbo);
    glBufferData(GL_ARRAY_BUFFER, rect.vertices_size, rect.vertices, usage);

    const Component components[] = {
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

    const size_t component_count = sizeof(components) / sizeof(Component);

    size_t numbers_per_vertex = 0;
    for (size_t i = 0; i < component_count; i++)
    {
        numbers_per_vertex += components[i].size;
    }
    rect.stride = numbers_per_vertex * sizeof(rect.vertices[0]);

    const GLboolean force_cast_to_float = GL_FALSE;
    const GLboolean normalise_fixed_point_values = GL_FALSE;

    unsigned char* first = 0;
    GLboolean error = GL_FALSE;

    for (size_t i = 0; i < component_count; i++)
    {
        const Component c = components[i];
        const GLint index = glGetAttribLocation(shader.id, c.name);
        if (index == -1)
        {
            fprintf(stderr, "Failed to find %s in shader\n", c.name);
            error = GL_TRUE;
        }
        glEnableVertexAttribArray(index);
        switch (c.type) {
            case GL_NONE:
                break;
            case GL_BYTE:
            case GL_UNSIGNED_BYTE:
            case GL_SHORT:
            case GL_UNSIGNED_SHORT:
            case GL_INT:
            case GL_UNSIGNED_INT:
                if (!force_cast_to_float)
                {
                    glVertexAttribIPointer(
                        index,
                        c.size,
                        c.type,
                        rect.stride,
                        first
                    );
                    break;
                }
            case GL_DOUBLE:
                if (!force_cast_to_float)
                {
                    glVertexAttribLPointer(
                        index,
                        c.size,
                        c.type,
                        rect.stride,
                        first
                    );
                    break;
                }
            default:
                glVertexAttribPointer(
                    index,
                    c.size,
                    c.type,
                    normalise_fixed_point_values,
                    rect.stride,
                    first
                );
        }

        first += c.size * sizeof(rect.vertices[0]);
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
    if (!rect.vao || !rect.vertices_size || !rect.stride) return;
    glBindVertexArray(rect.vao);
    glDrawArrays(GL_TRIANGLES, 0, rect.vertices_size / rect.stride);
    glBindVertexArray(0);
}
