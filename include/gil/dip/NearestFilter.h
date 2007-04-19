#ifndef GIL_NEAREST_FILTER_H
#define GIL_NEAREST_FILTER_H

#include <algorithm>
#include "Filter.h"

namespace gil {

	template<class DstImage>
	class NearestFilter: public Filter<NearestFilter<DstImage>, DstImage> {
		friend class Filter<NearestFilter<DstImage>, DstImage>;
		public:
			NearestFilter(size_t x, size_t y): 
				Filter<NearestFilter<DstImage>, DstImage>(*this),
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
						int _x = static_cast<int>( x*ratio_x + 0.5 );
						int _y = static_cast<int>( y*ratio_y + 0.5 );

						_x = std::min( _x, static_cast<int>(src.width())-1 );
						_x = std::max( _x, 0 );

						_y = std::min( _y, static_cast<int>(src.height())-1 );
						_y = std::max( _y, 0 );

						dst(x, y) = src(
							static_cast<size_t>(_x), static_cast<size_t>(_y)
						);
					}
				}
			}
		private:
			size_t my_x;
			size_t my_y;
	};

}

#endif
