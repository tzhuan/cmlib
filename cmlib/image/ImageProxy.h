/*
Copyright NTU CSIE CMLAB 2005 - 2012
Distributed under the Boost Software License, Version 1.0.
(See accompanying file ../../LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef CMLIB_IMAGE_IMAGEPROXY_H
#define CMLIB_IMAGE_IMAGEPROXY_H

namespace cmlib {
namespace image {

	template<class Filter, class DstImage, class SrcImage>
	class ImageProxy {
		public:
			typedef typename DstImage::value_type value_type;

			ImageProxy(const Filter& filter, const SrcImage& src):
				my_filter(filter), my_src(src)
			{
				// empty
			}

			inline DstImage& operator ()(DstImage& dst) const
			{
				my_filter(dst, my_src);
				return dst;
			}

		protected:
		private:
			const Filter& my_filter;
			const SrcImage& my_src;
	};
} // namespace image
} // namespace cmlib

#endif // CMLIB_IMAGE_IMAGEPROXY_H
