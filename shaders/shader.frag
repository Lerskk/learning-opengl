#version 410 core

in vec3 colour;
out vec4 frag_colour;
uniform float time;

void main() {
  vec3 temp_colour = vec3(1.0, 1.0, 1.0);
  frag_colour = vec4(temp_colour, 1.0);
};

