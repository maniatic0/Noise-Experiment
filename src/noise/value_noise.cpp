
#include <cassert>

#include "noise/value_noise.hpp"
#include "utils/lerp.hpp"

namespace noise {

template <typename Engine, typename T>
ValueNoise1D<Engine, T>::ValueNoise1D(unsigned seed) {
    //srand48(seed); 
    for (unsigned i = 0; i < kMaxVertices; ++i) { 
        r[i] = distribution(generator); 
    } 
}

template <typename Engine, typename T>
T ValueNoise1D<Engine, T>::eval(const T &x) { 
    int xMin = (int)x; 
    assert(xMin <= kMaxVertices - 1); 
    T t = x - xMin; 

    return utils::lerp(r[xMin], r[xMin + 1], t); 
} 

} // namespace noise
#include <iostream>

int main()
{ 
    noise::ValueNoise1D noise;
    for(size_t i = 0; i < 8; i++)
    {
        std::cout << noise.eval(0.5 + (float) i) << std::endl;
    }
    
}