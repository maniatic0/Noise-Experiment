
#ifndef VALUE_NOISE_H
#define VALUE_NOISE_H

#include <array>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <random>
#include <type_traits>

#include "utils/constants.hpp"
#include "utils/int_fit.hpp"
#include "vec/vec2.hpp"
#include "vec/vec3.hpp"

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

template <uint_least16_t Period = 256,
          typename Engine = std::default_random_engine,
          typename Result_Type = float,
          RemapFunction<Result_Type> Remap_Func = smoothstepRemap<Result_Type>>
class ValueNoise1D {
public:
  static_assert(std::is_floating_point<Result_Type>(),
                "Result_Type must be a floating point type");

  using Dist = typename std::uniform_real_distribution<Result_Type>;
  using Seed_Type = typename Dist::result_type;

  ValueNoise1D(Seed_Type seed = 2011);
  virtual ~ValueNoise1D();

  // Evaluate the noise function at position x
  Result_Type eval(const Result_Type x) const;

  // Copy Constructor and Assignment
  ValueNoise1D(const ValueNoise1D &other);
  ValueNoise1D &operator=(const ValueNoise1D &other);

  // Move Constructor and Assignment
  ValueNoise1D(ValueNoise1D &&other) noexcept;
  ValueNoise1D &operator=(ValueNoise1D &&other) noexcept;

protected:
  using Conv_Type = typename utils::int_least_fit_t<Seed_Type>;

  static_assert(Period > 1 && !(Period & (Period - 1)),
                "Period must be power of 2 different from 0");
  static constexpr auto kMaxVertices{Period};
  static constexpr auto kMaxVerticesMask{Period - 1};
  static constexpr Result_Type low{0.0};
  static constexpr Result_Type high{1.0};
  std::array<Result_Type, kMaxVertices> r{0.0};
};

template <uint_least8_t Dimension = 2, uint_least16_t Period = 256,
          typename Engine = std::default_random_engine,
          typename Result_Type = float,
          RemapFunction<Result_Type> Remap_Func = smoothstepRemap<Result_Type>>
class ValueNoiseND
    : public ValueNoise1D<Period, Engine, Result_Type, Remap_Func> {
public:
  static_assert(
      2 <= Dimension && Dimension <= 5,
      "Dimension must be between 2 and 5. For 1 Dimensions use ValueNoise1D");

  using ValueNoise1D = ValueNoise1D<Period, Engine, Result_Type, Remap_Func>;

  using Dist = typename ValueNoise1D::Dist;
  using Seed_Type = typename ValueNoise1D::Seed_Type;

  ValueNoiseND(Seed_Type seed = 2011);
  virtual ~ValueNoiseND();

  using Vec2_Type = typename vector::Vec2<Result_Type>;
  using Vec3_Type = typename vector::Vec3<Result_Type>;

  Result_Type eval(const Vec2_Type &p) const;

  // Implementation for 3D noise
  template <uint_least8_t T = Dimension>
  std::enable_if_t<3 <= T, Result_Type> eval(const Vec3_Type &p) const;

  // TODO : create implementation for 4D and 5D Noise 

  // Copy Constructor and Assignment
  ValueNoiseND(const ValueNoiseND &other);
  ValueNoiseND &operator=(const ValueNoiseND &other);

  // Move Constructor and Assignment
  ValueNoiseND(ValueNoiseND &&other) noexcept;
  ValueNoiseND &operator=(ValueNoiseND &&other) noexcept;

protected:
  using Conv_Type = typename ValueNoise1D::Conv_Type;

  using ValueNoise1D::kMaxVertices;
  using ValueNoise1D::kMaxVerticesMask;
  using ValueNoise1D::r;

  std::array<Result_Type, kMaxVertices * 2> permutationTable{0.0};
};

using ValueNoise2D = ValueNoiseND<2>;

using ValueNoise3D = ValueNoiseND<3>;

} // namespace noise

#include "noise/value_noise_impl.hpp"

#endif // !VALUE_NOISE_H