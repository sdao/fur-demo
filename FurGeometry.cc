#include "FurGeometry.h"

using namespace std;

GLuint FurGeometry::initVao(GLuint buffer, ShaderProgram prog) {
  GLint posAttribute = prog.getAttribute("pos");
  GLint textureAttribute = prog.getAttribute("texCoord");
  GLint layerAttribute = prog.getAttribute("layer");
  //GLint normAttribute = prog.getAttribute("norm");
  
  // Initialize vertex array.
  GLuint array;
  glGenVertexArrays(1, &array);
  glBindVertexArray(array);
  
  // Configure attributes.
  glBindBuffer(GL_ARRAY_BUFFER, array);
  
  glVertexAttribPointer(
    posAttribute,
    3,
    GL_FLOAT,
    GL_FALSE,
    sizeof(struct FurAttributes),
    (void*)offsetof(struct FurAttributes, xyzPosition)
  );
  glEnableVertexAttribArray(posAttribute);
  
  /*
  glVertexAttribPointer(
    normAttribute,
    3,
    GL_FLOAT,
    GL_FALSE,
    sizeof(struct FurAttributes),
    (void*)offsetof(struct FurAttributes, xyzNormal)
  );
  glEnableVertexAttribArray(normAttribute);
  */
  
  glVertexAttribPointer(
    textureAttribute,
    2,
    GL_FLOAT,
    GL_FALSE,
    sizeof(struct FurAttributes),
    (void*)offsetof(struct FurAttributes, uvTexCoord)
  );
  glEnableVertexAttribArray(textureAttribute);
  
  glVertexAttribPointer(
    layerAttribute,
    1,
    GL_FLOAT,
    GL_FALSE,
    sizeof(struct FurAttributes),
    (void*)offsetof(struct FurAttributes, layer)
  );
  glEnableVertexAttribArray(layerAttribute);
  
  // Rebind the default state.
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  
  return array;
}
  
FurGeometry::FurGeometry(vector<FurAttributes>& geom, ShaderProgram& prog,
  int layers, int maxHairLength) {
  vector<FurAttributes> newGeom;
  
  for (int i = 0; i < layers; i++) {
    float layer = (float)i/(float)(layers - 1);
    float layerHairLength = maxHairLength * layer;
    for (FurAttributes f : geom) {
      f.xyzPosition = f.xyzPosition + f.xyzNormal * layerHairLength;
      f.layer = layer;
      newGeom.push_back(f);
    }
  }
  
  GLuint buffer;
  glGenBuffers(1, &buffer);
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(struct FurAttributes) * newGeom.size(),
    newGeom.data(), GL_STATIC_DRAW);
  
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  
  _vao = initVao(buffer, prog);
  _indices = newGeom.size();
}

void FurGeometry::draw() const {
  glBindVertexArray(_vao);
  glDrawArrays(GL_TRIANGLES, 0, _indices);
}
