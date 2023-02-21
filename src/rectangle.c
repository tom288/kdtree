#include "rectangle.h"
#include <stdlib.h> // malloc
#include <string.h> // memcpy

Rectangle rectangle_init(Shader shader)
{
    const float vertices[] = {
        -0.5f, -0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
    };

    const size_t vertices_size = sizeof(vertices);
    float* graph_vertices = malloc(vertices_size);
    memcpy(graph_vertices, vertices, vertices_size);

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

    const size_t component_count = sizeof(components) / sizeof(Component);

    Rectangle rect = {
        .graph = graph_init(
            shader,
            vertices_size,
            graph_vertices,
            component_count,
            components
        ),
    };

    return rect;
}

GLboolean rectangle_ok(const Rectangle rect) { return graph_ok(rect.graph); }

void rectangle_draw(const Rectangle rect) { graph_draw(rect.graph); }

void rectangle_kill(Rectangle* rect) { graph_kill(&rect->graph); }
