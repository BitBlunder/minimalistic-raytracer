#ifndef NUMERICAL_INL
#define NUMERICAL_INL

#include <Math/Numerical.hpp>

template<typename InType>
const QuadraticSolution<InType>
find_quadratic_root(InType a, InType b, InType c)
{
	static_assert(std::is_arithmetic<InType>::value,
		"find_quadratic_root requires an arithmetic type for InType.");

	if (a == InType(0))
	{
		if (b == InType(0))
			return { 0, constants_nan<float>(), constants_nan<float>() };

		InType result = -c / b;
		return { 1, result, result };
	}

	InType det = (b * b) - (4 * a * c);
	if(det < InType(0))
		return { 0, constants_nan<float>(), constants_nan<float>() };

	InType q = (b + sign(b) * sqrt(det)) / InType(-2);

	InType t1 = q / a;
	InType t2 = c / q;

	return { 2, InType(t1), InType(t2) };
}

#endif