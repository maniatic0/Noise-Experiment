
#include <cassert>
#include <cmath>

#include "noise/value_noise.hpp"
#include "utils/lerp.hpp"
#include "utils/constants.hpp"

namespace noise
{

template <typename T>
T cosineRemap(const T a, const T b, const T t)
{
    assert(t >= 0 && t <= 1);
    T tRemapCosine = (1 - std::cos(t * utils::pi<T>)) * 0.5;
    return utils::lerp<T>(a, b, tRemapCosine);
}

template <typename T>
T smoothstepRemap(const T a, const T b, const T t)
{
    T tRemapSmoothstep = t * t * (3 - 2 * t);
    return utils::lerp<T>(a, b, tRemapSmoothstep);
}

template <typename T>
T perlinRemap(const T a, const T b, const T t)
{
    T tRemapPerlin = t * t * t * (10 - 15 * t + 6 * t * t);
    return utils::lerp<T>(a, b, tRemapPerlin);
}

template <uint_least16_t Period, typename Engine, typename Result_Type,
          RemapFunction<Result_Type> Remap_Func>
ValueNoise1D<Period, Engine, Result_Type, Remap_Func>::ValueNoise1D(Seed_Type seed)
{
    Dist distribution{ValueNoise1D::low, ValueNoise1D::high};
    Engine generator;

    generator.seed(seed);
    for (unsigned i = 0; i < kMaxVertices; ++i)
    {
        r[i] = distribution(generator);
    }
}

// Auto Generated destructor
template <uint_least16_t Period, typename Engine, typename Result_Type,
          RemapFunction<Result_Type> Remap_Func>
ValueNoise1D<Period, Engine, Result_Type, Remap_Func>::~ValueNoise1D() = default;

// Copy and Move auto generated members

template <uint_least16_t Period, typename Engine, typename Result_Type,
          RemapFunction<Result_Type> Remap_Func>
ValueNoise1D<Period, Engine, Result_Type, Remap_Func>::ValueNoise1D(const ValueNoise1D &other) = default;

template <uint_least16_t Period, typename Engine, typename Result_Type,
          RemapFunction<Result_Type> Remap_Func>
ValueNoise1D<Period, Engine, Result_Type, Remap_Func> &
ValueNoise1D<Period, Engine, Result_Type, Remap_Func>::operator=(const ValueNoise1D &other) = default;

template <uint_least16_t Period, typename Engine, typename Result_Type,
          RemapFunction<Result_Type> Remap_Func>
ValueNoise1D<Period, Engine, Result_Type, Remap_Func>::ValueNoise1D(ValueNoise1D &&other) noexcept = default;

template <uint_least16_t Period, typename Engine, typename Result_Type,
          RemapFunction<Result_Type> Remap_Func>
ValueNoise1D<Period, Engine, Result_Type, Remap_Func> &
ValueNoise1D<Period, Engine, Result_Type, Remap_Func>::operator=(ValueNoise1D &&other) noexcept = default;



template <uint_least16_t Period, typename Engine, typename Result_Type,
          RemapFunction<Result_Type> Remap_Func>
Result_Type
ValueNoise1D<Period, Engine, Result_Type, Remap_Func>::eval(const Result_Type x) const
{
    // Floor using Integer trunc function
    Conv_Type xi = static_cast<Conv_Type>(x) - (x < 0 && x != static_cast<Conv_Type>(x));

    Result_Type t = x - static_cast<Result_Type>(xi);

    // Modulo using the fact that kMaxVerticesMask is a power of 2
    Conv_Type xMin = xi & static_cast<Conv_Type>(kMaxVerticesMask);
    Conv_Type xMax = (xMin + 1) & static_cast<Conv_Type>(kMaxVerticesMask);

    assert(xMin <= kMaxVertices - 1);
    assert(xMax <= kMaxVertices - 1);

    return (*Remap_Func)(r[xMin], r[xMax], t);
}

} // namespace noise

#include <iostream>

int main()
{
    noise::ValueNoise1D valueNoise1D;
    static const int numSteps = 256;

    for (int i = 0; i < numSteps; ++i)
    {
        // x varies from -10 to 10
        float x = (2 * (i / float(numSteps - 1)) - 1) * 10;
        std::cout << "Noise at " << x << ": " << valueNoise1D.eval(x) << std::endl;
    }
    std::cout << "Noise size "
              << ": " << sizeof(valueNoise1D) << std::endl;
}