#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

typedef struct Window {
    GLFWwindow* win;
    GLboolean ok;
    GLboolean test_depth;
} Window;

Window* window_init();
GLboolean window_ok(Window* window);
void window_swap(Window* window);
void window_kill(Window* window);
