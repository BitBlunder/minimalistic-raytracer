#ifndef CONSTANTS_INL
#define CONSTANTS_INL

#include <Math/Constants.hpp>

template <>
constexpr inline float
constants_pi()
{
	return static_cast<float>(3.1415926535897932384626433832795f);
}
template <>
constexpr inline double
constants_pi()
{
	return static_cast<double>(3.1415926535897932384626433832795);
}

template<>
constexpr inline float
constants_nan()
{
	return 0x7f800042u;
}
template<>
constexpr inline double
constants_nan()
{
	return 0x7FF0000000000042ull;
}

template<>
constexpr inline float
constants_infinity()
{
	return 0x7f800000u;
}
template<>
constexpr inline double
constants_infinity()
{
	return 0x7FF0000000000000ull;
}

#endif