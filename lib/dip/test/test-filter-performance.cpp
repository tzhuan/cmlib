#include <cstdio>
#include <cstdlib>
#include <cerrno>

#include <vector>
#include <string>

#include <sys/time.h>

#include <cmlib/image.h>
#include <cmlib/dip.h>
#include <cmlib/imageio/all.h>
#include <cmlib/imageio.h>

#if SUPPORT_GIL
#include <gil/gil.h>
#include <gil/dip.h>
#endif

#include "gaussian.h"

typedef cmlib::image::DoubleImage1 ImageType;

void timer_stop(timeval& time)
{
	if (gettimeofday(&time, NULL) < 0) {
		std::cerr << "error: gettimeofday: " << std::strerror(errno) << std::endl;
		exit(1);
	}
}

void output_timeval_diff(const timeval &begin, const timeval &end)
{
	time_t sec = 0;
	suseconds_t usec = 0;

	if (begin.tv_sec > end.tv_sec ||
		(begin.tv_sec == end.tv_sec && begin.tv_usec > end.tv_usec)) {
		std::cerr << "error: begin > end" << std::endl;
		exit(1);
	}

	if (begin.tv_sec == end.tv_sec) {
		usec = end.tv_usec - begin.tv_usec;
	} else if (begin.tv_usec <= end.tv_usec) {
		sec = end.tv_sec - begin.tv_sec;
		usec = end.tv_usec - begin.tv_usec;
	} else {
		sec = end.tv_sec - begin.tv_sec - 1;
		usec = 1000000 + end.tv_usec - begin.tv_usec;
	}
	std::cout << sec << " second, " << usec << " microseconds" << std::endl;
}

template<class ImageType>
void filter_cmlib(const ImageType& src, ImageType& dst, int n)
{
	cmlib::dip::GaussianFilter<ImageType, ImageType> filter(kernel_sigma);
	for (int i = 0; i < n; ++i)
		filter(cmlib::dip::default_sampler(src), dst);
}

template<class ImageType>
void filter_cmlib_inplace(const ImageType& src, ImageType& dst, int n)
{
	cmlib::dip::GaussianFilter<ImageType, ImageType, true> filter(kernel_sigma);
	for (int i = 0; i < n; ++i)
		filter(cmlib::dip::default_sampler(src), dst);
}

#if SUPPORT_GIL
template<class ImageType>
void filter_gil(const ImageType& src, ImageType& dst, int n)
{
	gil::GaussianFilter<ImageType> filter(kernel_sigma, kernel_sigma);
	for (int i = 0; i < n; ++i)
		filter(dst, src);
}
#endif

int main(int argc, char* argv[]) 
{
	if (argc == 1) {
		std::cerr << "usage: " << argv[0] << " operations..." << std::endl
			<< "available operations: " << std::endl
			<< "  cmlib" << std::endl
			<< "  cmlib-inplace" << std::endl
#if SUPPORT_GIL
			<< "  gil" << std::endl
#endif
			<< std::endl;
		return 0;
	}

	const int count = 10000;

	ImageType src(source_width, source_height);
	ImageType kernel(kernel_width, kernel_height);
	ImageType dst(source_width, source_height);

	std::copy(gaussian_source, gaussian_source + src.size(), src.begin());
	std::copy(gaussian_kernel, gaussian_kernel + kernel.size(), kernel.begin());

	for (int i = 1; i < argc; ++i) {
		std::string operation(argv[i]);
		timeval begin, end;
		timer_stop(begin);
		if (operation == "cmlib") {
			filter_cmlib(src, dst, count);
		} else if (operation == "cmlib-inplace") {
			filter_cmlib_inplace(src, dst, count);
		}
#if SUPPORT_GIL
		else if (operation == "gil") {
			filter_gil(src, dst, count);
		}
#endif
		else {
			std::cerr << "error: invalid operation \""
				<< operation << '"' << std::endl;
			continue;
		}
		timer_stop(end);
		std::cout << operation << ": ";
		output_timeval_diff(begin, end);
	}
    return 0;
}
