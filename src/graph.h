#pragma once

#include "shader.h"

/// @brief VRAM and RAM vertex data pair
typedef struct {
    GLuint vao;
    GLuint vbo;
    GLuint ebo;
    void* vertices;
    GLuint* indices;
    size_t stride;
} Graph;

/// @brief Vertex attribute
typedef struct {
    char* name;
    size_t size;
    GLenum type;
    GLboolean normalised;
} Attribute;

/// @brief Upload and label vertex data so that it can be used by the GPU
/// @param shader Shader object to use for finding name positions, or NULL
/// @param vertex_type Type of vertex buffer data
/// @param vertices Vertex buffer data
/// @param indices Element buffer data
/// @param attribute_count Number of vertex attributes
/// @param attributes Vertex attributes
/// @return Graph
Graph graph_init
(
    Shader* shader,
    GLenum vertex_type,
    void* vertices,
    GLuint* indices,
    size_t attribute_count,
    Attribute attributes[]
);

/// @brief Create an empty graph, most useful for maintaining safe error cases
/// @return Empty graph
Graph graph_init_empty();

/// @brief Verify success of graph initialisation
/// @param graph Graph to inspect
/// @return Whether graph initialisation succeeded
GLboolean graph_ok(Graph graph);

/// @brief Draw contents of graph
/// @param graph Graph to draw
void graph_draw(Graph graph);

/// @brief Free graph vertices, without changing GPU data
/// @param graph Graph of vertices to free
void graph_free_vertices(Graph* graph);

/// @brief Free graph vertices and indices, without changing GPU data
/// @param graph Graph of vertices to free, optionally with indices to free too
void graph_free_all(Graph* graph);

/// @brief Destroy graph, free allocated memory and associated GPU memory
/// @param graph Graph to destroy
void graph_kill(Graph* graph);
