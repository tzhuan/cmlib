#ifndef CMLIB_DIP_FILTER_BOXFILTER_H
#define CMLIB_DIP_FILTER_BOXFILTER_H

#include "Filter.h"

namespace cmlib {
namespace dip {

	template<class DstImage, class Kernel = cmlib::image::FloatImage1>
	class BoxFilter {
	public:

		typedef typename Kernel::size_type size_type;

		explicit BoxFilter(size_type size)
			: my_kernel(size, size)
		{
			init_kernel();
		}

		BoxFilter(size_type size_x, size_type size_y)
			: my_kernel(size_x, size_y)
		{
			init_kernel();
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
			return Filter<DstImage, Kernel, true>(my_kernel)(basic_sampler(src), dst);
		}

	protected:

		void init_kernel()
		{
			typedef typename Kernel::value_type value_type;
			typedef typename Kernel::size_type size_type;

			value_type value(
				static_cast<value_type>(1) / 
				(my_kernel.width() * my_kernel.height())
			);

			for (size_type y = 0; y < my_kernel.height(); ++y)
				for (size_type x = 0; x < my_kernel.width(); ++x)
					my_kernel(x, y) = value;
		}

	private:
		Kernel my_kernel;
	};

	template<class SrcImage, class DstImage>
	DstImage& box_filter(const SrcImage& src, DstImage& dst, size_t size)
	{
		return (BoxFilter<DstImage>(size))(src, dst);
	}

	template<class SrcImage, class DstImage>
	DstImage& box_filter(const SrcImage& src, DstImage& dst, size_t size_x, size_t size_y)
	{
		return (BoxFilter<DstImage>(size_x, size_y))(src, dst);
	}

	template<class SrcImage, class DstImage>
	const DstImage box_filter(const SrcImage& src, size_t size)
	{
		return (BoxFilter<DstImage>(size))(src);
	}

	template<class SrcImage, class DstImage>
	const DstImage box_filter(const SrcImage& src, size_t size_x, size_t size_y)
	{
		return (BoxFilter<DstImage>(size_x, size_y))(src);
	}

} // namespace dip
} // namespace cmlib

#endif // CMLIB_DIP_FILTER_BOXFILTER_H
