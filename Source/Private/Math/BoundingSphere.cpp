#include <Math/BoundingSphere.hpp>

const HitRecord
_hit_impl(Hittable* hittable, const Ray& ray, double ray_tmin, double ray_tmax)
{
	HitRecord hit_record {};

	BoundingSphere* sphere = (BoundingSphere*)hittable;
	hit_record.t = bounding_sphere_intersect(*sphere, ray);

	if (hit_record.t > 0.0f && hit_record.t < FLT_MAX)
	{
		hit_record.hit = true;
		hit_record.point = ray_point_at(ray, hit_record.t);
		hit_record.normal = vector_normalize(hit_record.point - sphere->center);
	}

	return hit_record;
}

void
bound_sphere_create(BoundingSphere& self, float radius)
{
	self.hit = _hit_impl;

	self.radius = radius;
	self.center = { 0.0f, 0.0f, 0.0f };
}
void
bound_sphere_create(BoundingSphere& self, float radius, Vec3f center)
{
	self.hit = _hit_impl;

	self.radius = radius;
	self.center = center;
}

float
bounding_sphere_intersect(const BoundingSphere& self, const Ray& ray)
{
	Vec3f ray_to_sphere = self.center - ray.origin;

	auto a = vector_length_squared(ray.direction);
	auto b = vector_dot(ray.direction, ray_to_sphere) * -2.0f;
	auto c = vector_length_squared(ray_to_sphere) - self.radius * self.radius;

	auto solution = find_quadratic_root(a, b, c);

	if (solution.roots == 0)
		return FLT_MAX;

	if (solution.solution1 > solution.solution2)
		std::swap(solution.solution1, solution.solution2);

	if (solution.solution1 >= 0.0f)
		return solution.solution1;

	if (solution.solution2 >= 0.0f)
		return 0.0f;

	return FLT_MAX;
}