#pragma once

#include "shader.h"
#include "graph.h"

typedef struct Rectangle {
    Graph graph;
    GLboolean ok;
} Rectangle;

Rectangle rectangle_init(Shader* shader);
GLboolean rectangle_ok(Rectangle rect);
void rectangle_draw(Rectangle rect);
void rectangle_kill(Rectangle* rect);
