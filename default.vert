#version 330

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in float layer;
//layout(location = 3) in vec3 norm;

uniform mat4 modelView;
uniform mat4 projection;
uniform vec3 displacement;

out vec2 fragTexCoord;
out float fragLayer;

void main(void) {
  vec3 layerDisplacement = pow(layer, 3.0) * displacement;
  vec4 newPos = vec4(pos + layerDisplacement, 1.0);
  gl_Position = projection * modelView * newPos;
  
  fragTexCoord = texCoord;
  fragLayer = layer;
}