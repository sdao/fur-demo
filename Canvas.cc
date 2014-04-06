#include <cstdlib>
#include <iostream>
#include <cassert>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <png.h>
#include "Texture.h"
#include "FurTexture.h"
#include "ShaderProgram.h"
#include "Vector3.h"

using namespace std;

const int CANVAS_WIDTH = 500;
const int CANVAS_HEIGHT = 500;
const int FUR_DIM = 256;
const float FUR_DENSITY = 0.3f;
const int FUR_HEIGHT = 40;

struct Attributes {
  GLfloat xyzPosition[3];
  GLfloat xyzNormal[3];
  GLfloat uvTexCoord[2];
};

// A---B
// |   |
// C---D
struct Attributes geom[] = {
  {{ 10.0, -10.0, 0.0}, {0.0, 0.0, 1.0}, {1.0, 0.0}}, // D
  {{ 10.0,  10.0, 0.0}, {0.0, 0.0, 1.0}, {1.0, 1.0}}, // B
  {{-10.0, -10.0, 0.0}, {0.0, 0.0, 1.0}, {0.0, 0.0}}, // C
  
  {{ 10.0,  10.0, 0.0}, {0.0, 0.0, 1.0}, {1.0, 1.0}}, // B
  {{-10.0,  10.0, 0.0}, {0.0, 0.0, 1.0}, {0.0, 1.0}}, // A
  {{-10.0, -10.0, 0.0}, {0.0, 0.0, 1.0}, {0.0, 0.0}}, // C
};

GLuint initVbo() {
  GLuint buffer;
  glGenBuffers(1, &buffer);
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(geom), geom,
    GL_STATIC_DRAW);
  
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  return buffer;
}

GLuint initVao(GLuint buffer, ShaderProgram prog) {
  GLint posAttribute = prog.getAttribute("pos");
  GLint normAttribute = prog.getAttribute("norm");
  GLint textureAttribute = prog.getAttribute("texCoord");
  
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
    sizeof(struct Attributes),
    (void*)offsetof(struct Attributes, xyzPosition)
  );
  glEnableVertexAttribArray(posAttribute);
  
  glVertexAttribPointer(
    normAttribute,
    3,
    GL_FLOAT,
    GL_FALSE,
    sizeof(struct Attributes),
    (void*)offsetof(struct Attributes, xyzNormal)
  );
  glEnableVertexAttribArray(normAttribute);
  
  glVertexAttribPointer(
    textureAttribute,
    2,
    GL_FLOAT,
    GL_FALSE,
    sizeof(struct Attributes),
    (void*)offsetof(struct Attributes, uvTexCoord)
  );
  glEnableVertexAttribArray(textureAttribute);
  
  // Rebind the default state.
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  
  return array;
}

int main(int argc, char** argv) {
  GLFWwindow* window;

  if (!glfwInit()) {
    return EXIT_FAILURE;
  }
  
  // Ask for the OpenGL 3.3 Core Profile.    
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  
  // Initialize GLFW window.
  window = glfwCreateWindow(CANVAS_WIDTH, CANVAS_HEIGHT, "gldemo", NULL, NULL);
  if (!window) {
    glfwTerminate();
    return EXIT_FAILURE;
  }
  glfwMakeContextCurrent(window);	
  cout << "OpenGL version: " << glGetString(GL_VERSION) << "\n";
  
  // Initialize GLEW.
  glewExperimental = true; /* glGenVertexArrays() fails without this. */
  GLenum err = glewInit();
  if (err != GLEW_OK)
  {
    glfwTerminate();
    return EXIT_FAILURE;
  }
  cout << "GLEW version: " << glewGetString(GLEW_VERSION) << "\n";
 
  // Initialize shaders.
  ShaderProgram prog("default.vert", "default.frag");
  assert(prog.hasAttribute("pos"));
  assert(prog.hasAttribute("norm"));
  assert(prog.hasAttribute("texCoord"));
  assert(prog.hasUniform("modelView"));
  assert(prog.hasUniform("projection"));
  assert(prog.hasUniform("fur"));
  assert(prog.hasUniform("color"));
  assert(prog.hasUniform("currentLayer"));
  assert(prog.hasUniform("maxHairLength"));
  assert(prog.hasUniform("displacement"));

  prog.use();
    
  // Load textures.
  glActiveTexture(GL_TEXTURE0);
  FurTexture fur(FUR_DIM, FUR_DIM, FUR_HEIGHT, FUR_DENSITY);
  glUniform1i(prog.getUniform("fur"), 0);
  
  glActiveTexture(GL_TEXTURE1);
  Texture furColor("bigtiger.png");
  glUniform1i(prog.getUniform("color"), 1);
  
  // Initialize vertex buffer object.
  GLuint vertexBuffer = initVbo();
  
  // Initialize vertex array object.
  GLuint vertexArray = initVao(vertexBuffer, prog);

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

  // Simple physics.
  Vector3 gravity(0.0f, -0.8f, 0.0f);

  while (!glfwWindowShouldClose(window)) {
    float ratio;
    int width, height;
    
    glfwGetFramebufferSize(window, &width, &height);
    ratio = width / (float) height;
    
    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Projection and model-view matrices.
    glm::vec3 xAxis(1.0f, 0.0f, 0.0f);
    glm::vec3 yAxis(0.0f, 1.0f, 0.0f);
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -30.0f)) *
      glm::rotate(glm::mat4(1.0f), -10.0f, xAxis) *
      glm::rotate(glm::mat4(1.0f), -10.0f, yAxis);
    glm::mat4 projection = glm::perspective(60.0f, ratio, 0.1f, 100.0f);
    glUniformMatrix4fv(prog.getUniform("modelView"), 1, GL_FALSE,
      glm::value_ptr(view));
    glUniformMatrix4fv(prog.getUniform("projection"), 1, GL_FALSE,
      glm::value_ptr(projection));
    
    // Other uniform attributes.
    Vector3 force(sin(glfwGetTime()) * 0.5f, 0.0f, 0.0f);
    Vector3 disp = gravity + force;
    glUniform3f(prog.getUniform("displacement"), disp.x, disp.y, disp.z);
    glUniform1f(prog.getUniform("maxHairLength"), 2.0f);
  
    // Loop over layers of fur.
    for (int i = 0; i < FUR_HEIGHT; i++) {
      // Position and color attributes.
      glUniform1f(prog.getUniform("currentLayer"), (float)i/FUR_HEIGHT);
      glUniform3f(prog.getUniform("color"), 1.0, 1.0, (float)i/FUR_HEIGHT);
      glBindVertexArray(vertexArray);
      glDrawArrays(GL_TRIANGLES, 0, sizeof(geom)/sizeof(struct Attributes));
    }

    // Display and continue.
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return EXIT_SUCCESS;
}
