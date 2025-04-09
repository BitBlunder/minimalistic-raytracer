#ifndef WINDOW_H
#define WINDOW_H

#include <cstdint>

struct FrameBuffer;
struct ApplicationWindow;

enum class FrameBufferFormat
{
	FMT_NULL = 0,

	FMT_RGB24,
	FMT_RGBA32,
};

FrameBuffer*
framebuffer_new();
void
framebuffer_free(FrameBuffer* self);

void
framebuffer_create(FrameBuffer* self, size_t width, size_t height, FrameBufferFormat format);
void
framebuffer_destroy(FrameBuffer* self);

size_t
framebuffer_get_bps(const FrameBuffer* self);
size_t
framebuffer_get_pitch(const FrameBuffer* self);
size_t
framebuffer_get_width(const FrameBuffer* self);
size_t
framebuffer_get_height(const FrameBuffer* self);

void
framebuffer_swap(FrameBuffer* self);
void
framebuffer_update(FrameBuffer* self, void* buffer);
void
framebuffer_resize(FrameBuffer* self, size_t width, size_t height);


ApplicationWindow*
application_window_new();
void
application_window_free(ApplicationWindow* self);

void
application_window_create(ApplicationWindow* self, const char* title, size_t width, size_t height);
void
application_window_destroy(ApplicationWindow* self);

void
application_window_update(ApplicationWindow* self);
void
application_window_render(ApplicationWindow* self);

void
application_window_handle_event(ApplicationWindow* self, void* event);

#endif