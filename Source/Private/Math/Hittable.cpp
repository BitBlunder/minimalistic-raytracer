#include <Math/Hittable.hpp>

void
hittable_list_add(HittableList& hittable_list, std::shared_ptr<Hittable> hittable)
{
	hittable_list.push_back(hittable);
}

const HitRecord
hittable_list_hit(HittableList& hittable_list, const Ray& ray, float ray_tmin, float ray_tmax)
{
	HitRecord best_hit_record {};

	float closest = ray_tmax;
	for (auto& hittable : hittable_list)
	{
		HitRecord temp_record = hittable->hit(hittable, ray, ray_tmin, closest);
		if (temp_record.hit)
		{
			closest = temp_record.t;
			best_hit_record = temp_record;
		}
	}

	return best_hit_record;
}

void
hittable_list_clear(HittableList& hittable_list)
{
	hittable_list.clear();
}