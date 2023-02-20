#include "window.h"
#include "shader.h"
#include "rectangle.h"

int main(int argc, char *argv[])
{
    Window* win = window_init();
    Shader shader = shader_init("src/glsl/colour.vert", "src/glsl/colour.frag", "");
    if (!shader.ok)
    {
        window_kill(win);
        return 1;
    }
    shader_use(shader);
    Rectangle rect = rectangle_init(shader);

    // Choose the background colour
    glClearColor(0.1f, 0.0f, 0.3f, 0.0f);

    // Swap the buffers and process pending events until the window is not OK
    while (window_ok(win))
    {
        rectangle_draw(rect);
        window_swap(win);
    }

    rectangle_kill(&rect);
    shader_kill(&shader);
    window_kill(win);

    return 0;
}
