#version 330

in vec3 fragColor;
in vec2 fragTexCoord;
out vec4 outputColor;
uniform sampler2D tex;

void main(void) {
  outputColor = texture(tex, fragTexCoord) * vec4(fragColor, 1.0);
}