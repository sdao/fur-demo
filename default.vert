#version 330

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 color;
layout(location = 2) in vec2 texCoord;
out vec3 fragColor;
out vec2 fragTexCoord;
uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;

void main(void) {
  gl_Position = projectionMatrix * modelViewMatrix * vec4(pos, 1.0);
  fragColor = color;
  fragTexCoord = texCoord;
}