#include <cstdlib>
#include <iostream>
#include <cassert>
#include <GL/glew.h>
#include <GLFW/glfw3.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <png.h>
#include "Texture.h"
#include "ShaderProgram.h"

using namespace std;

int CANVAS_WIDTH = 500;
int CANVAS_HEIGHT = 500;

struct attributes {
  GLfloat xyz[3];
  GLfloat rgb[3];
  GLfloat uv[2];
};

struct attributes triangleAttributes[] = {
  {{-0.6, -0.4, 0.0}, {1.0, 0.0, 0.0}, {0.0, 0.0}},
  {{ 0.6, -0.4, 0.0}, {0.0, 1.0, 0.0}, {1.0, 0.0}},
  {{ 0.0,  0.6, 0.0}, {0.0, 0.0, 1.0}, {0.0, 1.0}}
};

GLuint initTriangleVbo() {
  GLuint triangleBuffer;
  glGenBuffers(1, &triangleBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, triangleBuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(triangleAttributes), triangleAttributes,
    GL_STATIC_DRAW);
  
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  
  return triangleBuffer;
}

GLuint initTriangleVao(GLuint triangleBuffer, ShaderProgram prog) {
  GLint posAttribute = prog.getAttribute("pos");
  GLint colorAttribute = prog.getAttribute("color");
  GLint textureAttribute = prog.getAttribute("texCoord");
    
  // Initialize vertex array.
  GLuint triangleArray;
  glGenVertexArrays(1, &triangleArray);
  glBindVertexArray(triangleArray);
  
  // Configure attributes.
  glBindBuffer(GL_ARRAY_BUFFER, triangleBuffer);
  
  glVertexAttribPointer(
    posAttribute,                            // attribute
    3,                                       // number of elements per vertex, here (x,y,z)
    GL_FLOAT,                                // the type of each element
    GL_FALSE,                                // take our values as-is
    sizeof(struct attributes),               // no extra data between each position
    (void*)offsetof(struct attributes, xyz)  // offset in the buffer
  );
  glEnableVertexAttribArray(posAttribute);

  glVertexAttribPointer(
    colorAttribute,                          // attribute
    3,                                       // number of elements per vertex, here (r,g,b)
    GL_FLOAT,                                // the type of each element
    GL_FALSE,                                // take our values as-is
    sizeof(struct attributes),               // no extra data between each position
    (void*)offsetof(struct attributes, rgb)  // offset in the buffer
  );
  glEnableVertexAttribArray(colorAttribute);
  
  glVertexAttribPointer(
    textureAttribute,                        // attribute
    2,                                       // number of elements per vertex, here (u,v)
    GL_FLOAT,                                // the type of each element
    GL_FALSE,                                // take our values as-is
    sizeof(struct attributes),               // no extra data between each position
    (void*)offsetof(struct attributes, uv)   // offset in the buffer
  );
  glEnableVertexAttribArray(textureAttribute);
  
  // Rebind the default state.
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  
  return triangleArray;
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
  assert(prog.hasAttribute("color"));
  assert(prog.hasAttribute("texCoord"));
  assert(prog.hasUniform("modelViewMatrix"));
  assert(prog.hasUniform("projectionMatrix"));
  assert(prog.hasUniform("tex"));

  prog.use();
    
  // Load textures.
  glActiveTexture(GL_TEXTURE0);
  Texture t("sample.png");
  glUniform1i(prog.getUniform("tex"), 0);
  
  // Initialize vertex buffer object.
  GLuint triangleBuffer = initTriangleVbo();
  
  // Initialize vertex array object.
  GLuint triangleArray = initTriangleVao(triangleBuffer, prog);

  while (!glfwWindowShouldClose(window)) {
    float ratio;
    int width, height;
    
    glfwGetFramebufferSize(window, &width, &height);
    ratio = width / (float) height;
    
    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT);
    
    // Projection and model-view matrices.
    glm::vec3 zAxis(0, 0, 1);
    glm::mat4 transform = glm::rotate(glm::mat4(1.0f), (float) glfwGetTime() * 50.f, zAxis);
    glm::mat4 projection = glm::ortho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
    glUniformMatrix4fv(prog.getUniform("modelViewMatrix"), 1, GL_FALSE,
      glm::value_ptr(transform));
    glUniformMatrix4fv(prog.getUniform("projectionMatrix"), 1, GL_FALSE,
      glm::value_ptr(projection));
  
    // Position and color attributes.
    glBindVertexArray(triangleArray);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // Display and continue.
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return EXIT_SUCCESS;
}
