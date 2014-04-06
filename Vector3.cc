#include "Vector3.h"
#include <cmath>

#define self (*this)

const Vector3 Vector3::ZERO = Vector3(0.0f, 0.0f, 0.0f);

Vector3::Vector3(float xx, float yy, float zz): x(xx), y(yy), z(zz) {}

float Vector3::dot(const Vector3& rhs) const {
  return x * rhs.x + y * rhs.y + z * rhs.z;
}
  
Vector3 Vector3::cross(const Vector3& rhs) const {
  return Vector3(y * rhs.z - z * rhs.y, z * rhs.x - x * rhs.z, x * rhs.y - y * rhs.x);
}

Vector3 Vector3::operator*(float k) const {
  return Vector3(x * k, y * k, z * k);
}

Vector3 Vector3::operator/(float k) const {
  return Vector3(x / k, y / k, z / k);
}

Vector3 Vector3::operator+(const Vector3& rhs) const {
  return Vector3(x + rhs.x, y + rhs.y, z + rhs.z);
}

Vector3 Vector3::operator-(const Vector3& rhs) const {
  return Vector3(x - rhs.x, y - rhs.y, z - rhs.z);
}

Vector3 Vector3::operator-() const {
  return Vector3(-x, -y, -z);
}

float Vector3::length() const {
  return sqrt(x * x + y * y + z * z);
}

Vector3 Vector3::normalize() const {
  return self / length();
}
