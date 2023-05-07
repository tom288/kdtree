#include "window.h"
#include "shader.h"
#include "kdtree.h"
#include "camera.h"
#include "killer.h"

int main(int argc, char* argv[])
{
    Killer killer = killer_init();
    Window* win = window_init();
    if (!killer_assert(killer, !!win)) return 1;
    killer_target(killer, (kill_fn)window_kill, win);

    Shader rectangle_shader = shader_init(
        "src/glsl/rectangle.vert",
        "src/glsl/rectangle.geom",
        "src/glsl/colour.frag"
    );
    if (!killer_assert(killer, rectangle_shader.ok)) return 1;
    killer_target(killer, (kill_fn)shader_kill, &rectangle_shader);

    Shader texture_shader = shader_init(
        "src/glsl/rectangle.vert",
        "src/glsl/texture.geom",
        "src/glsl/colour.frag"
    );
    if (!killer_assert(killer, texture_shader.ok)) return 1;
    killer_target(killer, (kill_fn)shader_kill, &texture_shader);

    Shader blit_shader = shader_init(
        "src/glsl/blit.vert",
        NULL,
        "src/glsl/blit.frag"
    );
    if (!killer_assert(killer, blit_shader.ok)) return 1;
    killer_target(killer, (kill_fn)shader_kill, &blit_shader);

    Graph tree = kdtree_init(&rectangle_shader);
    if (!killer_assert(killer, graph_ok(tree))) return 1;
    killer_target(killer, (kill_fn)graph_kill, &tree);

    // Choose the background colour
    window_clear_colour(win, 0.1f, 0.0f, 0.3f);

    vec2 win_size;
    window_size(win, win_size);
    Camera camera = camera_init(win_size);

    window_reset_time(win);

    // Swap the buffers and process pending events until the window is not OK
    while (window_ok(win))
    {
        if (window_action(win, randomise, GL_TRUE)) kdtree_randomise(&tree);
        if (window_action(win, rasterise, GL_TRUE)) graph_free_textures(&tree);

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

        GLboolean prepped = graph_prep_texture(
            &tree,
            GL_POINTS,
            texture_shader,
            blit_shader
        );

        camera_use(camera, prepped ? blit_shader : rectangle_shader);
        graph_draw(&tree, GL_POINTS);
        glClearColor(0.1f, 0.0f, 0.3f, 0.0f);
        window_swap(win);
    }

    return killer_kill(killer);
}
