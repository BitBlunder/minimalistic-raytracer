#ifndef WINDOW_H
#define WINDOW_H

#include <cstdint>
#include <functional>

#include <mutex>
#include <atomic>
#include <thread>
#include <condition_variable>

#include <IMGUI/imgui.h>

struct FrameBuffer;
struct ApplicationWindow;

using ApplicationCallback = std::function<void(ApplicationWindow*)>;
using ApplicationResizeCallback = std::function<void(ApplicationWindow*, size_t, size_t)>;

enum class FrameBufferFormat
{
	FMT_NULL = 0,

	FMT_RGB24,
	FMT_RGBA32,
};

struct ApplicationState
{
        bool is_dirty;
        bool is_running;

        std::atomic_uint32_t render_version{0};

        std::mutex mtx;
        std::condition_variable cv;
        std::thread render_worker_thread;
} static g_state;

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

FrameBuffer*
application_window_get_framebuffer(ApplicationWindow* self);

void
application_window_create(ApplicationWindow* self, const char* title, size_t width = 0, size_t height = 0);
void
application_window_destroy(ApplicationWindow* self);

void
application_window_init_imgui(ApplicationWindow* self);
void
application_window_shutdown_imgui(ApplicationWindow* self);

void
application_window_on_create(ApplicationWindow* self, ApplicationCallback callback_fn);
void
application_window_on_update(ApplicationWindow* self, ApplicationCallback callback_fn);
void
application_window_on_render(ApplicationWindow* self, ApplicationCallback callback_fn);
void
application_window_on_ui_render(ApplicationWindow* self, ApplicationCallback callback_fn);

void
application_window_on_resize(ApplicationWindow* self, ApplicationResizeCallback callback_fn);

void
application_window_handle_loop(ApplicationWindow* self);

#endif