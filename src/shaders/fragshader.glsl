#version 330 core

// Define constants
#define M_PI 3.141593

// Specify the inputs to the fragment shader
in vec3 N;
in vec3 V;
in vec3 L;
in vec2 textureCoords;

// Texture
uniform sampler2D sampler;

// Material properties
// Ambient / Diffuse / Specular constants
uniform float ka;
uniform float kd;
uniform float ks;
// Specular exponent
uniform int p;

uniform vec3 lightColor;

// Output color
out vec4 fColor;

void main() {
  vec3 textureColor = texture(sampler, textureCoords).rgb;

  float NL = dot(N,L);

  // Bounce vector direction
  vec3 R = normalize((2 * NL * N) - L);

  vec3 Ia = ka * textureColor;
  vec3 Id = max(0, NL) * textureColor * lightColor * kd;

  vec3 Is;
  if (NL < 0) {
    Is = vec3(0,0,0);
  } else {
    Is = pow(max(0, dot(R,V)),p)  *  lightColor * ks;
  }

  fColor = vec4(Ia + Id + Is, 1.0F);
}
