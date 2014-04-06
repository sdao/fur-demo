#ifndef _SHADERPROGRAM_H_
#define _SHADERPROGRAM_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <map>
#include <boost/optional.hpp>
#include "Shader.h"

/**
 * A shader program consisting of a vertex shader, a geometry shader, and a fragment shader.
 * The geometry shader is optional.
 */
class ShaderProgram {
  std::map<string, GLint> _attributeNames;
  std::map<string, GLint> _uniformNames;
  GLuint _program;
  void init(VertexShader vs, FragmentShader fs, boost::optional<GeometryShader> gs);
  
public:
  /**
   * Constructs a shader program from the given vertex and fragment shaders.
   * @param vs a vertex shader
   * @param fs a fragment shader
   * @param gs (optional) a geometry shader
   * @throws ShaderError if the given shaders are invalid
   * @throws GLSLError if there was a GLSL linking error
   */
  ShaderProgram(VertexShader vs, FragmentShader fs, boost::optional<GeometryShader> gs = boost::optional<GeometryShader>());
  
  /**
   * Constructs a shader program by loading a vertex and fragment shader from disk.
   * Note: the vertex and fragment shaders will be deleted after the program is compiled.
   * @param vs the path to a vertex shader
   * @param fs the path to a fragment shader
   * @param gs (optional) the path to a geometry shader; pass an empty string if none
   * @throws ifstream::failure if one of the shader text files could not be read
   * @throws GLSLError if there was a GLSL linking error
   */
  ShaderProgram(const char* vsFileName, const char* fsFileName, const char* gsFileName);
  
  /**
   * Indicates whether the program is available for use.
   * If this is true, then the program was linked and is ready to be used.
   * If this is false, then the program has been destroyed, and it cannot be used from
   * within OpenGL.
   * @return whether the program is available for use.
   */
  bool valid() const;
  
  /**
   * Sets the program for use in the current OpenGL context.
   * @return whether the program was set for use successfully
   */
  bool use() const;
  
  
  /**
   * Tells OpenGL to mark the program for deletion.
   */
  void destroy();
  
  /**
   * Gets the location of a given shader attribute.
   * @param the name of the attribute
   * @return the attribute's integer location
   */
  GLint getAttribute(string attributeName);
  
  /**
   * Gets the location of a given shader uniform.
   * @param the name of the uniform
   * @return the uniform's integer location
   */
  GLint getUniform(string uniformName);
  
  /**
   * Determines whether the given attribute can be found in the current program.
   * @param the name of the attribute
   * @return whether the attribute is in the program
   */
  bool hasAttribute(string attributeName);
  
  /**
   * Determines whether the given uniform can be found in the current program.
   * @param the name of the uniform
   * @return whether the uniform is in the program
   */
  bool hasUniform(string uniformName);
};

#endif
