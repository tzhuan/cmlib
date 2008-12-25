#ifndef CMLIB_DIP_FILTER_BILATERALFILTER_H
#define CMLIB_DIP_FILTER_BILATERALFILTER_H

#define NO_XML

#include "truncated_kernel_bf/geom.h"
#include "truncated_kernel_bf/fast_lbf.h"

namespace {
	template<class Image>
	class ImageBridge {
	public:
		typedef typename Image::value_type value_type;
		typedef typename Image::size_type size_type;
		typedef typename Image::iterator iterator;
		typedef typename Image::const_iterator const_iterator;

		explicit ImageBridge(Image& image)
			: my_image(image)
		{
			// empty
		}

		value_type& operator ()(size_type x, size_type y)
		{
			return my_image(x, y);
		}

		const value_type& operator ()(size_type x, size_type y) const
		{
			return my_image(x, y);
		}

		void resize(size_type x, size_type y)
		{
			return my_image.resize(x, y);
		}

		size_type x_size() const
		{
			return my_image.width();
		}

		size_type y_size() const
		{
			return my_image.height();
		}

		iterator begin()
		{
			return my_image.begin();
		}

		const_iterator begin() const
		{
			return my_image.begin();
		}

		iterator end()
		{
			return my_image.end();
		}

		const_iterator end() const
		{
			return my_image.end();
		}
	private:
		Image& my_image;
	};

	template<class Image>
	ImageBridge<Image> image_bridge(Image& image)
	{
		return ImageBridge<Image>(image);
	}
} // namespace anonymous

namespace cmlib {
namespace dip {

	template<class DstImage, typename Type = float>
	class BilateralFilter {
	public:

		BilateralFilter(Type sigma_space, Type sigma_range)
			: my_sigma_space(sigma_space), my_sigma_range(sigma_range)
		{
			// empty
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
			typedef typename DstImage::size_type size_type;
			typedef typename DstImage::value_type value_type;

			Type sigma_s2 = my_sigma_space * my_sigma_space;
			Type sigma_r2 = my_sigma_range * my_sigma_range;

			for (size_type py = 0; py < dst.height(); ++py) {
				for (size_type px = 0; px < dst.width(); ++px) {

					// initialization
					value_type wp = 0;
					dst(px, py) = 0;

					for (int qy = py - 3*my_sigma_space; qy <= py + 3*my_sigma_space; ++qy) {
						if (qy < 0)
							continue;
						if (qy >= src.height())
							break;

						for (int qx = px - 3*my_sigma_space; qx <= px + 3*my_sigma_space; ++qx) {
							if (qx < 0)
								continue;
							if (qx >= src.width())
								break;

							Type space2 = norm2<Type>(px, py, qx, qy);
							Type range = src(px, py) - src(qx, qy);

							Type w = 
								gaussian(sigma_s2, space2) * 
								gaussian(sigma_r2, range*range);


							dst(px, py) += w * src(qx, qy);
							wp += w;

						}
					}

					// normalization
					dst(px, py) /= wp;

				}
			}
			return dst;
		}

	protected:

		template<typename T>
		T gaussian(T sigma2, T x2) const
		{
			return std::exp( -x2/(2*sigma2) );
		}

		template<typename T>
		T norm2(T x1, T y1, T x2, T y2) const
		{
			T dx = x2 - x1;
			T dy = y2 - y1;
			return dx*dx* + dy*dy;
		}

	private:
		const Type my_sigma_space, my_sigma_range;
	};

	template<class DstImage, typename Type = float>
	class FastBilateralFilter {
	public:

		FastBilateralFilter(Type sigma_space, Type sigma_range)
			: my_sigma_space(sigma_space), my_sigma_range(sigma_range)
		{
			// empty
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

			ImageBridge<SrcImage> input(const_cast<SrcImage&>(src));
			ImageBridge<DstImage> output(dst);

			Image_filter::fast_LBF(
				input, input,
				my_sigma_space, my_sigma_range, false, 
				&output, &output
			); 
			return dst;
		}

	private:
		const Type my_sigma_space, my_sigma_range;
	};

	/**
	 * @brief Bilateral filter
	 *
	 * This is the standard version of bilateral filter, which is slow but
	 * precise.
	 *
	 * @param src input image
	 * @param dst output image
	 * @param sigma_space spatial parameter
	 * @param sigma_range range parameter
	 * @return return the reference of the output image (dst)
	 */
	template<class SrcImage, class DstImage>
	DstImage& 
	bilateral_filter(const SrcImage& src, DstImage& dst, float sigma_space, float sigma_range)
	{
		return (BilateralFilter<DstImage>(sigma_space, sigma_range))(src, dst);
	}

	/**
	 * @brief Bilateral filter
	 *
	 * This is the standard version of bilateral filter, which is slow but
	 * precise.
	 *
	 * @param src input image
	 * @param sigma_space spatial parameter
	 * @param sigma_range range parameter
	 * @return return the result image
	 */
	template<class SrcImage, class DstImage>
	const DstImage bilateral_filter(const SrcImage& src, float sigma_space, float sigma_range)
	{
		return (BilateralFilter<DstImage>(sigma_space, sigma_range))(src);
	}

	/**
	 * @brief Fast bilateral filter
	 *
	 * This is the accelerative version of bilateral filter by
	 * "Fast Bilateral Filtering for the Display of High-Dynamic-Range Images"
	 * (http://people.csail.mit.edu/fredo/PUBLI/Siggraph2002)
	 *
	 * @param src input image
	 * @param dst output image
	 * @param sigma_space spatial parameter
	 * @param sigma_range range parameter
	 * @return return the reference to output image (dst)
	 */
	template<class SrcImage, class DstImage>
	DstImage& 
	fast_bilateral_filter(const SrcImage& src, DstImage& dst, float sigma_space, float sigma_range)
	{
		return (FastBilateralFilter<DstImage, float>(sigma_space, sigma_range))(src, dst);
	}

	/**
	 * @brief Fast bilateral filter
	 *
	 * This is the accelerative version of bilateral filter by
	 * "Fast Bilateral Filtering for the Display of High-Dynamic-Range Images"
	 * (http://people.csail.mit.edu/fredo/PUBLI/Siggraph2002)
	 *
	 * @param src input image
	 * @param sigma_space spatial parameter
	 * @param sigma_range range parameter
	 * @return return the result image
	 */
	template<class SrcImage, class DstImage>
	const DstImage fast_bilateral_filter(const SrcImage& src, float sigma_space, float sigma_range)
	{
		return (FastBilateralFilter<DstImage, float>(sigma_space, sigma_range))(src);
	}

} // namespace dip
} // namespace cmlib

#endif // CMLIB_DIP_FILTER_BILATERALFILTER_H
