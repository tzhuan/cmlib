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

#define BOOST_TEST_MODULE cmlib::dip::filter test
#include <boost/test/unit_test.hpp>

#include <cmlib/image.h>
#include <cmlib/imageio/all.h>
#include <cmlib/imageio.h>
#include <cmlib/dip.h>

#include "gaussian.h"

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

ImageType src(source_width, source_height);
ImageType kernel(kernel_width, kernel_height);
ImageType dst(source_width, source_height);

struct FilterConfig {

	FilterConfig()
	{
		std::copy(gaussian_source, gaussian_source + src.size(), src.begin());
		std::copy(gaussian_kernel, gaussian_kernel + kernel.size(), kernel.begin());
	}

	~FilterConfig()
	{
	}
};

BOOST_GLOBAL_FIXTURE( FilterConfig );

BOOST_AUTO_TEST_SUITE( filter_test_suite )

BOOST_AUTO_TEST_CASE( filter_test_default )
{
	(cmlib::dip::Filter<ImageType, ImageType, false> (kernel))
		(cmlib::dip::DefaultSampler<ImageType>(src), dst);
	BOOST_CHECK( equal(gaussian_default_result, dst) );
}

BOOST_AUTO_TEST_CASE( filter_test_default_0_1 )
{
	(cmlib::dip::Filter<ImageType, ImageType, false> (kernel))
		(cmlib::dip::DefaultSampler<ImageType>(src, default_value), dst);
	BOOST_CHECK( equal(gaussian_default_0_1_result, dst) );
}

BOOST_AUTO_TEST_CASE( filter_test_replicate )
{
	(cmlib::dip::Filter<ImageType, ImageType, false> (kernel))
		(cmlib::dip::ReplicateSampler<ImageType>(src), dst);
	BOOST_CHECK( equal(gaussian_replicate_result, dst) );
}

BOOST_AUTO_TEST_CASE( filter_test_symmetric )
{
	(cmlib::dip::Filter<ImageType, ImageType, false> (kernel))
		(cmlib::dip::SymmetricSampler<ImageType>(src), dst);
	BOOST_CHECK( equal(gaussian_symmetric_result, dst) );
}

BOOST_AUTO_TEST_CASE( filter_test_circular )
{
	(cmlib::dip::Filter<ImageType, ImageType, false> (kernel))
		(cmlib::dip::CircularSampler<ImageType>(src), dst);
	BOOST_CHECK( equal(gaussian_circular_result, dst) );
}

BOOST_AUTO_TEST_SUITE_END();



BOOST_AUTO_TEST_SUITE( gaussian_filter_test_suite );

BOOST_AUTO_TEST_CASE( gaussian_filter_test_default )
{
	(cmlib::dip::GaussianFilter<ImageType, ImageType>(kernel_sigma))
		(cmlib::dip::DefaultSampler<ImageType>(src), dst);
	BOOST_CHECK( equal(gaussian_default_result, dst) );
}

BOOST_AUTO_TEST_CASE( gaussian_filter_test_default_0_1 )
{
	(cmlib::dip::GaussianFilter<ImageType, ImageType>(kernel_sigma))
		(cmlib::dip::DefaultSampler<ImageType>(src, default_value), dst);
	BOOST_CHECK( equal(gaussian_default_0_1_result, dst) );
}

BOOST_AUTO_TEST_CASE( gaussian_filter_test_replicate )
{
	(cmlib::dip::GaussianFilter<ImageType, ImageType>(kernel_sigma))
		(cmlib::dip::ReplicateSampler<ImageType>(src), dst);
	BOOST_CHECK( equal(gaussian_replicate_result, dst) );
}

BOOST_AUTO_TEST_CASE( gaussian_filter_test_circular )
{
	(cmlib::dip::GaussianFilter<ImageType, ImageType>(kernel_sigma))
		(cmlib::dip::CircularSampler<ImageType>(src), dst);
	BOOST_CHECK( equal(gaussian_circular_result, dst) );
}

BOOST_AUTO_TEST_CASE( gaussian_filter_test_symmetric )
{
	(cmlib::dip::GaussianFilter<ImageType, ImageType>(kernel_sigma))
		(cmlib::dip::SymmetricSampler<ImageType>(src), dst);
	BOOST_CHECK( equal(gaussian_symmetric_result, dst) );
}

BOOST_AUTO_TEST_SUITE_END();


BOOST_AUTO_TEST_SUITE( gaussian_filter_helper_test_suite );

BOOST_AUTO_TEST_CASE( gaussian_filter_helper_test )
{
	cmlib::dip::gaussian_filter(src, dst, kernel_sigma);
	BOOST_CHECK( equal(gaussian_default_result, dst) );
}

BOOST_AUTO_TEST_SUITE_END();
