#ifndef COLORSPACE_H
#define COLORSPACE_H

/* 
 * reference: 
 * 	http://en.wikipedia.org/wiki/Color_space
 * 	RGB: http://en.wikipedia.org/wiki/SRGB
 * 	CYMK: http://en.wikipedia.org/wiki/CMYK
 *  YUV: http://en.wikipedia.org/wiki/YUV
 *  YCbCr: http://en.wikipedia.org/wiki/YCbCr
 *  HSV: http://en.wikipedia.org/wiki/HSV_color_space
 *
 *  Conversion: 
 *    http://cmlab.csie.org/~tzhuan/www/resources/OpenCV/opencvref_cv.htm#cv_imgproc_filters
 *    http://www.easyrgb.com/math.php
 *
 */

#include <algorithm>
#include <cassert>

#include "../Color.h"
#include "../Image.h"

namespace gil {

	template<typename Converter>
	void convert(
		Image<typename Converter::To> &dst, 
		const Image<typename Converter::From> &src, 
		const Converter &converter)
	{
		transform(src.begin(), src.end(), dst.begin(), converter);
	}

	template<typename Converter>
	Image<typename Converter::To> 
	convert(const Image<typename Converter::From> &src, 
			const Converter &converter)
	{
		Image<typename Converter::To> dst(src.width(), src.height());
		convert(dst, src, converter);
		return dst;
	}

	template<typename To, typename From> struct RgbToGray;
	template<typename T, size_t C> 
	struct RgbToGray<T, Color<T, C> >
	{
		typedef T To;
		typedef Color<T, C> From;
		T operator()(const From &from) const
		{
			return static_cast<T>(
				0.299*from[0] + 0.587*from[1] + 0.114*from[2]
			);
		}
	};
	template<typename T> struct RgbToGray<T, Color<T, 2> >;

	template<typename T>
	struct RgbToXyz {
		typedef T To;
		typedef T From;
		To operator()(const From &from) const 
		{
			return To(
				0.412453*from[0] + 0.357580*from[1] + 0.180423*from[2],
				0.212671*from[0] + 0.715160*from[1] + 0.072169*from[2],
				0.019334*from[0] + 0.119193*from[1] + 0.950227*from[2]
			);
		}
	};
	template<typename T> struct RgbToXyz< Color<T, 2> >;

	template<typename T>
	struct XyzToRgb {
		typedef T To;
		typedef T From;
		To operator()(const From &from) const 
		{
			return To(
				3.240479*from[0] + -1.53715*from[1] + -0.498535*from[2],
				-0.969256*from[0] + 1.875991*from[1] + 0.041556*from[2],
				0.055648*from[0] + -0.204043*from[1] + 1.057311*from[2]
			);
		}
	};
	template<typename T> struct XyzToRgb<Color<T, 2> >;
}

#endif
