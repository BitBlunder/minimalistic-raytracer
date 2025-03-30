#include "Image/PPMHandler.hpp"

#include <iostream>

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static errno_t
_file_read_line(FILE* fp, uint8_t* buffer, size_t size)
{
	char c;
	uint8_t* buffer_ptr = buffer;

	while (size && (c = fgetc(fp)) != EOF)
	{
		*buffer_ptr = c;

		if(c == '\n')
			break;

		size--;
		buffer_ptr++;
	}

	return buffer == buffer_ptr ? -1 : 1;
}

static errno_t
_read_file(Image* image)
{
	FILE* fp = fopen(image->filename, "rb");
	if (!fp)
		return -1;

	bool ascii;

	uint8_t buffer[256];
	uint32_t width, height, max_pixel_value, current_hdr_line = 1;

	while (current_hdr_line <= 3 && _file_read_line(fp, buffer, sizeof(buffer)))
	{
		if (buffer[0] == '#')
			continue;

		switch (current_hdr_line)
		{
		case 1:
		{
			if (!(buffer[0] == 'P' && buffer[1] == '3'))
				return -1;

			ascii = true;

			break;
		}
		case 2:
			if (!sscanf((char*)buffer, "%d %d\n", &width, &height))
				return -1;
			break;
		case 3:
			if (!sscanf((char*)buffer, "%d\n", &max_pixel_value))
				return -1;
			break;
		default:
			break;
		}

		current_hdr_line++;
	}

	if (width < 1 || height < 1 || max_pixel_value > 65535)
		return -1;

	image->width = width; image->height = height;

	if (max_pixel_value == 255) image->size = 3;
	else if (max_pixel_value == 65535) image->size = 6;
	else return -1;

	if (image->size == 3) image->format = Image::FMT_RGB24;
	else if (image->size == 6) image->format = Image::FMT_RGBF48;
	else return -1;

	size_t image_buffer_size = image->width * image->height * image->size;

	image_set_buffer(image, nullptr);

	if (ascii)
	{
		uint8_t* image_buffer_ptr = (uint8_t*)image->buffer;

		for (int i = 0; i < image_buffer_size; i++)
		{
			uint8_t* buffer_ptr = buffer;

			char c;
			while((c = fgetc(fp)) != EOF && isspace(c))
				continue;

			fseek(fp, -1, SEEK_CUR);
			while((c = fgetc(fp)) != EOF && !isspace(c))
				*(buffer_ptr++) = c;

			*(buffer_ptr++) = '\0';
			*(image_buffer_ptr++) = atoi((char*)buffer) * (255 / max_pixel_value);
		}
	}

	fclose(fp);
	return 1;
}
static errno_t
_write_file(Image* image)
{
	FILE* fp = fopen(image->filename, "wb");
	if (!fp)
		return -1;

	uint8_t header_buffer[256];
	const char* header_format = "P%d\n# minimalistic-raytracer\n%d %d\n%d\n";

	switch (image->format)
	{
	case Image::Format::FMT_RGB24:
	{
		sprintf((char*)header_buffer,
			header_format, 3, image->width, image->height, 255);

		size_t header_length = strlen((char*)header_buffer);
		if (fwrite(header_buffer, sizeof(uint8_t), header_length, fp) < header_length)
			return -1;

		for (int y = 0; y < image->height; y++)
		{
			for (int x = 0; x < image->width; x++)
			{
				uint8_t r = ((uint8_t*)image->buffer)[((x + y * image->width) * 3) + 0];
				uint8_t g = ((uint8_t*)image->buffer)[((x + y * image->width) * 3) + 1];
				uint8_t b = ((uint8_t*)image->buffer)[((x + y * image->width) * 3) + 2];

				if (!fprintf(fp, "%d %d %d ", r, g, b))
					return -1;
			}

			fprintf(fp, "\n");
		}

		break;
	}
	default:
		break;
	}

	fclose(fp);
	return 1;
}

static errno_t
_read_memory_stream(Image* image, MemoryStream* stream)
{
	return -1;
}
static errno_t
_write_memory_stream(Image* image, MemoryStream* stream)
{
	return -1;
}

ImageHandler*
ppm_handler_new()
{
	auto handler = (ImageHandler*)malloc(sizeof(ImageHandler));

	handler->ops.read_file = &_read_file;
	handler->ops.write_file = &_write_file;

	handler->ops.read_memory_stream = &_read_memory_stream;
	handler->ops.write_memory_stream = &_write_memory_stream;

	handler->exts = "ppm:pgm:pnm";

	return handler;
}