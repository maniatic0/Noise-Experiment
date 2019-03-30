
#ifndef INT_FIT_H
#define INT_FIT_H

#include <type_traits>
#include <cstdint>

namespace utils
{

template <typename T, typename Int_Type, typename Fallback>
struct size_select {
    using type = typename std::conditional<sizeof(T) <= sizeof(Int_Type), Int_Type, Fallback>::type;
};
    
template <typename T>
struct int_least_fit { 
    static_assert(std::is_arithmetic<T>::value, "Type T must be arithmetic");
    using type = 
    typename size_select<T, int_least8_t, 
        typename  size_select<T, int_least16_t, 
            typename  size_select<T, int_least32_t, 
                typename size_select<T, int_least64_t, intmax_t
                >::type
            >::type
        >::type
    >::type; 
};

template <typename T>
struct uint_least_fit { 
    static_assert(std::is_arithmetic<T>::value, "Type T must be arithmetic");
    using type = 
    typename size_select<T, uint_least8_t, 
        typename  size_select<T, uint_least16_t, 
            typename  size_select<T, uint_least32_t, 
                typename size_select<T, uint_least64_t, uintmax_t
                >::type
            >::type
        >::type
    >::type; 
};

template <typename T>
struct int_fast_fit { 
    static_assert(std::is_arithmetic<T>::value, "Type T must be arithmetic");
    using type = 
    typename size_select<T, int_fast8_t, 
        typename  size_select<T, int_fast16_t, 
            typename  size_select<T, int_fast32_t, 
                typename size_select<T, int_fast64_t, intmax_t
                >::type
            >::type
        >::type
    >::type; 
};

template <typename T>
struct uint_fast_fit { 
    static_assert(std::is_arithmetic<T>::value, "Type T must be arithmetic");
    using type = 
    typename size_select<T, uint_fast8_t, 
        typename  size_select<T, uint_fast16_t, 
            typename  size_select<T, uint_fast32_t, 
                typename size_select<T, uint_fast64_t, uintmax_t
                >::type
            >::type
        >::type
    >::type; 
};


} // namespace utils

#endif // INT_FIT_H