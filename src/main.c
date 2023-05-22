#include "window.h"
#include "shader.h"
#include "kdtree.h"
#include "camera.h"
#include "killer.h"
#include "utility.h"

/// @brief Path to glsl source file directory
char* shader_dir = NULL;

/// @brief Path to rules definition file directory
char* rule_dir = NULL;

int main(int argc, char* argv[])
{
    Killer killer = killer_init();

    // Prepare paths
    shader_dir = dir_path(argv[0], "../src/glsl/");
    if (!killer_assert(&killer, !!shader_dir)) return 1;
    killer_target(&killer, (kill_fn)free, shader_dir);

    rule_dir = dir_path(argv[0], "../src/world/");
    if (!killer_assert(&killer, !!rule_dir)) return 1;
    killer_target(&killer, (kill_fn)free, rule_dir);

    // Create window
    Window* win = window_init();
    if (!killer_assert(&killer, !!win)) return 1;
    killer_target(&killer, (kill_fn)window_kill, win);

    // Create rectangle to screen rasterisation shader
    Shader rectangle_shader = shader_init(
        "rectangle.vert",
        "rectangle.geom",
        "colour.frag"
    );
    if (!killer_assert(&killer, rectangle_shader.ok)) return 1;
    killer_target(&killer, (kill_fn)shader_kill, &rectangle_shader);

    // Create rectangle to texture rasterisation shader
    Shader texture_shader = shader_init(
        "rectangle.vert",
        "texture.geom",
        "colour.frag"
    );
    if (!killer_assert(&killer, texture_shader.ok)) return 1;
    killer_target(&killer, (kill_fn)shader_kill, &texture_shader);

    // Create texture to screen 'blit' shader
    Shader blit_shader = shader_init(
        "blit.vert",
        NULL,
        "blit.frag"
    );
    if (!killer_assert(&killer, blit_shader.ok)) return 1;
    killer_target(&killer, (kill_fn)shader_kill, &blit_shader);

    // Create main kd-tree
    Graph tree = kdtree_init(&rectangle_shader);
    if (!killer_assert(&killer, graph_ok(tree))) return 1;
    killer_target(&killer, (kill_fn)graph_kill, &tree);

    // Choose the background colour
    window_clear_colour(win, 0.1f, 0.0f, 0.3f);

    // Read window size and hand it to the camera so that it can scale correctly
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

    return killer_kill(&killer);
}
