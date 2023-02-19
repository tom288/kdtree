#include "shader.c"

GLuint shader_init(char* vertex, char* fragment, char* geometry);
void shader_use(GLuint id);
void shader_kill(GLuint id);
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
