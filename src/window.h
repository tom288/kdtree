#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

/// @brief Different discrete input actions that an agent can perform
typedef enum Action {
    up,
    down,
    left,
    right,
} Action;

typedef struct Binding {
    Action action;
    uint_fast8_t mouse : 1;
    uint_fast16_t button : 15;
} Binding;

/// @brief Input state at a given time
typedef struct Input {
    uint_fast32_t hold;
    uint_fast32_t press;
    vec2 mouse;
} Input;

/// @brief All persistent information related to a visible window
typedef struct Window {
    GLFWwindow* win;
    GLboolean ok;
    GLbitfield clear_mask;
    Input input;
    size_t binding_count;
    Binding* bindings;
} Window;

/// @brief Create a window
/// @return Window created, or NULL if malloc fails
Window* window_init(void);

/// @brief Check window status, polling input events in the process
/// @param window Window to check
/// @return Whether the window is ok
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
