#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

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
    printf("GLFW error #%d\n%s\n", error_code, description);
}


int main() {
    const int resizable = 0;
    const int msaa_samples = 16;

    const int width = 1920;
    const int height = 1080;
    const char* title = "Title";

    // These two are not currently used
    GLFWmonitor* monitor = NULL;
    GLFWwindow* share_window = NULL;

    const int show_cursor = 1;
    const int cull_faces = 0;
    const int test_depth = 0;
    const int vertical_sync = 1;

    // Prepare for window creation
    glfwInit();
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
    GLFWwindow* win = glfwCreateWindow(
        width,
        height,
        title,
        monitor,
        share_window
    );
    glfwMakeContextCurrent(win);

    // Configure input
    if (!show_cursor) glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetKeyCallback(win, key_callback);
    glfwSetMouseButtonCallback(win, mouse_button_callback);
    glfwSetCursorPosCallback(win, cursor_pos_callback);
    glfwSetScrollCallback(win, scroll_callback);
    glfwSetErrorCallback(error_callback);

    // Configure triangle visibility
    if (cull_faces) glEnable(GL_CULL_FACE);
    if (test_depth) glEnable(GL_DEPTH_TEST);

    // Configure additional window properties
    if (!vertical_sync) glfwSwapInterval(0);
    if (msaa_samples) glEnable(GL_MULTISAMPLE);

    // This assignment to glewExperimental used to be necessary for older
    // versions of GLEW (<=1.13), but may now be redundant, I haven't checked
    glewExperimental = 1;
    glewInit();

    // Choose the background colour
    glClearColor(0.1f, 0.0f, 0.3f, 0.0f);

    while (win && !glfwWindowShouldClose(win)) {
        glClear(GL_COLOR_BUFFER_BIT | test_depth * GL_DEPTH_BUFFER_BIT);
        glfwPollEvents();
        glfwSwapBuffers(win);
    }

    glfwDestroyWindow(win);
    win = NULL;
    glfwTerminate();
    return 0;
}
