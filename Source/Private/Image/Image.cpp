#include "Image/Image.hpp"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

ImageHandlerNode* Image::s_handler_list = nullptr;

errno_t
image_register_handler(ImageHandler* handler)
{
	assert(handler);

	if (!Image::s_handler_list)
	{
		Image::s_handler_list = (ImageHandlerNode*)malloc(sizeof(ImageHandlerNode));
		if (!Image::s_handler_list)
			return -1;

		Image::s_handler_list->data = handler;
		Image::s_handler_list->next = nullptr;

		return 1;
	}

	ImageHandlerNode* node = (ImageHandlerNode*)malloc(sizeof(ImageHandlerNode));
	if (node)
		return -1;

	node->data = handler;
	node->next = Image::s_handler_list;

	Image::s_handler_list = node;

	return 1;
}

size_t
image_get_handler_count()
{
	size_t counter = 0;

	auto node = Image::s_handler_list;
	for (; node; node = node->next)
		++counter;

	return counter;
}

ImageHandler*
image_get_handler_by_index(size_t index)
{
	size_t counter = 0;

	auto node = Image::s_handler_list;
	for (; node || index != counter; node = node->next)
		++counter;

	return node->data;
}
ImageHandler*
image_get_handler_by_format(const char* exts)
{
	auto node = Image::s_handler_list;
	for (; node || strcmp(node->data->exts, exts) != 0; node = node->next);

	return node->data;
}

ImageHandler*
image_find_handler_by_filename(const char* filename)
{
	const char* suffix_result = strrchr(filename, '.');
	if (!suffix_result)
		return nullptr;

	const size_t suffix_result_len = strlen(suffix_result);

	auto node = Image::s_handler_list;
	for (; node; node = node->next)
	{
		char* start_ptr;
		char* end_ptr;

		const char* exts = node->data->exts;
		while(exts)
		{
			start_ptr = strstr(exts, suffix_result + 1);
			if (!start_ptr)
				break;

			end_ptr = start_ptr + suffix_result_len - 1;

			if (*end_ptr == ':' || *end_ptr == '\0')
				return node->data;

			exts = end_ptr;
		}
	}

	return nullptr;
}


Image*
image_new()
{
	auto image = (Image*)malloc(sizeof(Image));

	image->format = Image::FMT_NULL;

	image->buffer = nullptr;
	image->filename = nullptr;

	image->size = 0;
	image->width = 0;
	image->height = 0;

	return image;
}
errno_t
image_free(Image* image)
{
	assert(image);

	free(image->buffer);
	free(image->filename);
	free(image);

	return 1;
}

errno_t
image_load_file(Image* image)
{
	assert(image);

	if(!Image::s_handler_list)
		return -1;

	ImageHandler* handler;
	if (!(handler = image_find_handler_by_filename(image->filename)))
		return -1;

	if (!handler->ops.read_file(image))
		return -1;

	return 1;
}
errno_t
image_save_file(Image* image)
{
	assert(image);

	if(!Image::s_handler_list)
		return -1;

	ImageHandler* handler;
	if (!(handler = image_find_handler_by_filename(image->filename)))
		return -1;

	if (!handler->ops.write_file(image))
		return -1;

	return 1;
}

errno_t
image_load_memory(Image* image, MemoryStream* stream)
{
	assert(image && stream);

	if(!Image::s_handler_list)
		return -1;

	ImageHandler* handler;
	if (!(handler = image_find_handler_by_filename(image->filename)))
		return -1;

	if (!handler->ops.read_memory_stream(image, stream))
		return -1;

	return 1;
}
errno_t
image_save_memory(Image* image, MemoryStream* stream)
{
	assert(image && stream);

	if(!Image::s_handler_list)
		return -1;

	ImageHandler* handler;
	if (!(handler = image_find_handler_by_filename(image->filename)))
		return -1;

	if (!handler->ops.write_memory_stream(image, stream))
		return -1;

	return 1;
}

errno_t
image_get_pixel_size(Image* image)
{
	assert(image);

	switch(image->format)
	{
		case Image::FMT_GREY8:  image->size = 1; break;
		case Image::FMT_RGB24:  image->size = 3; break;
		case Image::FMT_RGBA32: image->size = 4; break;
		case Image::FMT_RGBF48: image->size = 6; break;

		default: return -1;
	}

	return 1;
}

errno_t
image_set_buffer(Image* image, void* buffer)
{
	assert(image);

	if (image->width < 1 || image->height < 1)
		return -1;

	if (!image_get_pixel_size(image))
		return -1;

	size_t buffer_size = image->width * image->height * image->size;
	void* pixel_buffer = malloc(buffer_size);

	if (buffer) memcpy(pixel_buffer, buffer, buffer_size);
	else memset(pixel_buffer, 0, buffer_size);

	free(image->buffer);
	image->buffer = pixel_buffer;

	return 1;
}

errno_t
image_set_pixel(Image* image, uint32_t x, uint32_t y, void* pixel)
{
	assert(image);

	if (x >= image->width || y >= image->height)
		return -1;

	uint8_t* dst = (uint8_t*)image->buffer + ((x + y * image->width) * image->size);
	memcpy(dst, pixel, image->size);

	return 1;
}
errno_t
image_get_pixel(Image* image, uint32_t x, uint32_t y, void* pixel)
{
	assert(image);

	if (x >= image->width || y >= image->height)
		return -1;

	uint8_t* src = (uint8_t*)image->buffer + ((x + y * image->width) * image->size);
	memcpy(pixel, src, image->size);

	return 1;
}

errno_t
image_set_pixel1i(Image* image, uint32_t x, uint32_t y, int pixel)
{
	assert(image);

	return image_set_pixel(image, x, y, &pixel);
}
errno_t
image_get_pixel1i(Image* image, uint32_t x, uint32_t y, int* pixel)
{
	assert(image);

	return image_get_pixel(image, x, y, pixel);
}

errno_t
image_set_pixel3i(Image* image, uint32_t x, uint32_t y, int r, int g, int b)
{
	assert(image);

	uint8_t pixels[] = { r, g, b };
	return image_set_pixel(image, x, y, pixels);
}
errno_t
image_get_pixel3i(Image* image, uint32_t x, uint32_t y, int* r, int* g, int* b)
{
	assert(image);

	uint8_t pixels[3];
	if (!image_get_pixel(image, x, y, pixels))
		return -1;

	*r = pixels[0];
	*g = pixels[1];
	*b = pixels[2];

	return 1;
}

errno_t
image_set_pixel4i(Image* image, uint32_t x, uint32_t y, int r, int g, int b, int a)
{
	assert(image);

	uint8_t pixels[] = { r, g, b, a };
	return image_set_pixel(image, x, y, pixels);
}
errno_t
image_get_pixel4i(Image* image, uint32_t x, uint32_t y, int *r, int *g, int *b, int *a)
{
	assert(image);

	uint8_t pixels[4];
	if (!image_get_pixel(image, x, y, pixels))
		return -1;

	*r = pixels[0];
	*g = pixels[1];
	*b = pixels[2];
	*a = pixels[4];

	return 1;
}

