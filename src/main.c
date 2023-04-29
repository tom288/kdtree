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

    Shader texture_shader = shader_init(
        "src/glsl/texture.vert",
        NULL,
        "src/glsl/texture.frag"
    );
    if (!texture_shader.ok)
    {
        shader_kill(&shader);
        window_kill(win);
        return 1;
    }

    Graph tree = kdtree_init(&shader);
    if (!graph_ok(tree))
    {
        shader_kill(&texture_shader);
        shader_kill(&shader);
        window_kill(win);
        return 1;
    }

    // Choose the background colour
    window_clear_colour(win, 0.1f, 0.0f, 0.3f);

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
        shader_use(shader);
        graph_draw(tree, GL_POINTS, &texture_shader);
        glClearColor(0.1f, 0.0f, 0.3f, 0.0f);
        window_swap(win);
        if (movement_input[0])
        {
            kdtree_randomise(&tree);
        }
    }

    graph_kill(&tree);
    shader_kill(&texture_shader);
    shader_kill(&shader);
    window_kill(win);

    return 0;
}
