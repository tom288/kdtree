#include "window.h"
#include "shader.h"
#include "kdtree.h"
#include "camera.h"

int main(int argc, char* argv[])
{
    Window* win = window_init();
    if (!win) return 1;

    Shader shader = shader_init("src/glsl/camera.vert", "src/glsl/colour.frag", "");
    if (!shader.ok)
    {
        window_kill(win);
        return 1;
    }

    Graph tree = kdtree_init(&shader);
    if (!graph_ok(tree))
    {
        shader_kill(&shader);
        window_kill(win);
        return 1;
    }

    shader_use(shader);
    // Choose the background colour
    glClearColor(0.1f, 0.0f, 0.3f, 0.0f);

    vec2 win_size;
    window_size(win, win_size);
    Camera camera = camera_init(win_size);

    window_reset_time(win);

    // Swap the buffers and process pending events until the window is not OK
    while (window_ok(win))
    {
        vec2 movement_input;
        window_movement_input(win, movement_input);
        camera_step(&camera, movement_input, window_delta_time(win));
        camera_use(camera, shader);

        graph_draw(tree);
        window_swap(win);
    }

    graph_kill(&tree);
    shader_kill(&shader);
    window_kill(win);

    return 0;
}
