
#ifndef INT_FIT_H
#define INT_FIT_H

#include <type_traits>
#include <cstdint>

namespace utils
{

template <typename T, typename Int_Type, typename Fallback>
using size_select = typename std::conditional_t<sizeof(T) <= sizeof(Int_Type), Int_Type, Fallback>;
    
template <typename T>
struct int_least_fit { 
    static_assert(std::is_arithmetic<T>::value, "Type T must be arithmetic");
    using type = 
    size_select<T, int_least8_t, 
        size_select<T, int_least16_t, 
            size_select<T, int_least32_t, 
                size_select<T, int_least64_t, intmax_t
                >
            >
        >
    >; 
};

template <typename T>
using int_least_fit_t = typename int_least_fit<T>::type;

template <typename T>
struct uint_least_fit { 
    static_assert(std::is_arithmetic<T>::value, "Type T must be arithmetic");
    using type = 
    size_select<T, uint_least8_t, 
        size_select<T, uint_least16_t, 
            size_select<T, uint_least32_t, 
                size_select<T, uint_least64_t, uintmax_t
                >
            >
        >
    >;
};

template <typename T>
using uint_least_fit_t = typename uint_least_fit<T>::type;

template <typename T>
struct int_fast_fit { 
    static_assert(std::is_arithmetic<T>::value, "Type T must be arithmetic");
    using type = 
    size_select<T, int_fast8_t, 
        size_select<T, int_fast16_t, 
            size_select<T, int_fast32_t, 
                size_select<T, int_fast64_t, intmax_t
                >
            >
        >
    >;
};

template <typename T>
using int_fast_fit_t = typename int_fast_fit<T>::type;

template <typename T>
struct uint_fast_fit { 
    static_assert(std::is_arithmetic<T>::value, "Type T must be arithmetic");
    using type = 
    size_select<T, uint_fast8_t, 
        size_select<T, uint_fast16_t, 
            size_select<T, uint_fast32_t, 
                size_select<T, uint_fast64_t, uintmax_t
                >
            >
        >
    >; 
};

template <typename T>
using uint_fast_fit_t = typename uint_fast_fit<T>::type;


} // namespace utils

#endif // INT_FIT_H