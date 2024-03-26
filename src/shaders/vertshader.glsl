#version 330 core

// Define constants
#define M_PI 3.141593

// Specify the input locations of attributes
layout(location = 0) in vec3 vertCoordinates_in;
layout(location = 1) in vec3 vertNormal_in;
layout(location = 2) in vec2 vertTextureCoords_in;

// Light properties
uniform vec3 lightCoordinates;

// Specify the Uniforms of the vertex shader
uniform mat4 modelViewTransform;
uniform mat4 projectionTransform;
uniform mat3 normalMatrix;

// Specify the output of the vertex stage
out vec3 N;
out vec3 V;
out vec3 L;
out vec2 textureCoords;
out vec3 vertNormal;

void main() {
  vec4 P = modelViewTransform * vec4(vertCoordinates_in, 1.0F);
  // gl_Position is the output (a vec4) of the vertex shader
  gl_Position = projectionTransform * P;

  // We are assuming that camera is outside model, so no need to flip normal
  N = normalize(normalMatrix * vertNormal_in);

  // Direction to light from point
  L = normalize(lightCoordinates - P.xyz);

  // Direction to camera, which is placed at the origin
  V = normalize(-P.xyz);

  textureCoords = vertTextureCoords_in;
  vertNormal = normalize(normalMatrix * vertNormal_in);
}
