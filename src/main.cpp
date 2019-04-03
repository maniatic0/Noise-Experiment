#include <iostream>
#include <fstream>
#include <cstdio>
#include <random>
#include <functional>

#include "noise/value_noise.hpp"
#include "vec/vec2.hpp"
#include "vec/vec3.hpp"

int main()
{
  noise::ValueNoise1D valueNoise1D;
#if 0
  static const int numSteps = 256;

  for (int i = 0; i < numSteps; ++i)
  {
    // x varies from -10 to 10
    float x = (2 * (i / float(numSteps - 1)) - 1) * 10;
    std::cout << "Noise at " << x << ": " << valueNoise1D.eval(x) << std::endl;
  }

#endif

  unsigned imageWidth = 512;
  unsigned imageHeight = 512;
  float *noiseMap = new float[imageWidth * imageHeight];
#if 0 
    // generate white noise
    unsigned seed = 2016; 
    std::mt19937 gen(seed); 
    std::uniform_real_distribution distr; 
    auto dice = std::bind(distr, gen); // std::function<float()> 
 
    for (unsigned j = 0; j < imageHeight; ++j) { 
        for (unsigned i = 0; i < imageWidth; ++i) { 
            // generate a float in the range [0:1]
            noiseMap[j * imageWidth + i] = dice(); 
        } 
    }
#else
#if 0
  // generate value noise
  noise::ValueNoise2D noise;
  float frequency = 0.05f;
  for (unsigned j = 0; j < imageHeight; ++j)
  {
    for (unsigned i = 0; i < imageWidth; ++i)
    {
      // generate a float in the range [0:1]
      noiseMap[j * imageWidth + i] = noise.eval(vector::Vec2<float>(i, j) * frequency);
    }
  }
#else
  noise::ValueNoiseND<2, 512> noise;
  unsigned numLayers = 5;
  float maxNoiseVal = 0;
  float frequency = 0.01f;
  for (unsigned j = 0; j < imageHeight; ++j)
  {
    for (unsigned i = 0; i < imageWidth; ++i)
    {
      vector::Vec2f pNoise = vector::Vec2f(i, j) * frequency;
      float amplitude = 1.0f / frequency;
      noiseMap[j * imageWidth + i] = 0;
      for (unsigned l = 0; l < numLayers; ++l)
      {
        noiseMap[j * imageWidth + i] += noise.eval(pNoise) * amplitude;
        pNoise *= 2.0;
        amplitude *= 0.5;
      }
      if (noiseMap[j * imageWidth + i] > maxNoiseVal)
        maxNoiseVal = noiseMap[j * imageWidth + i];
    }
  }

  for (size_t i = 0; i < imageWidth * imageHeight; i++)
  {
    noiseMap[i] /= maxNoiseVal;
  }

#endif
#endif

  // output noise map to PPM
  std::ofstream ofs;
  ofs.open("./noise.ppm", std::ios::out | std::ios::binary);
  ofs << "P6\n"
      << imageWidth << " " << imageHeight << "\n255\n";
  for (unsigned k = 0; k < imageWidth * imageHeight; ++k)
  {
    unsigned char n = static_cast<unsigned char>(noiseMap[k] * 255);
    ofs << n << n << n;
  }
  ofs.close();

  delete[] noiseMap;

  noise::ValueNoise2D valueNoise2D;
  noise::ValueNoise3D valueNoise3D;

  valueNoise3D.eval(vector::Vec3f(-1, 1, 0) * 0.5f);

  std::cout << "ValueNoise1D size "
            << ": " << sizeof(valueNoise1D) << std::endl;

  std::cout << "ValueNoise2D size "
            << ": " << sizeof(valueNoise2D) << std::endl;

  std::cout << "ValueNoise3D size "
            << ": " << sizeof(valueNoise3D) << std::endl;

  return 0;
}