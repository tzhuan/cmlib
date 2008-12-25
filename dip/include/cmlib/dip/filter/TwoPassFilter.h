#ifndef CMLIB_DIP_TWO_PASS_FILTER_H
#define CMLIB_DIP_TWO_PASS_FILTER_H

#include <gil/Color.h>

namespace cmlib {
namespace dip {

	struct XSelector { 
		static int offset(size_t x, size_t, int i) 
		{
			return static_cast<int>(x) + i;
		}

		template<class ImageType>
		static int size(const ImageType& image)
		{
			return image.width();
		}

		template<class ImageType>
		static const typename ImageType::value_type 
		color(const ImageType& image, size_t, size_t y, size_t i)
		{
			return image(i, y);
		}
	}; 

	struct YSelector { 
		static int offset(size_t, size_t y, int i)
		{
			return static_cast<int>(y) + i;
		}

		template<class ImageType> 
		static int size(const ImageType& image)
		{
			return image.height();
		}

		template<class ImageType> 
		static const typename ImageType::value_type 
		color(const ImageType& image, size_t x, size_t, size_t i)
		{
			return image(x, i);
		}
	}; 

	template<class DstImage, class XKernel, class YKernel = XKernel>
	class TwoPassFilter {
	public:
		TwoPassFilter(const XKernel& xkernel, const YKernel& ykernel)
			: my_xkernel(xkernel), my_ykernel(ykernel)
		{
			// empty
		}

		template<class SrcImage>
		void operator()(const SrcImage& src, DstImage& dst) const
		{
			DstImage tmp(src.width(), src.height());
			filter<XSelector>(src, tmp, my_xkernel);
			filter<YSelector>(tmp, dst, my_ykernel);
		}

		template<class SrcImage>
		DstImage operator()(const SrcImage& src) const
		{
			DstImage dst(src.width(), src.height());
			(*this)(src, dst);
			return dst;
		}

	protected:

		template<class SrcImage, class Kernel, class Selector>
		void filter(const SrcImage& src, DstImage& dst, Kernel& kernel) const
		{
			using namespace cmlib::gil;
			typedef typename SrcImage::value_type SrcColor;
			typedef typename ColorTrait<SrcColor>::ExtendedColor sum_type;

			const int r = kernel.size()/2;

			for (size_t y = 0; y < dst.height(); ++y) {
				for (size_t x = 0; x < dst.width(); ++x) {

					sum_type sum(0);
					typename Kernel::value_type num = 0;

					for (int i = -r; i <= r; ++i) {
						int cur = Selector::offset(x, y, i);
						if ( cur < 0 ) 
							continue;
						if ( cur >= Selector::size(src) ) 
							break;

						for (size_t c = 0; c < dst.channels(); ++c) {
							ColorTrait<sum_type>::select_channel(sum, c) += 
								kernel(i) * 
								ColorTrait<
									typename SrcImage::value_type
								>::select_channel(
									Selector::color(src, x, y, cur),
									c
								);
						}
						num += kernel(i);
					}

					if (num)
						dst(x, y) = sum / num;
				}
			}
		}

		const XKernel& my_xkernel;
		const YKernel& my_ykernel;
	};

} // namespace dip
} // namespace gil

#endif // CMLIB_DIP_TWO_PASS_FILTER_H
