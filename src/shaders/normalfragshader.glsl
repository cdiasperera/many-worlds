#version 330 core

// Specify the inputs to the fragment shader
in vec3 vertNormal;

// Specify the output of the fragment shader
out vec4 fColor;

void main() {
  fColor = vec4(0.5*normalize(vertNormal)+0.5, 1.0F);
}
