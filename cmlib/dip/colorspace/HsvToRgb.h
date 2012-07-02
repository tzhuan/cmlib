/*
Copyright NTU CSIE CMLAB 2005 - 2012
Distributed under the Boost Software License, Version 1.0.
(See accompanying file ../../../LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

/**
 * @file HsvToRgb.h
 * @author Yili Zhao <panovr@gmail.com>
 */

#ifndef CMLIB_DIP_COLORSPACE_HSVTORGB_H
#define CMLIB_DIP_COLORSPACE_HSVTORGB_H

#include <cmlib/image.h>

namespace cmlib {
namespace dip {

	/**
	 * R: [0, 255]
	 * G: [0, 255]
	 * B: [0, 255]
	 *
	 * H: [0.0, 360.0)
	 * S: [0.0, 1.0]
	 * V: [0.0, 1.0]
	 *
	 * If output is RGB of ByteImage3,  then R, G, B is scaled to [0, 255];
	 * If output is RGB of FloatImage3, then R, G, B is scaled to [0.0, 1.0].
	 */
	template<class DstColor>
	struct HsvToRgb {
		template<class SrcColor>
		DstColor operator()(const SrcColor& src) const 
		{
			using namespace cmlib::image;
			typedef typename ColorTrait<SrcColor>::BaseType value_type;
			typedef typename TypeTrait<value_type>::ExtendedType tmp_type;
			typedef typename ColorTrait<SrcColor>::ExtendedColor tmp_color;
			typedef typename TypeTrait<
				typename ColorSelector<
					typename TypeTrait<value_type>::channel_type,
					typename TypeTrait<tmp_type>::channel_type
				>::priority_type>::extended_type ratio_type;
			const ratio_type ratio =
				static_cast<ratio_type>(TypeTrait<tmp_type>::domain()) /
				static_cast<ratio_type>(TypeTrait<value_type>::domain());

			// std::cout << "ratio = " << ratio << std::endl;

			tmp_type h = src[0] * 360 * ratio;
			tmp_type s = src[1] * ratio;
			tmp_type v = src[2] * ratio;

			h /= 60;
			int hi = static_cast<int>(h);
			tmp_type f = h - hi;
			tmp_type p = v*( 1 - s );
			tmp_type q = v*( 1 - f*s );
			tmp_type t = v*( 1 - ( 1 - f )*s);
			tmp_type r, g, b;
			switch (hi) {
			case 0:
				r = v; g = t; b = p;
				break;
			case 1:
				r = q; g = v; b = p;
				break;
			case 2:
				r = p ; g = v; b = t;
				break;
			case 3:
				r = p ; g = q; b = v;
				break ;
			case 4:
				r = t ; g = p ; b = v ;
				break ;
			case 5:
				r = v ; g = p ; b = q ;
				break;
			case 6:
				if (h == static_cast<tmp_type>(360)) {
					r = v; g = t; b = p;
					break;
				}
			default:
				throw std::runtime_error("HsvToRgb error");
			}

			return DefaultConvert<DstColor>()(tmp_color(r, g, b));
		}
	};

	template<class SrcImage, class DstImage>
	DstImage& hsv_to_rgb(const SrcImage& src, DstImage& dst)
	{
		return cmlib::image::Convert<DstImage, HsvToRgb>()(src, dst);
	}

	template<class SrcImage, class DstImage>
	DstImage hsv_to_rgb(const SrcImage& src)
	{
		return cmlib::image::Convert<DstImage, HsvToRgb>()(src);
	}

} // namespace dip
} // namespace cmlib

#endif // CMLIB_DIP_COLORSPACE_HSVTORGB_H
