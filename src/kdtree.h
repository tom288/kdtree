#pragma once

#include "shader.h"
#include "graph.h"
#include <cglm/cglm.h>

typedef struct KDTree {
    Graph graph;
} KDTree;

typedef struct Node {
    vec3 colour;
    vec2 min_corner;
    vec2 size;
    GLboolean split_axis;
    float split;
    struct Node* children[2];
} Node;

KDTree kdtree_init(Shader* shader);
GLboolean kdtree_ok(KDTree tree);
void kdtree_draw(KDTree tree);
void kdtree_kill(KDTree* tree);
