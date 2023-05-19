#pragma once

#include "shader.h"

/// @brief VRAM and RAM vertex data pair
typedef struct Graph {
    GLuint vao;
    GLuint* vbos;
    GLuint ebo;
    GLuint fbo;
    void** vertices;
    GLuint* indices;
    size_t* strides;
    struct Graph* quad;
    GLuint quad_texture;
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
/// @param vertices Vertex buffer data
/// @param indices Element buffer data
/// @param attributes Vertex attributes
/// @return Graph
Graph graph_init
(
    Shader* shader,
    void** vertices,
    GLuint* indices,
    Attribute** attributes
);

/// @brief Create an empty graph, most useful for maintaining safe error cases
/// @return Empty graph
Graph graph_init_empty();

/// @brief Verify success of graph initialisation
/// @param graph Graph to inspect
/// @return Whether graph initialisation succeeded
GLboolean graph_ok(Graph graph);

/// @brief Decide whether to render a graph to a texture
/// @param graph Graph to evaluate and optionally render
/// @param mode Render mode
/// @param texture_shader Shader to use for generating the texture
/// @param blit_shader Shader to use once the texture is ready
GLboolean graph_prep_texture(Graph* graph, GLenum mode, Shader texture_shader, Shader blit_shader);

/// @brief Draw contents of graph
/// @param graph Graph to draw
/// @param mode Kind of primitive to render
void graph_draw(Graph* graph, GLenum mode);

/// @brief Free old graph vertices and assign new vertices
/// @param graph Graph to update
/// @param vertices Vertices to use
/// @param types Types of vertices to reassign
void graph_update_vertices(Graph* graph, void** vertices, GLenum* types);

/// @brief Free graph textures, requiring them to be regenerated
/// @param graph Graph of textures to free
void graph_free_textures(Graph* graph);

/// @brief Free graph vertices, without changing GPU data
/// @param graph Graph of vertices to free
void graph_free_vertices(Graph* graph);

/// @brief Free graph vertices and indices, without changing GPU data
/// @param graph Graph of vertices to free, optionally with indices to free too
void graph_free_all(Graph* graph);

/// @brief Destroy graph, free allocated memory and associated GPU memory
/// @param graph Graph to destroy
void graph_kill(Graph* graph);
