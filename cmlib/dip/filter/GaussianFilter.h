/*
Copyright NTU CSIE CMLAB 2005 - 2012
Distributed under the Boost Software License, Version 1.0.
(See accompanying file ../../../LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef CMLIB_DIP_FILTER_GAUSSIANFILTER_H
#define CMLIB_DIP_FILTER_GAUSSIANFILTER_H

#include <cmath>

#include <cmlib/dip/filter/Filter.h>

namespace cmlib {
namespace dip {

	template<class DstImage, class Kernel = cmlib::image::FloatImage1, bool Inplace = false>
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

		template<class Sampler>
		const DstImage operator ()(const Sampler& sampler) const
		{
			DstImage dst(sampler.width(), sampler.height());
			return (*this)(sampler, dst);
		}

		template<class Sampler>
		DstImage& operator ()(const Sampler& sampler, DstImage& dst) const
		{
			return Filter<DstImage, Kernel, false, Inplace>(my_kernel)(sampler, dst);
		}

	protected:

		void init_kernel(value_type sigma_x, value_type sigma_y)
		{
			typedef typename Kernel::value_type value_type;
			typedef typename Kernel::size_type size_type;
			typedef typename Kernel::difference_type difference_type;

			// value_type eps = 0.00000000000000022204460492503130808472633361816406;

			value_type sum(0);
			difference_type dy = -static_cast<difference_type>(my_kernel.height()/2);
			for (size_type y = 0; y < my_kernel.height(); ++y, ++dy) {
				difference_type dx = -static_cast<difference_type>(my_kernel.width()/2);
				for (size_type x = 0; x < my_kernel.width(); ++x, ++dx) {
					my_kernel(x, y) =
						std::exp( -(dx*dx + dy*dy) / (2*sigma_x*sigma_y) );

					/*
					if (my_kernel(x, y) < eps)
						my_kernel(x, y) = 0;
					*/

					sum += my_kernel(x, y);
				}	
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
		return (GaussianFilter<DstImage>(sigma))(
			DefaultSampler<SrcImage>(src), dst
		);
	}

	template<class SrcImage, class DstImage>
	DstImage& gaussian_filter(const SrcImage& src, DstImage& dst, float sigma_x, float sigma_y)
	{
		return (GaussianFilter<DstImage>(sigma_x, sigma_y))(
			DefaultSampler<SrcImage>(src), dst
		);
	}

	template<class SrcImage, class DstImage>
	const DstImage gaussian_filter(const SrcImage& src, float sigma)
	{
		return (GaussianFilter<DstImage>(sigma))(
			DefaultSampler<SrcImage>(src)
		);
	}

	template<class SrcImage, class DstImage>
	const DstImage gaussian_filter(const SrcImage& src, float sigma_x, float sigma_y)
	{
		return (GaussianFilter<DstImage>(sigma_x, sigma_y))(
			DefaultSampler<SrcImage>(src)
		);
	}

} // namespace dip
} // namespace cmlib

#endif // CMLIB_DIP_FILTER_GAUSSIANFILTER_H
