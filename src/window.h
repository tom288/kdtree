#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

void window_init();
GLFWwindow* window_get();
int window_ok();
void window_swap();
void window_kill();
