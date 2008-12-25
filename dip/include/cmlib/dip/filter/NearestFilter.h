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
			const Type ratio_x = src.width() / static_cast<Type>(my_width);
			const Type ratio_y = src.height() / static_cast<Type>(my_height);
			for (size_type y = 0; y < dst.height(); ++y) {
				for (size_type x = 0; x < dst.width(); ++x) {
					int _x = static_cast<int>( x*ratio_x + 0.5 );
					int _y = static_cast<int>( y*ratio_y + 0.5 );

					_x = std::min( _x, static_cast<int>(src.width())-1 );
					_x = std::max( _x, 0 );

					_y = std::min( _y, static_cast<int>(src.height())-1 );
					_y = std::max( _y, 0 );

					dst(x, y) = src(
						static_cast<size_type>(_x), static_cast<size_type>(_y)
					);
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
