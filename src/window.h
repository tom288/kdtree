#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

typedef enum Action {
    up,
    down,
    left,
    right,
} Action;

typedef struct Input {
    size_t hold;
    size_t press;
    vec2 mouse;
} Input;

typedef struct Window {
    GLFWwindow* win;
    GLboolean ok;
    GLbitfield clear_mask;
    Input input;
} Window;

Window* window_init(void);
GLboolean window_ok(Window* window);

/// @brief Show the updated window state and optionally clear the framebuffer
/// @param window Window to update
void window_swap(Window* window);

/// @brief Destroy window and free associated memory
/// @param window Window to destroy
void window_kill(Window* window);

/// @brief Get state of input action
/// @param window Window to fetch state of
/// @param action Action to query
/// @param press Whether the action must have just been pressed
/// @return Whether the action is active
GLboolean window_action(Window* window, Action action, GLboolean press);
