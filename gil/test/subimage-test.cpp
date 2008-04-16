#include <exception>
#include <iostream>
#include <string>
#include <cstdlib>
#include <cstddef>
#include "gil/gil.h"

using namespace std;
using namespace gil;

int main(int argc, char **argv)
{
	string input = "lena.ppm";
	if (argc == 1)
		cerr << "[use default test image: lena.ppm]" << endl;
	else if (argc == 2)
		input = argv[1];
	else {
		cerr << "usage: " << argv[0] << " image-file" << endl;
		return 1;
	}

	srand(time(0));

	try {
		ByteImage3 image;
		read(image, input);
		size_t pos_x = rand() % image.width();
		size_t pos_y = rand() % image.height();
		size_t width = rand() % (image.width() - pos_x);
		size_t height = rand() % (image.width() - pos_y);

		cerr << "(" << pos_x << ", " << pos_y << ") - (" 
			<< width << ", " << height << ")" << endl;

		ByteImage3 image2 = image;
		
		size_t pos_x2 = rand() % (image2.width()-width);
		size_t pos_y2 = rand() % (image2.height()-height);

		cerr << "(" << pos_x2 << ", " << pos_y2 << ")" << endl;

		image2.replace(
			sub_image(image, pos_x, pos_y, width, height),
			pos_x2,
			pos_y2
		);
		// ByteImage3 subimage = sub_image(image, 10, 10, 400, 400);
		write(image2, "image2.png");
	} catch (exception &e) {
		cerr << "error: " << e.what() << endl;
	}
	return 0;
}
