#ifndef CMLIB_DIP_FILTER_FILTER_H
#define CMLIB_DIP_FILTER_FILTER_H

#include "algorithm"
#include "Sampler.h"

namespace { // anonymous namesapce

	/**
	 * @brief partial 2-D filter 
	 *
	 * This function filter the block of 2-D array (eg. image) with kernel, 
	 * and store the result on dst. The block is (xbegin, xend, ybegin, yend)
	 *
	 * @tparam Normalize determine the filter should be normalized or not
	 * @param sampler input image with specified sampling method
	 * @param dst output image
	 * @param kernel filter kernel
	 * @param cx x coordinate of kernel center
	 * @param cy y coordinate of kernel center
	 * @param xbegin x's begin index
	 * @param xend x' end index plus one
	 * @param ybegin y's begin index
	 * @param yend y's end index plus one
	 */
	template<class Sampler, class DstImage, class Kernel, bool Normalize, bool Outside>
	DstImage&
	filter(
		const Sampler& sampler, 
		DstImage& dst, 
		const Kernel& kernel, 
		typename Kernel::size_type cx,
		typename Kernel::size_type cy,
		typename DstImage::size_type xbegin,
		typename DstImage::size_type xend,
		typename DstImage::size_type ybegin,
		typename DstImage::size_type yend
	)
	{
		typedef typename DstImage::size_type size_type;
		typedef typename Sampler::difference_type difference_type;
		for (size_type y = ybegin; y < yend; ++y) {
			for (size_type x = xbegin; x < xend; ++x) {
				typename DstImage::value_type value(0);
				typename Kernel::value_type weight(0);
				for (int iy = 0; iy < kernel.height(); ++iy) {
					difference_type sy = y + iy - cy;
					for (int ix = 0; ix < kernel.width(); ++ix) {
						difference_type sx = x + ix - cx;
						bool countable = true;
						if (Outside)
							value += sampler(sx, sy, countable) * kernel(ix, iy);
						else 
							value += sampler(sx, sy) * kernel(ix, iy);

						if (Normalize && Outside && countable)
							weight += kernel(ix, iy);
					}
				}
				dst(x, y) = value;
				if (Normalize)
					dst(x, y) /= weight;
			}
		}
		return dst;
	}

	/**
	 * @brief 2-D filter
	 *
	 * This function filter the sampler with kernel, and store the result
	 * on dst.
	 *
	 * @tparam Normalize determine the filter should be normalized or not
	 * @param sampler input image with specified sampling method
	 * @param dst output image
	 * @param kernel filter kernel
	 * @param cx x coordinate of kernel center
	 * @param cy y coordinate of kernel center
	 */
	template<class Sampler, class DstImage, class Kernel, bool Normalize>
	DstImage& 
	filter(
		const Sampler& sampler, 
		DstImage& dst, 
		const Kernel& kernel, 
		typename Kernel::size_type cx,
		typename Kernel::size_type cy
	) {
		typedef typename DstImage::size_type size_type;
		typedef typename DstImage::value_type value_type;
		typedef typename Kernel::difference_type difference_type;


		/* 
		 * +---+---+---+
		 * |     1     |
		 * +---+---+---+
		 * | 2 | 3 | 4 |
		 * +---+---+---+
		 * |     5     |
		 * +---+---+---+
		 *
		 * In 1, 2, 4, 5, call sampler(x, y, countable)
		 * In 3, call sampler(x, y)
		 */

		// 1
		filter<Sampler, DstImage, Kernel, Normalize, true>
		(sampler, dst, kernel, cx, cy, 0, dst.width(), 0, cy);

		// 2 
		filter<Sampler, DstImage, Kernel, Normalize, true>
		(sampler, dst, kernel, cx, cy, 0, cx, cy, dst.height() - kernel.height() + cy);

		// 3 
		filter<Sampler, DstImage, Kernel, false, false>
		(sampler, dst, kernel, cx, cy, cx, dst.width() - kernel.width() + cx, cy, dst.height() - kernel.height() + cy);

		// 4 
		filter<Sampler, DstImage, Kernel, Normalize, true>
		(sampler, dst, kernel, cx, cy, dst.width() - kernel.width() + cx, dst.width(), cy, dst.height() - kernel.height() + cy);

		// 5 
		filter<Sampler, DstImage, Kernel, Normalize, true>
		(sampler, dst, kernel, cx, cy, 0, dst.width(), dst.height() - kernel.height() + cy, dst.height());

		return dst;
	}

	/**
	 * @brief 2-D filter
	 *
	 * This function filter the sampler with kernel, and store the 
	 * result on dst. The center of kernel is set to the 
	 * default: ((width-1)/2, (height-1)/2).
	 *
	 * @tparam determine the filter should be normalized or not
	 * @param sampler input image with specified sampling method
	 * @param dst output image
	 * @param kernel filter kernel
	 */
	template<class Sampler, class DstImage, class Kernel, bool Normalize>
	DstImage& 
	filter(
		const Sampler& sampler, 
		DstImage& dst, 
		const Kernel& kernel
	) {
		return filter<Sampler, DstImage, Kernel, Normalize>
			(sampler, dst, kernel, (kernel.width()-1)/2, (kernel.height()-1)/2);
	}

	/**
	 * @brief 2-D convolution
	 *
	 * This function convolve the sampler with kernel, and store the 
	 * result on dst.
	 *
	 * @tparam determine the filter should be normalized or not
	 * @param sampler input image with specified sampling method
	 * @param dst output image
	 * @param kernel filter kernel
	 * @param cx x coordinate of kernel center
	 * @param cy y coordinate of kernel center
	 */
	template<class Sampler, class DstImage, class Kernel, bool Normalize>
	DstImage& 
	convolve(
		const Sampler& sampler, 
		DstImage& dst, 
		Kernel kernel,
		typename Kernel::size_type cx,
		typename Kernel::size_type cy
	) {
		// reverse the kernel
		typedef typename Kernel::size_type size_type;
		for (size_type y1 = 0, y2 = kernel.height()-1; y1 < y2; ++y1, --y2)
			for (size_type x1 = 0, x2 = kernel.width()-1; x1 < x2; ++x1, --x2)
				std::swap(kernel(x1, y1), kernel(x2, y2)); 

		return filter<Sampler, DstImage, Kernel, Normalize>
			(sampler, dst, kernel, kernel.width()-cx, kernel.height()-cy);
	}

	/**
	 * @brief 2-D convolution
	 *
	 * This function convolve the sampler with kernel, and store the 
	 * result on dst. The center of kernel is set to the default: 
	 * ((width-1)/2, (height-1)/2).
	 *
	 * @tparam determine the filter should be normalized or not
	 * @param sampler input image with specified sampling method
	 * @param dst output image
	 * @param sampler used for sampling
	 * @param kernel filter kernel
	 */
	template<class Sampler, class DstImage, class Kernel, bool Normalize>
	DstImage& 
	convolve(
		const Sampler& sampler, 
		DstImage& dst, 
		const Kernel& kernel
	) {
		return convolve<Sampler, DstImage, Kernel, Normalize>
			(sampler, dst, kernel, (kernel.width()-1)/2, (kernel.height()-1)/2);
	}

} // anonymous namespace

namespace cmlib {
namespace dip {

	template<class DstImage, class Kernel, bool Normalize = false>
	class Convolve {
	public:

		typedef typename Kernel::value_type value_type;

		explicit Convolve(const Kernel& kernel)
			: my_kernel(kernel), my_cx((kernel.width()-1)/2), my_cy((kernel.height()-1)/2)
		{
			// empty
		}

		Convolve(const Kernel& kernel, value_type cx, value_type cy)
			: my_kernel(kernel), my_cx(cx), my_cy(cy)
		{
			// empty
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
			return convolve<Sampler, DstImage, Kernel, Normalize>
				(sampler, dst, my_kernel, my_cx, my_cy);
		}

	private:
		const Kernel& my_kernel;
		const value_type my_cx, my_cy;
	};


	template<class DstImage, class Kernel, bool Normalize = false>
	class Filter {
	public:

		typedef typename Kernel::value_type value_type;
		typedef typename Kernel::size_type size_type;

		Filter(const Kernel& kernel, size_type cx, size_type cy)
			: my_kernel(kernel), my_cx(cx), my_cy(cy)
		{
			// empty
		}

		explicit Filter(const Kernel& kernel)
			: my_kernel(kernel), my_cx((kernel.width()-1)/2), my_cy((kernel.height()-1)/2)
		{
			// empty
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
			return ::filter<Sampler, DstImage, Kernel, Normalize>
				(sampler, dst, my_kernel, my_cx, my_cy);
		}

	private:
		const Kernel& my_kernel;
		const value_type my_cx, my_cy;
	};

	template<class Sampler, class DstImage, class Kernel>
	DstImage& convolve(const Sampler& sampler, DstImage& dst, const Kernel& kernel, typename Kernel::size_type x, typename Kernel::size_type y, bool normalize = false)
	{
		if (normalize)
			return (Convolve<DstImage, Kernel, true>(kernel, x, y))(sampler, dst);
		else
			return (Convolve<DstImage, Kernel, false>(kernel, x, y))(sampler, dst);
	}

	template<class Sampler, class DstImage, class Kernel>
	DstImage& convolve(const Sampler& sampler, DstImage& dst, const Kernel& kernel, bool normalize = false)
	{
		if (normalize)
			return (Convolve<DstImage, Kernel, true>(kernel))(sampler, dst);
		else
			return (Convolve<DstImage, Kernel, false>(kernel))(sampler, dst);
	}

	template<class Sampler, class DstImage, class Kernel>
	const DstImage convolve(const Sampler& sampler, const Kernel& kernel, typename Kernel::size_type x, typename Kernel::size_type y, bool normalize = false)
	{
		if (normalize)
			return (Convolve<DstImage, Kernel, true>(kernel, x, y))(sampler);
		else
			return (Convolve<DstImage, Kernel, false>(kernel, x, y))(sampler);
	}

	template<class Sampler, class DstImage, class Kernel>
	const DstImage convolve(const Sampler& sampler, const Kernel& kernel, bool normalize = false)
	{
		if (normalize)
			return (Convolve<DstImage, Kernel, true>(kernel))(sampler);
		else
			return (Convolve<DstImage, Kernel, false>(kernel))(sampler);
	}

	template<class Sampler, class DstImage, class Kernel>
	DstImage& filter(const Sampler& sampler, DstImage& dst, const Kernel& kernel, typename Kernel::size_type x, typename Kernel::size_type y, bool normalize = false)
	{
		if (normalize)
			return (Filter<DstImage, Kernel, true>(kernel, x, y))(sampler, dst);
		else
			return (Filter<DstImage, Kernel, false>(kernel, x, y))(sampler, dst);
	}

	template<class Sampler, class DstImage, class Kernel>
	DstImage& filter(const Sampler& sampler, DstImage& dst, const Kernel& kernel, bool normalize = false)
	{
		if (normalize)
			return (Filter<DstImage, Kernel, true>(kernel))(sampler, dst);
		else
			return (Filter<DstImage, Kernel, false>(kernel))(sampler, dst);
	}

	template<class SrcImage, class DstImage, class Kernel>
	const DstImage filter(const SrcImage& src, const Kernel& kernel, typename Kernel::size_type x, typename Kernel::size_type y, bool normalize = false)
	{
		if (normalize)
			return (Filter<DstImage, Kernel, true>(kernel, x, y))(src);
		else
			return (Filter<DstImage, Kernel, false>(kernel, x, y))(src);
	}

	template<class SrcImage, class DstImage, class Kernel>
	const DstImage filter(const SrcImage& src, const Kernel& kernel, bool normalize = false)
	{
		if (normalize)
			return (Filter<DstImage, Kernel, true>(kernel))(src);
		else
			return (Filter<DstImage, Kernel, false>(kernel))(src);
	}

} // namespace dip
} // namespace cmlib

#endif // CMLIB_DIP_FILTER_FILTER_H
