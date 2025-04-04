#ifndef BOUNDING_SPHERE_H
#define BOUNDING_SPHERE_H

#include <Math/Ray.hpp>
#include <Math/Vector.hpp>
#include <Math/Hittable.hpp>

#include <Graphics/Renderable.hpp>

struct BoundingSphere : public Hittable, Renderable
{
	float radius;
	Vec3f center;
};

BoundingSphere*
bound_sphere_new();

float
bounding_sphere_intersect(const BoundingSphere& self, const Ray& ray);

#endif