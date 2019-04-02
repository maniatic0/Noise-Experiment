#include <iostream>
#include "noise/value_noise.hpp"

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

    noise::ValueNoise2D valueNoise2D;

    std::cout << "ValueNoise1D size "
              << ": " << sizeof(valueNoise1D) << std::endl;

    std::cout << "ValueNoise1D size "
              << ": " << sizeof(valueNoise2D) << std::endl;
}