#pragma once

#include "window.c"

void window_init();
GLFWwindow* window_get();
int window_ok();
void window_swap();
void window_kill();
