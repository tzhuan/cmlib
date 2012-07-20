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
#include <algorithm>

#include <cmlib/image.h>
#include <cmlib/imageio/all.h>
#include <cmlib/imageio.h>
#include <cmlib/dip.h>

#include "gaussian.h"

// #include "filter-groundtruth.cpp"

using namespace std;

using namespace cmlib::image;
using namespace cmlib::dip;

template<typename Type, class Image>
bool equal(const Type answer[], const Image& image)
{
	typedef typename Image::size_type size_type;
	// assert(image.size() == answer.size());
	for (size_type i = 0; i < image.size(); ++i) {
		double error = fabs(static_cast<double>(image[i] - answer[i]))/answer[i];
		if (error > 0.0001)
			return false;
	}
	return true;
}

typedef cmlib::image::DoubleImage1 ImageType;

int main()
{
	const int width = 10;
	const int height = 10;
	const double sigma = 1.0;
	const double value = 0.1;

	ImageType src(width, height);
	std::copy(gaussian_source, gaussian_source + src.size(), src.begin());

	ImageType kernel(7, 7);
	std::copy(gaussian_kernel, gaussian_kernel + kernel.size(), kernel.begin());

	ImageType dst(width, width);

	cmlib::dip::Filter<ImageType, ImageType, false> f(kernel);
	f(cmlib::dip::SymmetricSampler<ImageType>(src), dst);

	cmlib::dip::gaussian_filter(src, dst, sigma);
	cerr << equal(gaussian_default_result, dst) << endl;

	cmlib::dip::GaussianFilter<ImageType, ImageType> filter(sigma);

	filter(cmlib::dip::DefaultSampler<ImageType>(src), dst);
	cerr << equal(gaussian_default_result, dst) << endl;

	filter(cmlib::dip::DefaultSampler<ImageType>(src, value), dst);
	cerr << equal(gaussian_default_0_1_result, dst) << endl;

	filter(cmlib::dip::ReplicateSampler<ImageType>(src), dst);
	cerr << equal(gaussian_replicate_result, dst) << endl;

	filter(cmlib::dip::CircularSampler<ImageType>(src), dst);
	cerr << equal(gaussian_circular_result, dst) << endl;

	filter(cmlib::dip::SymmetricSampler<ImageType>(src), dst);
	cerr << equal(gaussian_symmetric_result, dst) << endl;

	return 0;
}
