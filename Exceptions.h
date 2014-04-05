#ifndef _EXCEPTIONS_H_
#define _EXCEPTIONS_H_

#include <string>
#include <stdexcept>

using namespace std;

class GLSLError : public runtime_error {
public:
  GLSLError(const string& error) : runtime_error(error) {}
};

class ShaderError : public runtime_error {
public:
  ShaderError(const string& error) : runtime_error(error) {}
};

class PNGError : public runtime_error {
public:
  PNGError(const string& error) : runtime_error(error) {}
};

#endif
