#define __USE_MINGW_ANSI_STDIO 1 // Make MinGW printf support size_t with %zu
#include "kdtree.h"
#include "camera.h"
#include "utility.h"
#include "rules.h"
#include <time.h> // time
#include <stb_ds.h>
#include <cglm/cglm.h>

Node random_node_child(Node parent, GLboolean child_index)
{
    Node child = {
        .colour = { rand() & 255, rand() & 255, rand() & 255 },
        .min_corner = { parent.min_corner[0], parent.min_corner[1] },
        .size = { parent.size[0], parent.size[1] },
        .split_axis = rand_bool(),
        .split = bias_float(rand_float(), 0.5, 2.0f),
    };

    const GLboolean axis = parent.split_axis;
    float split = parent.split;

    if (child_index)
    {
        child.min_corner[axis] += parent.size[axis] * split;
        split = 1.0f - split;
    }
    child.size[axis] *= split;

    return child;
}

Node node_child(
    Node parent,
    GLboolean child_index,
    Replacement replacement,
    NodeType* types
) {
    vec3 r;
    glm_vec3_scale(types[replacement.types_indices[child_index]].col, 255, r);

    Node child = {
        .colour = { r[0], r[1], r[2] },
        .min_corner = { parent.min_corner[0], parent.min_corner[1] },
        .size = { parent.size[0], parent.size[1] },
        .type = &types[replacement.types_indices[child_index]],
    };

    const GLboolean axis = parent.split_axis;
    float split = parent.split;
    const size_t ori = replacement.orientation;

    if (ori & ORIENTATION_ABSOLUTE) split /= parent.size[axis];
    if (ori & ORIENTATION_SQUARE) split = glm_vec2_min(parent.size)
                                        / glm_vec2_max(parent.size);
    if (ori & ORIENTATION_INDEX) split = 1.0f - split;

    if (child_index)
    {
        child.min_corner[axis] += parent.size[axis] * split;
        split = 1.0f - split;
    }
    child.size[axis] *= split;

    return child;
}

Node* gen_random_nodes(float min_area, Camera camera)
{
    // Guess the amount of memory needed to avoid both waste and growth
    Node* nodes = NULL;
    size_t expected_node_count = 6.0f / min_area;
    arrsetcap(nodes, expected_node_count);

    // Define the head of the k-d tree, which is never a leaf, so has no colour
    arrput(nodes, ((Node) {
        .colour = { 0 },
        .min_corner = { -1.0f, -1.0f },
        .size = { 2.0f, 2.0f },
        .split_axis = rand_bool(),
        .split = bias_float(rand_float(), 0.5, 2.0f),
    }));

    size_t num_nodes_finished = 0;

    // Expand all nodes until they are finished
    while (arrlenu(nodes) > num_nodes_finished)
    {
        const Node node = nodes[num_nodes_finished];
        if (node.size[0] * node.size[1] > min_area)
        {
            arrput(nodes, random_node_child(node, 0));
            nodes[num_nodes_finished] = random_node_child(node, 1);
        }
        else num_nodes_finished++;
    }

    return nodes;
}

Node* gen_nodes(float min_area, Camera camera)
{
    // Guess the amount of memory needed to avoid both waste and growth
    Node* nodes = NULL;
    size_t expected_node_count = 6.0f / min_area;
    arrsetcap(nodes, expected_node_count);
    NodeType* types = rules_read(); // Read node types from file

    vec3 r;
    glm_vec3_scale(types[0].col, 255, r);

    // Define the head of the k-d tree
    arrput(nodes, ((Node) {
        .colour = { r[0], r[1], r[2] },
        .min_corner = { -1.0f, -1.0f },
        .size = { 2.0f, 2.0f },
        .type = &types[0],
    }));

    size_t num_nodes_finished = 0;

    // Expand all nodes until they are finished
    while (arrlenu(nodes) > num_nodes_finished)
    {
        Node node = nodes[num_nodes_finished];
        size_t replacement_count = arrlenu(node.type->replacements);
        if (node.size[0] * node.size[1] > min_area && replacement_count > 0)
        {
            const size_t n = rand_int(replacement_count, true);
            Replacement replacement = node.type->replacements[n];

            size_t n_copy = n;
            GLboolean too_small = GL_FALSE;

            // If absolute units exceed the parent size then try the next rep
            while (replacement.orientation & ORIENTATION_ABSOLUTE
                && node.size[replacement.orientation & ORIENTATION_AXIS]
                < replacement.splitDecimal)
            {
                n_copy++;
                n_copy %= replacement_count;
                if (n_copy == n)
                {
                    too_small = GL_TRUE;
                    break;
                }
                replacement = node.type->replacements[n_copy];
            }

            // If there is no valid replacement then skip this node
            if (too_small)
            {
                num_nodes_finished++;
                continue;
            }

            node.split_axis = replacement.orientation & ORIENTATION_AXIS;
            node.split = replacement.splitDecimal;
            arrput(nodes, node_child(node, 0, replacement, types));
            nodes[num_nodes_finished] = node_child(node, 1, replacement, types);
        }
        else num_nodes_finished++;
    }

    return nodes;
}

void** gen_vertices_from_nodes(Node* nodes)
{
    float* vertex_floats = NULL;
    arrsetcap(vertex_floats, arrlenu(nodes) * 4);
    GLubyte* vertex_colours = NULL;
    arrsetcap(vertex_floats, arrlenu(nodes) * 3);

    for (Node* n = nodes; n <= &arrlast(nodes); ++n)
    {
        for (size_t i = 0; i < 2; ++i) arrput(vertex_floats, n->min_corner[i]);
        for (size_t i = 0; i < 2; ++i) arrput(vertex_floats, n->size[i]);
        for (size_t i = 0; i < 3; ++i) arrput(vertex_colours, n->colour[i]);
    }

    // Free all remaining nodes
    arrfree(nodes);

    void** vertices = NULL;
    arrput(vertices, vertex_floats);
    arrput(vertices, vertex_colours);

    return vertices;
}

void** gen_vertices(Camera camera)
{
    srand(0);
    const size_t target_node_count = 1000 * 1000;
    const float min_area = 5.992f / target_node_count;
    Node* nodes = gen_random_nodes(min_area, camera); // Use gen_nodes for rule usage
    return gen_vertices_from_nodes(nodes);
}

void kdtree_calc_bounds(Graph* tree)
{
    if (!graph_ok(*tree)) return;

    size_t dims = sizeof(tree->size) / sizeof(*tree->size);
    const float* floats = tree->vertices[0];
    memcpy(tree->min_corner, floats, sizeof(tree->min_corner));

    for (size_t i = 0; i < dims; ++i)
    {
        tree->size[i] = tree->min_corner[i] + floats[i + dims];
    }
    for (size_t i = dims * 2; i < arrlenu(tree->vertices[0]); i += dims * 2)
    {
        for (size_t j = 0; j < dims; ++j)
        {
            const float c = floats[i + j];
            tree->min_corner[j] = min(tree->min_corner[j], c);
            tree->size[j] = max(tree->size[j], c + floats[i + j + dims]);
        }
    }
    for (size_t i = 0; i < 2; ++i) tree->size[i] -= tree->min_corner[i];
}

Graph kdtree_init(Shader* shader, Camera camera)
{
    // Seed the random number generator
    // srand(time(NULL));
    void** vertices = gen_vertices(camera);

    Attribute* layout_attributes = NULL;

    arrput(layout_attributes, ((Attribute) {
        .name = "min_corner",
        .size = 2,
        .type = GL_FLOAT,
    }));

    arrput(layout_attributes, ((Attribute) {
        .name = "size",
        .size = 2,
        .type = GL_FLOAT,
    }));

    Attribute* colour_attributes = NULL;

    arrput(colour_attributes, ((Attribute) {
        .name = "colour",
        .size = 3,
        .type = GL_UNSIGNED_BYTE,
    }));

    Attribute** attributes = NULL;
    arrput(attributes, layout_attributes);
    arrput(attributes, colour_attributes);

    Graph tree = graph_init(
        shader,
        vertices,
        NULL,
        attributes
    );

    kdtree_calc_bounds(&tree);

    return tree;
}

void kdtree_regenerate(Graph *tree, Camera camera)
{
    GLenum* types = NULL;
    arrput(types, GL_FLOAT);
    arrput(types, GL_UNSIGNED_BYTE);
    graph_update_vertices(tree, gen_vertices(camera), types);
    kdtree_calc_bounds(tree);
    arrfree(types);
}

void kdtree_check_camera(Graph* tree, Camera camera)
{
    // Regenerate if we are about to show an ungenerated region
    // OR if the zoom level has doubled
    // For each corner of the camera (i < 4; ++i)
    //     If the corner is outside the kdtree then regenerate, return
    // If the area of the camera is less than 1/16th of the tree, regen, ret

    if (tree->size[0] * tree->size[1] / 256 >
        camera.scaled_size[0] * camera.scaled_size[1]
    ) {
        kdtree_regenerate(tree, camera);
        return;
    }

    vec2 local[2] = { { 0, 0 }, { 0, 0 } };
    for (size_t dim = 0; dim < 2; ++dim)
    {
        local[dim][dim] = camera.scaled_size[dim];
        glm_vec2_rotate(local[dim], -camera.rotation, local[dim]);
    }

    for (size_t i = 0; i < 4; ++i)
    {
        vec2 corner;
        glm_vec2_copy(camera.position, corner);
        for (size_t dim = 0; dim < 2; ++dim)
        {
            if (i & (1 << dim)) glm_vec2_add(corner, local[dim], corner);
            else                glm_vec2_sub(corner, local[dim], corner);
        }
        for (size_t dim = 0; dim < 2; ++dim)
        {
            if (corner[dim] <= tree->min_corner[dim] ||
                corner[dim] >= tree->min_corner[dim] + tree->size[dim])
            {
                kdtree_regenerate(tree, camera);
                return;
            }
        }
    }
}

void node_info(Node* node)
{
    printf("Node %zu info:\n", (size_t)node / sizeof(Node));
    if (!node) return;
    printf("Min corner %f %f\n", node->min_corner[0], node->min_corner[1]);
    printf("Size %f %f\n", node->size[0], node->size[1]);
    printf("Split %c %f\n", node->split_axis ? 'Y' : 'X', node->split);
    printf(
        "Colour %" PRIu8 " %" PRIu8 " %" PRIu8 "\n",
        node->colour[0],
        node->colour[1],
        node->colour[2]
    );
}
