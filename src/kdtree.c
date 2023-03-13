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

// It is important that we pass a pointer to an array of vertices, because
// the stb macros like arrput will write to the pointer for reallocation. By
// passing a float* instead we would have dangling pointer issues outside.
void dfs_bake_leaves(Node* node, float** vertices, size_t* leaves)
{
    Node* child = NULL;

    for (size_t i = 0; i < 2; ++i)
    {
        if (node->children[i])
        {
            child = node->children[i];
            dfs_bake_leaves(child, vertices, leaves);
        }
    }

    if (child) return;

    for (size_t vertex = 0; vertex < rect_vertices; ++vertex)
    {
        for (size_t floats = 0; floats < vertex_floats; ++floats)
        {
            float f;
            if (floats < 2)
            {
                f = node->min_corner[floats];
                if (
                    (floats == 0 && vertex % 2 > 0) ||
                    (floats == 1 && vertex > 0 && vertex < 4)
                ) {
                    f += node->size[floats];
                }
            }
            else
            {
                f = node->colour[floats - 2];
            }

            arrput(*vertices, f);
        }
    }
    (*leaves)++;
}

Graph kdtree_init(Shader* shader)
{
    // Seed the random number generator
    srand(time(NULL));

    // Allocate enough room to draw max_leaves nodes
    const size_t max_leaves = 1 << 16;

    Node* nodes = NULL;
    // Guess the amount of memory needed to avoid both waste and growth
    arrsetcap(nodes, max_leaves * 3);

    // Define the head of the k-d tree, which is never a leaf, so no colour
    arrput(nodes, ((Node) {
        .colour = { 0.0f },
        .min_corner = { -1.0f, -1.0f },
        .size = { 2.0f, 2.0f },
        .split_axis = rand_bool(),
        .split = rand_float(),
        .children = { NULL, NULL },
    }));
    Node* head = &nodes[0];

    // Find a random child with value NULL
    // Assign a node to this child
    // Update the number of leaves
    for (size_t leaves = 0; leaves < max_leaves;) // We increment conditionally
    {
        Node* node = head;
        GLboolean child_index;

        while (node->children[child_index = rand_bool()])
        {
            node = node->children[child_index];
        }

        arrput(nodes, ((Node) {
            .colour = { 0.0f },
            .min_corner = { node->min_corner[0], node->min_corner[1] },
            .size = { node->size[0], node->size[1] },
            .split_axis = rand_bool(),
            .split = bias_float(rand_float(), 0.5, 2.0f),
            .children = { NULL, NULL },
        }));

        Node* leaf = &arrlast(nodes);
        node->children[child_index] = leaf;
        rand_vec3(leaf->colour);

        if (node->children[!child_index])
        {
            ++leaves;
        }
        else
        {
            node->colour[0] = -1.0f;
        }

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
    }

    float* vertices = NULL;
    arrsetcap(vertices, max_leaves * rectangle_floats);
    size_t leaves = 0;
    dfs_bake_leaves(nodes, &vertices, &leaves);

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
