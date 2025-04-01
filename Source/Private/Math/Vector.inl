#ifndef VECTOR_INL
#define VECTOR_INL

#include <Math/Vector.hpp>

template<typename InType, size_t InDims, typename Operation>
inline VectorN<InType, InDims>
_unary_op(const VectorN<InType, InDims>& v, Operation op)
{
	VectorN<InType, InDims> result;

	for (size_t i = 0; i < InDims; i++)
		result[i] = op(v[i]);

	return result;
}

template<typename InType, size_t InDims, typename Operation>
inline VectorN<InType, InDims>
_binary_op(const VectorN<InType, InDims>& lhs, const InType& s, Operation op)
{
	VectorN<InType, InDims> result;

	for (size_t i = 0; i < InDims; i++)
		result[i] = op(lhs[i], s);

	return result;
}

template<typename InType, size_t InDims, typename Operation>
inline VectorN<InType, InDims>
_binary_op(const VectorN<InType, InDims>& lhs, const VectorN<InType, InDims>& rhs, Operation op)
{
	VectorN<InType, InDims> result;

	for (size_t i = 0; i < InDims; i++)
		result[i] = op(lhs[i], rhs[i]);

	return result;
}


template<typename InType, size_t InDims>
static inline void
vector_normalize(VectorN<InType, InDims>& v)
{
	v *= InType(1) / vector_length(v);
}

template<typename InType, size_t InDims>
static inline InType
vector_length(const VectorN<InType, InDims>& v)
{
	return sqrt(vector_length_squared(v));
}

template<typename InType, size_t InDims>
static inline InType
vector_distance(const VectorN<InType, InDims>& lhs, const VectorN<InType, InDims>& rhs)
{
	return vector_length(lhs - rhs);
}

template<typename InType, size_t InDims>
static inline InType
vector_length_squared(const VectorN<InType, InDims>& v)
{
	return vector_dot(v, v);
}

template<typename InType, size_t InDims>
static inline InType
vector_distance_squared(const VectorN<InType, InDims>& lhs, const VectorN<InType, InDims>& rhs)
{
	return vector_length_squared(lhs - rhs);
}

template<typename InType, size_t InDims>
static inline InType
vector_dot(const VectorN<InType, InDims>& lhs, const VectorN<InType, InDims>& rhs)
{
	InType result = 0;
	for (size_t i = 0; i < InDims; i++)
		result += lhs[i] * rhs[i];

	return result;
}

template<typename InType>
static inline VectorN<InType, 3>
vector_cross(const VectorN<InType, 3>& lhs, const VectorN<InType, 3>& rhs)
{
	return {lhs[1] * rhs[2] - lhs[2] * rhs[1],
			lhs[2] * rhs[0] - lhs[0] * rhs[2],
			lhs[0] * rhs[1] - lhs[1] * rhs[0]};
}


template<typename InType, size_t InDims>
inline VectorN<InType, InDims>
operator-(const VectorN<InType, InDims>& v)
{
	return _unary_op(v, [](const InType& x) { return -x; });
}


template<typename InType, size_t InDims>
inline bool
operator==(const VectorN<InType, InDims>& lhs, const VectorN<InType, InDims>& rhs)
{
	for (size_t i = 0; i < InDims; i++)
		if (lhs[i] != rhs[i])
			return false;

	return true;
}

template<typename InType, size_t InDims>
inline bool
operator!=(const VectorN<InType, InDims>& lhs, const VectorN<InType, InDims>& rhs)
{
	for (size_t i = 0; i < InDims; i++)
		if (lhs[i] != rhs[i])
			return true;

	return false;
}


template<typename InType, size_t InDims>
inline VectorN<InType, InDims>
operator+=(VectorN<InType, InDims>& lhs, const InType& s)
{
	lhs = _binary_op(lhs, s, [](const InType& a, const InType& b) {
		return a + b;
	});

	return lhs;
}

template<typename InType, size_t InDims>
inline VectorN<InType, InDims>
operator-=(VectorN<InType, InDims>& lhs, const InType& s)
{
	lhs = _binary_op(lhs, s, [](const InType& a, const InType& b) {
		return a - b;
	});

	return lhs;
}

template<typename InType, size_t InDims>
inline VectorN<InType, InDims>
operator*=(VectorN<InType, InDims>& lhs, const InType& s)
{
	lhs = _binary_op(lhs, s, [](const InType& a, const InType& b) {
		return a * b;
	});

	return lhs;
}

template<typename InType, size_t InDims>
inline VectorN<InType, InDims>
operator/=(VectorN<InType, InDims>& lhs, const InType& s)
{
	lhs = _binary_op(lhs, s, [](const InType& a, const InType& b) {
		return a / b;
	});

	return lhs;
}


template<typename InType, size_t InDims>
inline VectorN<InType, InDims>
operator+(const VectorN<InType, InDims>& lhs, InType s)
{
	return _binary_op(lhs, s, [](const InType& a, const InType& b) {
		return a + b;
	});
}
template<typename InType, size_t InDims>
inline VectorN<InType, InDims>
operator+(InType s, const VectorN<InType, InDims>& rhs)
{
	return _binary_op(rhs, s, [](const InType& a, const InType& b) {
		return a + b;
	});
}

template<typename InType, size_t InDims>
inline VectorN<InType, InDims>
operator-(const VectorN<InType, InDims>& lhs, InType s)
{
	return _binary_op(lhs, s, [](const InType& a, const InType& b) {
		return a - b;
	});
}
template<typename InType, size_t InDims>
inline VectorN<InType, InDims>
operator-(InType s, const VectorN<InType, InDims>& rhs)
{
	return _binary_op(rhs, s, [](const InType& a, const InType& b) {
		return a - b;
	});
}

template<typename InType, size_t InDims>
inline VectorN<InType, InDims>
operator*(const VectorN<InType, InDims>& lhs, InType s)
{
	return _binary_op(lhs, s, [](const InType& a, const InType& b) {
		return a * b;
	});
}
template<typename InType, size_t InDims>
inline VectorN<InType, InDims>
operator*(InType s, const VectorN<InType, InDims>& rhs)
{
	return _binary_op(rhs, s, [](const InType& a, const InType& b) {
		return a * b;
	});
}

template<typename InType, size_t InDims>
inline VectorN<InType, InDims>
operator/(const VectorN<InType, InDims>& lhs, InType s)
{
	return _binary_op(lhs, s, [](const InType& a, const InType& b) {
		return a / b;
	});
}
template<typename InType, size_t InDims>
inline VectorN<InType, InDims>
operator/(InType s, const VectorN<InType, InDims>& rhs)
{
	return _binary_op(rhs, s, [](const InType& a, const InType& b) {
		return a / b;
	});
}


template<typename InType, size_t InDims>
inline VectorN<InType, InDims>
operator+=(VectorN<InType, InDims>& lhs, const VectorN<InType, InDims>& rhs)
{
	lhs = _binary_op(lhs, rhs, [](const InType& a, const InType& b) {
		return a + b;
	});

	return lhs;
}

template<typename InType, size_t InDims>
inline VectorN<InType, InDims>
operator-=(VectorN<InType, InDims>& lhs, const VectorN<InType, InDims>& rhs)
{
	lhs = _binary_op(lhs, rhs, [](const InType& a, const InType& b) {
		return a - b;
	});

	return lhs;
}

template<typename InType, size_t InDims>
inline VectorN<InType, InDims>
operator*=(VectorN<InType, InDims>& lhs, const VectorN<InType, InDims>& rhs)
{
	lhs = _binary_op(lhs, rhs, [](const InType& a, const InType& b) {
		return a * b;
	});

	return lhs;
}

template<typename InType, size_t InDims>
inline VectorN<InType, InDims>
operator/=(VectorN<InType, InDims>& lhs, const VectorN<InType, InDims>& rhs)
{
	lhs = _binary_op(lhs, rhs, [](const InType& a, const InType& b) {
		return a / b;
	});

	return lhs;
}


template<typename InType, size_t InDims>
inline VectorN<InType, InDims>
operator+(const VectorN<InType, InDims>& lhs, const VectorN<InType, InDims>& rhs)
{
	return _binary_op(lhs, rhs, [](const InType& a, const InType& b) {
		return a + b;
	});
}

template<typename InType, size_t InDims>
inline VectorN<InType, InDims>
operator-(const VectorN<InType, InDims>& lhs, const VectorN<InType, InDims>& rhs)
{
	return _binary_op(lhs, rhs, [](const InType& a, const InType& b) {
		return a - b;
	});
}

template<typename InType, size_t InDims>
inline VectorN<InType, InDims>
operator*(const VectorN<InType, InDims>& lhs, const VectorN<InType, InDims>& rhs)
{
	return _binary_op(lhs, rhs, [](const InType& a, const InType& b) {
		return a * b;
	});
}

template<typename InType, size_t InDims>
inline VectorN<InType, InDims>
operator/(const VectorN<InType, InDims>& lhs, const VectorN<InType, InDims>& rhs)
{
	return _binary_op(lhs, rhs, [](const InType& a, const InType& b) {
		return a / b;
	});
}


template<typename InType, size_t InDims>
std::ostream& operator<<(std::ostream& os, const VectorN<InType, InDims>& v)
{
	os << "(";
	for (size_t i = 0; i < InDims; ++i)
	{
		os << v[i];
		if (i != InDims - 1)
			os << ", ";
	}
	os << ")";

	return os;
}

#endif