#include "window.h"
#include "shader.h"
#include "kdtree.h"
#include "camera.h"

int main(int argc, char* argv[])
{
    Window* win = window_init();
    if (!win) return 1;

    Shader shader = shader_init(
        "src/glsl/rectangle.vert",
        "src/glsl/rectangle.geom",
        "src/glsl/colour.frag"
    );
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

        vec2 camera_input;
        window_camera_input(win, camera_input);

        camera_step(
            &camera,
            movement_input,
            camera_input,
            window_delta_time(win)
        );
        camera_use(camera, shader);
        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        graph_draw(tree, GL_POINTS);
        window_swap(win);
        kdtree_randomise(&tree);
    }

    graph_kill(&tree);
    shader_kill(&shader);
    window_kill(win);

    return 0;
}
