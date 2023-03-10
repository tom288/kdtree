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

    float* vertices = NULL;
    arrsetlen(vertices, sizeof(const_vertices) / sizeof(*const_vertices));
    memcpy(vertices, const_vertices, sizeof(const_vertices));

    Attribute* attributes = NULL;

    arrput(attributes, ((Attribute) {
        .name = "position",
        .size = 2,
        .type = GL_FLOAT,
    }));

    arrput(attributes, ((Attribute) {
        .name = "colour",
        .size = 3,
        .type = GL_FLOAT,
    }));

    return graph_init(
        shader,
        GL_FLOAT,
        vertices,
        NULL,
        attributes
    );
}
