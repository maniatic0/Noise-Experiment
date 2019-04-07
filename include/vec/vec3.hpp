

#ifndef VEC_3_H
#define VEC_3_H

namespace vector {
template <typename T = float> class Vec3 {
public:
  constexpr Vec3() : x(T(0)), y(T(0)), z(T(0)) {}
  constexpr Vec3(T xx, T yy, T zz) : x(xx), y(yy), z(zz) {}
  constexpr Vec3 operator*(const T &r) const;
  constexpr Vec3 &operator*=(const T &r);
  T x, y, z;
};

template <typename T = float>
constexpr T dot(const Vec3<T> &a, const Vec3<T> &b);

using Vec3f = Vec3<float>;

} // namespace vector

#include "vec/vec3_impl.hpp"

#endif // !VEC_3_H
