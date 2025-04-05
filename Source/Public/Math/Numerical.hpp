#ifndef NUMERICAL_HPP
#define NUMERICAL_HPP

#include <cmath>
#include <utility>
#include <type_traits>

#include <Math/Utils.hpp>

template<typename InType>
struct QuadraticSolution
{
	int roots;

	InType solution1;
	InType solution2;
};

template<typename InType>
const QuadraticSolution<InType>
find_quadratic_root(InType a, InType b, InType c);

#endif

#include "../../Private/Math/Numerical.inl"