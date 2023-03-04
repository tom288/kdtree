#pragma once

#include "shader.h"
#include "graph.h"
#include <cglm/cglm.h>

/// @brief Node for kd-tree
typedef struct Node {
    vec3 colour;
    vec2 min_corner;
    vec2 size;
    GLboolean split_axis;
    float split;
    struct Node* children[2];
} Node;

/// @brief Create a random kd-tree spanning the entire -1 to 1 range
/// @param shader Shader to use for attribute position names
/// @return Graph holding kd-tree data
Graph kdtree_init(Shader* shader);

/// @brief Display information about a particular node
/// @param shader
void node_info(Node* node);
