#pragma once

#include "shader.h"

typedef struct Graph {
    GLuint vao;
    GLuint vbo;
    size_t vertices_size;
    float* vertices;
    size_t stride;
} Graph;

typedef struct Component {
    char* name;
    size_t size;
    GLenum type;
    GLboolean normalised;
} Component;

Graph graph_init
(
    Shader shader,
    size_t vertices_size,
    float* vertices,
    size_t component_count,
    Component components[]
);
GLboolean graph_ok(Graph graph);
void graph_draw(Graph graph);
void graph_kill(Graph* graph);
