#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <stdint.h>

struct Image;
struct MemoryStream;

using ReadFileCallback = errno_t(*)(Image*);
using WriteFileCallback = errno_t(*)(Image*);

using ReadMemoryStreamCallback = errno_t(*)(Image*, MemoryStream*);
using WriteMemoryStreamFileCallback = errno_t(*)(Image*, MemoryStream*);

struct ImageHandler
{
	struct {
		ReadFileCallback read_file;
		WriteFileCallback write_file;

		ReadMemoryStreamCallback read_memory_stream;
		WriteMemoryStreamFileCallback write_memory_stream;
	} ops;

	const char* exts;
};

struct ImageHandlerNode
{
	ImageHandler* data;
	ImageHandlerNode* next;
};

struct Image
{
	enum Format {
		FMT_NULL = 0,

		FMT_GREY8,
		FMT_RGB24,
		FMT_RGBA32,
		FMT_RGBF48
	} format;

	void* buffer;
	char* filename;

	uint8_t size;
	uint32_t width, height;

	static ImageHandlerNode* s_handler_list;
};

#ifdef __cplusplus
extern "C" {
#endif

errno_t
image_register_handler(ImageHandler* handler);

size_t
image_get_handler_count();

ImageHandler*
image_get_handler_by_index(size_t index);
ImageHandler*
image_get_handler_by_format(const char* exts);

ImageHandler*
image_find_handler_by_filename(const char* filename);


Image*
image_new();
errno_t
image_free(Image* image);

errno_t
image_load_file(Image* image);
errno_t
image_save_file(Image* image);

errno_t
image_load_memory_stream(Image* image, MemoryStream* stream);
errno_t
image_save_memory_stream(Image* image, MemoryStream* stream);

errno_t
image_get_pixel_size(Image* image);

errno_t
image_set_buffer(Image* image, void* buffer);

errno_t
image_set_pixel(Image* image, uint32_t x, uint32_t y, void* pixel);
errno_t
image_get_pixel(Image* image, uint32_t x, uint32_t y, void* pixel);

errno_t
image_set_pixel1i(Image* image, uint32_t x, uint32_t y, int pixel);
errno_t
image_get_pixel1i(Image* image, uint32_t x, uint32_t y, int* pixel);

errno_t
image_set_pixel3i(Image* image, uint32_t x, uint32_t y, int r, int g, int b);
errno_t
image_get_pixel3i(Image* image, uint32_t x, uint32_t y, int* r, int* g, int* b);

errno_t
image_set_pixel4i(Image* image, uint32_t x, uint32_t y, int r, int g, int b, int a);
errno_t
image_get_pixel4i(Image* image, uint32_t x, uint32_t y, int* r, int* g, int* b, int* a);


#ifdef __cplusplus
}
#endif

#endif