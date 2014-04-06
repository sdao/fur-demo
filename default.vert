#version 330

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 norm;
layout(location = 2) in vec2 texCoord;

uniform mat4 modelView;
uniform mat4 projection;

uniform float currentLayer;
uniform float maxHairLength;
uniform vec3 displacement;

out vec2 fragTexCoord;
out float fragCurrentLayer;

void main(void) {
  vec3 newPos = pos + norm * maxHairLength * currentLayer;
  
  float dispFactor = pow(currentLayer, 3.0);
  vec3 dispPos = newPos + displacement * dispFactor;
  
  gl_Position = projection * modelView * vec4(dispPos, 1.0);
  fragTexCoord = texCoord;
  fragCurrentLayer = currentLayer;
}