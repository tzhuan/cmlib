#ifndef GIL_ONE_PASS_FILTER_H
#define GIL_ONE_PASS_FILTER_H

#include "Filter.h"

namespace gil {

	template<class DstImage, typename T, class Kernel>
	class OnePassFilter: 
		public Filter< OnePassFilter<DstImage, T, Kernel>, DstImage > 
	{
		friend 
			class Filter<OnePassFilter<DstImage, T, Kernel>, DstImage>;

		public:
			template<class RealFilter, typename S>
			OnePassFilter<DstImage, T, Kernel>(
				RealFilter& ref, S x, S y
			): 
				Filter< OnePassFilter<DstImage, T, Kernel>, DstImage >(ref), 
				my_kernel(x, y)
			{
				// empty
			}

		protected:

			template<class SrcImage>
			void filter(DstImage& dst, const SrcImage& src) 
			{
				typedef 
					typename ColorTrait< 
						typename SrcImage::value_type 
					>::ExtendedColor sum_type;


				dst.resize(src.width(), src.height());

				const int width = static_cast<int>(src.width());
				const int height = static_cast<int>(src.height());
				const int rx = my_kernel.sizex()/2;
				const int ry = my_kernel.sizey()/2;

				for (size_t y = 0; y < dst.height(); ++y) {
					for (size_t x = 0; x < dst.width(); ++x) {
						sum_type sum(0);
						T num = 0;

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
										ColorTrait<
											typename SrcImage::value_type
										>::select_channel(src(cur_x, cur_y), c);
								}
								num += my_kernel(w, h);
							}
						}

						assert(num);
						dst(x, y) = sum / num;
					}
				}
			}

			Kernel my_kernel;
	};

}

#endif
