#include <iostream>
#include <cmlib/image.h>

using namespace std;
using namespace cmlib::image;

ostream& operator<<(ostream& os, FloatImage1& image)
{
	for (size_t h = 0; h < image.height(); ++h) {
		for (size_t w = 0; w < image.width(); ++w)
			os << image(w, h) << " ";
		os << std::endl;
	}
	return os;
}

int main()
{
	FloatImage1 image1(3, 3);
	image1.fill(10);

	image1 /= 3.8;

	cout << image1 << endl;

	ByteImage1 image2(3, 3);
	image2.fill(8);

	image1 /= image2;
	cout << image1 << endl;


	return 0;
}
