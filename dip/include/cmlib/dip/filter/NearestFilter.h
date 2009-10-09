#ifndef CMLIB_DIP_FILTER_NEARESTFILTER_H
#define CMLIB_DIP_FILTER_NEARESTFILTER_H

#include <algorithm>

namespace cmlib {
namespace dip {

	template<class DstImage, typename Type = float>
	class NearestFilter {
	public:

		typedef typename DstImage::size_type size_type;

		NearestFilter(size_type w, size_type h)
			: my_width(w), my_height(h)
		{
			// empty
		}

		template<class SrcImage>
		DstImage& operator()(const SrcImage& src, DstImage& dst) const
		{
			Type ratio_x = 1, ratio_y = 1;
			if (my_width > 1)
				ratio_y = (src.height()-1) / (static_cast<Type>(my_height)-1);
			if (my_height > 1)
				ratio_x = (src.width()-1) / (static_cast<Type>(my_width)-1);

			for (size_type y = 0; y < dst.height(); ++y) {
				for (size_type x = 0; x < dst.width(); ++x) {
					size_type _x = static_cast<size_type>( x*ratio_x + 0.5 );
					size_type _y = static_cast<size_type>( y*ratio_y + 0.5 );
					dst(x, y) = src(_x, _y);
				}
			}
			return dst;
		}

		template<class SrcImage>
		const DstImage operator()(const SrcImage& src) const
		{
			DstImage dst(my_width, my_height);
			return (*this)(src, dst);
		}
	private:
		size_type my_width;
		size_type my_height;
	};

	template<class SrcImage, class DstImage>
	DstImage& nearest_filter(const SrcImage& src, DstImage& dst)
	{
		return (NearestFilter<DstImage>(dst.width(), dst.height()))(src, dst);
	}

	template<class SrcImage, class DstImage>
	DstImage& nearest_filter(const SrcImage& src, DstImage& dst, size_t width, size_t height)
	{
		dst.resize(width, height);
		return (NearestFilter<DstImage>(dst.width(), dst.height()))(src, dst);
	}

	template<class SrcImage, class DstImage>
	const DstImage nearest_filter(const SrcImage& src, size_t w, size_t h)
	{
		return (NearestFilter<DstImage>(w, h))(src);
	}

} // namespace dip
} // namespace cmlib

#endif // CMLIB_DIP_FILTER_NEARESTFILTER_H
