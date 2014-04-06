#include "FurTexture.h"
#include <cstdlib>

using namespace std;

FurTexture::FurTexture(int width, int height, float density) :
  _tex(make_shared<vector<RGBColor>>(width * height)) {
  int totalPixels = width * height;
  vector<RGBColor> texArray = *_tex;
  
  // Initialize colors to transparent black.
  for (int i = 0; i < totalPixels; i++) {
    texArray[i] = RGBColor();
  }
  
  // Compute the number of opaque pixels (hair strands).
  int numStrands = (int)(density * totalPixels);
  
  // Fill texture with opaque pixels.
  for (int i = 0; i < numStrands; i++) {
    // Choose a random position on the texture.
    int x = rand() % height;
    int y = rand() % width;
    texArray[x * width + y] = RGBColor(200, 215, 0, 255);
  }
  
  GLuint textureId;
  glGenTextures(1, &textureId);
  glBindTexture(GL_TEXTURE_2D, textureId);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
    GL_RGBA, GL_UNSIGNED_BYTE, texArray.data());
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glGenerateMipmap(GL_TEXTURE_2D);
}