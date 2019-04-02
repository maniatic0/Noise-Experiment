
#ifndef FAST_CONVERTION_H
#define FAST_CONVERTION_H

namespace utils
{
    template <typename Floating_Type, typename Integral_Conv_Type>
    constexpr Integral_Conv_Type fast_int_trunc(const Floating_Type x) {
        return static_cast<Integral_Conv_Type>(x) - (x < 0 && x != static_cast<Integral_Conv_Type>(x));
    }
    
} // utils


#endif // !FAST_CONVERTION_H