
#ifndef VEC_3_IMPL_H
#define VEC_3_IMPL_H

#include "vec/vec3.hpp"

namespace vector
{

template <typename T>
Vec3<T> Vec3<T>::operator*(const T &r) const
{
  return Vec3<T>(x * r, y * r, z * r);
}

} // namespace vector

#endif // !VEC_3_IMPL_H