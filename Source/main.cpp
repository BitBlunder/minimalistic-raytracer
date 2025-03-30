#include <iostream>

#include <Image/Image.hpp>
#include <Image/PPMHandler.hpp>

uint8_t data[] = {
	255,   0,  0,   0, 255,   0,  0,  0, 255,
	255, 255,  0, 255, 255, 255,  0,  0,   0
};

int main(int argc, char** args)
{
	{
		if(!image_register_handler(ppm_handler_new()))
			std::cerr << "Error occured while registering PPM Handler\n";
	}

	Image* img = image_new();

	img->format = Image::Format::FMT_RGB24;
	img->filename = "../Assets/test.ppm";
	img->width = 3; img->height = 2;

	if (!image_set_buffer(img, data))
		std::cerr << "Error occured while setting pixels to PPM file from memory\n";

	image_set_pixel3i(img, 0, 0, 100, 200, 50);

	if (!image_save_file(img))
		std::cerr << "Error occured while saving PPM file to disk\n";

	return 0;
}