
#ifndef VALUE_NOISE_H
#define VALUE_NOISE_H

#include <random>
#include <array>
#include <type_traits>

#include "utils/int_fit.hpp"

namespace noise {
template <typename Engine = std::default_random_engine, typename T = float>
class ValueNoise1D 
{ 
public:

    static_assert(std::is_floating_point<T>(), "T must be a floating point type");

    using Dist = std::uniform_real_distribution<T>;
    using Seed_Type = typename Dist::result_type; 

    ValueNoise1D( Seed_Type seed = 2011 );
 
    // Evaluate the noise function at position x
    T eval(const T &x);
 
private:

    using Conv_Type = typename utils::uint_least_fit<Seed_Type>::type;

    static constexpr unsigned kMaxVertices = {10}; 
    static constexpr T low = {0.0};
    static constexpr T high = {1.0};
    std::array<T, kMaxVertices> r {0.0};

    Dist distribution {ValueNoise1D::low, ValueNoise1D::high};
    Engine generator; 
};

} // namespace noise 

#endif // VALUE_NOISE_H