#include "rectangle.h"
#include <stdlib.h> // malloc
#include <string.h> // memcpy
#include <stdio.h> // memcpy
#include <stb_ds.h>

Graph rectangle_init(Shader* shader)
{
    const float const_vertices[] = {
        -0.5f, -0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
    };

    float* vertex_floats = NULL;
    arrsetlen(vertex_floats, sizeof(const_vertices) / sizeof(*const_vertices));
    memcpy(vertex_floats, const_vertices, sizeof(const_vertices));

    void** vertices = NULL;
    arrput(vertices, vertex_floats);

    Attribute* float_attributes = NULL;

    arrput(float_attributes, ((Attribute) {
        .name = "position",
        .size = 2,
        .type = GL_FLOAT,
    }));

    arrput(float_attributes, ((Attribute) {
        .name = "colour",
        .size = 3,
        .type = GL_FLOAT,
    }));

    Attribute** attributes = NULL;
    arrput(attributes, float_attributes);

    return graph_init(
        shader,
        vertices,
        NULL,
        attributes
    );
}
