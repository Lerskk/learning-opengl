#include "shader.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

char* readShader(const char* path) {
  FILE* fptr;

  fptr = fopen(path, "r");
  if (fptr == NULL) {
    printf("hi %s\n", path);
    fprintf(stderr, "Shader failed to open\n");
    return NULL;
  }

  fseek(fptr, 0L, SEEK_END);
  long shader_size = ftell(fptr);
  fseek(fptr, 0L, SEEK_SET);

  char* shader = (char*)malloc(shader_size + 1);
  if (!shader) {
    fprintf(stderr, "Shader allocation failed\n");
    fclose(fptr);
    return NULL;
  }

  const size_t ret_code = fread(shader, 1, shader_size, fptr);
  shader[ret_code] = '\0';

  fclose(fptr);
  
  return shader;
}

GLuint create_shader_program_from_string(const char* vertex_shader, const char* fragment_shader) {
  GLuint vs = glCreateShader( GL_VERTEX_SHADER );
  glShaderSource(vs, 1, &vertex_shader, NULL);
  glCompileShader(vs);

  int params = -1;
  glGetShaderiv(vs, GL_COMPILE_STATUS, &params);
    
  if (GL_TRUE != params) {
    int max_length = 2048, actual_length = 0;
    char slog[2048];
    glGetShaderInfoLog(vs, max_length, &actual_length, slog);
    fprintf(stderr, "ERROR: shader index %u did not compile.\n%s\n", vs, slog);
    return 1;
  }

  GLuint fs = glCreateShader( GL_FRAGMENT_SHADER );
  glShaderSource(fs, 1, &fragment_shader, NULL);
  glCompileShader(fs);

  glGetShaderiv(fs, GL_COMPILE_STATUS, &params);

  if (GL_TRUE != params) {
    int max_length = 2048, actual_length = 0;
    char slog[2048];
    glGetShaderInfoLog(fs, max_length, &actual_length, slog);
    fprintf(stderr, "ERROR: shader index %u did not compile.\n%s\n", fs, slog);
    return 1;
  }

  GLuint shader_program = glCreateProgram();
  glAttachShader(shader_program, fs);
  glAttachShader(shader_program, vs);
  glLinkProgram(shader_program);

  glGetProgramiv(shader_program, GL_LINK_STATUS, &params);

  if (GL_TRUE != params) {
    int max_length = 2048, actual_length = 0;
    char plog[2048];
    glGetProgramInfoLog(shader_program, max_length, &actual_length, plog);
    fprintf(stderr, "ERROR: Could not link shader program GL index %u.\n%s\n", shader_program, plog);
    return 1;
  }

  return shader_program;
}

GLuint create_shader_program_from_files(const char* vertex_shader_path, const char* fragment_shader_path) {
  assert(vertex_shader_path && fragment_shader_path);

  printf("Loading shader from files %s and %s\n", vertex_shader_path, fragment_shader_path);

  const char* vertex_shader = readShader(vertex_shader_path);
  const char* fragment_shader = readShader(fragment_shader_path);

  return create_shader_program_from_string(vertex_shader, fragment_shader);
}

void reload_shader_program(GLuint* shader_program, const char* vertex_shader_path, const char* fragment_shader_path) {
  assert(shader_program && vertex_shader_path && fragment_shader_path);

  GLuint reloaded_program = create_shader_program_from_files(vertex_shader_path, fragment_shader_path);
  
  if(reloaded_program) {
    glDeleteProgram(*shader_program);
    *shader_program = reloaded_program;
    return;
  }
  fprintf(stderr, "ERROR: new shader program creation failed");
}

