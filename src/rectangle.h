#pragma once

#include "shader.h"

typedef struct Rectangle {
    GLuint vao;
    GLuint vbo;
    size_t vertices_size;
    float* vertices;
    size_t stride;
} Rectangle;

Rectangle rectangle_init(Shader shader);
void rectangle_draw(Rectangle rect);
void rectangle_kill(Rectangle* rect);
