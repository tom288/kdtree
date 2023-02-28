#pragma once

#include "shader.h"
#include "graph.h"
#include <cglm/cglm.h>

typedef struct Node {
    vec3 colour;
    vec2 min_corner;
    vec2 size;
    GLboolean split_axis;
    float split;
    struct Node* children[2];
} Node;

Graph kdtree_init(Shader* shader);
