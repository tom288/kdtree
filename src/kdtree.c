#define __USE_MINGW_ANSI_STDIO 1 // Make MinGW printf support size_t with %zu
#include "kdtree.h"
#include "camera.h"
#include "utility.h"
#include "rand.h"
#include "rules.h"
#include <time.h> // time
#include <stb_ds.h>
#include <cglm/cglm.h>

Node _random_node(Node* parent, pcg32_random_t* rng)
{
    uint32_t col = pcg32_random_r(rng);
    uint32_t spl = pcg32_random_r(rng);

    vec2 min_corner = { -1.0f, -1.0f };
    vec2 size = { 2.0f, 2.0f };
    if (parent) for (size_t i = 0; i < 2; ++i)
    {
        min_corner[i] = parent->min_corner[i];
        size[i] = parent->size[i];
    }

    return (Node) {
        .colour = { (col >> 24) & 255, (col >> 16) & 255, (col >> 8) & 255 },
        .min_corner = { min_corner[0], min_corner[1] },
        .seed = pcg32_random_r(rng),
        .size = { size[0], size[1] },
        .split_axis = spl & 1,
        .split = bias_float((float)spl / (float)UINT32_MAX, 0.5, 2.0f),
    };
}

Node random_node_child(Node parent, GLboolean child_index)
{
    pcg32_random_t rng;
    pcg32_srandom_r(&rng, parent.seed + child_index, child_index ? 7 : 9);
    Node child = _random_node(&parent, &rng);

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

typedef struct {
    vec2 min;
    vec2 max;
    bool set; // Whether min and max have been set
    vec2 potential_min;
    vec2 potential_max;
    bool potential_set; // Whether potential_min and potential_max have been set
    Node* nodes;
} GeneratedNodes;

void update_min_max(Node n, GeneratedNodes* out, GLboolean discarded)
{
    for (size_t i = 0; i < 2; ++i)
    {
        float potential_min_rhs = out->min[i];
        float potential_max_rhs = out->max[i];
        if (discarded)
        {
            potential_min_rhs = n.min_corner[i];
            potential_max_rhs = n.size[i] + n.min_corner[i];
        }
        else
        {
            if (out->set)
            {
                out->min[i] = min(out->min[i], n.min_corner[i]);
                out->max[i] = max(out->max[i], n.size[i] + n.min_corner[i]);
            }
            else
            {
                out->min[i] = n.min_corner[i];
                out->max[i] = n.size[i] + n.min_corner[i];
            }
        }
        if (out->potential_set)
        {
            out->potential_min[i] = min(out->potential_min[i], potential_min_rhs);
            out->potential_max[i] = max(out->potential_max[i], potential_max_rhs);
        }
        else
        {
            out->potential_min[i] = potential_min_rhs;
            out->potential_max[i] = potential_max_rhs;
        }
    }
    out->set |= !discarded;
    out->potential_set = GL_TRUE;
}

// TODO rename and relocate
float calc_camera_hyp(Camera camera)
{
    float hyp = 0;
    for (size_t i = 0; i < 2; ++i)
    {
        hyp += camera.scaled_size[i] * camera.scaled_size[i];
    }
    // Make generated region 2x the camera region
    return sqrtf(hyp) * 2.0f;
}

GLboolean kdtree_should_generate_node(Node node, Camera camera)
{
    float hyp = calc_camera_hyp(camera);

    for (size_t i = 0; i < 2; ++i)
    {
        if (node.min_corner[i] > camera.position[i] + hyp ||
            node.min_corner[i] + node.size[i] < camera.position[i] - hyp
        ) return GL_FALSE;
    }
    return GL_TRUE;
}

GeneratedNodes gen_random_nodes(float min_area, Camera camera)
{
    // Guess the amount of memory needed to avoid both waste and growths
    Node* nodes = NULL;
    size_t expected_node_count = 6.0f / min_area;
    arrsetcap(nodes, expected_node_count);

    // Generate the k-d tree head
    pcg32_random_t rng;
    pcg32_srandom_r(&rng, 4, 5);
    arrput(nodes, _random_node(NULL, &rng));

    size_t num_nodes_finished = 0;
    GeneratedNodes out;

    // Expand all nodes until they are finished
    while (arrlenu(nodes) > num_nodes_finished)
    {
        const Node node = nodes[num_nodes_finished];
        if (!kdtree_should_generate_node(node, camera))
        {
            update_min_max(node, &out, true);
            arrdelswap(nodes, num_nodes_finished);
        }
        else if (node.size[0] * node.size[1] > min_area)
        {
            arrput(nodes, random_node_child(node, 0));
            nodes[num_nodes_finished] = random_node_child(node, 1);
        }
        else
        {
            update_min_max(node, &out, false);
            num_nodes_finished++;
        }
    }

    out.nodes = nodes;
    return out;
}

GeneratedNodes gen_nodes(float min_area, Camera camera)
{
    // Guess the amount of memory needed to avoid both waste and growth
    Node* nodes = NULL;
    size_t expected_node_count = 6.0f / min_area;
    arrsetcap(nodes, expected_node_count);

    // Read node types from file
    NodeType* types = rules_read();

    // Generate the k-d tree head
    vec3 r;
    glm_vec3_scale(types[0].col, 255, r);
    arrput(nodes, ((Node) {
        .colour = { r[0], r[1], r[2] },
        .min_corner = { -1.0f, -1.0f },
        .size = { 2.0f, 2.0f },
        .type = &types[0],
    }));

    size_t num_nodes_finished = 0;
    GeneratedNodes out;

    // Expand all nodes until they are finished
    while (arrlenu(nodes) > num_nodes_finished)
    {
        Node node = nodes[num_nodes_finished];
        size_t replacement_count = arrlenu(node.type->replacements);
        if (!kdtree_should_generate_node(node, camera))
        {
            update_min_max(node, &out, true);
            arrdelswap(nodes, num_nodes_finished);
        }
        if (node.size[0] * node.size[1] > min_area && replacement_count > 0)
        {
            pcg32_random_t rng;
            pcg32_srandom_r(&rng, node.seed, 3);
            const size_t n = pcg32_boundedrand_r(&rng, replacement_count);
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
        else
        {
            update_min_max(node, &out, false);
            num_nodes_finished++;
        }
    }

    out.nodes = nodes;
    return out;
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

typedef struct {
    vec2 min_corner;
    vec2 size;
    vec2 potential_min_corner;
    vec2 potential_size;
    void** vertices;
} GeneratedVertices;

GeneratedVertices gen_vertices(Camera camera)
{
    const size_t target_node_count = 1000 * 1000;
    const float min_area = 5.992f / target_node_count;
    // TODO replace with gen_nodes to use rules instead
    GeneratedNodes nodes = gen_random_nodes(min_area, camera);
    return (GeneratedVertices) {
        .min_corner = { nodes.min[0], nodes.min[1] },
        .size = { nodes.max[0] - nodes.min[0], nodes.max[1] - nodes.min[1] },
        .potential_min_corner = {
            nodes.potential_min[0],
            nodes.potential_min[1]
        },
        .potential_size = {
            nodes.potential_max[0] - nodes.potential_min[0],
            nodes.potential_max[1] - nodes.potential_min[1]
        },
        .vertices = gen_vertices_from_nodes(nodes.nodes),
    };
}

void kdtree_assign_bounds(KDTree* tree, GeneratedVertices generated, Camera cam)
{
    // TODO ensure min_corner and size are within camera constraints
    float hyp = calc_camera_hyp(cam);

    for (size_t i = 0; i < 2; ++i)
    {
        tree->min_corner[i] = max(generated.min_corner[i], cam.position[i] - hyp);
        tree->size[i] = min(hyp * 2.0f, generated.size[i]);
        tree->potential_min_corner[i] = generated.potential_min_corner[i];
        tree->potential_size[i] = generated.potential_size[i];
    }
}

KDTree kdtree_init(Shader* shader, Camera camera)
{
    GeneratedVertices generated = gen_vertices(camera);
    void** vertices = generated.vertices;

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

    KDTree tree = {
        .graph = graph_init(
            shader,
            vertices,
            NULL,
            attributes
        ),
    };

    kdtree_assign_bounds(&tree, generated, camera);

    return tree;
}

void kdtree_regenerate(KDTree* tree, Camera camera)
{
    GLenum* types = NULL;
    arrput(types, GL_FLOAT);
    arrput(types, GL_UNSIGNED_BYTE);
    GeneratedVertices generated = gen_vertices(camera);
    graph_update_vertices(&tree->graph, generated.vertices, types);
    kdtree_assign_bounds(tree, generated, camera);
    arrfree(types);
}

void kdtree_check_camera(KDTree* tree, Camera camera)
{
    // Regenerate if we are about to show an ungenerated region
    // OR if the zoom level has doubled

    // TODO Check if the tree can even be generated further in the desired dir
    // TODO Make min area take the zoom level into account

    // TODO add a mechanism to prevent this from occuring every frame in a
    // worst-case scenario where the camera is extremely zoomed
    // TODO try 64 instead
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
        for (size_t dim = 0; dim < 2; ++dim) (i & (1 << dim)
            ? glm_vec2_add
            : glm_vec2_sub) (corner, local[dim], corner);
        for (size_t dim = 0; dim < 2; ++dim)
        {
            if ((
                corner[dim] <= tree->min_corner[dim] &&
                tree->min_corner[dim] > tree->potential_min_corner[dim]
            ) || (
                corner[dim] >= tree->min_corner[dim] + tree->size[dim] &&
                tree->min_corner[dim] + tree->size[dim] >
                    tree->potential_min_corner[dim] + tree->potential_size[dim]
            ))
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
