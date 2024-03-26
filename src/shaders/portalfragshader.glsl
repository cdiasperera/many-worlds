#version 330 core

in vec2 textureCoords;

uniform float borderWidth;
uniform bool renderBorder;

// Output color
out vec4 fColor;

void main() {
  if (1 - abs(textureCoords.x) < borderWidth || 1 - abs(textureCoords.y) < borderWidth) {
      if (renderBorder) {
        fColor = vec4(0.99, 0.84, 0.4, 1);
      } else {
        discard;
      }
  } else {
    if (renderBorder) {
      discard;
    } else {
      fColor = vec4(1, 1, 1, 1);
    }
  }
}
