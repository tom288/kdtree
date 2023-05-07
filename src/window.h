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
    zoom_in,
    zoom_out,
    clockwise,
    anticlockwise,
    randomise,
    rasterise,
} Action;

/// @brief Binding from input to action
typedef struct {
    uint_fast8_t mouse : 1;
    uint_fast16_t button : 15;
    Action action;
} Binding;

/// @brief Input state at a given time
typedef struct {
    uint_fast32_t hold;
    uint_fast32_t press;
    vec2 mouse;
    float scroll;
} Input;

/// @brief All persistent information related to a visible window
typedef struct {
    GLFWwindow* win;
    GLboolean ok;
    GLbitfield clear_mask;
    Input input;
    Binding* binds;
    vec2 size;
    double last_frame_timestamp;
    double delta_time;
} Window;

/// @brief Create a window
/// @return Window created, or NULL if malloc fails
Window* window_init(void);

/// @brief Check window status, polling input events in the process
/// @param window Window to check
/// @return Whether the window is ok
GLboolean window_ok(Window* window);

/// @brief Set the clear colour and clear the window
/// @param window The window to modify
/// @param r Amount of red
/// @param g Amount of green
/// @param b Amount of blue
void window_clear_colour(Window* window, GLclampf r, GLclampf g, GLclampf b);

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

/// @brief Get movement input vector from window
/// @param window Window to query
/// @param dest Vector to write movement input to
void window_movement_input(Window* window, vec2 dest);

/// @brief Get camera input vector from window
/// @param window Window to query
/// @param dest Vector to write camera input to
void window_camera_input(Window* window, vec2 dest);

/// @brief Obtain window size in pixels
/// @param window Window to query
/// @param dest Vector to write window size to
void window_size(Window* window, vec2 dest);

/// @brief Reset window frame times
/// @param window Window to reset
void window_reset_time(Window* window);

/// @brief Obtain time in seconds between recent frames for a given window
/// @param window Window to query
/// @return Seconds between frames
double window_delta_time(Window* window);
