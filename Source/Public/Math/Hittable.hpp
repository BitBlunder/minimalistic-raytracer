#ifndef HITTABLE_HPP
#define HITTABLE_HPP

#include <Math/Ray.hpp>

struct HitRecord
{
	bool hit;

	float t;

	Vec3f point;
	Vec3f normal;
};

struct Hittable
{
	const HitRecord (*hit)(Hittable* hittable, const Ray& ray, double ray_tmin, double ray_tmax);
};

#endif