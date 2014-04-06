#include "Texture.h"
#include <iostream>
#include <fstream>
#include <stdexcept>
#include "Exceptions.h"

using namespace std;

void userReadData(png_structp pngRead, png_bytep data, png_size_t length) {
  png_voidp f = png_get_io_ptr(pngRead);
  ((ifstream*)f)->read((char*)data, length);
}

Texture::Texture(const char* fileName) {
  GLuint textureId = 0;
  
  // We need to declare these up here so they can be cleaned up
  // regardless of error condition.
  bool pngReadInited = false;
  bool pngInfoInited = false;
  png_structp pngRead;
  png_infop pngInfo;
  
  ifstream pngFile;
  pngFile.exceptions(ifstream::failbit | ifstream::badbit);
  
  try {
    pngFile.open(fileName, ios::binary);
  
    // Check the PNG header (8 bytes).
    png_byte header[8];
    pngFile.read((char*)header, 8);
    if (png_sig_cmp(header, 0, 8)) {
      throw PNGError("Bad PNG header");
    }

    // Read the PNG data using libpng.
    pngRead = png_create_read_struct(PNG_LIBPNG_VER_STRING,
      NULL, NULL, NULL);
    if (pngRead) {
      pngReadInited = true;
    }
    else {
      throw PNGError("Could not initialize PNG read");
    }

    pngInfo = png_create_info_struct(pngRead);
    if (pngInfo) {
      pngInfoInited = true;
    }
    else {
      throw PNGError("Could not initialize PNG info");
    }

    // This error-handling method is prescribed in the libpng manual.
    if (setjmp(png_jmpbuf(pngRead))) {
      // An error occurred, so clean up now.
      throw PNGError("Error occurred while reading PNG");
    }

    // Have libpng read from our C++ stream.
    png_set_read_fn(pngRead, (png_voidp)&pngFile, userReadData);
    
    // Tell libpng we've already read the 8-byte header.
    png_set_sig_bytes(pngRead, 8);

    // Read the entire PNG header.
    png_read_info(pngRead, pngInfo);

    png_uint_32 pngWidth = png_get_image_width(pngRead, pngInfo);
    png_uint_32 pngHeight = png_get_image_height(pngRead, pngInfo);
    _width = pngWidth;
    _height = pngHeight;

    png_uint_32 bitsPerChannel = png_get_bit_depth(pngRead, pngInfo);
    png_uint_32 channels = png_get_channels(pngRead, pngInfo);
    png_uint_32 colorType = png_get_color_type(pngRead, pngInfo);

    GLint glFormat;
    switch (colorType) {
      case PNG_COLOR_TYPE_PALETTE:
        png_set_palette_to_rgb(pngRead);
        channels = 3;
        glFormat = GL_RGB;
        break;
      case PNG_COLOR_TYPE_RGB:
        glFormat = GL_RGB;
        break;
      case PNG_COLOR_TYPE_RGB_ALPHA:
        glFormat = GL_RGBA;
        break;
      default:
        throw PNGError("Unsupported color type");
    }

    // Convert any transparency to a full alpha channel.
    if (png_get_valid(pngRead, pngInfo, PNG_INFO_tRNS)) {
      png_set_tRNS_to_alpha(pngRead);
      channels += 1;
    }

    // Convert 16-bit precision to 8-bit precision.
    if (bitsPerChannel == 16) png_set_strip_16(pngRead);

    shared_ptr<vector<png_bytep>> rows = make_shared<vector<png_bytep>>(pngHeight);
    // Note: divide by 8 bits/1 byte.
    shared_ptr<vector<png_byte>> pngBytes = make_shared<vector<png_byte>>(pngWidth *
      pngHeight * bitsPerChannel * channels / 8);
    // Length in bytes of one row.
    const unsigned int stride = pngWidth * bitsPerChannel * channels / 8;

    for (size_t row = 0; row < pngHeight; row++) {
      // We're setting the pointers "upside-down".
      png_uint_32 offset = (pngHeight - row - 1) * stride;
      (*rows)[row] = (png_bytep)(pngBytes->data()) + offset;
    }

    // Actually read the image!
    png_read_image(pngRead, rows->data());

    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, glFormat, pngWidth, pngHeight, 0,
      glFormat, GL_UNSIGNED_BYTE, pngBytes->data());
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
    
    if (pngReadInited && pngInfoInited) {
      png_destroy_read_struct(&pngRead, &pngInfo, (png_infopp)NULL);
    }
    
    _texture = textureId;
    _rows = rows;
    _img = pngBytes;
    _bitsPerChannel = bitsPerChannel;
    _channels = channels;
  }
  catch (...) {
    // Clean up data.
    if (textureId != 0) glDeleteTextures(1, &_texture);
    if (pngReadInited && pngInfoInited) {
      png_destroy_read_struct(&pngRead, &pngInfo, (png_infopp)NULL);
    }
    throw;
  }
}

int Texture::width() const {
  return _width;
}

int Texture::height() const {
  return _height;
}

bool Texture::valid() const {
  return glIsTexture(_texture);
}

void Texture::destroy() {
  if (valid()) {
    glDeleteTextures(1, &_texture);
  }
}

void Texture::bind() const {
  glBindTexture(GL_TEXTURE_2D, _texture);
}


void Texture::get(int col, int row, unsigned char& outR, unsigned char& outG,
  unsigned char& outB, unsigned char& outA) const {
  if (row > _height || row < 0)
    throw PNGError("row out of bounds");
  if (col > _width || col < 0)
    throw PNGError("col out of bounds");
  
  const int bytesPerPixel = _bitsPerChannel * _channels / 8;
  if (bytesPerPixel < 3) // PNG must have at least RGB bytes.
    throw PNGError("PNG cannot be sampled");
  
  png_bytep rowPtr = (*_rows)[row];
  png_bytep pxPtr = rowPtr + bytesPerPixel * col;
  outR = *(pxPtr);
  outG = *(pxPtr + 1);
  outB = *(pxPtr + 2);
  if (bytesPerPixel > 3) {
    outA = *(pxPtr + 3); // Use Alpha byte if PNG has it.
  }
  else {
    outA = 255; // If PNG has RGB but no Alpha, set Alpha to 255 (max).
  }
}
