#include <cstdio>
#include <fstream>
#include <functional>
#include <iostream>
#include <random>
#include <string>

#include "noise/perlin_noise.hpp"
#include "noise/value_noise.hpp"
#include "vec/vec2.hpp"
#include "vec/vec3.hpp"

void save2PPM(const char *filename, unsigned imageWidth, unsigned imageHeight,
              float *noiseMap) {
  // output noise map to PPM
  std::ofstream ofs;
  ofs.open(filename, std::ios::out | std::ios::binary);
  ofs << "P6\n" << imageWidth << " " << imageHeight << "\n255\n";
  for (unsigned k = 0; k < imageWidth * imageHeight; ++k) {
    unsigned char n = static_cast<unsigned char>(noiseMap[k] * 255);
    ofs << n << n << n;
  }
  ofs.close();
}

int main() {
  noise::PerlinNoise noiseTest;
#if 0
  static const int numSteps = 256;

  for (int i = 0; i < numSteps; ++i)
  {
    // x varies from -10 to 10
    float x = (2 * (i / float(numSteps - 1)) - 1) * 10;
    std::cout << "Noise at " << x << ": " << noiseTest.eval(x) << std::endl;
  }

#endif

  unsigned imageWidth = 512;
  unsigned imageHeight = 512;
  float *noiseMap = new float[imageWidth * imageHeight];

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

  // output white noise map to PPM
  save2PPM("./white_noise.ppm", imageWidth, imageHeight, noiseMap);
  delete[] noiseMap;
  noiseMap = new float[imageWidth * imageHeight];

  noise::ValueNoise2D noise;
  {
    // generate value noise
    float frequency = 0.05f;
    for (unsigned j = 0; j < imageHeight; ++j) {
      for (unsigned i = 0; i < imageWidth; ++i) {
        // generate a float in the range [0:1]
        noiseMap[j * imageWidth + i] =
            noise.eval(vector::Vec2<float>(i, j) * frequency);
      }
    }
  }

  // output value noise map to PPM
  save2PPM("./value_noise.ppm", imageWidth, imageHeight, noiseMap);
  delete[] noiseMap;
  noiseMap = new float[imageWidth * imageHeight];

  // Brown Noise
  {
    unsigned numLayers = 5;
    float maxNoiseVal = 0;
    float frequency = 0.01f;
    for (unsigned j = 0; j < imageHeight; ++j) {
      for (unsigned i = 0; i < imageWidth; ++i) {
        vector::Vec2f pNoise = vector::Vec2f(i, j) * frequency;
        float amplitude = 1.0f / frequency;
        noiseMap[j * imageWidth + i] = 0;
        for (unsigned l = 0; l < numLayers; ++l) {
          noiseMap[j * imageWidth + i] += noise.eval(pNoise) * amplitude;
          pNoise *= 2.0;
          amplitude *= 0.5;
        }
        if (noiseMap[j * imageWidth + i] > maxNoiseVal)
          maxNoiseVal = noiseMap[j * imageWidth + i];
      }
    }

    for (size_t i = 0; i < imageWidth * imageHeight; i++) {
      noiseMap[i] /= maxNoiseVal;
    }
  }

  // output brown noise map to PPM
  save2PPM("./brown_noise.ppm", imageWidth, imageHeight, noiseMap);
  delete[] noiseMap;
  noiseMap = new float[imageWidth * imageHeight];

  float frequency = 0.02f;
  float frequencyMult = 1.8; // lacunarity
  float amplitudeMult = 0.35;
  unsigned numLayers = 5;
  float maxNoiseVal = 0;
  for (unsigned j = 0; j < imageHeight; ++j) {
    for (unsigned i = 0; i < imageWidth; ++i) {
      vector::Vec2f pNoise = vector::Vec2f(i, j) * frequency;
      float amplitude = 1;
      noiseMap[j * imageWidth + i] = 0;
      for (unsigned l = 0; l < numLayers; ++l) {
//#define TURBULENCE
#ifdef TURBULENCE
#include <cmath>
        noiseMap[j * imageWidth + i] +=
            std::fabs(2 * noise.eval(pNoise) - 1) * amplitude;
#else
        noiseMap[j * imageWidth + i] += noise.eval(pNoise) * amplitude;
#endif // !TURBULENCE

        pNoise *= frequencyMult;
        amplitude *= amplitudeMult;
      }

//#define MARBEL_TEXTURE
#define WOOD_TEXTURE
#ifdef MARBEL_TEXTURE
#include "utils/constants.hpp"
#include <cmath>

      noiseMap[j * imageWidth + i] =
          (std::sin((i + noiseMap[j * imageWidth + i] * 100) * 2 *
                    utils::pi<float> / 200.f) +
           1) /
          2.f;
      maxNoiseVal = 1.0f;
#elif defined(WOOD_TEXTURE)
#include "utils/fast_convertion.hpp"
      constexpr int grain = 4; // Wood Grain
      float g = noise.eval(vector::Vec2f(i, j) * frequency) * grain;
      noiseMap[j * imageWidth + i] = g - static_cast<int>(g);
      maxNoiseVal = 1.0f;
#else
      if (noiseMap[j * imageWidth + i] > maxNoiseVal) {
        maxNoiseVal = noiseMap[j * imageWidth + i];
      }
#endif // MARBEL_TEXTURE
    }
  }
  for (unsigned i = 0; i < imageWidth * imageHeight; ++i)
    noiseMap[i] /= maxNoiseVal;

  // output noise map to PPM
  save2PPM("./noise.ppm", imageWidth, imageHeight, noiseMap);
  delete[] noiseMap;

  noise::ValueNoise1D valueNoise1D;
  noise::ValueNoise2D valueNoise2D;
  noise::ValueNoise3D valueNoise3D;
  noise::PerlinNoise perlinNoise3D;

  valueNoise3D.eval(vector::Vec3f(-1, 1, 0) * 0.5f);

  std::cout << "ValueNoise1D size "
            << ": " << sizeof(valueNoise1D) << std::endl;

  std::cout << "ValueNoise2D size "
            << ": " << sizeof(valueNoise2D) << std::endl;

  std::cout << "ValueNoise3D size "
            << ": " << sizeof(valueNoise3D) << std::endl;

  std::cout << "PerlinNoise size "
            << ": " << sizeof(perlinNoise3D) << std::endl;

  return 0;
}