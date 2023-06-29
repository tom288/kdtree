#pragma once
#include "shader.h"
#include "graph.h"
#include "rules.h"
#include "camera.h"
#include <cglm/cglm.h>

/// @brief Wrapper for a kd-tree graph
typedef struct {
    Graph graph;
    vec2 min_corner;
    vec2 size;
    vec2 potential_min_corner;
    vec2 potential_size;
} KDTree;

/// @brief Node for kd-tree
typedef struct Node {
    GLubyte colour[3];
    vec2 min_corner;
    vec2 size;
    GLboolean split_axis;
    float split;
    NodeType* type;
    uint16_t seed;
} Node;

/// @brief Create a random kd-tree spanning the entire -1 to 1 range
/// @param shader Shader to use for attribute position names
/// @param camera Camera to use for determining area to draw
/// @return Graph holding kd-tree data
KDTree kdtree_init(Shader* shader, Camera camera);

/// @brief Regenerate a kd-tree
/// @param tree Tree to be regenerated
/// @param camera Camera to use for determining area to draw
void kdtree_regenerate(KDTree* tree, Camera camera);

/// @brief Check whether regen is necessary based on the camera pos & size
/// @param tree Tree to possibly be regenerated
/// @param camera Camera to use
void kdtree_check_camera(KDTree* tree, Camera camera);

/// @brief Display node information
/// @param node Node to display
void node_info(Node* node);
