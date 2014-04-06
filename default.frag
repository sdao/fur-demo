#version 330

in vec2 fragTexCoord;
in float fragLayer;

uniform sampler2D fur;
uniform sampler2D color;

out vec4 outputColor;

void main(void) {
  float fakeShadow = mix(0.4, 1.0, fragLayer);
  
  vec4 furData = texture(fur, fragTexCoord);
  vec4 furColor = texture(color, fragTexCoord) * fakeShadow;
  
  float visibility = (fragLayer > furData.r) ? 0.0 : furData.a;
  furColor.a = (fragLayer == 0.0) ? 1.0 : visibility;
  
  outputColor = furColor;
}