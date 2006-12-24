#ifndef BOX_FILTER_H
#define BOX_FILTER_H

#include "Kernel.h"
#include "TwoPassFilter.h"

namespace gil {

	template<typename T>
	class BoxKernel: public LineKernel<T> {
		public:
			BoxKernel<T>(size_t size): LineKernel<T>(size)
			{
				this->fill(1);
			}
	};

	template<class DstImage, typename T = TypeTrait<Byte1>::MathType>
	class BoxFilter: 
		public TwoPassFilter< DstImage, T, BoxKernel<T>, BoxKernel<T> > 
	{
		typedef
			TwoPassFilter< DstImage, T, BoxKernel<T>, BoxKernel<T> > 
			RealFilter;

		friend class Filter<RealFilter, DstImage>;

		public:
			BoxFilter<DstImage, T>(size_t x, size_t y): RealFilter(*this, x, y)
			{
				// empty
			}
	};
}

#endif
