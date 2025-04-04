#include <cstring>
#include <iostream>

#include <Math/Ray.hpp>
#include <Math/Vector.hpp>

#include <Image/Image.hpp>
#include <Image/PPMHandler.hpp>

float hit_sphere(const Vec3f& center, double radius, const Ray& r)
{
	Vec3f oc = center - r.origin;
	auto a = vector_length_squared(r.direction);
	auto b = -2.0 * vector_dot(r.direction, oc);
	auto c = vector_length_squared(oc) - radius * radius;
	auto discriminant = b*b - 4*a*c;

	if (discriminant < 0) {
		return -1.0;
	} else {
		return (-b - std::sqrt(discriminant) ) / (2.0*a);
	}
}

Vec3f render(const Ray& r)
{
	auto t = hit_sphere({ 0.0f, 0.0f, -1.0f }, 0.5, r);

	if (t > 0.0)
	{
		Vec3f v1 = { 0.0f, 0.0f, -1.0f };
		Vec3f v2 = ray_point_at(r, t);
		Vec3f v3 = v2 - v1;

		Vec3f N = vector_normalize(v3);

		return 0.5f * Vec3f{ N.x + 1, N.y + 1, N.z + 1 };
	}

	return {0.0f, 0.0f, 0.0f};
}

int main(int argc, char** args)
{
	{
		if(!image_register_handler(ppm_handler_new()))
			std::cerr << "Error occured while registering PPM Handler\n";
	}

	float aspect_ratio = 16.0f / 9.0f;

	int image_width = 1920;
	int image_height = int(image_width / aspect_ratio);

	image_height = image_height < 1 ? 1 : image_height;

	float viewport_width = 5.0;
	float viewport_height = viewport_width / (double(image_width) / image_height);

	Vec3f camera_center = {0.0, 0.0, 0.0};

	Vec3f viewport_u = { viewport_width, 0, 0 };
	Vec3f viewport_v = { 0, -viewport_height, 0 };

	Vec3f pixel_delta_u = viewport_u / float(image_width);
	Vec3f pixel_delta_v = viewport_v / float(image_height);

	Vec3f focal_length = { 0, 0, 1.0f };

	Vec3f viewport_upper_left = camera_center - focal_length - (viewport_u / 2.0f) - (viewport_v / 2.0f);
	Vec3f pixel00_loc = viewport_upper_left + 0.5f * (pixel_delta_u + pixel_delta_v);

	uint8_t* buffer = (uint8_t*)malloc(image_width * image_height * 3);

	for (int y = 0; y < image_height; y++)
	{
		std::clog << "\rScanlines remaining: " << (image_height - y) << ' ' << std::flush;

		for (int x = 0; x < image_width; x++)
		{
			Vec3f pixel_center = pixel00_loc + (float(x) * pixel_delta_u) + (float(y) * pixel_delta_v);

			Vec3f direction = pixel_center - camera_center;

			Ray r { camera_center, direction };

			float t = static_cast<float>(x) / (image_width - 1);

			auto pixel_color = render(r);

			int index = (x + (y * image_width)) * 3;

			buffer[index + 0] = (uint8_t)(pixel_color.r * 255.0f);
			buffer[index + 1] = (uint8_t)(pixel_color.g * 255.0f);
			buffer[index + 2] = (uint8_t)(pixel_color.b * 255.0f);
		}
	}

	{
		Image* img = image_new();

		img->format = Image::Format::FMT_RGB24;
		img->filename = "../Assets/sample.ppm";
		img->width = image_width; img->height = image_height;

		if (!image_set_buffer(img, buffer))
			std::cerr << "Error occured while setting pixels to PPM file from memory\n";

		if (!image_save_file(img))
			std::cerr << "Error occured while saving PPM file to disk\n";
	}

	return 0;
}