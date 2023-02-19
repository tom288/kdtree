#define GLEW_STATIC
#include <GL/glew.h>
#include "utility.h"

static GLuint vbo;
static GLuint vao;

float vertices[] = {
     0.0f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f, -0.5f,  0.0f,  0.0f,  1.0f,
};

void triangle_init() {
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Attribute properties for the vertex positions

    // Here we are using manual indices and then referring to them in the
    // fragment shaders. This grants opportunities for very confusing mistakes.
    // We could use glGetAttribLocation(id, "position") instead, but we'd need
    // to be able to interrogate the shader. It returns -1 if not found.
    GLint index = 0;
    GLint components = 2;
    GLint total_components = 5;
    GLenum type = GL_FLOAT;
    GLboolean normalise = GL_FALSE;
    GLsizei stride = total_components * sizeof(vertices[0]);
    void* first = 0;

    glEnableVertexAttribArray(index);
    glVertexAttribPointer(index, components, type, normalise, stride, first);

    index++;
    first += components * sizeof(vertices[0]);
    components = 3;

    glEnableVertexAttribArray(index);
    glVertexAttribPointer(index, components, type, normalise, stride, first);

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
