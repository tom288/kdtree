#pragma once

#include "shader.h"

typedef struct Rectangle {
    GLuint vao;
    GLuint vbo;
    GLint numbers_per_vertex;
} Rectangle;

Rectangle rectangle_init(Shader shader);
void rectangle_draw(Rectangle rect);
void rectangle_kill(Rectangle* rect);
