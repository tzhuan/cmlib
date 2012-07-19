/*
Copyright NTU CSIE CMLAB 2005 - 2012
Distributed under the Boost Software License, Version 1.0.
(See accompanying file ../../../LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

/**
 * @file test-filter.cpp
 * @author Tz-Huan Huang <tzhuan@gmail.com>
 */

#include <cassert>

#include <iostream>
#include <iomanip>

#include <cmlib/image.h>
#include <cmlib/imageio/all.h>
#include <cmlib/imageio.h>
#include <cmlib/dip.h>

#include "filter-groundtruth.cpp"

using namespace std;

using namespace cmlib::image;
using namespace cmlib::dip;

template<class Image>
bool equal(const Image& answer, const Image& image)
{
	typedef typename Image::size_type size_type;
	assert(image.size() == answer.size());
	for (size_type i = 0; i < answer.size(); ++i)
		if (fabs(static_cast<double>(image[i] - answer[i]))/answer[i] > 0.0001)
			return false;
	return true;
}


int main()
{
	const int width = 100;
	const int height = 100;
	const float sigma = 2.0;

	FloatImage1 src(width, width);
	FloatImage1 answer(width, width);

	setup_image(src);
	setup_answer(answer);

	FloatImage1 dst(width, height);
	gaussian_filter(src, dst, sigma);

	cerr << equal(answer, dst) << endl;

	/*
	for (size_t i = 0; i < src.size(); ++i)
		src[i] = static_cast<double>(rand())/RAND_MAX;

	// write(src, "src.png");
	for (size_t i = 0; i < src.size(); ++i)
		cout << "image[" << setw(5) << i << "] = " << setprecision(18) << src[i] << ";" << endl;

	FloatImage1 dst(src.width(), src.height());
	gaussian_filter(src, dst, 2.0f);

	// write(dst, "dst.png");
	for (size_t i = 0; i < src.size(); ++i)
		cout << "answer[" << setw(5) << i << "] = " << setprecision(18) << dst[i] << ";" << endl;
	*/
	return 0;
}
