#ifndef UTILS_HPP
#define UTILS_HPP

#include <Math/Constants.hpp>

template <typename InType>
constexpr InType
sign(InType v);

template <typename InType>
constexpr InType
radians_to_degrees(InType radians);

template <typename InType>
constexpr InType
degrees_to_radians(InType degrees);

#endif

#include "../../Private/Math/Utils.inl"