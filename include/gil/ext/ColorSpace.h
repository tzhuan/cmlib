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

namespace gil {
	template<typename R, typename I, typename C>
	void convert(R &dst, const I &src, const C &converter)
	{
		transform(src.begin(), src.end(), dst.begin(), converter);
	}

	template<typename R, typename I, typename C>
	R convert(const I &src, const C &converter)
	{
		R dst(src.width(), src.height());
		convert(dst, src, converter);
		return dst;
	}

	template<typename To, typename From> struct RGB2Gray;
	template<typename T, size_t C> 
	struct RGB2Gray<T, Color<T, C> >
	{
		typedef Color<T, C> From;
		T operator()(const From &from) const
		{
			// FIXME: use compile-time assert instead of assert
			assert(C >= 3);
			return 0.299*from[0] + 0.587*from[1] + 0.114*from[2];
		}
	};

	template<typename To, typename From> struct RGB2XYZ;
	template<typename T, size_t Ct, size_t Cf>
	struct RGB2XYZ< Color<T, Ct>, Color<T, Cf> > {
		typedef Color<T, Ct> To;
		typedef Color<T, Cf> From;
		To operator()(const From &from) const 
		{
			// FIXME: use compile-time assert instead of assert
			assert(Ct >= 3 && Cf >= 3);
			return To(
				0.412453*from[0] + 0.357580*from[1] + 0.180423*from[2],
				0.212671*from[0] + 0.715160*from[1] + 0.072169*from[2],
				0.019334*from[0] + 0.119193*from[1] + 0.950227*from[2]
			);
		}
	};

	template<typename To, typename From> struct XYZ2RGB;
	template<typename T, size_t Ct, size_t Cf>
	struct XYZ2RGB< Color<T, Ct>, Color<T, Cf> > {
		typedef Color<T, Ct> To;
		typedef Color<T, Cf> From;
		To operator()(const From &from) const 
		{
			// FIXME: use compile-time assert instead of assert
			assert(Ct >= 3 && Cf >= 3);
			return To(
				3.240479*from[0] + -1.53715*from[1] + -0.498535*from[2],
				-0.969256*from[0] + 1.875991*from[1] + 0.041556*from[2],
				0.055648*from[0] + -0.204043*from[1] + 1.057311*from[2]
			);
		}
	};
}

#endif
