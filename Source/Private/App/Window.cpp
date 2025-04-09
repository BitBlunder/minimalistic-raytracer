#include <App/Window.h>

#include <cstring>
#include <assert.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>

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
};

FrameBuffer*
framebuffer_new()
{
	FrameBuffer* self = (FrameBuffer*)malloc(sizeof(FrameBuffer));
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
	return self;
}
void
application_window_free(ApplicationWindow* self)
{
	assert(self);

	free(self);
}

void
application_window_create(ApplicationWindow* self, const char* title, size_t width, size_t height)
{
	assert(self);

	self->sdl_window = SDL_CreateWindow(title,
		width, height, SDL_WINDOW_RESIZABLE | SDL_WINDOW_INPUT_FOCUS);
	if (!self->sdl_window)
	{
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return;
	}

	SDL_SetWindowPosition(self->sdl_window,
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

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
}
void
application_window_destroy(ApplicationWindow* self)
{
	assert(self);

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
application_window_update(ApplicationWindow* self)
{
	SDL_UpdateTexture(self->sdl_texture,
		NULL, self->frame_buffer->front_buffer, self->frame_buffer->pitch);
}
void
application_window_render(ApplicationWindow* self)
{
	SDL_RenderClear(self->sdl_renderer);
	SDL_RenderTexture(self->sdl_renderer, self->sdl_texture, NULL, NULL);
	SDL_RenderPresent(self->sdl_renderer);
}

void
application_window_handle_event(ApplicationWindow* self, void* event)
{
	const SDL_Event* sdl_event = (const SDL_Event*)event;

	if (sdl_event->type == SDL_EVENT_WINDOW_RESIZED)
	{
		int new_width = sdl_event->window.data1;
		int new_height = sdl_event->window.data2;

		framebuffer_resize(self->frame_buffer, new_width, new_height);

		if (self->sdl_texture)
			SDL_DestroyTexture(self->sdl_texture);

		self->sdl_texture = SDL_CreateTexture(self->sdl_renderer,
			SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_TARGET, new_width, new_height);
		if (!self->sdl_texture)
		{
			SDL_Log("Failed to recreate texture after resize: %s", SDL_GetError());
			return;
		}
	}
}