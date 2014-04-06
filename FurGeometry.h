#ifndef _FURGEOMETRY_H_
#define _FURGEOMETRY_H_

#include <vector>
#include <glm/glm.hpp>
#include "ShaderProgram.h"

struct FurAttributes {
  glm::vec3 xyzPosition;
  glm::vec3 xyzNormal;
  glm::vec2 uvTexCoord;
  GLfloat layer;
};

class FurGeometry {
  GLuint _vao;
  int _indices;
  GLuint initVao(GLuint buffer, ShaderProgram prog);
  
public:
  FurGeometry(std::vector<FurAttributes>& geom, ShaderProgram& prog,
    int layers, int maxHairLength);
  void draw() const;
};

#endif
