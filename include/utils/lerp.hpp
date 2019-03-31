
#ifndef LERP_UTILITY_H
#define LERP_UTILITY_H

namespace utils
{

template <typename T = float>
constexpr inline T lerp(const T &lo, const T &hi, const T &t) 
{ 
    return lo * (1 - t) + hi * t; 
}

} // namespace utils

#endif // !LERP_UTILITY_H
