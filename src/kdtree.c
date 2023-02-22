#include "kdtree.h"
#include "utility.h"
#include <cglm/cglm.h>
#include <stdlib.h> // malloc
#include <string.h> // memcpy
#include <time.h> // time

typedef struct Node {
    vec3 colour;
    vec2 min_corner;
    vec2 size;
    GLboolean split_axis;
    float split;
    struct Node* children[2];
} Node;

KDTree kdtree_init(Shader shader)
{
    // Seed the random number generator
    srand(time(NULL));

    // Allocate enough room to draw max_leaves nodes
    const size_t max_leaves = 1000;
    const size_t rect_vertices = 6;
    const size_t vertex_floats = 5;
    const size_t rectangle_floats = rect_vertices * vertex_floats;
    const size_t vertices_size = max_leaves * rectangle_floats * sizeof(float);
    float* const vertices = malloc(vertices_size);

    // Define the head of the k-d tree
    // The head is never
    Node* const head = malloc(sizeof(Node));
    *head = (Node) {
        .colour = { 0.0f },
        .min_corner = { -1.0f, -1.0f },
        .size = { 2.0f, 2.0f },
        .split_axis = rand_bool(),
        .split = rand_float(),
        .children = { NULL, NULL },
    };


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

        Node* const leaf = malloc(sizeof(Node));
        node->children[child_index] = leaf;

        *leaf = (Node) {
            .colour = { 0.0f },
            .min_corner = { node->min_corner[0], node->min_corner[1] },
            .size = { node->size[0], node->size[1] },
            .split_axis = rand_bool(),
            .split = bias_float(rand_float(), 0.5, 2.0f),
            .children = { NULL, NULL },
        };

        rand_vec3(&leaf->colour);

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

    // Now that we have our tree we can use it to fill our vertices buffer.
    // We could not do this before because we did not know which nodes would
    // end up being leaves and would therefore need to be drawn.

    // Find a leaf
    // Add it to the buffer of vertices
    // Free the memory allocated for the leaf
    for (size_t leaves = 0; leaves < max_leaves;) // We increment conditionally
    {
        Node* leaf = head;
        Node* parent = NULL;
        GLboolean child_index = GL_FALSE;

        while (leaf->children[0] || leaf->children[1])
        {
            parent = leaf;
            child_index = !leaf->children[0];
            leaf = child_index
                ? leaf->children[1]
                : leaf->children[0];
        }

        if (leaf->colour[0] < 0)
        {
            free(leaf);
            parent->children[child_index] = NULL;
            continue;
        }

        for (size_t vertex = 0; vertex < rect_vertices; ++vertex)
        {
            for (size_t floats = 0; floats < vertex_floats; ++floats)
            {
                float f;
                if (floats < 2)
                {
                    f = leaf->min_corner[floats];
                    if (
                        (floats == 0 && vertex % 2 > 0) ||
                        (floats == 1 && vertex > 0 && vertex < 4)
                    ) {
                        f += leaf->size[floats];
                    }
                }
                else
                {
                    f = leaf->colour[floats - 2];
                }

                vertices[
                    leaves * rectangle_floats + vertex * vertex_floats + floats
                ] = f;
            }
        }
        ++leaves;

        free(leaf);
        parent->children[child_index] = NULL;
    }

    // Free all remaining nodes
    while (GL_TRUE)
    {
        Node* leaf = head;
        Node* parent = NULL;
        GLboolean child_index;

        while (leaf->children[0] || leaf->children[1])
        {
            parent = leaf;
            child_index = !leaf->children[0];
            leaf = child_index
                ? leaf->children[1]
                : leaf->children[0];
        }

        free(leaf);
        if (!parent) break; // There is no longer a head
        parent->children[child_index] = 0;
    }

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

    return (KDTree) {
        .graph = graph_init(
            shader,
            vertices_size,
            vertices,
            attribute_count,
            attributes
        ),
    };
}

GLboolean kdtree_ok(const KDTree tree) { return graph_ok(tree.graph); }

void kdtree_draw(const KDTree tree) { graph_draw(tree.graph); }

void kdtree_kill(KDTree* tree) { graph_kill(&tree->graph); }
