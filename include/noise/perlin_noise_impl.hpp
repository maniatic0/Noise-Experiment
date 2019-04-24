
#ifndef PERLIN_NOISE_IMPL_H
#define PERLIN_NOISE_IMPL_H

#include <cmath>
#include <functional>

#include "noise/perlin_noise.hpp"

#include "noise/noise_remap.hpp"
#include "utils/constants.hpp"
#include "utils/fast_convertion.hpp"
#include "utils/lerp.hpp"
#include "vec/vec3.hpp"


namespace noise {

template <uint_least16_t Period, typename Engine, typename Result_Type>
PerlinNoise3D<Period, Engine, Result_Type>::PerlinNoise3D(Seed_Type seed) {
  Dist distribution{low, high};
  Engine generator;

  auto dice = std::bind(distribution, generator);

  Result_Type theta, phi;

  generator.seed(seed);
  for (auto i = 0; i < kTableSize; ++i) {

    theta = std::acos(2.0 * dice() - 1.0);
    phi = 2.0 * dice() * utils::pi<Result_Type>;

    gradients[i].x = std::cos(phi) * std::sin(theta);
    gradients[i].y = std::sin(phi) * std::sin(theta);
    gradients[i].z = std::cos(theta);

    permutationTable[i] = i;
  }

  // shuffle values of the permutation table
  std::uniform_int_distribution distrUInt{0, kTableSizeMask};
  auto randUInt = std::bind(distrUInt, generator);
  for (auto k = 0; k < kTableSize; ++k) {
    auto i = randUInt();
    std::swap(permutationTable[k], permutationTable[i]);
    permutationTable[k + kTableSize] = permutationTable[k];
  }
}

template <uint_least16_t Period, typename Engine, typename Result_Type>
PerlinNoise3D<Period, Engine, Result_Type>::~PerlinNoise3D() = default;

template <uint_least16_t Period, typename Engine, typename Result_Type>
Result_Type
PerlinNoise3D<Period, Engine, Result_Type>::eval(const Result_Type x) const {

  constexpr auto fast_int_trunc = utils::fast_int_trunc<Result_Type, Conv_Type>;

  const Conv_Type posX = fast_int_trunc(x);

  const Conv_Type xi0 = posX & kTableSizeMask;

  const Conv_Type xi1 = (xi0 + 1) & kTableSizeMask;

  const Result_Type tx = x - static_cast<Result_Type>(posX);

  const Result_Type u = perlinRemap<Result_Type>(tx);

  // gradients at the corner of the cell
  const Vec3_Type &c0 = gradients[hash(xi0)];
  const Vec3_Type &c1 = gradients[hash(xi1)];

  // generate vectors going from the grid points to p
  const Result_Type x0 = tx, x1 = tx - 1;

  const Vec3_Type p0 = Vec3_Type(x0, 0, 0);
  const Vec3_Type p1 = Vec3_Type(x1, 0, 0);

  // linear interpolation
  constexpr auto lerp = utils::lerp<Result_Type>;

  return lerp(vector::dot(c0, p0), dot(c1, p1), u); // g
}

template <uint_least16_t Period, typename Engine, typename Result_Type>
Result_Type
PerlinNoise3D<Period, Engine, Result_Type>::eval(const Vec2_Type &p) const {

  constexpr auto fast_int_trunc = utils::fast_int_trunc<Result_Type, Conv_Type>;

  const Conv_Type posX = fast_int_trunc(p.x);
  const Conv_Type posY = fast_int_trunc(p.y);

  const Conv_Type xi0 = posX & kTableSizeMask;
  const Conv_Type yi0 = posY & kTableSizeMask;

  const Conv_Type xi1 = (xi0 + 1) & kTableSizeMask;
  const Conv_Type yi1 = (yi0 + 1) & kTableSizeMask;

  const Result_Type tx = p.x - static_cast<Result_Type>(posX);
  const Result_Type ty = p.y - static_cast<Result_Type>(posY);

  const Result_Type u = perlinRemap<Result_Type>(tx);
  const Result_Type v = perlinRemap<Result_Type>(ty);

  // gradients at the corner of the cell
  const Vec3_Type &c00 = gradients[hash(xi0, yi0)];
  const Vec3_Type &c10 = gradients[hash(xi1, yi0)];
  const Vec3_Type &c01 = gradients[hash(xi0, yi1)];
  const Vec3_Type &c11 = gradients[hash(xi1, yi1)];

  // generate vectors going from the grid points to p
  const Result_Type x0 = tx, x1 = tx - 1;
  const Result_Type y0 = ty, y1 = ty - 1;

  const Vec3_Type p00 = Vec3_Type(x0, y0, 0);
  const Vec3_Type p10 = Vec3_Type(x1, y0, 0);
  const Vec3_Type p01 = Vec3_Type(x0, y1, 0);
  const Vec3_Type p11 = Vec3_Type(x1, y1, 0);

  // linear interpolation
  constexpr auto lerp = utils::lerp<Result_Type>;
  const Result_Type a = lerp(vector::dot(c00, p00), dot(c10, p10), u);
  const Result_Type b = lerp(vector::dot(c01, p01), dot(c11, p11), u);

  return lerp(a, b, v); // g
}

template <uint_least16_t Period, typename Engine, typename Result_Type>
Result_Type
PerlinNoise3D<Period, Engine, Result_Type>::eval(const Vec3_Type &p) const {

  constexpr auto fast_int_trunc = utils::fast_int_trunc<Result_Type, Conv_Type>;

  const Conv_Type posX = fast_int_trunc(p.x);
  const Conv_Type posY = fast_int_trunc(p.y);
  const Conv_Type posZ = fast_int_trunc(p.z);

  const Conv_Type xi0 = posX & kTableSizeMask;
  const Conv_Type yi0 = posY & kTableSizeMask;
  const Conv_Type zi0 = posZ & kTableSizeMask;

  const Conv_Type xi1 = (xi0 + 1) & kTableSizeMask;
  const Conv_Type yi1 = (yi0 + 1) & kTableSizeMask;
  const Conv_Type zi1 = (zi0 + 1) & kTableSizeMask;

  const Result_Type tx = p.x - static_cast<Result_Type>(posX);
  const Result_Type ty = p.y - static_cast<Result_Type>(posY);
  const Result_Type tz = p.z - static_cast<Result_Type>(posZ);

  constexpr auto remap = perlinRemap<Result_Type>;

  const Result_Type u = remap(tx);
  const Result_Type v = remap(ty);
  const Result_Type w = remap(tz);

  // gradients at the corner of the cell
  const Vec3_Type &c000 = gradients[hash(xi0, yi0, zi0)];
  const Vec3_Type &c100 = gradients[hash(xi1, yi0, zi0)];
  const Vec3_Type &c010 = gradients[hash(xi0, yi1, zi0)];
  const Vec3_Type &c110 = gradients[hash(xi1, yi1, zi0)];

  const Vec3_Type &c001 = gradients[hash(xi0, yi0, zi1)];
  const Vec3_Type &c101 = gradients[hash(xi1, yi0, zi1)];
  const Vec3_Type &c011 = gradients[hash(xi0, yi1, zi1)];
  const Vec3_Type &c111 = gradients[hash(xi1, yi1, zi1)];

  // generate vectors going from the grid points to p
  const Result_Type x0 = tx, x1 = tx - 1;
  const Result_Type y0 = ty, y1 = ty - 1;
  const Result_Type z0 = tz, z1 = tz - 1;

  const Vec3_Type p000 = Vec3_Type(x0, y0, z0);
  const Vec3_Type p100 = Vec3_Type(x1, y0, z0);
  const Vec3_Type p010 = Vec3_Type(x0, y1, z0);
  const Vec3_Type p110 = Vec3_Type(x1, y1, z0);

  const Vec3_Type p001 = Vec3_Type(x0, y0, z1);
  const Vec3_Type p101 = Vec3_Type(x1, y0, z1);
  const Vec3_Type p011 = Vec3_Type(x0, y1, z1);
  const Vec3_Type p111 = Vec3_Type(x1, y1, z1);

  // linear interpolation
  constexpr auto lerp = utils::lerp<Result_Type>;
  const Result_Type a = lerp(vector::dot(c000, p000), dot(c100, p100), u);
  const Result_Type b = lerp(vector::dot(c010, p010), dot(c110, p110), u);
  const Result_Type c = lerp(vector::dot(c001, p001), dot(c101, p101), u);
  const Result_Type d = lerp(vector::dot(c011, p011), dot(c111, p111), u);

  const Result_Type e = lerp(a, b, v);
  const Result_Type f = lerp(c, d, v);

  return lerp(e, f, w); // g
}

template <uint_least16_t Period, typename Engine, typename Result_Type>
Result_Type
PerlinNoise3D<Period, Engine, Result_Type>::eval(const Vec3_Type &p, Vec3_Type &deriv) const {

  constexpr auto fast_int_trunc = utils::fast_int_trunc<Result_Type, Conv_Type>;

  const Conv_Type posX = fast_int_trunc(p.x);
  const Conv_Type posY = fast_int_trunc(p.y);
  const Conv_Type posZ = fast_int_trunc(p.z);

  const Conv_Type xi0 = posX & kTableSizeMask;
  const Conv_Type yi0 = posY & kTableSizeMask;
  const Conv_Type zi0 = posZ & kTableSizeMask;

  const Conv_Type xi1 = (xi0 + 1) & kTableSizeMask;
  const Conv_Type yi1 = (yi0 + 1) & kTableSizeMask;
  const Conv_Type zi1 = (zi0 + 1) & kTableSizeMask;

  const Result_Type tx = p.x - static_cast<Result_Type>(posX);
  const Result_Type ty = p.y - static_cast<Result_Type>(posY);
  const Result_Type tz = p.z - static_cast<Result_Type>(posZ);

  constexpr auto remap = perlinRemap<Result_Type>;

  const Result_Type u = remap(tx);
  const Result_Type v = remap(ty);
  const Result_Type w = remap(tz);

  constexpr auto remapDeriv = perlinRemapDeriv<Result_Type>;

  const Result_Type du = remapDeriv(tx); 
  const Result_Type dv = remapDeriv(ty); 
  const Result_Type dw = remapDeriv(tz); 

  // gradients at the corner of the cell
  const Vec3_Type &c000 = gradients[hash(xi0, yi0, zi0)];
  const Vec3_Type &c100 = gradients[hash(xi1, yi0, zi0)];
  const Vec3_Type &c010 = gradients[hash(xi0, yi1, zi0)];
  const Vec3_Type &c110 = gradients[hash(xi1, yi1, zi0)];

  const Vec3_Type &c001 = gradients[hash(xi0, yi0, zi1)];
  const Vec3_Type &c101 = gradients[hash(xi1, yi0, zi1)];
  const Vec3_Type &c011 = gradients[hash(xi0, yi1, zi1)];
  const Vec3_Type &c111 = gradients[hash(xi1, yi1, zi1)];

  // generate vectors going from the grid points to p
  const Result_Type x0 = tx, x1 = tx - 1;
  const Result_Type y0 = ty, y1 = ty - 1;
  const Result_Type z0 = tz, z1 = tz - 1;

  const Vec3_Type p000 = Vec3_Type(x0, y0, z0);
  const Vec3_Type p100 = Vec3_Type(x1, y0, z0);
  const Vec3_Type p010 = Vec3_Type(x0, y1, z0);
  const Vec3_Type p110 = Vec3_Type(x1, y1, z0);

  const Vec3_Type p001 = Vec3_Type(x0, y0, z1);
  const Vec3_Type p101 = Vec3_Type(x1, y0, z1);
  const Vec3_Type p011 = Vec3_Type(x0, y1, z1);
  const Vec3_Type p111 = Vec3_Type(x1, y1, z1);

  // linear interpolation

  const Result_Type a = vector::dot(c000, p000); 
  const Result_Type b = vector::dot(c100, p100); 
  const Result_Type c = vector::dot(c010, p010); 
  const Result_Type d = vector::dot(c110, p110); 
  const Result_Type e = vector::dot(c001, p001); 
  const Result_Type f = vector::dot(c101, p101); 
  const Result_Type g = vector::dot(c011, p011); 
  const Result_Type h = vector::dot(c111, p111); 

  const Result_Type k0 = (b - a); 
  const Result_Type k1 = (c - a); 
  const Result_Type k2 = (e - a); 
  const Result_Type k3 = (a + d - b - c); 
  const Result_Type k4 = (a + f - b - e); 
  const Result_Type k5 = (a + g - c - e); 
  const Result_Type k6 = (b + c + e + h - a - d - f - g); 

  derivs.x = du *(k0 + v * k3 + w * k4 + v * w * k6); 
  derivs.y = dv *(k1 + u * k3 + w * k5 + u * w * k6); 
  derivs.z = dw *(k2 + u * k4 + v * k5 + u * v * k6); 

  return a + u * k0 + v * k1 + w * k2 + u * v * k3 + u * w * k4 + v * w * k5 + u * v * w * k6;
}

} // namespace noise

#endif // !PERLIN_NOISE_IMPL_H