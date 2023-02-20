#pragma once

#include "shader.c"

Shader shader_init(char* vertex, char* fragment, char* geometry);
void shader_use(Shader id);
void shader_kill(Shader* id);
void shader_set_1i(GLuint id, char* name, GLint n);
void shader_set_1ui(GLuint id, char* name, GLuint n);
void shader_set_1f(GLuint id, char* name, GLfloat n);
void shader_set_2i(GLuint id, char* name, GLint x, GLint y);
void shader_set_2ui(GLuint id, char* name, GLuint x, GLuint y);
void shader_set_2f(GLuint id, char* name, GLfloat x, GLfloat y);
void shader_set_3i(GLuint id, char* name, GLint x, GLint y, GLint z);
void shader_set_3ui(GLuint id, char* name, GLuint x, GLuint y, GLuint z);
void shader_set_3f(GLuint id, char* name, GLfloat x, GLfloat y, GLfloat z);
void shader_set_4i(GLuint id, char* name, GLint x, GLint y, GLint z, GLint w);
void shader_set_4ui(GLuint id, char* name, GLuint x, GLuint y, GLuint z, GLuint w);
void shader_set_4f(GLuint id, char* name, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
void shader_set_1iv(GLuint id, char* name, GLsizei count, GLint* v);
void shader_set_1uiv(GLuint id, char* name, GLsizei count, GLuint* v);
void shader_set_1fv(GLuint id, char* name, GLsizei count, GLfloat* v);
void shader_set_2iv(GLuint id, char* name, GLsizei count, GLint* v);
void shader_set_2uiv(GLuint id, char* name, GLsizei count, GLuint* v);
void shader_set_2fv(GLuint id, char* name, GLsizei count, GLfloat* v);
void shader_set_3iv(GLuint id, char* name, GLsizei count, GLint* v);
void shader_set_3uiv(GLuint id, char* name, GLsizei count, GLuint* v);
void shader_set_3fv(GLuint id, char* name, GLsizei count, GLfloat* v);
void shader_set_4iv(GLuint id, char* name, GLsizei count, GLint* v);
void shader_set_4uiv(GLuint id, char* name, GLsizei count, GLuint* v);
void shader_set_4fv(GLuint id, char* name, GLsizei count, GLfloat* v);
void shader_set_m2fv(GLuint id, char* name, GLsizei count, GLfloat* m);
void shader_set_m3fv(GLuint id, char* name, GLsizei count, GLfloat* m);
void shader_set_m4fv(GLuint id, char* name, GLsizei count, GLfloat* m);
void shader_set_m2x3fv(GLuint id, char* name, GLsizei count, GLfloat* m);
void shader_set_m3x2fv(GLuint id, char* name, GLsizei count, GLfloat* m);
void shader_set_m2x4fv(GLuint id, char* name, GLsizei count, GLfloat* m);
void shader_set_m4x2fv(GLuint id, char* name, GLsizei count, GLfloat* m);
void shader_set_m3x4fv(GLuint id, char* name, GLsizei count, GLfloat* m);
void shader_set_m4x3fv(GLuint id, char* name, GLsizei count, GLfloat* m);
