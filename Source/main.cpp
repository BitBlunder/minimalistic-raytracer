#include <cstring>
#include <iostream>

#include <SDL3/SDL_main.h>

#include <App/Window.h>

#include <SDL3/SDL_events.h>

#include <Math/Ray.hpp>
#include <Math/Vector.hpp>
#include <Math/Hittable.hpp>
#include <Math/BoundingSphere.hpp>

struct RenderContext
{
	void* temp_buffer;
	FrameBuffer* framebuffer;

	Vec3f camera_center = { 0.0f, 0.0f, 0.0f };
	Vec3f camera_focal_length = { 0.0f, 0.0f, 10.0f };

	float viewport_width = 5.0f;
	float viewport_height;

	Vec3f pixel_delta_u;
	Vec3f pixel_delta_v;
	Vec3f viewport_upper_left;
};

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
	ApplicationWindow* window = application_window_new();

	HittableList world;
	RenderContext context;

	application_window_on_create(window, [&world, &context](ApplicationWindow* self) -> void {
		context.framebuffer = application_window_get_framebuffer(self);

		size_t framebuffer_bps = framebuffer_get_bps(context.framebuffer);
		size_t framebuffer_width = framebuffer_get_width(context.framebuffer);
		size_t framebuffer_height = framebuffer_get_height(context.framebuffer);

		context.temp_buffer = malloc(framebuffer_width * framebuffer_height * framebuffer_bps);

		std::shared_ptr<BoundingSphere> sphere1_ptr((BoundingSphere*)malloc(sizeof(BoundingSphere)));
		std::shared_ptr<BoundingSphere> sphere2_ptr((BoundingSphere*)malloc(sizeof(BoundingSphere)));

		bound_sphere_create(*(sphere1_ptr.get()), 20.0f, {10.0f, 0.0f, -150.0f});
		bound_sphere_create(*(sphere2_ptr.get()), 10.0f, {0.0f, -5.0f, -100.0f});

		hittable_list_add(world, sphere1_ptr);
		hittable_list_add(world, sphere2_ptr);

		g_state.is_dirty = true;
	});

	application_window_on_update(window, [&world, &context](ApplicationWindow* self) -> void {
		size_t framebuffer_width = framebuffer_get_width(context.framebuffer);
		size_t framebuffer_height = framebuffer_get_height(context.framebuffer);

		context.viewport_height = context.viewport_width / (float(framebuffer_width) / float(framebuffer_height));

		Vec3f viewport_u = { context.viewport_width, 0.0f, 0.0f };
		Vec3f viewport_v = { 0.0f, -context.viewport_height, 0.0f };

		context.pixel_delta_u = viewport_u / float(framebuffer_width);
		context.pixel_delta_v = viewport_v / float(framebuffer_height);

		context.viewport_upper_left = context.camera_center - context.camera_focal_length - (viewport_u / 2.0f) - (viewport_v / 2.0f);
	});

        application_window_on_render(window, [&world, &context](ApplicationWindow* self) -> void {
                size_t framebuffer_width = framebuffer_get_width(context.framebuffer);
                size_t framebuffer_height = framebuffer_get_height(context.framebuffer);

                uint32_t start_ver = g_state.render_version.load(std::memory_order_relaxed);

                for (int y = 0; y < framebuffer_height; y++)
                {
                        for (int x = 0; x < framebuffer_width; x++)
                        {
                                if (start_ver != g_state.render_version.load(std::memory_order_relaxed))
                                {
                                        std::lock_guard<std::mutex> lock(g_state.mtx);
                                        g_state.is_dirty = true;
                                        return;
                                }
				Vec3f pixel_center = context.viewport_upper_left +
					(float(x) * context.pixel_delta_u) +
					(float(y) * context.pixel_delta_v);

				Vec3f direction = pixel_center - context.camera_center;

				Ray ray { context.camera_center, vector_normalize(direction) };

				auto pixel_color = ray_color(ray, world);

				int index = (x + (y * framebuffer_width)) * 3;

				auto buffer = (uint8_t*)context.temp_buffer;
				buffer[index + 0] = (uint8_t)(pixel_color.r * 255.0f);
				buffer[index + 1] = (uint8_t)(pixel_color.g * 255.0f);
				buffer[index + 2] = (uint8_t)(pixel_color.b * 255.0f);
			}
		}

		framebuffer_update(context.framebuffer, context.temp_buffer);
	});

	application_window_on_ui_render(window, [&context](ApplicationWindow* self) -> void {
		ImGui::Begin("Camera/Viewport Settings");

			// Camera Focal Point
			{
				ImGui::Text("Camera Focal Point");

				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
				if (ImGui::SliderFloat("##FocalPoint", &context.camera_focal_length.z, 0.1f, 100.0f, "%.3f"))
				{
                                        {
                                                std::lock_guard<std::mutex> lock(g_state.mtx);
                                                g_state.is_dirty = true;
                                                g_state.render_version.fetch_add(1, std::memory_order_relaxed);
                                        }

                                        g_state.cv.notify_one();
				}
			}

			ImGui::Separator();
			ImGui::Spacing();

			// Camera Center
			{
				ImGui::Text("Camera Center");

				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
				if (ImGui::SliderFloat3("##CameraCenter", context.camera_center.data, -1000.0f, 1000.0f, "%.3f"))
				{
                                        {
                                                std::lock_guard<std::mutex> lock(g_state.mtx);
                                                g_state.is_dirty = true;
                                                g_state.render_version.fetch_add(1, std::memory_order_relaxed);
                                        }

                                        g_state.cv.notify_one();
				}
			}

			ImGui::Separator();
			ImGui::Spacing();

			// Viewport Dimensions
			{
				ImGui::Text("Viewport Dimensions");

				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x / 2.0f);
				if (ImGui::SliderFloat("##ViewportWidth", &context.viewport_width, 0.0, 1920.0, "%.3f"))
				{
                                        {
                                                std::lock_guard<std::mutex> lock(g_state.mtx);
                                                g_state.is_dirty = true;
                                                g_state.render_version.fetch_add(1, std::memory_order_relaxed);
                                        }

                                        g_state.cv.notify_one();
				}

				ImGui::SameLine();

				ImGui::BeginDisabled();
					ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
					ImGui::InputFloat("##ViewportHeight", &context.viewport_height, 0.0, 0.0, "%.3f");
				ImGui::EndDisabled();
			}

			ImGui::Separator();
			ImGui::Spacing();

			// Pixel Delta
			{
				ImGui::Text("Pixel Delta");

				ImGui::BeginDisabled(true); // Disable editing

					ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
					ImGui::InputFloat3("##PixelDeltaU", context.pixel_delta_u.data, "%.3f");

					ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
					ImGui::InputFloat3("##PixelDeltaV", context.pixel_delta_v.data, "%.3f");
				ImGui::EndDisabled();
			}

		ImGui::End();
	});

	application_window_on_resize(window, [&context](ApplicationWindow* self, size_t width, size_t height) -> void {
		free(context.temp_buffer);

		context.framebuffer = application_window_get_framebuffer(self);

		size_t framebuffer_bps = framebuffer_get_bps(context.framebuffer);
		size_t framebuffer_width = framebuffer_get_width(context.framebuffer);
		size_t framebuffer_height = framebuffer_get_height(context.framebuffer);

		context.temp_buffer = malloc(framebuffer_width * framebuffer_height * framebuffer_bps);
	});

	application_window_create(window, "minimalistic-raytracer");
	application_window_init_imgui(window);

	application_window_handle_loop(window);

	application_window_shutdown_imgui(window);
	application_window_destroy(window);

	return 0;
}