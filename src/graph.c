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
/// @param attributes Attributes
/// @param stride Pointer to the graph stride for the graph VBO
/// @param shader Shader to query for attribute names, or NULL for positions
/// @return Whether an error occurred
GLboolean init_attributes
(
    Attribute* attributes,
    size_t* stride,
    Shader* shader
) {
    *stride = 0;
    for (size_t i = 0; i < arrlenu(attributes); ++i)
    {
        const Attribute attr = attributes[i];
        *stride += attr.size * gl_sizeof(attr.type);
    }

    GLubyte* first = 0;
    size_t location_index = 0;

    for (size_t attr_index = 0; attr_index < arrlenu(attributes); ++attr_index)
    {
        const Attribute attr = attributes[attr_index];

        // Skip nameless attributes, allowing them to act as gaps
        if (attr.name && *attr.name)
        {
            // If shader is NULL then use location indices instead
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
    for (size_t i = 0; i < arrlenu(graph->vertices); ++i)
    {
        arrfree(graph->vertices[i]);
    }
    arrfree(graph->vertices);
    arrfree(graph->strides);
}

void graph_free_all(Graph* graph)
{
    arrfree(graph->indices);
    // Without index data there is no point in keeping vertex data
    graph_free_vertices(graph);
}

void graph_kill(Graph* graph)
{
    glDeleteVertexArrays(1, &graph->vao);
    graph->vao = 0;
    glDeleteBuffers(arrlenu(graph->vbos), graph->vbos);
    arrfree(graph->vbos);
    glDeleteBuffers(1, &graph->ebo);
    graph->ebo = 0;
    graph_free_all(graph);
}

const GLenum index_type = GL_UNSIGNED_INT;

Graph graph_init
(
    Shader* shader,
    GLenum* vertex_types,
    void** vertices,
    GLuint* indices,
    Attribute** attributes
) {
    Graph graph = {
        .vbos = NULL,
        .vertices = vertices,
        .indices = indices,
        .strides = NULL,
    };

    if (!arrlenu(graph.vertices)) return graph;

    glGenVertexArrays(1, &graph.vao);
    glBindVertexArray(graph.vao);

    const GLenum usage = GL_STATIC_DRAW;

    arrsetlen(graph.vbos, arrlenu(graph.vertices));
    glGenBuffers(arrlenu(graph.vertices), graph.vbos);
    arrsetlen(graph.strides, arrlenu(graph.vertices));

    GLboolean error = GL_FALSE;

    for (size_t i = 0; i < arrlenu(graph.vertices); ++i)
    {
        glBindBuffer(GL_ARRAY_BUFFER, graph.vbos[i]);
        glBufferData(
            GL_ARRAY_BUFFER,
            arrlenu(graph.vertices[i]) * gl_sizeof(vertex_types[i]),
            graph.vertices[i],
            usage
        );

        error |= init_attributes(
            attributes[i],
            &(graph.strides[i]),
            shader
        );

        arrfree(attributes[i]);

        graph.strides[i] /= gl_sizeof(vertex_types[i]);
    }

    arrfree(attributes);

    if (arrlenu(graph.indices))
    {
        glGenBuffers(1, &graph.ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, graph.ebo);
        glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            arrlenu(graph.indices) * gl_sizeof(index_type),
            graph.indices,
            usage
        );
    }

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
    return graph_init(NULL, NULL, NULL, NULL, NULL);
}

GLboolean graph_ok(Graph graph)
{
    return graph.vao
        && arrlenu(graph.vertices)
        && arrlenu(graph.vertices[0])
        && arrlenu(graph.strides);
}

void graph_draw(Graph graph, GLenum mode)
{
    if (!graph_ok(graph)) return;
    glBindVertexArray(graph.vao);

    if (arrlenu(graph.indices))
    {
        glDrawElements(mode, arrlenu(graph.indices), index_type, 0);
    }
    else
    {
        glDrawArrays(mode, 0, arrlenu(graph.vertices[0]) / graph.strides[0]);
    }

    glBindVertexArray(0);
}
