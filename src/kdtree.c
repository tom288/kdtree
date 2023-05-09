#define __USE_MINGW_ANSI_STDIO 1 // Make MinGW printf support size_t with %zu
#include "kdtree.h"
#include "utility.h"
#include "rules.h"
#include <time.h> // time
#include <stb_ds.h>

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

Node node_child(Node parent, GLboolean child_index, Replacement replacement, NodeType* types)
{
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

    if (child_index)
    {
        child.min_corner[axis] += parent.size[axis] * split;
        split = 1.0f - split;
    }
    child.size[axis] *= split;

    return child;
}

Node* gen_random_nodes(float min_area)
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

Node* gen_nodes(float min_area)
{
    // Guess the amount of memory needed to avoid both waste and growth
    Node* nodes = NULL;
    size_t expected_node_count = 6.0f / min_area;
    arrsetcap(nodes, expected_node_count);
    NodeType* types = readRules(); // Read node types from file

    // TEMPORAY
    rules_print(types);

    NodeType last_type = types[arrlenu(types) - 1];
    Replacement* last_reps = last_type.replacements;
    Replacement random_rep = last_reps[rand_int(arrlenu(last_reps), true)];

    // Define the head of the k-d tree
    arrput(nodes, ((Node) {
        .colour = { 0 },
        .min_corner = { -1.0f, -1.0f },
        .size = { 2.0f, 2.0f },
        .type = &last_type,
    }));

    size_t num_nodes_finished = 0;

    // Expand all nodes until they are finished
    while (arrlenu(nodes) > num_nodes_finished)
    {
        Node node = nodes[num_nodes_finished];
        uint32_t replacement_count = arrlenu(node.type->replacements);
        if (node.size[0] * node.size[1] > min_area && replacement_count > 0)
        {
            size_t n = rand_int(replacement_count, true);
            Replacement replacement = node.type->replacements[n];
            node.split_axis = replacement.orientation;
            node.split = replacement.splitPercent / 100.0f;
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

    for (size_t i = 0; i < arrlenu(nodes); ++i)
    {
        const Node n = nodes[i];

        for (size_t i = 0; i < 2; ++i) arrput(vertex_floats, n.min_corner[i]);
        for (size_t i = 0; i < 2; ++i) arrput(vertex_floats, n.size[i]);
        for (size_t i = 0; i < 3; ++i) arrput(vertex_colours, n.colour[i]);
    }

    // Free all remaining nodes
    arrfree(nodes);

    void** vertices = NULL;
    arrput(vertices, vertex_floats);
    arrput(vertices, vertex_colours);

    return vertices;
}

void** gen_random_vertices()
{
    const size_t target_node_count = 1000 * 1000;
    const float min_area = 5.992f / target_node_count;
    Node* nodes = gen_random_nodes(min_area);
    return gen_vertices_from_nodes(nodes);
}

void** gen_vertices()
{
    const size_t target_node_count = 1000 * 1000;
    const float min_area = 5.992f / target_node_count;
    Node* nodes = gen_nodes(min_area);
    return gen_vertices_from_nodes(nodes);
}

Graph kdtree_init(Shader* shader)
{
    // Seed the random number generator
    srand(time(NULL));
    void* vertices = gen_vertices();

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

    return graph_init(
        shader,
        vertices,
        NULL,
        attributes
    );
}

void kdtree_randomise(Graph *tree)
{
    GLenum* types = NULL;
    arrput(types, GL_FLOAT);
    arrput(types, GL_UNSIGNED_BYTE);
    graph_update_vertices(tree, gen_vertices(), types);
    arrfree(types);
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
