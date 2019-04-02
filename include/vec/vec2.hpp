

#ifndef VEC_2_H
#define VEC_2_H

namespace vector
{
template <typename T = float>
class Vec2
{
  public:
    Vec2() : x(T(0)), y(T(0)) {}
    Vec2(T xx, T yy) : x(xx), y(yy) {}
    Vec2 operator*(const T &r) const;
    T x, y;
};

using Vec2f = Vec2<float>;

} // namespace vector

#endif // !VEC_2_H
