#ifndef CMLIB_DIP_FILTER_BILINEARFILTER_H
#define CMLIB_DIP_FILTER_BILINEARFILTER_H

#include <algorithm>

namespace cmlib {
namespace dip {

	template<class DstImage, typename Type = float>
	class BilinearFilter {
	public:

		typedef typename DstImage::size_type size_type;

		BilinearFilter(size_type w, size_type h)
			: my_width(w), my_height(h)
		{
			// empty
		}

		template<class SrcImage>
		DstImage& operator()(const SrcImage& src, DstImage& dst) const
		{
			const float ratio_x = src.width()/static_cast<Type>(my_width);
			const float ratio_y = src.height()/static_cast<Type>(my_height);

			for (size_type y = 0; y < dst.height(); ++y) {
				for (size_type x = 0; x < dst.width(); ++x) {
					Type _x = x*ratio_x;
					Type _y = y*ratio_y;

					_x = std::min( _x, static_cast<Type>(src.width()-1) );
					_x = std::max( _x, static_cast<Type>(0) );

					_y = std::min( _y, static_cast<Type>(src.height()-1) );
					_y = std::max( _y, static_cast<Type>(0) );

					dst(x, y) = src.lerp(_x, _y);
				}
			}
			return dst;
		}

		template<class SrcImage>
		const DstImage operator()(const SrcImage& src) const
		{
			DstImage dst(my_width, my_height);
			(*this)(src, dst);
			return dst;
		}

	private:
		size_type my_width;
		size_type my_height;
	};

	template<class SrcImage, class DstImage>
	DstImage& bilinear_filter(const SrcImage& src, DstImage& dst)
	{
		return (BilinearFilter<DstImage>(dst.width(), dst.height()))(src, dst);
	}

	template<class SrcImage, class DstImage>
	DstImage& bilinear_filter(const SrcImage& src, DstImage& dst, size_t width, size_t height)
	{
		dst.resize(width, height);
		return (BilinearFilter<DstImage>(dst.width(), dst.height()))(src, dst);
	}

	template<class SrcImage, class DstImage>
	const DstImage bilinear_filter(const SrcImage& src, size_t w, size_t h)
	{
		return (BilinearFilter<DstImage>(w, h))(src);
	}

} // namespace dip
} // namespace cmlib

#endif // CMLIB_DIP_FILTER_BILINEARFILTER_H
