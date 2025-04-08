#ifndef UTILS_INL
#define UTILS_INL

template <typename InType>
constexpr static inline InType
sign(InType v)
{
	static_assert(std::is_arithmetic<InType>::value,
		"sign requires an arithmetic type for InType.");

	return v < 0 ? InType(-1) : InType(1);
}

template <typename InType>
constexpr static inline InType
radians_to_degrees(InType radians)
{
	static_assert(std::is_arithmetic<InType>::value,
		"radians_to_degrees requires an arithmetic type for InType.");

	return radians * (InType(180) / constants_pi<InType>());
}

template <typename InType>
constexpr static inline InType
degrees_to_radians(InType degrees)
{
	static_assert(std::is_arithmetic<InType>::value,
		"degrees_to_radians requires an arithmetic type for InType.");

	return degrees * (constants_pi<InType>()/ InType(180));
}

#endif