#ifndef GIL_GABOR_FILTER_H
#define GIL_GABOR_FILTER_H

#include <cmath>

#include "Kernel.h"
#include "OnePassFilter.h"

namespace gil {

	/* GaborFilter: 
	 *   a linear filter whose impulse response is defined by a harmonic 
	 *   function multiplied by a Gaussian function. 
	 *
	 * Reference:
	 *   http://en.wikipedia.org/wiki/Gabor_filter
	 */

    template<typename T>
    class GaborKernel: public SquareKernel<T> {
        public:
            GaborKernel<T>(size_t w, size_t h): SquareKernel<T>(w, h)
            {   
            }

			void set_parameters(
				T o, // orientation 
				T p, // phrase offset
				T w, // wavelength of cosine factor
				T b, // bandwidth 
				T a  // the aspect ratio ( x:y, I guess :p )
			) {
				my_orientation = o;
				my_phrase = p;

				T sigma = 
					sqrt( std::log( static_cast<T>(2) ) / 2 ) *
					( w * ( std::pow(2, b)+1 ) ) / 
					(M_PI*(std::pow(static_cast<T>(2), b)-1));
				my_factor = static_cast<T>(-1)/(2*sigma*sigma);
				my_gamma = a*a;
				my_coef = 2*M_PI/w;

				int width = 0;
				int height = 0;

				for (; width < std::numeric_limits<int>::max(); ++width)
					if (g(width, 0) < 1e-6)
						break;
				for (; height < std::numeric_limits<int>::max(); ++height)
					if (g(height, 0) < 1e-6)
						break;

				this->resize(2*width+1, 2*height+1);

				for (int y = -static_cast<int>(this->my_radius_y); 
						y <= static_cast<int>(this->my_radius_y); ++y)
					for (int x = -static_cast<int>(this->my_radius_x);
							x <= static_cast<int>(this->my_radius_x); ++x)
						(*this)(x, y) = g(x, y);
			}

		protected:
			T g(size_t x, size_t y) const
			{
				const T& o = my_orientation;
				T x_ = x*std::cos(o) - y*std::sin(o);
				T y_ = x*std::sin(o) - y*std::cos(o);
				T mu = (x_*x_ + my_gamma*y_*y_) * my_factor;
				return std::exp(mu) * std::cos(my_coef*x_ + my_phrase);
			}
		private:
			T my_orientation;
			T my_phrase;
			T my_factor;
			T my_gamma;
			T my_coef;
    };


	template<class DstImage, typename T = typename TypeTrait<Byte1>::MathType >
	class GaborFilter: public OnePassFilter<DstImage, T, GaborKernel<T> > {
		friend class Filter<GaborFilter<DstImage>, DstImage>;

        typedef
            OnePassFilter< DstImage, T, GaborKernel<T> >
            RealFilter;
        friend class Filter<RealFilter, DstImage>;

		public:
			GaborFilter(
				T o, // orientation 
				T p, // phrase offset
				T w, // wavelength of cosine factor
				T b, // bandwidth 
				T a  // the aspect ratio ( x:y, I guess :p )
			): RealFilter(*this, 0, 0)
			{
				this->my_kernel.set_parameters(o, p, w, b, a);
			}
	};

}

#endif
