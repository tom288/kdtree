#pragma once

#include "rectangle.c"

Rectangle rectangle_init(Shader shader);
void rectangle_draw(Rectangle rect);
void rectangle_kill(Rectangle* rect);
