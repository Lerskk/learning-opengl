#version 410 core
layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_colour;

out vec3 colour;
uniform vec2 mouse_pos;

void main() {
  vec3 pos = vertex_position + vec3(mouse_pos, 0.0);

  colour = vertex_colour;

  gl_Position = vec4(pos, 1.0);
}
