#ifndef CMLIB_DIP_ONE_PASS_FILTER_H
#define CMLIB_DIP_ONE_PASS_FILTER_H

#include <gil/Color.h>

namespace cmlib {
namespace dip {

	template<class DstImage, class KernelType>
	class OnePassFilter {
	public:
		OnePassFilter(const KernelType& kernel)
			: my_kernel(kernel)
		{
			// empty
		}

		template<class SrcImage>
		void operator()(const SrcImage& src, DstImage& dst) const
		{
			using namespace cmlib::gil;
			typedef typename SrcImage::value_type SrcColor;
			typedef typename ColorTrait<SrcColor>::ExtendedColor sum_type;

			const int width = static_cast<int>(src.width());
			const int height = static_cast<int>(src.height());
			const int rx = my_kernel.xsize()/2;
			const int ry = my_kernel.ysize()/2;

			for (size_t y = 0; y < dst.height(); ++y) {
				for (size_t x = 0; x < dst.width(); ++x) {
					sum_type sum(0);
					typename KernelType::value_type num = 0;

					for (int h = -ry; h <= ry; ++h) {

						const int cur_y = static_cast<int>(y) + h;
						if (cur_y < 0) continue;
						else if (cur_y >= height)
							break;

						for (int w = -rx; w <= rx; ++w) {

							const int cur_x = static_cast<int>(x) + w;
							if (cur_x < 0) continue;
							else if (cur_x >= width)
								break;

							for (size_t c = 0; c < dst.channels(); ++c) {
								ColorTrait<sum_type>::select_channel(sum, c)
									+= 
									my_kernel(w, h) * 
									ColorTrait<SrcColor>::select_channel(
										src(cur_x, cur_y), c
									);
							}
							num += my_kernel(w, h);
						}
					}

					if (num)
						dst(x, y) = sum / num;
				}
			}
		}

		template<class SrcImage>
		DstImage operator()(const SrcImage& src) const
		{
			DstImage dst(src.width(), src.height());
			(*this)(src, dst);
			return dst;
		}

	protected:
		const KernelType& my_kernel;
	};

	template<class SrcImage, class DstImage, class KernelType>
	void filter(const SrcImage& src, DstImage& dst, const KernelType& kernel)
	{
		( OnePassFilter<DstImage, KernelType>(kernel) )(src, dst);
	}

	template<class SrcImage, class DstImage, class KernelType>
	DstImage filter(const SrcImage& src, const KernelType& kernel)
	{
		return ( OnePassFilter<DstImage, KernelType>(kernel) )(src);
	}

} // namespace dip
} // namespace cmlib

#endif // CMLIB_DIP_ONE_PASS_FILTER_H
