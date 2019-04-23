
#ifndef PERLIN_NOISE_H
#define PERLIN_NOISE_H

#include <array>
#include <cassert>
#include <cstdint>
#include <random>

#include "noise/noise_remap.hpp"
#include "utils/int_fit.hpp"
#include "vec/vec2.hpp"
#include "vec/vec3.hpp"


namespace noise {

template <uint_least16_t Period = 256,
          typename Engine = std::default_random_engine,
          typename Result_Type = float>
class PerlinNoise3D {
public:
  static_assert(std::is_floating_point<Result_Type>(),
                "Result_Type must be a floating point type");

  using Dist = typename std::uniform_real_distribution<Result_Type>;
  using Seed_Type = typename Dist::result_type;

  using Vec2_Type = typename vector::Vec2<Result_Type>;
  using Vec3_Type = typename vector::Vec3<Result_Type>;

  PerlinNoise3D(Seed_Type seed = 2011);
  ~PerlinNoise3D();

  Result_Type eval(Result_Type p) const;

  Result_Type eval(const Vec2_Type &p) const;

  Result_Type eval(const Vec3_Type &p) const;

private:
  using Conv_Type = typename utils::int_least_fit_t<Seed_Type>;

  static_assert(Period > 1 && !(Period & (Period - 1)),
                "Period must be power of 2 different from 0");
  static constexpr auto kTableSize{Period};
  static constexpr auto kTableSizeMask{Period - 1};
  static constexpr Result_Type low{0.0};
  static constexpr Result_Type high{1.0};
  std::array<Vec3_Type, kTableSize> gradients{};
  std::array<Conv_Type, kTableSize * 2> permutationTable{0};

  inline Conv_Type hash(const Conv_Type x, const Conv_Type y,
                        const Conv_Type z) const {
    return permutationTable[permutationTable[permutationTable[x] + y] + z];
  }

  inline Conv_Type hash(const Conv_Type x, const Conv_Type y) const {
    return permutationTable[permutationTable[x] + y];
  }

  inline Conv_Type hash(const Conv_Type x) const { return permutationTable[x]; }
};

using PerlinNoise = PerlinNoise3D<>;

} // namespace noise

#include "noise/perlin_noise_impl.hpp"

#endif // !PERLIN_NOISE_H