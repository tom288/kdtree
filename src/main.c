#include "window.h"

int main(int argc, char *argv[])
{
    // Create the window
    window_init();

    // Choose the background colour
    glClearColor(0.1f, 0.0f, 0.3f, 0.0f);

    // Swap the buffers and process pending events until the window is not OK
    while (window_ok())
    {
        window_swap();
    }

    // Destroy the window
    window_kill();

    return 0;
}
