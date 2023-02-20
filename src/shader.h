#pragma once

#include "shader.c"

Shader shader_init(char* vertex, char* fragment, char* geometry);
void shader_use(Shader shader);
void shader_kill(Shader* shader);
GLboolean shader_set_1i(Shader shader, char* name, GLint n);
GLboolean shader_set_1ui(Shader shader, char* name, GLuint n);
GLboolean shader_set_1f(Shader shader, char* name, GLfloat n);
GLboolean shader_set_2i(Shader shader, char* name, GLint x, GLint y);
GLboolean shader_set_2ui(Shader shader, char* name, GLuint x, GLuint y);
GLboolean shader_set_2f(Shader shader, char* name, GLfloat x, GLfloat y);
GLboolean shader_set_3i(Shader shader, char* name, GLint x, GLint y, GLint z);
GLboolean shader_set_3ui(Shader shader, char* name, GLuint x, GLuint y, GLuint z);
GLboolean shader_set_3f(Shader shader, char* name, GLfloat x, GLfloat y, GLfloat z);
GLboolean shader_set_4i(Shader shader, char* name, GLint x, GLint y, GLint z, GLint w);
GLboolean shader_set_4ui(Shader shader, char* name, GLuint x, GLuint y, GLuint z, GLuint w);
GLboolean shader_set_4f(Shader shader, char* name, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
GLboolean shader_set_1iv(Shader shader, char* name, GLsizei count, GLint* v);
GLboolean shader_set_1uiv(Shader shader, char* name, GLsizei count, GLuint* v);
GLboolean shader_set_1fv(Shader shader, char* name, GLsizei count, GLfloat* v);
GLboolean shader_set_2iv(Shader shader, char* name, GLsizei count, GLint* v);
GLboolean shader_set_2uiv(Shader shader, char* name, GLsizei count, GLuint* v);
GLboolean shader_set_2fv(Shader shader, char* name, GLsizei count, GLfloat* v);
GLboolean shader_set_3iv(Shader shader, char* name, GLsizei count, GLint* v);
GLboolean shader_set_3uiv(Shader shader, char* name, GLsizei count, GLuint* v);
GLboolean shader_set_3fv(Shader shader, char* name, GLsizei count, GLfloat* v);
GLboolean shader_set_4iv(Shader shader, char* name, GLsizei count, GLint* v);
GLboolean shader_set_4uiv(Shader shader, char* name, GLsizei count, GLuint* v);
GLboolean shader_set_4fv(Shader shader, char* name, GLsizei count, GLfloat* v);
GLboolean shader_set_m2(Shader shader, char* name, GLsizei count, GLfloat* m);
GLboolean shader_set_m3(Shader shader, char* name, GLsizei count, GLfloat* m);
GLboolean shader_set_m4(Shader shader, char* name, GLsizei count, GLfloat* m);
GLboolean shader_set_m2x3(Shader shader, char* name, GLsizei count, GLfloat* m);
GLboolean shader_set_m3x2(Shader shader, char* name, GLsizei count, GLfloat* m);
GLboolean shader_set_m2x4(Shader shader, char* name, GLsizei count, GLfloat* m);
GLboolean shader_set_m4x2(Shader shader, char* name, GLsizei count, GLfloat* m);
GLboolean shader_set_m3x4(Shader shader, char* name, GLsizei count, GLfloat* m);
GLboolean shader_set_m4x3(Shader shader, char* name, GLsizei count, GLfloat* m);
