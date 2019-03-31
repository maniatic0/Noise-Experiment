
#include <cassert>
#include <cmath>

#include "noise/value_noise.hpp"
#include "utils/lerp.hpp"
#include "utils/constants.hpp"

namespace noise {


template<typename T>
T cosineRemap(const T a, const T b, const T t) {
    assert(t >= 0 && t <= 1); 
    T tRemapCosine = (1 - std::cos(t * utils::pi<T>)) * 0.5; 
    return utils::lerp<T>(a, b, tRemapCosine); 
}

template<typename T>
T smoothstepRemap(const T a, const T b, const T t) {
    T tRemapSmoothstep = t * t * (3 - 2 * t); 
    return utils::lerp<T>(a, b, tRemapSmoothstep); 
}

template<typename T>
T perlinRemap(const T a, const T b, const T t) {
    T tRemapPerlin = t * t * t * (10 - 15 * t + 6 * t * t); 
    return utils::lerp<T>(a, b, tRemapPerlin); 
}

template <typename Engine, typename Result_Type, RemapFunction<Result_Type> Remap_Func>
ValueNoise1D<Engine, Result_Type, Remap_Func>::ValueNoise1D(Seed_Type seed) {
    generator.seed(seed);
    for (unsigned i = 0; i < kMaxVertices; ++i) { 
        r[i] = distribution(generator); 
    } 
}

template <typename Engine, typename Result_Type, RemapFunction<Result_Type> Remap_Func>
Result_Type ValueNoise1D<Engine, Result_Type, Remap_Func>::eval(const Result_Type x) {
    // Integer trunc function
    Conv_Type xi = static_cast<Conv_Type>(x);; 
    Conv_Type xMin = xi % static_cast<Conv_Type>(kMaxVertices);
    Result_Type t = x - static_cast<Result_Type>(xi); 
    Conv_Type xMax = (xMin == static_cast<Conv_Type>(kMaxVertices) - 1) ? 0 : xMin + 1; 


    assert(xMin <= kMaxVertices - 1); 
    assert(xMax <= kMaxVertices - 1); 

    return (*Remap_Func)(r[xMin], r[xMax], t);  
} 

} // namespace noise

#include <iostream>

int main()
{ 
    std::cout << noise::cosineRemap<float>(0,1,1)<< std::endl;
    noise::ValueNoise1D noise;
    for(size_t i = 85; i < 100; i++)
    {
        std::cout << noise.eval(0.5 + (float) i) << std::endl;
    }
    
}