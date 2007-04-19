#ifndef GIL_GAUSSIAN_FILTER_H
#define GIL_GAUSSIAN_FILTER_H

#include <cmath>

#include "Kernel.h"
#include "TwoPassFilter.h"

namespace gil {

	template<typename T>
	class GaussianKernel: public LineKernel<T> {
		public:
			GaussianKernel<T>(T sigma): 
				LineKernel<T>( 2*static_cast<size_t>(3*sigma) + 1 )
			{
				for (int i = - static_cast<int>(this->my_radius); 
						i <= static_cast<int>(this->my_radius); ++i) {
					(*this)(i) = std::exp( -(i*i) / (2*sigma*sigma) );
				}
			}
	};

	template<class DstImage, typename T = typename TypeTrait<Byte1>::MathType >
	class GaussianFilter: 
		public 
			TwoPassFilter<DstImage, T, GaussianKernel<T>, GaussianKernel<T> > 
	{
		typedef
			TwoPassFilter< DstImage, T, GaussianKernel<T>, GaussianKernel<T> > 
			RealFilter;

		friend class Filter<RealFilter, DstImage>;

		public:
			GaussianFilter<DstImage, T>(T sigmax, T sigmay): 
				RealFilter(*this, sigmax, sigmay)
			{
				// empty
			}

		protected:
	};

}

#endif
