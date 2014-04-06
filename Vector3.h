#ifndef _VECTOR3_H_
#define _VECTOR3_H_

#include <cstdlib>
#include <iostream>
#include <OpenGL/gl.h>
#include <GLUT/glut.h> 

struct Vector3 {
  static const Vector3 ZERO;
  const float x;
  const float y;
  const float z;
  
  Vector3(float xx, float yy, float zz);
  float dot(const Vector3& rhs) const;
  Vector3 cross(const Vector3& rhs) const;
  Vector3 operator*(float k) const;
  Vector3 operator/(float k) const;
  Vector3 operator+(const Vector3& rhs) const;
  Vector3 operator-(const Vector3& rhs) const;
  Vector3 operator-() const;
  float length() const;
  Vector3 normalize() const;
};

#endif
