#define GLEW_STATIC
#include <GL/glew.h>

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
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(vertices[0]), (GLvoid*)0); // 2 

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
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
