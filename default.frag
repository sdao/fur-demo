#version 330

in vec2 fragTexCoord;
in float fragCurrentLayer;
uniform sampler2D fur;
uniform sampler2D color;

out vec4 outputColor;

void main(void) {
  float fakeShadow = mix(0.4, 1.0, fragCurrentLayer);
  
  vec4 furData = texture(fur, fragTexCoord);
  vec4 furColor = texture(color, fragTexCoord) * fakeShadow;
  
  furColor.a = (fragCurrentLayer == 0.0) ? 1.0 : furData.a;
  
  outputColor = furColor;
}