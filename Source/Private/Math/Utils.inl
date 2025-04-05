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

#endif