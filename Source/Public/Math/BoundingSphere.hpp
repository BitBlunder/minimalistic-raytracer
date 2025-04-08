#ifndef BOUNDING_SPHERE_H
#define BOUNDING_SPHERE_H

#include <utility>

#include <Math/Ray.hpp>
#include <Math/Vector.hpp>
#include <Math/Hittable.hpp>
#include <Math/Constants.hpp>
#include <Math/Numerical.hpp>

#include <Graphics/Renderable.hpp>

struct BoundingSphere : public Hittable, Renderable
{
	float radius;
	Vec3f center;
};

void
bound_sphere_create(BoundingSphere& self, float radius);
void
bound_sphere_create(BoundingSphere& self, float radius, Vec3f center);

float
bounding_sphere_intersect(const BoundingSphere& self, const Ray& ray);

#endif