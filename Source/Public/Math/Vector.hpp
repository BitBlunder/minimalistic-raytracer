#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <cmath>
#include <cstdint>
#include <iostream>
#include <type_traits>

template<typename InType, size_t InDims>
struct VectorN
{
	static_assert(std::is_arithmetic<InType>::value,
		"VectorN requires an arithmetic type for InType.");

	InType data[InDims];

	InType& operator[](size_t index)
	{
		return this->data[index];
	}
	const InType& operator[](size_t index) const
	{
		return this->data[index];
	}
};

template<typename InType>
struct VectorN<InType, 2>
{
	static_assert(std::is_arithmetic<InType>::value,
		"VectorN requires an arithmetic type for InType.");

	union
	{
		InType data[2];

		struct { InType x, y; };
		struct { InType u, v; };
	};

	InType& operator[](size_t index)
	{
		return this->data[index];
	}
	const InType& operator[](size_t index) const
	{
		return this->data[index];
	}
};

template<typename InType>
struct VectorN<InType, 3>
{
	static_assert(std::is_arithmetic<InType>::value,
		"VectorN requires an arithmetic type for InType.");

	union
	{
		InType data[3];

		struct { InType x, y, z; };
		struct { InType u, v, t; };
		struct { InType r, g, b; };
	};

	InType& operator[](size_t index)
	{
		return this->data[index];
	}
	const InType& operator[](size_t index) const
	{
		return this->data[index];
	}
};

template<typename InType>
struct VectorN<InType, 4>
{
	static_assert(std::is_arithmetic<InType>::value,
		"VectorN requires an arithmetic type for InType.");

	union
	{
		InType data[4];

		struct { InType x, y, z, w; };
	};

	InType& operator[](size_t index)
	{
		return this->data[index];
	}
	const InType& operator[](size_t index) const
	{
		return this->data[index];
	}
};


template<typename InType, size_t InDims>
constexpr VectorN<InType, InDims>
vector_normalize(VectorN<InType, InDims>& v);

template<typename InType, size_t InDims>
constexpr InType
vector_length(const VectorN<InType, InDims>& v);

template<typename InType, size_t InDims>
constexpr InType
vector_distance(const VectorN<InType, InDims>& lhs, const VectorN<InType, InDims>& rhs);

template<typename InType, size_t InDims>
constexpr InType
vector_length_squared(const VectorN<InType, InDims>& v);

template<typename InType, size_t InDims>
constexpr InType
vector_distance_squared(const VectorN<InType, InDims>& lhs, const VectorN<InType, InDims>& rhs);

template<typename InType, size_t InDims>
constexpr InType
vector_dot(const VectorN<InType, InDims>& lhs, const VectorN<InType, InDims>& rhs);

template<typename InType, size_t InDims>
constexpr InType
vector_angle(const VectorN<InType, InDims>& lhs, const VectorN<InType, InDims>& rhs);

template<typename InType>
constexpr VectorN<InType, 3>
vector_cross(const VectorN<InType, 3>& lhs, const VectorN<InType, 3>& rhs);


template<typename InType, size_t InDims>
VectorN<InType, InDims>
operator-(const VectorN<InType, InDims>& v);


template<typename InType, size_t InDims>
bool
operator==(const VectorN<InType, InDims>& lhs, const VectorN<InType, InDims>& rhs);

template<typename InType, size_t InDims>
bool
operator!=(const VectorN<InType, InDims>& lhs, const VectorN<InType, InDims>& rhs);


template<typename InType, size_t InDims>
VectorN<InType, InDims>
operator+=(VectorN<InType, InDims>& lhs, const InType& s);

template<typename InType, size_t InDims>
VectorN<InType, InDims>
operator-=(VectorN<InType, InDims>& lhs, const InType& s);

template<typename InType, size_t InDims>
VectorN<InType, InDims>
operator*=(VectorN<InType, InDims>& lhs, const InType& s);

template<typename InType, size_t InDims>
VectorN<InType, InDims>
operator/=(VectorN<InType, InDims>& lhs, const InType& s);


template<typename InType, size_t InDims>
VectorN<InType, InDims>
operator+(const VectorN<InType, InDims>& lhs, InType s);

template<typename InType, size_t InDims>
VectorN<InType, InDims>
operator+(InType s, const VectorN<InType, InDims>& rhs);

template<typename InType, size_t InDims>
VectorN<InType, InDims>
operator-(const VectorN<InType, InDims>& lhs, InType s);

template<typename InType, size_t InDims>
VectorN<InType, InDims>
operator-(InType s, const VectorN<InType, InDims>& rhs);

template<typename InType, size_t InDims>
VectorN<InType, InDims>
operator*(const VectorN<InType, InDims>& lhs, InType s);

template<typename InType, size_t InDims>
VectorN<InType, InDims>
operator*(InType s, const VectorN<InType, InDims>& rhs);

template<typename InType, size_t InDims>
VectorN<InType, InDims>
operator/(const VectorN<InType, InDims>& lhs, InType s);

template<typename InType, size_t InDims>
VectorN<InType, InDims>
operator/(InType s, const VectorN<InType, InDims>& rhs);


template<typename InType, size_t InDims>
VectorN<InType, InDims>
operator+=(VectorN<InType, InDims>& lhs, const VectorN<InType, InDims>& rhs);

template<typename InType, size_t InDims>
VectorN<InType, InDims>
operator-=(VectorN<InType, InDims>& lhs, const VectorN<InType, InDims>& rhs);

template<typename InType, size_t InDims>
VectorN<InType, InDims>
operator*=(VectorN<InType, InDims>& lhs, const VectorN<InType, InDims>& rhs);

template<typename InType, size_t InDims>
VectorN<InType, InDims>
operator/=(VectorN<InType, InDims>& lhs, const VectorN<InType, InDims>& rhs);


template<typename InType, size_t InDims>
VectorN<InType, InDims>
operator+(const VectorN<InType, InDims>& lhs, const VectorN<InType, InDims>& rhs);

template<typename InType, size_t InDims>
VectorN<InType, InDims>
operator-(const VectorN<InType, InDims>& lhs, const VectorN<InType, InDims>& rhs);

template<typename InType, size_t InDims>
VectorN<InType, InDims>
operator*(const VectorN<InType, InDims>& lhs, const VectorN<InType, InDims>& rhs);

template<typename InType, size_t InDims>
VectorN<InType, InDims>
operator/(const VectorN<InType, InDims>& lhs, const VectorN<InType, InDims>& rhs);


template<typename InType, size_t InDims>
std::ostream& operator<<(std::ostream& os, const VectorN<InType, InDims>& v);


using Vec2i = VectorN<int, 2>;
using Vec3i = VectorN<int, 3>;
using Vec4i = VectorN<int, 4>;

using Vec2f = VectorN<float, 2>;
using Vec3f = VectorN<float, 3>;
using Vec4f = VectorN<float, 4>;

using Vec2d = VectorN<double, 2>;
using Vec3d = VectorN<double, 3>;
using Vec4d = VectorN<double, 4>;

#endif

#include "../../Private/Math/Vector.inl"