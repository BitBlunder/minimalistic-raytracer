#ifndef RAY_HPP
#define RAY_HPP

#include <Math/Vector.hpp>


struct Ray
{
	Vec3f origin;
	Vec3f direction;
};

static Vec3f
ray_point_at(const Ray& ray, float t);

#endif

#include "../../Private/Math/Ray.inl"