#include "window.h"
#include "shader.h"
#include "rectangle.h"
#include "utility.h"

int main(int argc, char* argv[])
{
    Window* win = window_init();

    Shader shader = shader_init("src/glsl/wobble.vert", "src/glsl/wobble.frag", "");
    if (!shader.ok)
    {
        window_kill(win);
        return 1;
    }

    Rectangle rect = rectangle_init(shader);
    if (!rectangle_ok(rect))
    {
        shader_kill(&shader);
        window_kill(win);
        return 1;
    }

    shader_use(shader);
    // Choose the background colour
    glClearColor(0.1f, 0.0f, 0.3f, 0.0f);

    if (!shader_set_1f(shader, "uv_zoom", 1.9f)) win->ok = GL_FALSE;

    // Swap the buffers and process pending events until the window is not OK
    while (window_ok(win))
    {
        if (!shader_set_1f(shader, "time", glfwGetTime())) win->ok = GL_FALSE;
        rectangle_draw(rect);
        window_swap(win);
    }

    rectangle_kill(&rect);
    shader_kill(&shader);
    window_kill(win);

    return 0;
}
