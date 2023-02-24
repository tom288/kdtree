#include "rectangle.h"
#include <stdlib.h> // malloc
#include <string.h> // memcpy

Rectangle rectangle_init(Shader* shader)
{
    const float const_vertices[] = {
        -0.5f, -0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
    };

    const size_t vertices_size = sizeof(const_vertices);
    float* const vertices = malloc(vertices_size);
    memcpy(vertices, const_vertices, vertices_size);

    Attribute attributes[] = {
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

    const size_t attribute_count = sizeof(attributes) / sizeof(Attribute);

    return (Rectangle) {
        .graph = graph_init(
            shader,
            vertices_size,
            vertices,
            0,
            NULL,
            attribute_count,
            attributes
        ),
    };
}

GLboolean rectangle_ok(const Rectangle rect) { return graph_ok(rect.graph); }

void rectangle_draw(const Rectangle rect) { graph_draw(rect.graph); }

void rectangle_kill(Rectangle* rect) { graph_kill(&rect->graph); }
