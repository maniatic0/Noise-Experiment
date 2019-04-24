
#ifndef NOISE_REMAP_H
#define NOISE_REMAP_H

#include <cmath>
#include <cassert>

#include "utils/constants.hpp"

namespace noise {
template <typename T = float> using RemapFunction = T (*)(const T);

template <typename T> constexpr T cosineRemap(const T t) {
  assert(t >= 0 && t <= 1);
  return (1 - std::cos(t * utils::pi<T>)) * 0.5;
}

template <typename T> constexpr T smoothstepRemap(const T t) {
  return t * t * (3 - 2 * t);
}

template <typename T> constexpr T perlinRemap(const T t) {
  return t * t * t * (10 - 15 * t + 6 * t * t);
}

template <typename T> constexpr T perlinRemapDeriv(const T t) {
  const T a = t - 1;
  return 30 * t * t * a * a;
}


} // namespace noise

#endif // !NOISE_REMAP_H