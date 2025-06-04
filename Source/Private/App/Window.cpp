#include <App/Window.h>

#include <cstring>
#include <assert.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>

#include <IMGUI/backends/imgui_impl_sdl3.h>
#include <IMGUI/backends/imgui_impl_sdlrenderer3.h>

struct FrameBuffer
{
	size_t pitch;
	size_t width, height;

	FrameBufferFormat format;

	void *front_buffer, *back_buffer;
};

struct ApplicationWindow
{
	SDL_Window* sdl_window;
	SDL_Texture* sdl_texture;
	SDL_Renderer* sdl_renderer;

	FrameBuffer* frame_buffer;

	ApplicationCallback on_create_fn;
	ApplicationCallback on_update_fn;
	ApplicationCallback on_render_fn;
	ApplicationCallback on_ui_render_fn;

	ApplicationResizeCallback on_resize_fn;
};

void
_render_worker_thread(ApplicationWindow* self)
{
	while (true)
	{
		std::unique_lock<std::mutex> lock(g_state.mtx);

		g_state.cv.wait(lock, []() {
			return g_state.is_dirty || !g_state.is_running;
		});

                if (!g_state.is_running)
                        break;

                g_state.is_dirty = false;
                g_state.render_version.load(std::memory_order_relaxed);
                lock.unlock();

		if (self->on_render_fn)
			self->on_render_fn(self);

		lock.lock();
		framebuffer_swap(self->frame_buffer);
		lock.unlock();
	}
}

FrameBuffer*
framebuffer_new()
{
	FrameBuffer* self = (FrameBuffer*)malloc(sizeof(FrameBuffer));
	memset(self, 0, sizeof(FrameBuffer));
	return self;
}
void
framebuffer_free(FrameBuffer* self)
{
	assert(self);

	free(self);
}

void
framebuffer_create(FrameBuffer* self, size_t width, size_t height, FrameBufferFormat format)
{
	assert(self);

	self->width = width;
	self->height = height;
	self->format = format;

	self->pitch = self->width * framebuffer_get_bps(self);

	size_t buffer_size = self->width * self->height * framebuffer_get_bps(self);

	self->front_buffer = malloc(buffer_size);
	memset(self->front_buffer, 0, buffer_size);

	self->back_buffer = malloc(buffer_size);
	memset(self->back_buffer, 0, buffer_size);
}
void
framebuffer_destroy(FrameBuffer* self)
{
	assert(self);

	free(self->front_buffer);
	free(self->back_buffer);

	self->front_buffer = nullptr;
	self->back_buffer = nullptr;
}

size_t
framebuffer_get_bps(const FrameBuffer* self)
{
	assert(self);

	switch(self->format)
	{
		case FrameBufferFormat::FMT_RGB24:  return 3;
		case FrameBufferFormat::FMT_RGBA32: return 4;
		default: return 0;
	}
}
size_t
framebuffer_get_pitch(const FrameBuffer* self)
{
	assert(self);

	return self->pitch;
}
size_t
framebuffer_get_width(const FrameBuffer* self)
{
	assert(self);

	return self->width;
}
size_t
framebuffer_get_height(const FrameBuffer* self)
{
	assert(self);

	return self->height;
}

void
framebuffer_swap(FrameBuffer* self)
{
	assert(self);

	void* temp = self->front_buffer;
	self->front_buffer = self->back_buffer;
	self->back_buffer = temp;
}
void
framebuffer_update(FrameBuffer* self, void* buffer)
{
	assert(self);

	memcpy(self->back_buffer, buffer, self->pitch * self->height);
}
void
framebuffer_resize(FrameBuffer* self, size_t width, size_t height)
{
	framebuffer_destroy(self);

	self->width = width;
	self->height = height;

	self->pitch = self->width * framebuffer_get_bps(self);

	size_t buffer_size = self->width * self->height * framebuffer_get_bps(self);

	self->front_buffer = malloc(buffer_size);
	memset(self->front_buffer, 0, buffer_size);

	self->back_buffer = malloc(buffer_size);
	memset(self->back_buffer, 0, buffer_size);
}


ApplicationWindow*
application_window_new()
{
	ApplicationWindow* self = (ApplicationWindow*)malloc(sizeof(ApplicationWindow));
	memset(self, 0, sizeof(ApplicationWindow));
	return self;
}
void
application_window_free(ApplicationWindow* self)
{
	assert(self);

	free(self);
}

FrameBuffer*
application_window_get_framebuffer(ApplicationWindow* self)
{
	return self->frame_buffer;
}

void
application_window_create(ApplicationWindow* self, const char* title, size_t width, size_t height)
{
	assert(self);

	if (!SDL_Init(SDL_INIT_VIDEO)) {
		SDL_Log("Failed to init SDL3: %s", SDL_GetError());
		return;
	}

	if (!width || !height)
	{
		self->sdl_window = SDL_CreateWindow(title,
			width, height, SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED);
		if (!self->sdl_window) {
			SDL_Log("Failed to create window: %s", SDL_GetError());
			return;
		}

		int temp_width, temp_height;
		if (!SDL_GetWindowSize(self->sdl_window, &temp_width, &temp_height)) {
			SDL_Log("Failed to get window size: %s", SDL_GetError());
			return;
		}

		width = temp_width;
		height = temp_height;
	}
	else
	{
		self->sdl_window = SDL_CreateWindow(title,
			width, height, SDL_WINDOW_RESIZABLE | SDL_WINDOW_INPUT_FOCUS);
		if (!self->sdl_window) {
			SDL_Log("Failed to create window: %s", SDL_GetError());
			return;
		}

		SDL_SetWindowPosition(self->sdl_window,
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
	}

	self->sdl_renderer = SDL_CreateRenderer(self->sdl_window, "software");
	if (!self->sdl_renderer)
	{
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		return;
	}

	self->sdl_texture = SDL_CreateTexture(self->sdl_renderer,
		SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_TARGET, width, height);
	if (!self->sdl_texture)
	{
		SDL_Log("Failed to create texture: %s", SDL_GetError());
		return;
	}

	self->frame_buffer = framebuffer_new();
	framebuffer_create(self->frame_buffer, width, height, FrameBufferFormat::FMT_RGB24);

	g_state.is_running = true;

	if (self->on_create_fn)
		self->on_create_fn(self);

	g_state.render_worker_thread = std::thread(_render_worker_thread, std::ref(self));
}
void
application_window_destroy(ApplicationWindow* self)
{
	assert(self);

	if (g_state.render_worker_thread.joinable())
		g_state.render_worker_thread.join();

	if (self->sdl_texture)
		SDL_DestroyTexture(self->sdl_texture);
	if (self->sdl_renderer)
		SDL_DestroyRenderer(self->sdl_renderer);
	if (self->sdl_window)
		SDL_DestroyWindow(self->sdl_window);

	framebuffer_destroy(self->frame_buffer);

	SDL_Quit();
}

void
application_window_init_imgui(ApplicationWindow* self)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui::StyleColorsDark();

	if (!ImGui_ImplSDL3_InitForSDLRenderer(self->sdl_window, self->sdl_renderer)) {
		SDL_Log("Failed to initialize ImGui SDL2 binding");
		return;
	}
	if (!ImGui_ImplSDLRenderer3_Init(self->sdl_renderer)) {
		SDL_Log("Failed to initialize ImGui SDLRenderer binding");
		return;
	}
}
void
application_window_shutdown_imgui(ApplicationWindow* self)
{
	ImGui_ImplSDLRenderer3_Shutdown();
	ImGui_ImplSDL3_Shutdown();
	ImGui::DestroyContext();
}

void
application_window_on_create(ApplicationWindow* self, ApplicationCallback callback_fn)
{
	self->on_create_fn = callback_fn;
}
void
application_window_on_update(ApplicationWindow* self, ApplicationCallback callback_fn)
{
	self->on_update_fn = callback_fn;
}
void
application_window_on_render(ApplicationWindow* self, ApplicationCallback callback_fn)
{
	self->on_render_fn = callback_fn;
}
void
application_window_on_ui_render(ApplicationWindow* self, ApplicationCallback callback_fn)
{
	self->on_ui_render_fn = callback_fn;
}

void
application_window_on_resize(ApplicationWindow* self, ApplicationResizeCallback callback_fn)
{
	self->on_resize_fn = callback_fn;
}

void
application_window_handle_loop(ApplicationWindow* self)
{
	while (g_state.is_running)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			ImGui_ImplSDL3_ProcessEvent(&event);

			if (event.type == SDL_EVENT_QUIT) {

				{
					std::lock_guard<std::mutex> lock(g_state.mtx);
					g_state.is_running = false;
				}

				g_state.cv.notify_one();
				break;
			}
			if (event.type == SDL_EVENT_WINDOW_RESIZED) {
				int new_width = event.window.data1;
				int new_height = event.window.data2;

				{
					std::lock_guard<std::mutex> lock(g_state.mtx);
					framebuffer_resize(self->frame_buffer, new_width, new_height);
				}

				if (self->sdl_texture)
					SDL_DestroyTexture(self->sdl_texture);

				self->sdl_texture = SDL_CreateTexture(self->sdl_renderer,
					SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_TARGET, new_width, new_height);
				if (!self->sdl_texture)
				{
					SDL_Log("Failed to recreate texture after resize: %s", SDL_GetError());
					return;
				}

				if (self->on_resize_fn)
					self->on_resize_fn(self, new_width, new_height);
			}
		}

		{
			if (self->on_update_fn)
				self->on_update_fn(self);
		}

		{
			SDL_RenderClear(self->sdl_renderer);

			{
				std::lock_guard<std::mutex> lock(g_state.mtx);

				SDL_UpdateTexture(self->sdl_texture,
					NULL, self->frame_buffer->front_buffer, self->frame_buffer->pitch);
				SDL_RenderTexture(self->sdl_renderer,
					self->sdl_texture, NULL, NULL);
			}

			{
				ImGui_ImplSDLRenderer3_NewFrame();
				ImGui_ImplSDL3_NewFrame();
				ImGui::NewFrame();

				if (self->on_ui_render_fn)
					self->on_ui_render_fn(self);

				ImGui::Render();
				ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), self->sdl_renderer);
			}

			SDL_RenderPresent(self->sdl_renderer);
		}
	}
}