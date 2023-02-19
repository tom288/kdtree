#include "window.h"
#include "shader.h"
#include "triangle.h"

int main(int argc, char *argv[])
{
    // Create the window
    window_init();

    // Create the shader program
    GLuint shader = shader_init("src/glsl/colour.vert", "src/glsl/colour.frag", "");

    shader_use(shader);

    // Create the triangle
    triangle_init();

    // Choose the background colour
    glClearColor(0.1f, 0.0f, 0.3f, 0.0f);

    // Swap the buffers and process pending events until the window is not OK
    while (window_ok())
    {
        triangle_draw();
        window_swap();
    }

    triangle_kill();

    shader_kill(shader);

    // Destroy the window
    window_kill();

    return 0;
}
