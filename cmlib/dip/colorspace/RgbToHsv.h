/*
Copyright NTU CSIE CMLAB 2005 - 2012
Distributed under the Boost Software License, Version 1.0.
(See accompanying file ../../../LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

/**
 * @file RgbToHsv.h
 * @author Yili Zhao <panovr@gmail.com>
 */

#ifndef CMLIB_DIP_COLORSPACE_RGBTOHSV_H
#define CMLIB_DIP_COLORSPACE_RGBTOHSV_H

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
	 * If output is HSV of ByteImage3, then H, S, V is scaled to [0, 255];
	 * If output is HSV of FloatImage3, then H, S, V is scaled to [0.0, 1.0].
	 */
	template<class DstColor>
	struct RgbToHsv { // {{{
		template<class SrcColor>
			DstColor operator()(const SrcColor& src) const 
		{
			using namespace cmlib::image;
			typedef typename ColorTrait<SrcColor>::BaseType value_type;
			typedef typename TypeTrait<value_type>::ExtendedType tmp_type;
			typedef typename ColorTrait<SrcColor>::ExtendedColor tmp_color;
			tmp_color tmp = cmlib::image::DefaultConvert<tmp_color>()(src);

			const value_type& r = src[0];
			const value_type& g = src[1];
			const value_type& b = src[2];
			const tmp_type& fr = tmp[0];
			const tmp_type& fg = tmp[1];
			const tmp_type& fb = tmp[2];
			value_type imax = std::max(std::max(r, g), b);
			value_type imin = std::min(std::min(r, g), b);

			typedef typename TypeTrait<
				typename ColorSelector<
					typename TypeTrait<value_type>::channel_type,
					typename TypeTrait<tmp_type>::channel_type
				>::priority_type>::extended_type ratio_type;
			const ratio_type ratio =
				static_cast<ratio_type>(TypeTrait<tmp_type>::domain()) /
				static_cast<ratio_type>(TypeTrait<value_type>::domain());

			// std::cout << "ratio = " << ratio << std::endl;

			tmp_type fmax = imax * ratio;
			tmp_type fmin = imin * ratio;
			tmp_type multiplier = (imin == imax) ? 0.0 : 60 / (fmax - fmin);
			tmp_type h, s, v;

			if (r == imax) {
				h = (multiplier * (fg - fb) + 360);
				if (h >= 360)
					h -= 360;
			} else if (g == imax) {
				h = multiplier * (fb - fr) + 120;
			} else {
				h = multiplier * (fr - fg) + 240;
			}

			s = imax == 0 ? 0 : 1 - (fmin / fmax);
			v = fmax;

			// std::cout << "h = " << h << ", s = " << s << ", v = " << v << std::endl;

			typedef typename ColorTrait<DstColor>::BaseType dst_type;

			/*
			std::cout << "h = " << static_cast<int>(h+0.5) << ", s = " << 
				static_cast<int>(s*100 + 0.5) << ", v = " <<
				static_cast<int>(v*100 + 0.5) << std::endl;
			*/
			return DefaultConvert<DstColor>()(tmp_color(h / 360, s, v));
		}
	};

	template<class SrcImage, class DstImage>
	DstImage& rgb_to_hsv(const SrcImage& src, DstImage& dst)
	{
		return cmlib::image::Convert<DstImage, RgbToHsv>()(src, dst);
	}

	template<class SrcImage, class DstImage>
	DstImage rgb_to_hsv(const SrcImage& src)
	{
		return cmlib::image::Convert<DstImage, RgbToHsv>()(src);
	}


} // namespace dip
} // namespace cmlib

#endif // CMLIB_DIP_COLORSPACE_RGBTOHSV_H
