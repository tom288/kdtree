#include "kdtree.h"
#include "utility.h"
#include <stdlib.h> // malloc
#include <string.h> // memcpy
#include <time.h> // time
#include <stb_ds.h>
#include <time.h>

const size_t rect_vertices = 6;
const size_t vertex_floats = 5;
const size_t rectangle_floats = rect_vertices * vertex_floats;

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
    const size_t max_leaves = 1 << 22;

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

    clock_t begin = clock();

    const size_t vertices_size = max_leaves * rectangle_floats * sizeof(float);
    float* vertices = NULL;
    arrsetcap(vertices, max_leaves * rectangle_floats);
    size_t leaves = 0;
    dfs_bake_leaves(nodes, &vertices, &leaves);

    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("%f\n", time_spent);

    // Free all remaining nodes
    arrfree(nodes);

    Attribute attributes[] = {
        {
            .name = "position",
            .size = 2,
            .type = GL_FLOAT,
        },
        {
            .name = "colour",
            .size = 3,
            .type = GL_FLOAT,
        },
    };

    const size_t attribute_count = sizeof(attributes) / sizeof(Attribute);

    return graph_init(
        shader,
        vertices_size,
        vertices,
        0,
        NULL,
        attribute_count,
        attributes
    );
}

void node_info(Node* node)
{
    printf("Node %llu info:\n", (uint64_t)node / sizeof(Node));
    if (!node) return;
    printf("Min corner %f %f\n", node->min_corner[0], node->min_corner[1]);
    printf("Size %f %f\n", node->size[0], node->size[1]);
    printf("Split %c %f\n", node->split_axis ? 'Y' : 'X', node->split);
    printf(
        "Children %llu %llu\n\n",
        (uint64_t)node->children[0] / sizeof(Node),
        (uint64_t)node->children[1] / sizeof(Node)
    );
}
