#include "shader.h"
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

void error_callback_glfw(int error, const char* description) {
  fprintf(stderr, "GLFW ERROR: code %i msg: %s.\n", error, description);
}

int main( void ) {
  printf("Starting GLFW %s.\n", glfwGetVersionString());

  glfwSetErrorCallback(error_callback_glfw);

  if ( !glfwInit() ) {
    fprintf( stderr, "ERROR: could not start GLFW3.\n" );
    return 1;
  }

  glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
  glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 1 );
  glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
  glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
  
  glfwWindowHint(GLFW_SAMPLES, 16);


  bool full_screen = false;

  GLFWmonitor *mon = NULL;  
  int win_w = 800, win_h = 600;
  
  if(full_screen) {
    mon = glfwGetPrimaryMonitor();  

    const GLFWvidmode* mode = glfwGetVideoMode(mon);

    // borderless full screen mode
    glfwWindowHint(GLFW_RED_BITS, mode->redBits); 
    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

    win_w = mode->width;
    win_h = mode->height;
  }

  GLFWwindow* window = glfwCreateWindow( 
      win_w,
      win_h,
      "Hello Triangle",
      mon,
      NULL
  );

  if ( !window ) {
    fprintf( stderr, "ERROR: Could not open window with GLFW3.\n" );
    glfwTerminate();
    return 1;
  }
  glfwMakeContextCurrent( window );
  glfwSwapInterval(0);
                                  
  int version_glad = gladLoadGL( glfwGetProcAddress );
  if ( version_glad == 0 ) {
    fprintf( stderr, "ERROR: Failed to initialize OpenGL context.\n" );
    return 1;
  }

  float points[] = {
    0.0f, 0.5f, 1.0f,
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f
  };

  int resolution = 100;
  float circle[resolution * 3];

  for(int i = 0; i < resolution; ++i) {
    float angle = (2.0f * M_PI * i) / (float)resolution; // radians
    float x = cosf(angle) * 0.5;
    float y = sinf(angle) * 0.5;

    circle[i * 3 + 0] = x;
    circle[i * 3 + 1] = y;
    circle[i * 3 + 2] = 0.0f;
  }

  float colours[] = {
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f
  };

  GLuint vbo_points = 0;
  glGenBuffers( 1, &vbo_points );
  glBindBuffer( GL_ARRAY_BUFFER, vbo_points );
  glBufferData( GL_ARRAY_BUFFER, resolution * 3 * sizeof(float), circle, GL_STATIC_DRAW);

  GLuint vbo_colours = 0;
  glGenBuffers( 1, &vbo_colours);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_colours);
  glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), colours, GL_STATIC_DRAW);


  GLuint vao = 0;
  glGenVertexArrays( 1, &vao );
  glBindVertexArray( vao );
  glEnableVertexAttribArray( 0 );
  glBindBuffer( GL_ARRAY_BUFFER, vbo_points );
  glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
  glBindBuffer( GL_ARRAY_BUFFER, vbo_colours );
  glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
  
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  
  GLuint shader_program = create_shader_program_from_files("../shaders/shader.vert", "../shaders/shader.frag");

  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  glEnable(GL_PROGRAM_POINT_SIZE);

  double prev_s = glfwGetTime();
  double title_countdown_s = 0.1;

  double mouse_pos = glGetUniformLocation(shader_program, "mouse_pos");

  double x, y;
  while (!glfwWindowShouldClose(window)) {
    double curr_s = glfwGetTime();
    double elapsed_s = curr_s - prev_s;
    prev_s = curr_s;

    title_countdown_s -= elapsed_s;
    if (title_countdown_s <= 0.0 && elapsed_s > 0.0) {
      double fps = 1.0 / elapsed_s;

      char tmp[256];
      sprintf(tmp, "FPS %.2lf", fps);
      glfwSetWindowTitle(window, tmp);
      title_countdown_s = 0.1;
    }

    glfwGetWindowSize(window, &win_w, &win_h);
    glViewport(0, 0, win_w, win_h);

    /* glClearColor(0.6f, 0.6f, 0.8f, 1.0f); */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(shader_program);
    glBindVertexArray(vao);

    glDrawArrays(GL_TRIANGLE_FAN, 0, resolution);
    glfwPollEvents();
    glfwSwapBuffers(window);

    if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_ESCAPE)) {
      glfwSetWindowShouldClose(window, 1);
    }

    if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_R)) {
      reload_shader_program(&shader_program, "../shaders/shader.vert", "../shaders/shader.frag");
    }

    glfwGetCursorPos(window, &x, &y);
    if(GLFW_PRESS == glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT)) {
      glUniform2f(mouse_pos, (x / win_w) * 2.0f - 1.0f, (1.0f - y / win_h) * 2.0f - 1.0f);
    } 
  }

  
  // Close OpenGL window, context, and any other GLFW resources.
  glfwTerminate();
  return 0;
}
