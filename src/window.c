#include "window.h"
#include <stdio.h>
#include <stdlib.h> // malloc, free
#include <limits.h>

static size_t window_count = 0;

// Keyboard input callback
void key_callback(GLFWwindow* win, int key, int scancode, int action, int mods)
{
    // printf(
    //     "key_callback: key = %d, scancode = %d, action = %d, mods = %d\n",
    //     key,
    //     scancode,
    //     action,
    //     mods
    // );

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(win, 1);
    }

    Window* const window = glfwGetWindowUserPointer(win);
    if (!window) return;

    for (size_t i = 0; i < window->binding_count; ++i)
    {
        const Binding binding = window->bindings[i];
        if (!binding.mouse && binding.button == key)
        {
            if (binding.action >= CHAR_BIT * sizeof(uint_fast32_t))
            {
                fprintf(stderr, "Action %d too large\n", binding.action);
            }
            window->input.press |= (action == GLFW_PRESS) << binding.action;
            window->input.hold |= (action == GLFW_PRESS) << binding.action;
            const GLboolean release = action == GLFW_RELEASE;
            window->input.press &= ~(release << binding.action);
            window->input.hold &= ~(release << binding.action);
        }
    }
}

// Mouse cursor movement callback
void cursor_pos_callback(GLFWwindow* win, double x_pos, double y_pos)
{
    // printf("cursor_pos_callback: x_pos = %f, y_pos = %f\n", x_pos, y_pos);
}

// Mouse button input callback
void mouse_button_callback(GLFWwindow* win, int button, int action, int mods)
{
    // printf(
    //     "mouse_button_callback: button = %d, action = %d, mods = %d\n",
    //     button,
    //     action,
    //     mods
    // );
}

// Mouse scroll wheel movement callback
void scroll_callback(GLFWwindow* win, double x_offset, double y_offset)
{
    // printf(
    //     "scroll_callback: x_offset = %f, y_offset = %f\n",
    //     x_offset,
    //     y_offset
    // );
}

// Error callback
void error_callback(int error_code, const char* description)
{
    fprintf(stderr, "GLFW error #%d\n%s\n", error_code, description);
}

void window_kill(Window* window)
{
    if (!window) return;
    if (window->win)
    {
        glfwDestroyWindow(window->win);
        if (!--window_count) glfwTerminate();
    }
    free(window->bindings);
    free(window);
}

Window* window_init()
{
    const GLboolean resizable = GL_FALSE;
    const int msaa_samples = 16;

    const int width = 1920;
    const int height = 1080;
    const char* title = "Rectangle";

    // These two are not currently used
    GLFWmonitor* monitor = NULL;
    GLFWwindow* share_window = NULL;

    const GLboolean show_cursor = GL_TRUE;
    const GLboolean raw_input = GL_FALSE;
    const GLboolean cull_faces = GL_FALSE;
    const GLboolean test_depth = GL_FALSE;
    const GLboolean clear_buffers = GL_TRUE;
    const GLboolean vertical_sync = GL_TRUE;

    Window* const window = malloc(sizeof(Window));

    if (!window)
    {
        fprintf(stderr, "Failed to malloc for Window\n");
        return NULL;
    }

    *window = (Window) {
        .win = NULL,
        .ok = GL_FALSE,
        .clear_mask = clear_buffers * (
            GL_DEPTH_BUFFER_BIT * test_depth |
            GL_COLOR_BUFFER_BIT
        ),
        .input = (Input) {
            .hold = 0,
            .press = 0,
            .mouse = { 0.0f, 0.0f },
            .scroll = 0.0,
        },
        .size = { width, height }
    };

    if (!window_count && !glfwInit())
    {
        const char* description;
        error_callback(glfwGetError(&description), description);
        return window;
    }

    // Prepare for window creation
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, resizable);
    if (msaa_samples > 1) glfwWindowHint(GLFW_SAMPLES, msaa_samples);

    // Fix compilation on OS X
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, 1);
#endif

    // Create window and put it in focus
    window->win = glfwCreateWindow(
        width,
        height,
        title,
        monitor,
        share_window
    );
    glfwMakeContextCurrent(window->win);

    // Configure input
    if (!show_cursor)
    {
        glfwSetInputMode(window->win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    if (raw_input && glfwRawMouseMotionSupported())
    {
        // Disable mouse motion acceleration and scaling
        glfwSetInputMode(window->win, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    }
    // This call to glfwSetWindowUserPointer allow us to use the whole window
    // struct in event callbacks, instead of just the GLFWwindow* win pointer.
    // This line is why we pass window around as a pointer, since otherwise
    // these callbacks would reference memory of an old Window that could be
    // freed at any time, and which is likely to hold outdated state anyway.
    glfwSetWindowUserPointer(window->win, window);
    glfwSetKeyCallback(window->win, key_callback);
    glfwSetMouseButtonCallback(window->win, mouse_button_callback);
    glfwSetCursorPosCallback(window->win, cursor_pos_callback);
    glfwSetScrollCallback(window->win, scroll_callback);
    glfwSetErrorCallback(error_callback);

    // Configure triangle visibility
    if (cull_faces) glEnable(GL_CULL_FACE);
    if (test_depth) glEnable(GL_DEPTH_TEST);

    // Configure additional window properties
    if (!vertical_sync) glfwSwapInterval(0);
    if (msaa_samples) glEnable(GL_MULTISAMPLE);

    // This assignment to glewExperimental used to be necessary for older
    // versions of GLEW (<=1.13), but may now be redundant, I haven't checked
    glewExperimental = GL_TRUE;

    // We call glewInit when creating a window if there are no other windows.
    // This can happen again whenever our program inits 1 or more windows,
    // kills all the windows, and then inits 1 or more.
    // From what I can tell by running this glewInit doesn't care.
    GLenum err;
    if (!window_count++ && (err = glewInit()) != GLEW_OK)
    {
        fprintf(stderr, "Failed to init GLEW: %s\n", glewGetErrorString(err));
    }
    else
    {
        window->ok = GL_TRUE;
    }

    window->binding_count = 4;
    window->bindings = malloc(window->binding_count * sizeof(Binding));
    window->bindings[0] = (Binding) {
        .button = GLFW_KEY_W,
        .action = up,
    };
    window->bindings[1] = (Binding) {
        .button = GLFW_KEY_S,
        .action = down,
    };
    window->bindings[2] = (Binding) {
        .button = GLFW_KEY_A,
        .action = left,
    };
    window->bindings[3] = (Binding) {
        .button = GLFW_KEY_D,
        .action = right,
    };
    return window;
}

GLboolean window_ok(Window* window)
{
    window->input.press = 0;
    window->input.scroll = 0;
    glfwPollEvents();
    return window
        ? (window->ok &= window->win && !glfwWindowShouldClose(window->win))
        : GL_FALSE;
}

void window_swap(Window* window)
{
    if (!window || !window->ok) return;
    glfwSwapBuffers(window->win);
    if (window->clear_mask) glClear(window->clear_mask);

    const double max_delta = 1.0f / 32.0f;
    double time = glfwGetTime();
    window->delta_time = __min(time - window->last_frame_timestamp, max_delta);
    window->last_frame_timestamp = time;
}

GLboolean window_action(Window* window, Action action, GLboolean press)
{   return window
        ? (press ? window->input.press : window->input.hold) >> action & 1
        : GL_FALSE;
}

void window_movement_input(Window* window, vec2 dest)
{
    glm_vec2_zero(dest);
    if (window_action(window, up, GL_FALSE)) dest[1] += 1.0f;
    if (window_action(window, down, GL_FALSE)) dest[1] -= 1.0f;
    if (window_action(window, left, GL_FALSE)) dest[0] -= 1.0f;
    if (window_action(window, right, GL_FALSE)) dest[0] += 1.0f;
}

void window_size(Window* window, vec2 dest)
{
    glm_vec2_copy(window->size, dest);
}

void window_reset_time(Window* window)
{
    window->last_frame_timestamp = glfwGetTime();
    window->delta_time = 0.0f;
}

double window_delta_time(Window* window)
{
    return window->delta_time;
}
