#define GLEW_STATIC
#include "graph.h"
#include "utility.h"
#include <GL/glew.h>
#include <stdio.h>
#include <stdlib.h> // free

/// @brief Inform OpenGL about our attribute
/// @param index Attribute index corresponding to graph args or shader location
/// @param attr Attribute information
/// @param stride The stride used in the graph VBO
/// @param first Pointer offset for the first value of this attribute
void init_attr(GLint index, Attribute attr, size_t stride, unsigned char* first)
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

void graph_kill(Graph* graph)
{
    glDeleteVertexArrays(1, &graph->vao);
    graph->vao = 0;
    glDeleteBuffers(1, &graph->vbo);
    graph->vbo = 0;
    graph->vertices_size = 0;
    free(graph->vertices);
    graph->vertices = NULL;
    graph->stride = 0;
}

Graph graph_init
(
    Shader* shader,
    size_t vertices_size,
    float* vertices,
    size_t attribute_count,
    Attribute attributes[]
) {
    Graph graph = {
        .vertices_size = vertices_size,
        .vertices = vertices,
        .stride = 0,
    };

    glGenVertexArrays(1, &graph.vao);
    glGenBuffers(1, &graph.vbo);
    glBindVertexArray(graph.vao);

    const GLenum usage = GL_STATIC_DRAW;

    glBindBuffer(GL_ARRAY_BUFFER, graph.vbo);
    glBufferData(GL_ARRAY_BUFFER, graph.vertices_size, graph.vertices, usage);

    for (size_t i = 0; i < attribute_count; ++i)
    {
        Attribute attribute = attributes[i];
        graph.stride += attribute.size * gl_sizeof(attribute.type);
    }

    unsigned char* first = 0;
    GLboolean error = GL_FALSE;
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
                GLint name_index = glGetAttribLocation(shader->id, attr.name);
                if (name_index == -1)
                {
                    fprintf(stderr, "Failed to find %s in shader\n", attr.name);
                }
                else
                {
                    index = name_index;
                }
            }

            init_attr(index, attr, graph.stride, first);
        }

        first += attr.size * gl_sizeof(attr.type);
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

GLboolean graph_ok(Graph graph)
{
    return !!graph.vao;
}

void graph_draw(Graph graph)
{
    if (!graph_ok(graph)) return;
    glBindVertexArray(graph.vao);
    glDrawArrays(GL_TRIANGLES, 0, graph.vertices_size / graph.stride);
    glBindVertexArray(0);
}
