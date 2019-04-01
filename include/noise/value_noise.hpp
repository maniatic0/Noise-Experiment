
#ifndef VALUE_NOISE_H
#define VALUE_NOISE_H

#include <random>
#include <array>
#include <type_traits>
#include <cstdint>

#include "utils/int_fit.hpp"
#include "vec/vec2.hpp"

namespace noise
{

template <typename T = float>
using RemapFunction = T (*)(const T, const T, const T);

template <typename T>
T cosineRemap(const T, const T, const T);

template <typename T>
T smoothstepRemap(const T, const T, const T);

template <typename T>
T perlinRemap(const T, const T, const T);

template <uint_least16_t Period = 256, typename Engine = std::default_random_engine, typename Result_Type = float,
          RemapFunction<Result_Type> Remap_Func = smoothstepRemap<Result_Type>>
class ValueNoise1D
{
  public:
    static_assert(std::is_floating_point<Result_Type>(), "Result_Type must be a floating point type");

    using Dist = typename std::uniform_real_distribution<Result_Type>;
    using Seed_Type = typename Dist::result_type;

    ValueNoise1D(Seed_Type seed = 2011);
    virtual ~ValueNoise1D();

    // Evaluate the noise function at position x
    Result_Type eval(const Result_Type x) const;

    // Copy Constructor and Copy Assigment
    ValueNoise1D(const ValueNoise1D &other);
    ValueNoise1D &operator=(const ValueNoise1D &other);

    // Move Constructor and Copy Assigment
    ValueNoise1D(ValueNoise1D &&other) noexcept;
    ValueNoise1D &operator=(ValueNoise1D &&other) noexcept;

  protected:
    using Conv_Type = typename utils::int_least_fit_t<Seed_Type>;

    static_assert(Period > 1 && !(Period & (Period - 1)), "Period must be power of 2 different from 0");
    static constexpr auto kMaxVertices{Period};
    static constexpr auto kMaxVerticesMask{Period - 1};
    static constexpr Result_Type low{0.0};
    static constexpr Result_Type high{1.0};
    std::array<Result_Type, kMaxVertices> r{0.0};
};

template <uint_least16_t Period = 256, typename Engine = std::default_random_engine, typename Result_Type = float,
          RemapFunction<Result_Type> Remap_Func = smoothstepRemap<Result_Type> >
class ValueNoise2D : public ValueNoise1D<Period, Engine, Result_Type, Remap_Func> 
{
  public:
    using ValueNoise1D = ValueNoise1D<Period, Engine, Result_Type, Remap_Func>;

    using Dist = typename ValueNoise1D::Dist;
    using Seed_Type = typename ValueNoise1D::Seed_Type;

    ValueNoise2D(Seed_Type seed = 2011);
    virtual ~ValueNoise2D();

    protected:

    using Conv_Type = typename ValueNoise1D::Conv_Type;

    using ValueNoise1D::kMaxVertices;
    using ValueNoise1D::kMaxVerticesMask;
    using ValueNoise1D::r;

    std::array<Result_Type, ValueNoise1D::kMaxVertices * 2> permutationTable {0.0};
};

} // namespace noise

#endif // !VALUE_NOISE_H