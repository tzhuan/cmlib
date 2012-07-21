/*
Copyright NTU CSIE CMLAB 2005 - 2012
Distributed under the Boost Software License, Version 1.0.
(See accompanying file ../../../LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef CMLIB_DIP_FILTER_FILTER_H
#define CMLIB_DIP_FILTER_FILTER_H

#include <cmlib/dip/filter/Sampler.h>

namespace { // anonymous namesapce

	/**
	 * @brief partial 2-D filter
	 *
	 * This function filter the block of 2-D array (eg. image) with kernel,
	 * and store the result on dst.
	 * Block size: width by height;
	 * sampler: [s_dx, s_dx+width) by [s_dy, s_dy+height);
	 * dst: [d_dx, d_dx+width) by [d_dy, d_dy+height).
	 *
	 * @tparam Normalize determine the filter should be normalized or not
	 *
	 * @param sampler input image with specified sampling method
	 * @param sx begin x index of sampler
	 * @param sy begin y index of sampler
	 *
	 * @param dst output image
	 * @param dx begin x index of dst
	 * @param dy begin y index of dst
	 *
	 * @param kernel filter kernel
	 * @param cx x coordinate of kernel center
	 * @param cy y coordinate of kernel center
	 *
	 * @param width width
	 * @param height height
	 */
	template<bool Normalize, bool Outside, class Sampler, class DstImage, class Kernel>
	DstImage&
	filter(
		const Sampler& sampler,
		typename Sampler::size_type sx,
		typename Sampler::size_type sy,

		DstImage& dst,
		typename DstImage::size_type dx,
		typename DstImage::size_type dy,

		const Kernel& kernel,
		typename Kernel::size_type cx,
		typename Kernel::size_type cy,

		typename DstImage::size_type width,
		typename DstImage::size_type height
	)
	{
		typedef typename DstImage::size_type size_type;
		typedef typename Sampler::difference_type difference_type;
		for (size_type h = 0, isy = sy, idy = dy; h < height; ++h, ++isy, ++idy) {
			for (size_type w = 0, isx = sx, idx = dx; w < width; ++w, ++isx, ++idx) {
				typename DstImage::value_type value(0);
				typename Kernel::value_type weight(0);
				for (difference_type iy = 0; iy < static_cast<difference_type>(kernel.height()); ++iy) {
					difference_type y = static_cast<difference_type>(isy) + iy - cy;
					for (difference_type ix = 0; ix < static_cast<difference_type>(kernel.width()); ++ix) {
						difference_type x = static_cast<difference_type>(isx) + ix - cx;
						if (Outside) {
							bool countable = true;
							value += sampler(x, y, countable) * kernel(ix, iy);
							if (Normalize && countable)
								weight += kernel(ix, iy);
						} else {
							value += sampler(x, y) * kernel(ix, iy);
							if (Normalize)
								weight += kernel(ix, iy);
						}
					}
				}
				dst(idx, idy) = value;
				if (Normalize)
					dst(dx, dy) /= weight;

			}
		}
		return dst;
	}

	/**
	 * @brief partial 2-D filter for BasicSampler
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
	/*
	template<bool Normalize, bool Outside, class SrcImage, class DstImage, class Kernel>
	DstImage&
	filter(
		const cmlib::dip::BasicSampler<SrcImage>& sampler,
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
		typedef typename cmlib::dip::BasicSampler<SrcImage>::difference_type difference_type;
		for (size_type y = ybegin; y < yend; ++y) {
			for (size_type x = xbegin; x < xend; ++x) {
				typename DstImage::value_type value(0);
				typename Kernel::value_type weight(0);
				for (difference_type iy = 0; iy < static_cast<difference_type>(kernel.height()); ++iy) {
					difference_type sy = y + iy - cy;
					if (Outside) {
						if (sy < 0)
							continue;
						else if (sy >= static_cast<difference_type>(sampler.height()))
							break;
					}
					for (difference_type ix = 0; ix < static_cast<difference_type>(kernel.width()); ++ix) {
						difference_type sx = x + ix - cx;
						if (Outside) {
							if (sx < 0)
								continue;
							else if (sx >= static_cast<difference_type>(sampler.width()))
								break;
						}
						value += sampler(sx, sy) * kernel(ix, iy);
						if (Normalize)
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
	*/

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
	template<bool Normalize, bool Inplace, class Sampler, class DstImage, class Kernel>
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
		 * If user requests inplace operation, or the sampler specify that
		 * the area should be normalized when outside, then inplace filtering
		 * should be used. Otherwise, do not use inplace operation to improve
		 * the performance.
		 */
		if (Inplace || Sampler::outside_normalize) {
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
			filter<Normalize || Sampler::outside_normalize, true>(
				sampler, 0, 0,
				dst, 0, 0,
				kernel, cx, cy,
				dst.width(), cy
			);

			// 2
			filter<Normalize || Sampler::outside_normalize, true>(
				sampler, 0, cy,
				dst, 0, cy,
				kernel, cx, cy,
				cx, dst.height() - kernel.height()
			);

			// 3
			filter<Normalize, false>(
				sampler, cx, cy,
				dst, cx, cy,
				kernel, cx, cy,
				dst.width() - kernel.width(),
				dst.height() - kernel.height()
			);

			// 4
			filter<Normalize || Sampler::outside_normalize, true>(
				sampler, dst.width() - kernel.width() + cx, cy,
				dst, dst.width() - kernel.width() + cx, cy,
				kernel, cx, cy,
				kernel.width() - cx, dst.height() - kernel.height()
			);

			// 5
			filter<Normalize || Sampler::outside_normalize, true>(
				sampler, 0, dst.height() - kernel.height() + cy,
				dst, 0, dst.height() - kernel.height() + cy,
				kernel, cx, cy,
				dst.width(), kernel.height() - cy
			);

		} else {
			typedef typename Sampler::image_type SrcImage;
			typedef typename Sampler::value_type src_value_type;
			typedef cmlib::image::Image<src_value_type> TmpImage;
			TmpImage tmp(
				sampler.width() + kernel.width(),
				sampler.height() + kernel.height()
			);

			for (size_type h = 0; h < tmp.height(); ++h) {
				difference_type y =
					static_cast<difference_type>(h) -
					static_cast<difference_type>(cy);
				for (size_type w = 0; w < tmp.width(); ++w) {
					difference_type x =
						static_cast<difference_type>(w) -
						static_cast<difference_type>(cx);
					bool dummy;
					tmp(w, h) = sampler(x, y, dummy);
				}
			}

			/*
			const SrcImage& src(sampler.image());
			for (size_type h = 0; h < sampler.height(); ++h) {
				std::copy(&src(h, 0), &src(h, 0) + src.width(), tmp(h + cy, cx));
			}
			*/

			filter<Normalize, false>(
				cmlib::dip::DefaultSampler<TmpImage>(tmp), cx, cy,
				dst, 0, 0,
				kernel, cx, cy,
				dst.width(),
				dst.height()
			);

		}

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
	template<bool Normalize, bool Inplace, class Sampler, class DstImage, class Kernel>
	DstImage&
	filter(
		const Sampler& sampler,
		DstImage& dst,
		const Kernel& kernel
	) {
		return filter<Normalize, Inplace>
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
	template<bool Normalize, bool Inplace, class Sampler, class DstImage, class Kernel>
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

		return filter<Normalize, Inplace>
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
	template<bool Normalize, bool Inplace, class Sampler, class DstImage, class Kernel>
	DstImage&
	convolve(
		const Sampler& sampler,
		DstImage& dst,
		const Kernel& kernel
	) {
		return convolve<Normalize, Inplace>
			(sampler, dst, kernel, (kernel.width()-1)/2, (kernel.height()-1)/2);
	}

} // anonymous namespace

namespace cmlib {
namespace dip {

	template<class DstImage, class Kernel, bool Normalize = false, bool Inplace = false>
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
			return convolve<Normalize, Inplace>
				(sampler, dst, my_kernel, my_cx, my_cy);
		}

	private:
		const Kernel& my_kernel;
		const value_type my_cx, my_cy;
	};


	template<class DstImage, class Kernel, bool Normalize = false, bool Inplace = false>
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
		DstImage operator ()(const Sampler& sampler) const
		{
			DstImage dst(sampler.width(), sampler.height());
			return (*this)(sampler, dst);
		}

		template<class Sampler>
		DstImage& operator ()(const Sampler& sampler, DstImage& dst) const
		{
			return ::filter<Normalize, Inplace>
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

	template<class Sampler, class DstImage, class Kernel>
	DstImage filter(const Sampler& sampler, const Kernel& kernel, typename Kernel::size_type x, typename Kernel::size_type y, bool normalize = false)
	{
		if (normalize)
			return (Filter<DstImage, Kernel, true>(kernel, x, y))(sampler);
		else
			return (Filter<DstImage, Kernel, false>(kernel, x, y))(sampler);
	}

	template<class Sampler, class DstImage, class Kernel>
	DstImage filter(const Sampler& sampler, const Kernel& kernel, bool normalize = false)
	{
		if (normalize)
			return (Filter<DstImage, Kernel, true>(kernel))(sampler);
		else
			return (Filter<DstImage, Kernel, false>(kernel))(sampler);
	}

} // namespace dip
} // namespace cmlib

#endif // CMLIB_DIP_FILTER_FILTER_H
