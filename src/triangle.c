#define GLEW_STATIC
#include <GL/glew.h>
#include "utility.h"

static GLuint vbo;
static GLuint vao;

float vertices[] = {
     0.0f,  0.5f, // Vertex 1 (X, Y)
     0.5f, -0.5f, // Vertex 2 (X, Y)
    -0.5f, -0.5f  // Vertex 3 (X, Y)
};

void triangle_init() {
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Attribute properties for the vertex positions
    GLint index = 0;
    GLint components = 2;
    GLint total_components = 2;
    GLenum type = GL_FLOAT;
    GLboolean normalise = GL_FALSE;
    GLsizei stride = total_components * sizeof(vertices[0]);
    void* first = 0;

    glEnableVertexAttribArray(index);
    glVertexAttribPointer(index, components, type, normalise, stride, first);

    // These steps would be necessary if we wanted to extract additional
    // attributes from the same buffer such as colour, normals, or solidity
    index++;
    first += components * sizeof(vertices[0]);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    check_gl_error();
}

void triangle_draw() {
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 3); // 3 = number of vertices
    glBindVertexArray(0);
}

void triangle_kill() {
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
}
