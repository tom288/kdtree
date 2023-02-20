#define GLEW_STATIC
#include <GL/glew.h>
#include "utility.h"

typedef struct Rectangle {
    GLuint vao;
    GLuint vbo;
    GLint total_components;
} Rectangle;

float vertices[] = {
    -0.5f, -0.5f,  0.0f,  0.0f,  1.0f,
     0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
    -0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
};

Rectangle rectangle_init() {
    Rectangle rect;
    glGenVertexArrays(1, &rect.vao);
    glGenBuffers(1, &rect.vbo);
    glBindVertexArray(rect.vao);

    glBindBuffer(GL_ARRAY_BUFFER, rect.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Attribute properties for the vertex positions

    // Here we are using manual indices and then referring to them in the
    // fragment shaders. This has opportunities for very confusing mistakes.
    // We could use glGetAttribLocation(id, "position") instead, but we'd need
    // to be able to interrogate the shader. It returns -1 if not found.

    rect.total_components = 5;
    GLint index = 0;
    GLint components = 2;
    GLenum type = GL_FLOAT;
    GLboolean normalise = GL_FALSE;
    GLsizei stride = rect.total_components * sizeof(vertices[0]);
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

    return rect;
}

void rectangle_draw(Rectangle rect) {
    glBindVertexArray(rect.vao);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / sizeof(vertices[0]) / rect.total_components);
    glBindVertexArray(0);
}

void rectangle_kill(Rectangle* rect) {
    glDeleteVertexArrays(1, &rect->vao);
    glDeleteBuffers(1, &rect->vbo);
    rect->vao = 0;
    rect->vbo = 0;
    rect->total_components = 0;
}
