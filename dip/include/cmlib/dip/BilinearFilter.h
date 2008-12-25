#ifndef GIL_BILINEAR_FILTER_H
#define GIL_BILINEAR_FILTER_H

#include <algorithm>
#include "Filter.h"

namespace gil {

	template<class DstImage, typename T = TypeTrait<Byte1>::MathType>
	class BilinearFilter: public Filter<BilinearFilter<DstImage>, DstImage> {
		friend class Filter<BilinearFilter<DstImage>, DstImage>;
		public:
			BilinearFilter(size_t x, size_t y): 
				Filter<BilinearFilter<DstImage>, DstImage>(*this),
				my_x(x), my_y(y)

			{
				// empty
			}
		protected:
			template<class SrcImage>
			void filter(DstImage& dst, const SrcImage& src) const
			{
				const float ratio_x = src.width() / static_cast<float>(my_x);
				const float ratio_y = src.height() / static_cast<float>(my_y);
				dst.resize(my_x, my_y);
				for (size_t y = 0; y < dst.height(); ++y) {
					for (size_t x = 0; x < dst.width(); ++x) {
						T _x = x*ratio_x;
						T _y = y*ratio_y;

						_x = std::min( _x, static_cast<T>( src.width()-1 ) );
						_x = std::max( _x, static_cast<T>(0) );

						_y = std::min( _y, static_cast<T>( src.height()-1 ) );
						_y = std::max( _y, static_cast<T>(0) );

						dst(x, y) = src.lerp(_x, _y);
					}
				}
			}
		private:
			size_t my_x;
			size_t my_y;
	};

}

#endif
