
#ifndef VEC_3_IMPL_H
#define VEC_3_IMPL_H

#include "vec/vec3.hpp"

namespace vector {

template <typename T> constexpr Vec3<T> Vec3<T>::operator*(const T &r) const {
  return Vec3<T>(x * r, y * r, z * r);
}

template <typename T> constexpr Vec3<T> &Vec3<T>::operator*=(const T &r) {
  x *= r;
  y *= r;
  z *= r;
  return *this;
}

template <typename T = float>
constexpr T dot(const Vec3<T> &a, const Vec3<T> &b) {
  return a.x * b.x + a.y * b.y + a.z * b.z;
}

} // namespace vector

#endif // !VEC_3_IMPL_H