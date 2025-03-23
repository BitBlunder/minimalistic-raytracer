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
		ImageHandler* ppm_handler = nullptr;
		ppm_handler_new(&ppm_handler);

		if(!image_register_handler(ppm_handler))
			std::cerr << "Error occured while registering PPM Handler\n";
	}

	Image* img = nullptr;
	image_new(&img);

	img->format = Image::Format::FMT_RGB24;
	img->filename = "../Assets/test.ppm";
	img->width = 3; img->height = 2;

	if (!image_set_pixel_buffer(img, data));
		std::cerr << "Error occured while setting pixels to PPM file from memory\n";

	image_set_pixel3i(img, 0, 0, 100, 200, 50);

	if (!image_save_file(img))
		std::cerr << "Error occured while saving PPM file to disk\n";

	return 1;
}