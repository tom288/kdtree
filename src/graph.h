#pragma once

#include "shader.h"

/// @brief VRAM and RAM vertex data pair
typedef struct Graph {
    GLuint vao;
    GLuint vbo;
    GLuint ebo;
    size_t vertices_size;
    void* vertices;
    size_t indices_size;
    GLuint* indices;
    size_t stride;
} Graph;

/// @brief Vertex attribute
typedef struct Attribute {
    char* name;
    size_t size;
    GLenum type;
    GLboolean normalised;
} Attribute;

/// @brief Upload and label vertex data so that it can be used by the GPU
/// @param shader Shader object to use for finding name positions, or NULL
/// @param vertices_size Size of vertex buffer data in bytes
/// @param vertices Vertex buffer data
/// @param attribute_count Number of vertex attributes
/// @param attributes Vertex attributes
/// @return Graph
Graph graph_init
(
    Shader* shader,
    size_t vertices_size,
    void* vertices,
    size_t indices_size,
    GLuint* indices,
    size_t attribute_count,
    Attribute attributes[]
);
GLboolean graph_ok(Graph graph);
void graph_draw(Graph graph);
void graph_kill(Graph* graph);
