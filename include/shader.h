#ifndef SHADER_H
#define SHADER_H

#include <glad/gl.h>

typedef struct shader_program {
  GLuint program_handle;
  char vertex_shader_path[1024];
  char fragment_shader_path[1024];
} shader_program;

char* readShader(const char* path);

GLuint create_shader_program_from_string(const char* vertex_shader, const char* fragment_shader); 

GLuint create_shader_program_from_files(const char* vertex_shader_path, const char* fragment_shader_path); 

void reload_shader_program(GLuint* shader_program, const char* vertex_shader_path, const char* fragment_shader_path); 

#endif //SHADER_H
