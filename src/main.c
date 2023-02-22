#include "window.h"
#include "shader.h"
#include "kdtree.h"
#include "utility.h"

int main(int argc, char* argv[])
{
    Window* win = window_init();

    Shader shader = shader_init("src/glsl/colour.vert", "src/glsl/colour.frag", "");
    if (!shader.ok)
    {
        window_kill(win);
        return 1;
    }

    KDTree tree = kdtree_init(shader);
    if (!kdtree_ok(tree))
    {
        shader_kill(&shader);
        window_kill(win);
        return 1;
    }

    shader_use(shader);
    // Choose the background colour
    glClearColor(0.1f, 0.0f, 0.3f, 0.0f);

    // Swap the buffers and process pending events until the window is not OK
    while (window_ok(win))
    {
        kdtree_draw(tree);
        window_swap(win);
    }

    kdtree_kill(&tree);
    shader_kill(&shader);
    window_kill(win);

    return 0;
}
