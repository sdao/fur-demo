#version 330

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 norm;
layout(location = 2) in vec2 texCoord;

uniform mat4 modelView;
uniform mat4 projection;

uniform float currentLayer;
uniform float maxHairLength;

out vec2 fragTexCoord;
out float fragCurrentLayer;

void main(void) {
  vec3 newPos = pos + norm * maxHairLength * currentLayer;
  gl_Position = projection * modelView * vec4(newPos, 1.0);
  fragTexCoord = texCoord;
  fragCurrentLayer = currentLayer;
}