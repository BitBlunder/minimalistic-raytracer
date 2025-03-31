#include <cstring>
#include <iostream>

#include <type_traits>

#include <Image/Image.hpp>
#include <Image/PPMHandler.hpp>

int main(int argc, char** args)
{
	{
		if(!image_register_handler(ppm_handler_new()))
			std::cerr << "Error occured while registering PPM Handler\n";
	}

	Image* img = image_new();

	img->format = Image::Format::FMT_RGB24;
	img->filename = "../Assets/sample.ppm";
	img->width = 300; img->height = 300;

	uint8_t* buffer = (uint8_t*)malloc(img->width * img->height * 3);

	for (int y = 0; y < img->height; y++)
		for (int x = 0; x < img->width; x++)
		{
			float t = static_cast<float>(x) / (img->width - 1);

			uint8_t r = static_cast<uint8_t>(t * 255);
			uint8_t g = static_cast<uint8_t>(t * 255);
			uint8_t b = 255;

			int index = (x + (y * img->width)) * 3;

			buffer[index + 0] = r;
			buffer[index + 1] = g;
			buffer[index + 2] = b;
		}

	if (!image_set_buffer(img, buffer))
		std::cerr << "Error occured while setting pixels to PPM file from memory\n";

	if (!image_save_file(img))
		std::cerr << "Error occured while saving PPM file to disk\n";

	return 0;
}