#define GLEW_STATIC
#include "graph.h"
#include "utility.h"
#include <GL/glew.h>
#include <stdio.h>
#include <stdlib.h> // free

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
    Shader shader,
    size_t vertices_size,
    float* vertices,
    size_t component_count,
    Component components[]
) {
    Graph graph = {
        .vertices_size = vertices_size,
        .vertices = vertices,
    };

    glGenVertexArrays(1, &graph.vao);
    glGenBuffers(1, &graph.vbo);
    glBindVertexArray(graph.vao);

    const GLenum usage = GL_STATIC_DRAW;

    glBindBuffer(GL_ARRAY_BUFFER, graph.vbo);
    glBufferData(GL_ARRAY_BUFFER, graph.vertices_size, graph.vertices, usage);

    size_t numbers_per_vertex = 0;
    for (size_t i = 0; i < component_count; i++)
    {
        numbers_per_vertex += components[i].size;
    }
    graph.stride = numbers_per_vertex * sizeof(graph.vertices[0]);

    const GLboolean force_cast_to_float = GL_FALSE;
    const GLboolean normalise_fixed_point_values = GL_FALSE;

    unsigned char* first = 0;
    GLboolean error = GL_FALSE;

    for (size_t i = 0; i < component_count; i++)
    {
        const Component c = components[i];
        const GLint index = glGetAttribLocation(shader.id, c.name);
        if (index == -1)
        {
            fprintf(stderr, "Failed to find %s in shader\n", c.name);
            error = GL_TRUE;
        }
        glEnableVertexAttribArray(index);
        switch (c.type) {
            case GL_NONE:
                break;
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
                        c.size,
                        c.type,
                        graph.stride,
                        first
                    );
                    break;
                }
            case GL_DOUBLE:
                if (!force_cast_to_float)
                {
                    glVertexAttribLPointer(
                        index,
                        c.size,
                        c.type,
                        graph.stride,
                        first
                    );
                    break;
                }
            default:
                glVertexAttribPointer(
                    index,
                    c.size,
                    c.type,
                    normalise_fixed_point_values,
                    graph.stride,
                    first
                );
        }

        first += c.size * sizeof(graph.vertices[0]);
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
