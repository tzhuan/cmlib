/*
Copyright NTU CSIE CMLAB 2005 - 2012
Distributed under the Boost Software License, Version 1.0.
(See accompanying file ../../../LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

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
