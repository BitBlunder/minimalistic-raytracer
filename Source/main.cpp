#include <cstring>
#include <iostream>

#include <Math/Ray.hpp>
#include <Math/Vector.hpp>
#include <Math/Hittable.hpp>
#include <Math/BoundingSphere.hpp>

#include <Image/Image.hpp>
#include <Image/PPMHandler.hpp>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

Vec3f ray_color(const Ray& ray, HittableList& world)
{
	HitRecord hit_record = hittable_list_hit(world, ray, 0.001f, constants_infinity<float>());

	if (hit_record.hit)
		return 0.5f * (hit_record.normal + Vec3f{ 1.0f, 1.0f, 1.0f });

	// Vec3f unit_direction = unit_vector(ray.direction);
	// auto a = 0.5*(unit_direction.y() + 1.0);
	// return (1.0-a)*color(1.0, 1.0, 1.0) + a*color(0.5, 0.7, 1.0);

	return Vec3f{ 0.0f, 0.0f, 0.0f };
}

int main(int argc, char *argv[])
{
	SDL_Window *window;                    // Declare a pointer
	bool done = false;

	SDL_Init(SDL_INIT_VIDEO);              // Initialize SDL3

	// Create an application window with the following settings:
	window = SDL_CreateWindow(
		"An SDL3 window",                  // window title
		640,                               // width, in pixels
		480,                               // height, in pixels
		SDL_WINDOW_OPENGL                  // flags - see below
	);

	// Check that the window was successfully created
	if (window == NULL) {
		// In the case that the window could not be made...
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create window: %s\n", SDL_GetError());
		return 1;
	}

	while (!done) {
		SDL_Event event;

		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_EVENT_QUIT) {
				done = true;
			}
		}

		// Do game logic, present a frame, etc.
	}

	// Close and destroy the window
	SDL_DestroyWindow(window);

	// Clean up
	SDL_Quit();
	return 0;

	{
		if(!image_register_handler(ppm_handler_new()))
			std::cerr << "Error occured while registering PPM Handler\n";
	}

	HittableList world;

	std::shared_ptr<BoundingSphere> sphere1_ptr((BoundingSphere*)malloc(sizeof(BoundingSphere)));
	std::shared_ptr<BoundingSphere> sphere2_ptr((BoundingSphere*)malloc(sizeof(BoundingSphere)));

	bound_sphere_create(*(sphere1_ptr.get()), 20.0f, {10.0f, 0.0f, -150.0f});
	bound_sphere_create(*(sphere2_ptr.get()), 10.0f, {0.0f, -5.0f, -100.0f});

	hittable_list_add(world, sphere1_ptr);
	hittable_list_add(world, sphere2_ptr);

	// Image
	float aspect_ratio = 16.0f / 9.0f;

	int image_width = 1920;
	int image_height = int(image_width / aspect_ratio);

	image_height = image_height < 1 ? 1 : image_height;

	// Camera
	Vec3f focal_length = {0.0f, 0.0f, 10.0f };

	float viewport_width = 5.0f;
	float viewport_height = viewport_width / (float(image_width) / float(image_height));

	Vec3f camera_center = { 0.0f, 0.0f, 0.0f };

	Vec3f viewport_u = { viewport_width, 0.0f, 0.0f };
	Vec3f viewport_v = { 0.0f, -viewport_height, 0.0f };

	// Pixels
	Vec3f pixel_delta_u = viewport_u / float(image_width);
	Vec3f pixel_delta_v = viewport_v / float(image_height);

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

			Ray ray { camera_center, vector_normalize(direction) };

			float t = static_cast<float>(x) / (image_width - 1);

			auto pixel_color = ray_color(ray, world);

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