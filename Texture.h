#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <png.h>
#include <memory>
#include <vector>

/**
 * A texture loaded from a PNG.
 * Upon construction, an OpenGL texture will automatically be created.
 */
class Texture {
  int _width;
  int _height;
  GLuint _texture;
  std::shared_ptr<std::vector<png_bytep>> _rows;
  std::shared_ptr<std::vector<png_byte>> _img;
  png_uint_32 _bitsPerChannel;
  png_uint_32 _channels;

 public:
  /**
   * Constructs a Texture from the specified PNG file path.
   
   * The constructor automatically attempts to read the PNG file and construct
   * an appropriate OpenGL texture.
   * In addition, the texture will also be bound during its construction, if possible.
   * To save the texture into a texture unit, call glActiveTexture(GL_TEXTUREn), where n
   * is an integer, before calling this constructor.
   * 
   * @param fileName the path to the PNG file
   * @throws ifstream::failure if the file could not be read
   * @throws PNGError if the PNG data was invalid or corrupt
   */
  Texture(const char* fileName);
  
  /**
   * Returns the width of the texture, in pixels, or 0 if the texture could not be loaded.
   * @return the width of the texture
   */
  int width() const;
  
  /**
   * Returns the height of the texture, in pixels, or 0 if the texture could not be
   * loaded.
   * @return the height of the texture
   */
  int height() const;
  
  /**
   * Indicates whether the texture is available for use.
   * If this is true, then the texture was loaded and is ready to be used.
   * If this is false, then the texture has been destroyed, and it cannot be used from
   * within OpenGL.
   * @return whether the texture is available for use.
   */
  bool valid() const;
  
  /**
   * Tells OpenGL to mark the texture for deletion.
   */
  void destroy();
  
  /**
   * Binds the texture to the current OpenGL context.
   */
  void bind() const;
  
  /**
   * Gets the color of the texture at the given coordinates. Each component is given
   * as a char, where 0=minimum and 255=maximum, e.g. alpha=0 is transparent and
   * alpha=255 is opaque.
   * @param col the x-coordinates
   * @param row the y-coordinates
   * @param outR where the output red value will be placed
   * @param outG where the output green value will be placed
   * @param outB where the output blue value will be placed
   * @param outA where the output alpha value will be placed (may just be 255 if the
   *             image has no alpha channel)
   */
  void get(int col, int row, unsigned char& outR, unsigned char& outG,
    unsigned char& outB, unsigned char& outA) const;
 };

 #endif

