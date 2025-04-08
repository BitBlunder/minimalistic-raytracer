#ifndef WINDOW_H
#define WINDOW_H

#include <cstdint>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

struct FrameBuffer
{
	enum Format
	{
		FMT_NULL = 0,

		FMT_RGB24,
		FMT_RGBA32,
	} format;

	size_t pitch;
	size_t width, height;

	void *front_buffer, *back_buffer;
};

errno_t
framebuffer_create(FrameBuffer& self, size_t width, size_t height, FrameBuffer::Format format);

errno_t
framebuffer_destroy(FrameBuffer& self);

errno_t
framebuffer_swap(FrameBuffer& self);


struct AppWindow
{
	SDL_Window* sdl_window;
};

#endif