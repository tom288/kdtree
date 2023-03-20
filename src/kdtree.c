#define __USE_MINGW_ANSI_STDIO 1 // Make MinGW printf support size_t with %zu
#include "kdtree.h"
#include "utility.h"
#include <stdlib.h> // malloc
#include <time.h> // time
#include <stb_ds.h>
#include <time.h>

Graph kdtree_init(Shader* shader)
{
    // Seed the random number generator
    srand(time(NULL));

    // Allocate enough room to draw max_leaves nodes
    const size_t max_leaves = 1 << 16;

    // Guess the amount of memory needed to avoid both waste and growth
    Node* nodes = NULL;
    arrsetcap(nodes, max_leaves * 1.65f);

    // Define the head of the k-d tree, which is never a leaf, so has no colour
    arrput(nodes, ((Node) {
        .colour = { 0.0f },
        .min_corner = { -1.0f, -1.0f },
        .size = { 2.0f, 2.0f },
        .split_axis = rand_bool(),
        .split = bias_float(rand_float(), 0.5, 2.0f),
        .children = { NULL, NULL },
    }));

    // Find a random child with value NULL
    // Assign a node to this child
    // Update the number of leaves
    for (size_t leaves = arrlenu(nodes); leaves < max_leaves;)
    {
        size_t random_index = rand_int(arrlen(nodes), GL_TRUE);
        Node* node = &nodes[random_index];
        GLboolean child_index = rand_bool();
        if (node->children[child_index]) child_index ^= 1;

        arrput(nodes, ((Node) {
            .colour = { rand() & 255, rand() & 255, rand() & 255 },
            .min_corner = { node->min_corner[0], node->min_corner[1] },
            .size = { node->size[0], node->size[1] },
            .split_axis = rand_bool(),
            .split = bias_float(rand_float(), 0.5, 2.0f),
            .children = { NULL, NULL },
        }));

        Node* leaf = &arrlast(nodes);
        const GLboolean axis = node->split_axis;

        if (child_index)
        {
            leaf->min_corner[axis] += node->size[axis] * node->split;
            leaf->size[axis] *= (1 - node->split);
        }
        else
        {
            leaf->size[axis] *= node->split;
        }

        if (node->children[!child_index])
        {
            arrdelswap(nodes, random_index);
            ++leaves;
        }
        else
        {
            node->children[child_index] = leaf;
        }
    }

    GLenum* vertex_types = NULL;
    arrput(vertex_types, GL_FLOAT);
    arrput(vertex_types, GL_UNSIGNED_BYTE);

    float* vertex_floats = NULL;
    arrsetcap(vertex_floats, max_leaves * 4);
    GLubyte* vertex_colours = NULL;
    arrsetcap(vertex_floats, max_leaves * 3);

    for (size_t i = 0; i < arrlenu(nodes); ++i)
    {
        const Node n = nodes[i];

        if (n.children[GL_FALSE] || n.children[GL_TRUE]) continue;

        for (size_t i = 0; i < 2; ++i) arrput(vertex_floats, n.min_corner[i]);
        for (size_t i = 0; i < 2; ++i) arrput(vertex_floats, n.size[i]);
        for (size_t i = 0; i < 3; ++i) arrput(vertex_colours, n.colour[i]);
    }

    // Free all remaining nodes
    arrfree(nodes);

    void** vertices = NULL;
    arrput(vertices, vertex_floats);
    arrput(vertices, vertex_colours);

    Attribute* float_attributes = NULL;

    arrput(float_attributes, ((Attribute) {
        .name = "min_corner",
        .size = 2,
        .type = GL_FLOAT,
    }));

    arrput(float_attributes, ((Attribute) {
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
    arrput(attributes, float_attributes);
    arrput(attributes, colour_attributes);

    return graph_init(
        shader,
        vertex_types,
        vertices,
        NULL,
        attributes
    );
}

void node_info(Node* node)
{
    printf("Node %zu info:\n", (size_t)node / sizeof(Node));
    if (!node) return;
    printf("Min corner %f %f\n", node->min_corner[0], node->min_corner[1]);
    printf("Size %f %f\n", node->size[0], node->size[1]);
    printf("Split %c %f\n", node->split_axis ? 'Y' : 'X', node->split);
    printf(
        "Children %zu %zu\n\n",
        (size_t)node->children[0] / sizeof(Node),
        (size_t)node->children[1] / sizeof(Node)
    );
}
