#ifndef _FURTEXTURE_H_
#define _FURTEXTURE_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <memory>
#include <vector>

struct RGBColor {
  unsigned char r;
  unsigned char g;
  unsigned char b;
  unsigned char a;
  
  RGBColor() : r(0), g(0), b(0), a(0) {}
  RGBColor(unsigned char rr, unsigned char gg, unsigned char bb, unsigned char aa) :
    r(rr), g(gg), b(bb), a(aa) {}
};

class FurTexture {
  const std::shared_ptr<std::vector<RGBColor>> _tex;
  
public:
  FurTexture(int width, int height, int layers, float density);
};

#endif