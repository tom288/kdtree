#include "rectangle.h"
#include <stb_ds.h>

Graph rectangle_init(Shader* shader) // for testing only
{
    const float const_vertices[] = {
        -0.02f, -0.02f,  0.0f,  0.8f,  0.0f,
         0.02f,  0.02f,  0.0f,  0.8f,  0.0f,
        -0.02f,  0.02f,  0.0f,  0.8f,  0.0f,

         0.02f,  0.02f,  0.0f,  0.8f,  0.0f,
        -0.02f, -0.02f,  0.0f,  0.8f,  0.0f,
         0.02f, -0.02f,  0.0f,  0.8f,  0.0f,
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
