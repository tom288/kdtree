#pragma once

#include "window.c"

Window* window_init();
GLboolean window_ok(Window* window);
void window_swap(Window* window);
void window_kill(Window* window);
