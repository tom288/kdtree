#pragma once

#include "window.c"

void window_init();
GLFWwindow* window_get();
GLboolean window_ok();
void window_swap();
void window_kill();
