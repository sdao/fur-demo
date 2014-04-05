#ifndef _SHADER_H_
#define _SHADER_H_

#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include "Exceptions.h"

using namespace std;

/**
 * A GLSL shader loaded from disk.
 */
template <GLenum shaderType>
class Shader {
  GLuint _shader;
  
public:
  /**
   * Constructs a new shader by loading it from disk.
   * The constructor will automatically compile the GLSL shader.
   * @param fileName the path to the GLSL shader text file
   * @throws ifstream::failure if the shader text file could not be read
   * @throws GLSLError if there was a GLSL compilation error
   */
  Shader(const char* fileName) {
    GLuint shaderId = 0;
    
    ifstream shaderFile;
    shaderFile.exceptions(ifstream::failbit | ifstream::badbit);
    
    try {
      shaderFile.open(fileName, ios::binary);
      string shaderText((istreambuf_iterator<char>(shaderFile)),
        istreambuf_iterator<char>());
      const char* shaderCString = shaderText.c_str();
    
      shaderId = glCreateShader(shaderType);
      glShaderSource(shaderId, 1, &shaderCString, NULL);
      glCompileShader(shaderId);
      
      GLint compiledOK = false;
      glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compiledOK);
      if (!compiledOK) {
        GLint logSize;
        glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &logSize);
        char* logMessage = new char[logSize];
        glGetShaderInfoLog(shaderId, logSize, NULL, logMessage);
        string logString(logMessage);
        delete[] logMessage;
        throw GLSLError(logString);
      }
      
      _shader = shaderId;
    }
    catch (...) {
      if (shaderId != 0) glDeleteShader(shaderId);
      throw;
    }
  }
  
  /**
   * Indicates whether the shader is available for use.
   * If this is true, then the shader was compiled and is ready to be used.
   * If this is false, then the shader has been destroyed, and it cannot be used from
   * within OpenGL.
   * @return whether the shader is available for use.
   */
  bool valid() const {
    return glIsShader(_shader);
  }
  
  /**
   * Tells OpenGL to mark the shader for deletion.
   */
  void destroy() {
    if (valid()) {
      glDeleteShader(_shader);
    }
  }
  
  /**
   * Returns the internal shader identifier used by OpenGL.
   * @return the OpenGL shader identifier
   */
  GLuint shaderId() const {
    return _shader;
  }
};

/**
 * A GLSL vertex shader.
 */
typedef Shader<GL_VERTEX_SHADER> VertexShader;

/**
 * A GLSL fragment shader.
 */
typedef Shader<GL_FRAGMENT_SHADER> FragmentShader;

#endif
