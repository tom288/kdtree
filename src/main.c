#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include "window.c"

int main(int argc, char *argv[])
{
    // Create the window
    window_init();

    // Choose the background colour
    glClearColor(0.1f, 0.0f, 0.3f, 0.0f);

    // Repeat until the window is not OK
    while (window_ok()) {
        // Swap the buffers and process all pending events
        window_swap();
    }

    // Destroy the window
    window_kill();

    return 0;
}
