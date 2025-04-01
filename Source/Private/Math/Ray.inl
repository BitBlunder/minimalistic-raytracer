#ifndef RAY_INL
#define RAY_INL

#include <Math/Ray.hpp>

static inline Vec3f
ray_point_at(const Ray& ray, float t)
{
	return ray.origin + t * ray.direction;
}

#endif