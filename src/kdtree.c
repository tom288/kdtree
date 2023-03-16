#define __USE_MINGW_ANSI_STDIO 1 // Make MinGW printf support size_t with %zu
#include "kdtree.h"
#include "utility.h"
#include <stdlib.h> // malloc
#include <time.h> // time
#include <stb_ds.h>
#include <time.h>

const size_t rect_vertices = 6;
const size_t vertex_floats = 5;
const size_t rectangle_floats = rect_vertices * vertex_floats;

Graph kdtree_init(Shader* shader)
{
    // Seed the random number generator
    srand(time(NULL));

    // Allocate enough room to draw max_leaves nodes
    const size_t max_leaves = 1 << 16;

    // Guess the amount of memory needed to avoid both waste and growth
    Node* nodes = NULL;
    arrsetcap(nodes, max_leaves * 1.65f);

    // Define the head of the k-d tree, which is never a leaf, so no colour
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
            .colour = { 0.0f },
            .min_corner = { node->min_corner[0], node->min_corner[1] },
            .size = { node->size[0], node->size[1] },
            .split_axis = rand_bool(),
            .split = bias_float(rand_float(), 0.5, 2.0f),
            .children = { NULL, NULL },
        }));

        Node* leaf = &arrlast(nodes);
        rand_vec3(leaf->colour);
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

    float* vertices = NULL;
    arrsetcap(vertices, max_leaves * rectangle_floats);

    for (size_t i = 0; i < arrlenu(nodes); ++i)
    {
        const Node node = nodes[i];

        if (node.children[GL_FALSE] || node.children[GL_TRUE]) continue;

        for (size_t vertex = 0; vertex < rect_vertices; ++vertex)
        {
            for (size_t floats = 0; floats < vertex_floats; ++floats)
            {
                float f;
                if (floats < 2)
                {
                    f = node.min_corner[floats];
                    if (
                        (floats == 0 && vertex % 2 > 0) ||
                        (floats == 1 && vertex > 0 && vertex < 4)
                    ) {
                        f += node.size[floats];
                    }
                }
                else f = node.colour[floats - 2];

                arrput(vertices, f);
            }
        }
    }

    // Free all remaining nodes
    arrfree(nodes);

    Attribute* attributes = NULL;

    arrput(attributes, ((Attribute) {
        .name = "position",
        .size = 2,
        .type = GL_FLOAT,
    }));

    arrput(attributes, ((Attribute) {
        .name = "colour",
        .size = 3,
        .type = GL_FLOAT,
    }));

    return graph_init(
        shader,
        GL_FLOAT,
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
