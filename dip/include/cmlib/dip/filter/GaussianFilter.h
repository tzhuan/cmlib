#ifndef CMLIB_DIP_FILTER_GAUSSIANFILTER_H
#define CMLIB_DIP_FILTER_GAUSSIANFILTER_H

#include <cmath>

#include "Filter.h"

namespace cmlib {
namespace dip {

	template<class DstImage, class Kernel = cmlib::image::FloatImage1>
	class GaussianFilter {
	public:

		typedef typename Kernel::value_type value_type;

		explicit GaussianFilter(value_type sigma)
			: my_kernel(std::ceil(sigma)*6+1, std::ceil(sigma)*6+1)
		{
			init_kernel(sigma, sigma);
		}

		GaussianFilter(value_type sigma_x, value_type sigma_y)
			: my_kernel(std::ceil(sigma_x)*6+1, std::ceil(sigma_y)*6+1)
		{
			init_kernel(sigma_x, sigma_y);
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
			return Filter<DstImage, Kernel, true>(my_kernel)
				(basic_sampler(src), dst);
		}

	protected:

		void init_kernel(value_type sigma_x, value_type sigma_y)
		{
			typedef typename Kernel::value_type value_type;
			typedef typename Kernel::size_type size_type;
			typedef typename Kernel::difference_type difference_type;

			value_type sum(0);
			difference_type dy = -(my_kernel.height()/2);
			for (size_type y = 0; y < my_kernel.height(); ++y, ++dy) {
				difference_type dx = -(my_kernel.width()/2);
				for (size_type x = 0; x < my_kernel.width(); ++x, ++dx)
					sum += my_kernel(x, y) = 
						std::exp( -(dx*dx + dy*dy) / (2*sigma_x*sigma_y) );
			}

			sum = static_cast<value_type>(1)/sum;
			for (size_type i = 0; i < my_kernel.size(); ++i)
				my_kernel[i] *= sum;

		}

	private:
		Kernel my_kernel;
	};

	template<class SrcImage, class DstImage>
	DstImage& gaussian_filter(const SrcImage& src, DstImage& dst, float sigma)
	{
		return (GaussianFilter<DstImage>(sigma))(src, dst);
	}

	template<class SrcImage, class DstImage>
	DstImage& gaussian_filter(const SrcImage& src, DstImage& dst, float sigma_x, float sigma_y)
	{
		return (GaussianFilter<DstImage>(sigma_x, sigma_y))(src, dst);
	}

	template<class SrcImage, class DstImage>
	const DstImage gaussian_filter(const SrcImage& src, float sigma)
	{
		return (GaussianFilter<DstImage>(sigma))(src);
	}

	template<class SrcImage, class DstImage>
	const DstImage gaussian_filter(const SrcImage& src, float sigma_x, float sigma_y)
	{
		return (GaussianFilter<DstImage>(sigma_x, sigma_y))(src);
	}

} // namespace dip
} // namespace cmlib

#endif // CMLIB_DIP_FILTER_GAUSSIANFILTER_H
