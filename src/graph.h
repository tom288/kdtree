#pragma once

#include "shader.h"

typedef struct Graph {
    GLuint vao;
    GLuint vbo;
    size_t vertices_size;
    void* vertices;
    size_t stride;
} Graph;

typedef struct Attribute {
    char* name;
    size_t size;
    GLenum type;
    GLboolean normalised;
} Attribute;

Graph graph_init
(
    Shader* shader,
    size_t vertices_size,
    float* vertices,
    size_t attribute_count,
    Attribute attributes[]
);
GLboolean graph_ok(Graph graph);
void graph_draw(Graph graph);
void graph_kill(Graph* graph);
