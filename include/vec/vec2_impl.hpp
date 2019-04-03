
#ifndef VEC_2_IMPL_H
#define VEC_2_IMPL_H

#include "vec/vec2.hpp"

namespace vector {

template <typename T> Vec2<T> Vec2<T>::operator*(const T &r) const {
  return Vec2<T>(x * r, y * r);
}

} // namespace vector

#endif // !VEC_2_IMPL_H