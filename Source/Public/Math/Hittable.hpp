#ifndef HITTABLE_HPP
#define HITTABLE_HPP

#include <vector>
#include <memory>

#include <Math/Ray.hpp>

struct HitRecord
{
	float t;
	float tmin;
	float tmax;

	Vec3f point;
	Vec3f normal;

	bool hit;
	bool front_face;
};

struct Hittable
{
	HitRecord (*hit)(std::shared_ptr<Hittable> hittable, const Ray& ray, double ray_tmin, double ray_tmax);
};

using HittableList = std::vector<std::shared_ptr<Hittable>>;

void
hittable_list_add(HittableList& self, std::shared_ptr<Hittable> hittable);

const HitRecord
hittable_list_hit(HittableList& self, const Ray& ray, float ray_tmin, float ray_tmax);

void
hittable_list_clear(HittableList& self);

#endif