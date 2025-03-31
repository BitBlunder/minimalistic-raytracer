#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <cstdint>
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

		struct { InType x, y };
		struct { InType u, v };
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

		struct { InType x, y, z };
		struct { InType u, v, t };
		struct { InType r, g, b };
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

		struct { InType x, y, z, w};
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
inline VectorN<InType, InDims>
operator-(const VectorN<InType, InDims>& v)
{
	VectorN<InType, InDims> result;

	for (size_t i = 0; i < InDims; i++)
		result[i] = -v[i];

	return result;
}


template<typename InType, size_t InDims>
inline VectorN<InType, InDims>
operator+(const VectorN<InType, InDims>& lhs, InType s)
{
	VectorN<InType, InDims> result;

	for (size_t i = 0; i < InDims; i++)
		result[i] = lhs[i] + s;

	return result;
}
template<typename InType, size_t InDims>
inline VectorN<InType, InDims>
operator+(InType s, const VectorN<InType, InDims>& rhs)
{
	VectorN<InType, InDims> result;

	for (size_t i = 0; i < InDims; i++)
		result[i] = s + rhs[i];

	return result;
}

template<typename InType, size_t InDims>
inline VectorN<InType, InDims>
operator-(const VectorN<InType, InDims>& lhs, InType s)
{
	VectorN<InType, InDims> result;

	for (size_t i = 0; i < InDims; i++)
		result[i] = lhs[i] - s;

	return result;
}
template<typename InType, size_t InDims>
inline VectorN<InType, InDims>
operator-(InType s, const VectorN<InType, InDims>& rhs)
{
	VectorN<InType, InDims> result;

	for (size_t i = 0; i < InDims; i++)
		result[i] = s - rhs[i];

	return result;
}

template<typename InType, size_t InDims>
inline VectorN<InType, InDims>
operator*(const VectorN<InType, InDims>& lhs, InType s)
{
	VectorN<InType, InDims> result;

	for (size_t i = 0; i < InDims; i++)
		result[i] = lhs[i] * s;

	return result;
}
template<typename InType, size_t InDims>
inline VectorN<InType, InDims>
operator*(InType s, const VectorN<InType, InDims>& rhs)
{
	VectorN<InType, InDims> result;

	for (size_t i = 0; i < InDims; i++)
		result[i] = s * rhs[i];

	return result;
}

template<typename InType, size_t InDims>
inline VectorN<InType, InDims>
operator/(const VectorN<InType, InDims>& lhs, InType s)
{
	VectorN<InType, InDims> result;

	for (size_t i = 0; i < InDims; i++)
		result[i] = lhs[i] / s;

	return result;
}
template<typename InType, size_t InDims>
inline VectorN<InType, InDims>
operator/(InType s, const VectorN<InType, InDims>& rhs)
{
	VectorN<InType, InDims> result;

	for (size_t i = 0; i < InDims; i++)
		result[i] = s / rhs[i];

	return result;
}


template<typename InType, size_t InDims>
inline VectorN<InType, InDims>
operator+(const VectorN<InType, InDims>& lhs, const VectorN<InType, InDims>& rhs)
{
	VectorN<InType, InDims> result;

	for (size_t i = 0; i < InDims; i++)
		result[i] = lhs[i] + rhs[i];

	return result;
}

template<typename InType, size_t InDims>
inline VectorN<InType, InDims>
operator-(const VectorN<InType, InDims>& lhs, const VectorN<InType, InDims>& rhs)
{
	VectorN<InType, InDims> result;

	for (size_t i = 0; i < InDims; i++)
		result[i] = lhs[i] - rhs[i];

	return result;
}

template<typename InType, size_t InDims>
inline VectorN<InType, InDims>
operator*(const VectorN<InType, InDims>& lhs, const VectorN<InType, InDims>& rhs)
{
	VectorN<InType, InDims> result;

	for (size_t i = 0; i < InDims; i++)
		result[i] = lhs[i] * rhs[i];

	return result;
}

template<typename InType, size_t InDims>
inline VectorN<InType, InDims>
operator/(const VectorN<InType, InDims>& lhs, const VectorN<InType, InDims>& rhs)
{
	VectorN<InType, InDims> result;

	for (size_t i = 0; i < InDims; i++)
		result[i] = lhs[i] / rhs[i];

	return result;
}

#endif