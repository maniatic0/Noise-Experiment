
#ifndef VALUE_NOISE_IMPL_H
#define VALUE_NOISE_IMPL_H

#include "noise/value_noise.hpp"

#include <cassert>
#include <functional>

#include "utils/fast_convertion.hpp"
#include "utils/int_fit.hpp"
#include "utils/lerp.hpp"
#include "vec/vec2.hpp"

namespace noise
{

template <uint_least16_t Period, typename Engine, typename Result_Type,
          RemapFunction<Result_Type> Remap_Func>
ValueNoise1D<Period, Engine, Result_Type, Remap_Func>::ValueNoise1D(
    Seed_Type seed)
{
  Dist distribution{ValueNoise1D::low, ValueNoise1D::high};
  Engine generator;

  generator.seed(seed);
  for (auto i = 0; i < kMaxVertices; ++i)
  {
    r[i] = distribution(generator);
  }
}

// Auto Generated destructor
template <uint_least16_t Period, typename Engine, typename Result_Type,
          RemapFunction<Result_Type> Remap_Func>
ValueNoise1D<Period, Engine, Result_Type, Remap_Func>::~ValueNoise1D() =
    default;

template <uint_least16_t Period, typename Engine, typename Result_Type,
          RemapFunction<Result_Type> Remap_Func>
Result_Type ValueNoise1D<Period, Engine, Result_Type, Remap_Func>::eval(
    const Result_Type x) const
{
  // Floor using Integer trunc function
  const Conv_Type xi = utils::fast_int_trunc<Result_Type, Conv_Type>(x);

  const Result_Type t = x - static_cast<Result_Type>(xi);

  // Modulo using the fact that kMaxVerticesMask is a power of 2
  const Conv_Type xMin = xi & static_cast<Conv_Type>(kMaxVerticesMask);
  const Conv_Type xMax = (xMin + 1) & static_cast<Conv_Type>(kMaxVerticesMask);

  assert(xMin <= kMaxVertices - 1);
  assert(xMax <= kMaxVertices - 1);

  const Result_Type tx = (*Remap_Func)(t);

  return utils::lerp<Result_Type>(r[xMin], r[xMax], tx);
}

// Copy and Move auto generated members

template <uint_least16_t Period, typename Engine, typename Result_Type,
          RemapFunction<Result_Type> Remap_Func>
ValueNoise1D<Period, Engine, Result_Type, Remap_Func>::ValueNoise1D(
    const ValueNoise1D &other) = default;

template <uint_least16_t Period, typename Engine, typename Result_Type,
          RemapFunction<Result_Type> Remap_Func>
ValueNoise1D<Period, Engine, Result_Type, Remap_Func> &
ValueNoise1D<Period, Engine, Result_Type, Remap_Func>::
operator=(const ValueNoise1D &other) = default;

template <uint_least16_t Period, typename Engine, typename Result_Type,
          RemapFunction<Result_Type> Remap_Func>
ValueNoise1D<Period, Engine, Result_Type, Remap_Func>::ValueNoise1D(
    ValueNoise1D &&other) noexcept = default;

template <uint_least16_t Period, typename Engine, typename Result_Type,
          RemapFunction<Result_Type> Remap_Func>
ValueNoise1D<Period, Engine, Result_Type, Remap_Func> &
ValueNoise1D<Period, Engine, Result_Type, Remap_Func>::
operator=(ValueNoise1D &&other) noexcept = default;

// ValueNoiseND

template <uint_least8_t Dimension, uint_least16_t Period, typename Engine,
          typename Result_Type, RemapFunction<Result_Type> Remap_Func>
ValueNoiseND<Dimension, Period, Engine, Result_Type, Remap_Func>::ValueNoiseND(
    Seed_Type seed)
{
  Dist distribution{ValueNoise1D::low, ValueNoise1D::high};
  Engine generator;

  generator.seed(seed);
  for (auto i = 0; i < kMaxVertices; ++i)
  {
    r[i] = distribution(generator);
    permutationTable[i] = i;
  }

  // shuffle values of the permutation table
  std::uniform_int_distribution distrUInt{0, kMaxVerticesMask};
  auto randUInt = std::bind(distrUInt, generator);
  for (auto k = 0; k < kMaxVertices; ++k)
  {
    auto i = randUInt();
    std::swap(permutationTable[k], permutationTable[i]);
    for (auto i = 1; i < Dimension; ++i)
    {
      permutationTable[k + (kMaxVertices * i)] = permutationTable[k];
    }
  }
}

// Auto Generated destructor
template <uint_least8_t Dimension, uint_least16_t Period, typename Engine,
          typename Result_Type, RemapFunction<Result_Type> Remap_Func>
ValueNoiseND<Dimension, Period, Engine, Result_Type,
             Remap_Func>::~ValueNoiseND() = default;

template <uint_least8_t Dimension, uint_least16_t Period, typename Engine,
          typename Result_Type, RemapFunction<Result_Type> Remap_Func>
Result_Type
ValueNoiseND<Dimension, Period, Engine, Result_Type, Remap_Func>::eval(
    const Vec2_Type &p) const
{
  constexpr auto fast_int_trunc = utils::fast_int_trunc<Result_Type, Conv_Type>;
  const Conv_Type xi = fast_int_trunc(p.x);
  const Conv_Type yi = fast_int_trunc(p.y);

  const Result_Type tx = p.x - static_cast<Result_Type>(xi);
  const Result_Type ty = p.y - static_cast<Result_Type>(yi);

  const Conv_Type rx0 = xi & kMaxVerticesMask;
  const Conv_Type rx1 = (rx0 + 1) & kMaxVerticesMask;
  const Conv_Type ry0 = yi & kMaxVerticesMask;
  const Conv_Type ry1 = (ry0 + 1) & kMaxVerticesMask;

  // random values at the corners of the cell using permutation table
  const auto &c00 = r[permutationTable[permutationTable[rx0] + ry0]];
  const auto &c10 = r[permutationTable[permutationTable[rx1] + ry0]];
  const auto &c01 = r[permutationTable[permutationTable[rx0] + ry1]];
  const auto &c11 = r[permutationTable[permutationTable[rx1] + ry1]];

  // remapping of tx and ty using the Smoothstep function
  const Result_Type sx = (*Remap_Func)(tx);
  const Result_Type sy = (*Remap_Func)(ty);

  // linearly interpolate values along the x axis
  constexpr auto lerp = utils::lerp<Result_Type>;
  const Result_Type nx0 = lerp(c00, c10, sx);
  const Result_Type nx1 = lerp(c01, c11, sx);

  // linearly interpolate the nx0/nx1 along they y axis
  return lerp(nx0, nx1, sy);
}

template <uint_least8_t Dimension, uint_least16_t Period, typename Engine,
          typename Result_Type, RemapFunction<Result_Type> Remap_Func>
template <uint_least8_t T>
std::enable_if_t<3 <= T, Result_Type>
ValueNoiseND<Dimension, Period, Engine, Result_Type, Remap_Func>::eval(
    const Vec3_Type &p) const
{
  static_assert(Dimension >= 3, "Eval function for Vector3 requires a "
                                "ValueNoiseND with 3 or more dimensions");
  constexpr auto fast_int_trunc = utils::fast_int_trunc<Result_Type, Conv_Type>;
  const Conv_Type xi = fast_int_trunc(p.x);
  const Conv_Type yi = fast_int_trunc(p.y);
  const Conv_Type zi = fast_int_trunc(p.z);

  const Result_Type tx = p.x - static_cast<Result_Type>(xi);
  const Result_Type ty = p.y - static_cast<Result_Type>(yi);
  const Result_Type tz = p.z - static_cast<Result_Type>(zi);

  const Conv_Type rx0 = xi & kMaxVerticesMask;
  const Conv_Type rx1 = (rx0 + 1) & kMaxVerticesMask;
  const Conv_Type ry0 = yi & kMaxVerticesMask;
  const Conv_Type ry1 = (ry0 + 1) & kMaxVerticesMask;
  const Conv_Type rz0 = zi & kMaxVerticesMask;
  const Conv_Type rz1 = (rz0 + 1) & kMaxVerticesMask;

  // random values at the corners of the cell using permutation table
  const auto &c000 = r[permutationTable[permutationTable[permutationTable[rx0] + ry0] + rz0]];
  const auto &c100 = r[permutationTable[permutationTable[permutationTable[rx1] + ry0] + rz0]];
  const auto &c010 = r[permutationTable[permutationTable[permutationTable[rx0] + ry1] + rz0]];
  const auto &c110 = r[permutationTable[permutationTable[permutationTable[rx1] + ry1] + rz0]];
  const auto &c001 = r[permutationTable[permutationTable[permutationTable[rx0] + ry0] + rz1]];
  const auto &c101 = r[permutationTable[permutationTable[permutationTable[rx1] + ry0] + rz1]];
  const auto &c011 = r[permutationTable[permutationTable[permutationTable[rx0] + ry1] + rz1]];
  const auto &c111 = r[permutationTable[permutationTable[permutationTable[rx1] + ry1] + rz1]];

  // remapping of tx, ty and tz using the Smoothstep function
  const Result_Type sx = (*Remap_Func)(tx);
  const Result_Type sy = (*Remap_Func)(ty);
  const Result_Type sz = (*Remap_Func)(tz);

  // linearly interpolate values along the x axis
  constexpr auto lerp = utils::lerp<Result_Type>;
  const Result_Type nx00 = lerp(c000, c100, sx);
  const Result_Type nx10 = lerp(c010, c110, sx);
  const Result_Type nx01 = lerp(c001, c101, sx);
  const Result_Type nx11 = lerp(c011, c111, sx);

  // linearly interpolate values along the y axis
  const Result_Type ny10 = lerp(nx00, nx10, sy);
  const Result_Type ny11 = lerp(nx01, nx11, sy);

  // linearly interpolate the ny10/ny11 along they z axis
  return lerp(ny10, ny11, sz);
}

// Copy and Move auto generated members

template <uint_least8_t Dimension, uint_least16_t Period, typename Engine,
          typename Result_Type, RemapFunction<Result_Type> Remap_Func>
ValueNoiseND<Dimension, Period, Engine, Result_Type, Remap_Func>::ValueNoiseND(
    const ValueNoiseND &other) = default;

template <uint_least8_t Dimension, uint_least16_t Period, typename Engine,
          typename Result_Type, RemapFunction<Result_Type> Remap_Func>
ValueNoiseND<Dimension, Period, Engine, Result_Type, Remap_Func> &
ValueNoiseND<Dimension, Period, Engine, Result_Type, Remap_Func>::
operator=(const ValueNoiseND &other) = default;

template <uint_least8_t Dimension, uint_least16_t Period, typename Engine,
          typename Result_Type, RemapFunction<Result_Type> Remap_Func>
ValueNoiseND<Dimension, Period, Engine, Result_Type, Remap_Func>::ValueNoiseND(
    ValueNoiseND &&other) noexcept = default;

template <uint_least8_t Dimension, uint_least16_t Period, typename Engine,
          typename Result_Type, RemapFunction<Result_Type> Remap_Func>
ValueNoiseND<Dimension, Period, Engine, Result_Type, Remap_Func> &
ValueNoiseND<Dimension, Period, Engine, Result_Type, Remap_Func>::
operator=(ValueNoiseND &&other) noexcept = default;

} // namespace noise

#endif // !VALUE_NOISE_IMPL_H