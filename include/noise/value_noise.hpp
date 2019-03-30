
#ifndef VALUE_NOISE_H
#define VALUE_NOISE_H

#include <random>
#include <array>

namespace noise {
template <typename Engine = std::default_random_engine, typename T = float>
class ValueNoise1D 
{ 
public:

    using Dist = std::uniform_real_distribution<T>;
    using Seed_Type = typename Dist::result_type;

    ValueNoise1D( Seed_Type seed = 2011 );
 
    // Evaluate the noise function at position x
    T eval(const T &x);
 
private:
    static constexpr unsigned kMaxVertices = {10}; 
    static constexpr T low = {0.0};
    static constexpr T high = {1.0};
    std::array<T, kMaxVertices> r {0.0};

    Dist distribution {ValueNoise1D::low, ValueNoise1D::high};
    Engine generator; 
};

} // namespace noise 

#endif // VALUE_NOISE_H