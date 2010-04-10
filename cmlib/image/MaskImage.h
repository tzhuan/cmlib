#ifndef CMLIB_IMAGE_MASKIMAGE_H
#define CMLIB_IMAGE_MASKIMAGE_H

#include <algorithm>
#include <cassert>
#include <stdexcept>

namespace cmlib {
namespace image {

	/** @brief take a mask over the Image
	 *
	 *  By MaskImage you can take a mask over the Image to prevent the 
	 *  modification of the value without masking. 
	 *
	 *  @tparam ImageType the type of Image
	 */
	template <class ImageType, class MaskType>
	class MaskImage {
	public:
		typedef MaskImage<ImageType> self_type;

		// STL-compliance
		typedef typename ImageType::value_type value_type;
		typedef typename ImageType::pointer pointer;
		typedef typename ImageType::const_pointer const_pointer;
		typedef typename ImageType::reference reference;
		typedef typename ImageType::const_reference const_reference;
		typedef typename ImageType::difference_type difference_type;
		typedef typename ImageType::size_type size_type;

		typedef typename ImageType::iterator iterator;
		typedef typename ImageType::const_iterator const_iterator;
		typedef typename ImageType::reverse_iterator reverse_iterator;
		typedef typename ImageType::const_reverse_iterator const_reverse_iterator;
		MaskImage(ImageType& i, const MaskType& m)
			: my_image(i), my_mask(m)
		{
			// empty
		}

		~MaskImage()
		{
			// empty
		}

		size_type width() const
		{
			return my_image.width();
		}

		size_type height() const
		{
			return my_image.height();	
		}

		size_type size() const
		{
			return my_image.size(); 
		}

		reference operator ()(size_type x, size_type y)
		{
			return ColorTrait<color_type>::select_channel(
				my_image(x, y), my_channel
			);
		}

		const_reference operator ()(size_type x, size_type y) const
		{
			return ColorTrait<color_type>::select_channel(
				my_image(x, y), my_channel
			);
		}

		void fill(const_reference pixel)
		{
			std::fill(begin(), end(), pixel);
		}

		template <typename I>
		void replace(const I& img, size_type pos_x = 0, size_type pos_y = 0)
		{
			// FIXME use exception instead of assert.
			assert( pos_x < width() );
			assert( pos_y < height() );
			assert( pos_x + img.width() <= width() );
			assert( pos_y + img.height() <= height() );

			for (size_type y(pos_y), iy(0); iy < img.height(); ++iy, ++y)
				for (size_type x(pos_x), ix(0); ix < img.width(); ++ix, ++x)
					(*this)(x, y) = img(ix, iy);
		}

		iterator begin()
		{
			return iterator(my_image.begin(), my_channel);
		}

		const_iterator begin() const
		{
			return const_iterator(my_image.begin(), my_channel);
		}

		iterator end()
		{
			return iterator(my_image.end(), my_channel);
		}

		const_iterator end() const
		{
			return const_iterator(my_image.end(), my_channel);
		}

		reverse_iterator rbegin()
		{
			// return reverse_iterator(my_image.rbegin(), my_channel);
			return reverse_iterator(end());
		}

		const_reverse_iterator rbegin() const
		{
			// return const_reverse_iterator(my_image.rbegin(), my_channel);
			return const_reverse_iterator(end());
		}

		reverse_iterator rend()
		{
			// return reverse_iterator(my_image.rend(), my_channel);
			return reverse_iterator(begin());
		}

		const_reverse_iterator rend() const
		{
			// return const_reverse_iterator(my_image.rend(), my_channel);
			return const_reverse_iterator(begin());
		}

		size_type channels() const
		{
			return 1;
		}

		void resize(size_type, size_type)
		{
			// empty
		}

	private:

		ImageType& my_image;
		MaskType my_mask;
		void operator =(const self_type &i);
	};

	/** @brief n + iterator
	 */
	template<typename ImageType, typename Type, typename Pointer, typename Reference>
	const typename MaskImage<ImageType>::template Iterator<Type, Pointer, Reference>
	operator +(
		typename ImageType::difference_type n, 
		const typename MaskImage<ImageType>::template Iterator<Type, Pointer, Reference>& i
	) {
		typename MaskImage<ImageType>::template Iterator<Type, Pointer, Reference> tmp(i);
		return (tmp += n);
	}

	/** @brief the function to help to make a MaskImage
	 *  @param image the base image
	 *  @param channel the slice channel number
	 */
	template<typename ImageType>
	const MaskImage<ImageType> mask_image(ImageType& image, size_t channel)
	{
		return MaskImage<ImageType>(image, channel);
	}
	
} // namespace image
} // namespace cmlib

#endif // IMAGE_MASKIMAGE_H
