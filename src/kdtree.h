#pragma once

#include "shader.h"
#include "graph.h"

typedef struct KDTree {
    Graph graph;
} KDTree;

KDTree kdtree_init(Shader* shader);
GLboolean kdtree_ok(KDTree tree);
void kdtree_draw(KDTree tree);
void kdtree_kill(KDTree* tree);
