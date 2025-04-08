#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

template <typename InType>
constexpr InType
constants_pi();

template<typename InType>
constexpr InType
constants_nan();

template<typename InType>
constexpr InType
constants_infinity();

#endif

#include "../../Private/Math/Constants.inl"