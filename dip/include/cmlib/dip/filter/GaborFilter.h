#ifndef CMLIB_DIP_FILTER_GABORFILTER_H
#define CMLIB_DIP_FILTER_GABORFILTER_H

#include <cmath>

#include "Filter.h"

namespace cmlib {
namespace dip {

	/* GaborFilter: 
	 *   a linear filter whose impulse response is defined by a harmonic 
	 *   function multiplied by a Gaussian function. 
	 *
	 * Reference:
	 *   http://en.wikipedia.org/wiki/Gabor_filter
	 */

	template<class DstImage, class Kernel = cmlib::image::FloatImage1>
	class GaborFilter {
	public:

		typedef typename Kernel::value_type value_type;
		typedef typename Kernel::size_type size_type;

		GaborFilter(
			value_type orientation, // orientation 
			value_type phrase, // phrase offset
			value_type wavelength, // wavelength of cosine factor
			value_type bandwidth, // bandwidth 
			value_type aspect // the aspect ratio ( x:y, I guess :p )
		)
		{
			value_type sigma = 
				std::sqrt( std::log( static_cast<value_type>(2) ) / 2 ) *
				( wavelength * ( std::pow(2, bandwidth)+1 ) ) / 
				(M_PI*(std::pow(static_cast<value_type>(2), bandwidth)-1));
			value_type factor = static_cast<value_type>(-1)/(2*sigma*sigma);
			value_type gamma = aspect*aspect;
			value_type coef = 2*M_PI/wavelength;

			size_type width = 0;
			size_type height = 0;

			for (; width < std::numeric_limits<size_type>::max(); ++width)
				if (g(width, 0, orientation, phrase, factor, gamma, coef)<1e-6)
					break;
			for (; height < std::numeric_limits<size_type>::max(); ++height)
				if (g(height, 0, orientation, phrase, factor, gamma, coef)<1e-6)
					break;

			my_kernel.resize(2*width+1, 2*height+1);

			int iy = -(my_kernel.height()/2);
			for (size_type y = 0; y < my_kernel.height(); ++y, ++iy) {
				int ix = -(my_kernel.width()/2);
				for (size_type x = 0; x < my_kernel.width(); ++x, ++ix)
					my_kernel(x, y) = 
						g(ix, iy, orientation, phrase, factor, gamma, coef);
			}
		}

		template<class SrcImage>
		const DstImage operator ()(const SrcImage& src) const
		{
			DstImage dst(src.width(), src.height());
			return (*this)(src, dst);
		}

		template<class SrcImage>
		DstImage& operator ()(const SrcImage& src, DstImage& dst) const
		{
			return filter(src, dst, my_kernel);
		}


	protected:
		value_type g(
			int x, int y,
			value_type orientation, 
			value_type phrase, 
			value_type factor, 
			value_type gamma, 
			value_type coef
		) const
		{
			const value_type& o = orientation;
			value_type x_ = x*std::cos(o) - y*std::sin(o);
			value_type y_ = x*std::sin(o) - y*std::cos(o);
			value_type mu = (x_*x_ + gamma*y_*y_) * factor;
			return std::exp(mu) * std::cos(coef*x_ + phrase);
		}

	private:
		Kernel my_kernel;
	};

	template<class SrcImage, class DstImage>
	DstImage& gaussian_filter(
		const SrcImage& src, DstImage& dst, 
		float o, float p, float w, float b, float a
	) {
		return (GaborFilter<DstImage>(o, p, w, b, a))(src, dst);
	}

	template<class SrcImage, class DstImage>
	const DstImage gaussian_filter(
		const SrcImage& src, float o, float p, float w, float b, float a
	) {
		return (GaborFilter<DstImage>(o, p, w, b, a))(src);
	}

} // namespace dip
} // namespace cmlib

#endif // CMLIB_DIP_FILTER_GABORFILTER_H
