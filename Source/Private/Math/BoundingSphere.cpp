#include <Math/BoundingSphere.hpp>

const HitRecord
_hit_impl(Hittable* hittable, const Ray& ray, double ray_tmin, double ray_tmax)
{
	HitRecord hit_record {};

	BoundingSphere* sphere = (BoundingSphere*)hittable;
	hit_record.t = bounding_sphere_intersect(*sphere, ray);

	if (hit_record.t > 0.0f)
	{
		hit_record.hit = true;
		hit_record.point = ray_point_at(ray, hit_record.t);
		hit_record.normal = vector_normalize(sphere->center - hit_record.point);
	}

	return hit_record;
}

BoundingSphere*
bound_sphere_new()
{
	BoundingSphere* self = (BoundingSphere*)malloc(sizeof(BoundingSphere));

	self->hit = _hit_impl;

	self->radius = 10.0f;
	self->center = Vec3f{0.0f, 0.0f, 0.0f};
}

float
bounding_sphere_intersect(const BoundingSphere& self, const Ray& ray)
{
	Vec3f ray_to_sphere = self.center - ray.origin;

	auto a = vector_length_squared(ray.direction);
	auto b = vector_dot(ray.direction, ray_to_sphere) * -2.0f;
	auto c = vector_length_squared(ray_to_sphere) - self.radius * self.radius;

	auto discriminant = b*b - 4*a*c;

	if (discriminant < 0) {
		return -1.0;
	} else {
		return (-b - std::sqrt(discriminant) ) / (2.0*a);
	}
}