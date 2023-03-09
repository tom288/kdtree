#define GLEW_STATIC
#include "graph.h"
#include "utility.h"
#include <GL/glew.h>
#include <stdio.h>
#include <stdlib.h> // free
#include <stb_ds.h>

/// @brief Inform OpenGL about our attribute
/// @param index Attribute index corresponding to graph args or shader location
/// @param attr Attribute
/// @param stride The stride used in the graph VBO
/// @param first Pointer offset for the first value of this attribute
void init_attribute(GLint index, Attribute attr, size_t stride, GLubyte* first)
{
    const GLboolean force_cast_to_float = GL_FALSE;
    const GLboolean normalise_fixed_point_values = GL_FALSE;

    glEnableVertexAttribArray(index);
    switch (attr.type)
    {
        case GL_BYTE:
        case GL_UNSIGNED_BYTE:
        case GL_SHORT:
        case GL_UNSIGNED_SHORT:
        case GL_INT:
        case GL_UNSIGNED_INT:
            if (!force_cast_to_float)
            {
                glVertexAttribIPointer(
                    index,
                    attr.size,
                    attr.type,
                    stride,
                    first
                );
                break;
            }
        case GL_DOUBLE:
            if (!force_cast_to_float)
            {
                glVertexAttribLPointer(
                    index,
                    attr.size,
                    attr.type,
                    stride,
                    first
                );
                break;
            }
        default:
            glVertexAttribPointer(
                index,
                attr.size,
                attr.type,
                normalise_fixed_point_values,
                stride,
                first
            );
    }
}

/// @brief Inform OpenGL about every attribute
/// @param attribute_count Number of attributes
/// @param attributes Attributes
/// @param stride Pointer to the graph stride for the graph VBO
/// @param shader Shader to query for attribute names, or NULL for positions
/// @return Whether an error occurred
GLboolean init_attributes
(
    size_t attribute_count,
    Attribute attributes[],
    size_t* stride,
    Shader* shader
) {
    for (size_t i = 0; i < attribute_count; ++i)
    {
        const Attribute attr = attributes[i];
        *stride += attr.size * gl_sizeof(attr.type);
    }

    GLubyte* first = 0;
    size_t location_index = 0;

    for (size_t attr_index = 0; attr_index < attribute_count; ++attr_index)
    {
        const Attribute attr = attributes[attr_index];

        // Skip nameless attributes, allowing them to act as gaps
        if (attr.name && *attr.name)
        {
            // If shader is NULL then use location indicies instead
            GLint index = location_index++;

            if (shader)
            {
                const GLint name_index = glGetAttribLocation(
                    shader->id,
                    attr.name
                );
                if (name_index == -1)
                {
                    fprintf(stderr, "Failed to find %s in shader\n", attr.name);
                    return GL_TRUE;
                }
                else
                {
                    index = name_index;
                }
            }

            init_attribute(index, attr, *stride, first);
        }

        first += attr.size * gl_sizeof(attr.type);
    }

    return GL_FALSE;
}

void graph_free_vertices(Graph* graph)
{
    arrfree(graph->vertices);
    graph->stride = 0;
}

void graph_free_all(Graph* graph)
{
    free(graph->indices);
    graph->indices = NULL;
    graph->indices_size = 0;
    // Without index data there is no point in keeping vertex data
    graph_free_vertices(graph);
}

void graph_kill(Graph* graph)
{
    glDeleteVertexArrays(1, &graph->vao);
    graph->vao = 0;
    glDeleteBuffers(1, &graph->vbo);
    graph->vbo = 0;
    glDeleteBuffers(1, &graph->ebo);
    graph->ebo = 0;
    graph_free_all(graph);
}

Graph graph_init
(
    Shader* shader,
    GLenum vertex_type,
    void* vertices,
    size_t indices_size,
    GLuint* indices,
    size_t attribute_count,
    Attribute attributes[]
) {
    Graph graph = {
        .vertices = vertices,
        .stride = 0,
        .indices_size = indices_size,
        .indices = indices,
    };

    if (!arrlen(graph.vertices)) return graph;

    glGenVertexArrays(1, &graph.vao);
    glBindVertexArray(graph.vao);

    const GLenum usage = GL_STATIC_DRAW;

    glGenBuffers(1, &graph.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, graph.vbo);
    glBufferData(GL_ARRAY_BUFFER, arrlen(graph.vertices) * gl_sizeof(vertex_type), graph.vertices, usage);

    if (graph.indices_size && graph.indices)
    {
        glGenBuffers(1, &graph.ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, graph.ebo);
        glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            graph.indices_size,
            graph.indices,
            usage
        );
    }

    const GLboolean error = init_attributes(
        attribute_count,
        attributes,
        &graph.stride,
        shader
    );

    graph.stride /= gl_sizeof(vertex_type);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    if (error || gl_error())
    {
        graph_kill(&graph);
    }

    return graph;
}

Graph graph_init_empty()
{
    return graph_init(NULL, GL_FLOAT, NULL, 0, NULL, 0, NULL);
}

GLboolean graph_ok(Graph graph)
{
    return !!graph.vao;
}

void graph_draw(Graph graph)
{
    if (!graph_ok(graph) || !arrlen(graph.vertices)) return;
    glBindVertexArray(graph.vao);
    if (graph.indices_size && graph.indices)
    {
        const GLenum index_type = GL_UNSIGNED_INT;
        const size_t num_indices = graph.indices_size / gl_sizeof(index_type);
        glDrawElements(GL_TRIANGLES, num_indices, index_type, 0);
    }
    else
    {
        glDrawArrays(GL_TRIANGLES, 0, arrlen(graph.vertices) / graph.stride);
    }
    glBindVertexArray(0);
}
