#include "window.h"
#include "shader.h"
#include "camera.h"
#include "killer.h"
#include "utility.h"
#include "rectangle.h"
#include "gameplay.h"

extern char* shader_dir;
extern char* rule_dir;

int main(int argc, char* argv[])
{
    Killer killer = killer_null;

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
        "alan.vert",
        NULL,
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
    Graph rect = rectangle_init(&rectangle_shader);
    if (!killer_assert(&killer, graph_ok(rect))) return 1;
    killer_target(&killer, (kill_fn)graph_kill, &rect);

    // Choose the background colour
    window_clear_colour(win, 0.1f, 0.0f, 0.3f);

    // Read window size and hand it to the camera so that it can scale correctly
    vec2 win_size;
    window_size(win, win_size);
    Camera camera = camera_init(win_size);

    window_reset_time(win);

    gameplay_init();
    gameplay_test();
    // Swap the buffers and process pending events until the window is not OK
    while (window_ok(win))
    {
        gameplay_test();
        vec2 input;
        window_movement_input(win, input);
        // glm_vec2_add(rect_pos, movement_input, rect_pos);
        gameplay_physics_tick(window_delta_time(win), input, &rectangle_shader);

        camera_use(camera, rectangle_shader);
        // shader_set_2fv(rectangle_shader, "translate", 1, rect_pos);

        graph_draw(&rect, GL_TRIANGLES);
        glClearColor(0.1f, 0.0f, 0.3f, 0.0f);
        window_swap(win);
    }

    return killer_kill(&killer);
}
