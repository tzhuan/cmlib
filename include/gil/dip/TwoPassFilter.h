#ifndef GIL_TWO_PASS_FILTER_H
#define GIL_TWO_PASS_FILTER_H

#include <cstddef>
#include "Filter.h"

namespace gil {

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

	template<class DstImage, typename T, class XKernel, class YKernel>
	class TwoPassFilter: 
		public Filter< TwoPassFilter<DstImage, T, XKernel, YKernel>, DstImage > 
	{
		friend 
			class Filter<
				TwoPassFilter<DstImage, T, XKernel, YKernel>, 
				DstImage
			>;

		public:
			template<class RealFilter, typename S>
			TwoPassFilter<DstImage, T, XKernel, YKernel>(
				RealFilter& ref, S x, S y
			): 
				Filter< 
					TwoPassFilter<DstImage, T, XKernel, YKernel>,
					DstImage
				>(ref), 
				my_xkernel(x), 
				my_ykernel(y)
			{
				// empty
			}

		protected:

			template<class SrcImage>
			void filter(DstImage& dst, const SrcImage& src) const
			{
				DstImage tmp(src.width(), src.height());
				filter<XSelector>(tmp, src, my_xkernel);

				dst.resize(tmp.width(), tmp.height());
				filter<YSelector>(dst, tmp, my_ykernel);
			}

			template< class Selector, class SrcImage, class Kernel >
			void 
			filter(
				DstImage& dst, 
				const SrcImage& src, 
				const Kernel &kernel
			) const
			{
				typedef 
					typename ColorTrait<
						typename SrcImage::value_type
					>::ExtendedColor sum_type;

				const int r = kernel.size()/2;

				for (size_t y = 0; y < dst.height(); ++y) {
					for (size_t x = 0; x < dst.width(); ++x) {

						sum_type sum(0);
						T num = 0;

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

						assert(num);
						dst(x, y) = sum / num;
					}
				}
			}

			XKernel my_xkernel;
			YKernel my_ykernel;
	};

}

#endif
