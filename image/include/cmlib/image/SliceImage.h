#ifndef CMLIB_IMAGE_SLICEIMAGE_H
#define CMLIB_IMAGE_SLICEIMAGE_H

#include <algorithm>
#include <cassert>
#include <stdexcept>

namespace cmlib {
namespace image {

	/** @brief class SliceImage is used to take one slice of the Image
	 *
	 *  SliceImage is a class which is used to extract one slice 
	 *  (say, the specified channel of the whole pixels) of the Image and
	 *  you can treat it as an one-channel Image. The operations on 
	 *  SliceImage will be affected on the reference Image as well. 
	 *  It supports almost all interface of the Image except the memory
	 *  operation like resize(), allocate(), etc.
	 *
	 *  @tparam ImageType the type of Image
	 */
	template <typename ImageType>
	class SliceImage {
	public:
		typedef SliceImage<ImageType> self_type;
		typedef typename ImageType::value_type color_type;

		typedef typename ColorTrait<color_type>::BaseType value_type;
		typedef typename ImageType::difference_type difference_type;
		typedef typename ImageType::size_type size_type;
		typedef value_type* pointer;
		typedef const pointer const_pointer;
		typedef value_type& reference;
		typedef const value_type& const_reference;

		typedef value_type ColorType;

		/** @brief iterator type of SliceImage
		 */
		template<typename Type, typename Pointer, typename Reference>
		class Iterator:  // {{{
			public std::iterator<
				std::random_access_iterator_tag, 
				typename ImageType::value_type, 
				typename ImageType::difference_type, 
				Pointer, 
				Reference
			> {
		public:

			friend class SliceImage;
			typedef Iterator<Type, Pointer, Reference> self_type;

			Iterator()
			{
				// empty
			}

			Iterator(const self_type& i)
				: my_iterator(i.my_iterator), my_channel(i.my_channel)
			{
				// empty
			}

			self_type& operator =(const self_type& i)
			{
				my_iterator = i.my_iterator;
				my_channel = i.my_channel;
			}

			reference operator *() const
			{
				return 
					ColorTrait<color_type>::select_channel(
						*my_iterator, my_channel
					);
			}

			pointer operator ->() const
			{
				return &(operator *());
			}

			self_type& operator ++()
			{
				++my_iterator;
				return *this;
			}

			const self_type operator ++(int)
			{
				self_type tmp(*this);
				++(*this);
				return tmp;
			}

			self_type& operator --()
			{
				--my_iterator;
				return *this;
			}

			const self_type operator --(int)
			{
				self_type tmp(*this);
				--(*this);
				return tmp;
			}

			self_type& operator +=(difference_type n)
			{
				my_iterator += n;
				return *this;
			}

			self_type& operator -=(difference_type n)
			{
				my_iterator -= n;
				return *this;
			}

			const self_type operator +(difference_type n) const
			{
				self_type tmp(*this);
				return (tmp += n);
			}

			const self_type operator -(difference_type n) const
			{
				self_type tmp(*this);
				retuern (tmp -= n);
			}

			reference operator [](difference_type n) const
			{
				return *(*this + n);
			}

			difference_type operator -(const self_type& rhs) const
			{
				return my_iterator - rhs.my_iterator;
			}

			bool operator <(const self_type& rhs) const
			{
				return (my_iterator < rhs.my_iterator);
			}

			bool operator >(const self_type& rhs) const
			{
				return (my_iterator > rhs.my_iterator);
			}

			bool operator ==(const self_type& rhs) const
			{
				return (my_iterator == rhs.my_iterator);
			}

			bool operator !=(const self_type& rhs) const
			{
				return !( (*this) == rhs );
			}

			bool operator <=(const self_type& rhs) const
			{
				return (my_iterator <= rhs.my_iterator);
			}

			bool operator >=(const self_type& rhs) const
			{
				return (my_iterator >= rhs.my_iterator);
			}

		private:

			Type my_iterator;
			size_type my_channel;

			Iterator(Type i, size_type c)
				: my_iterator(i), my_channel(c)
			{
				// empty
			}
		}; // }}}

		typedef Iterator<typename ImageType::iterator, pointer, reference> iterator;
		typedef Iterator<typename ImageType::const_iterator, const_pointer, const_reference> const_iterator;
		typedef std::reverse_iterator<iterator> reverse_iterator;
		typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

		SliceImage(ImageType& i, size_type c)
			: my_image(i), my_channel(c)
		{
			// empty
		}

		~SliceImage()
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
		size_type my_channel;
		void operator =(const self_type &i);
	};

	/** @brief n + iterator
	 */
	template<typename ImageType, typename Type, typename Pointer, typename Reference>
	const typename SliceImage<ImageType>::template Iterator<Type, Pointer, Reference>
	operator +(
		typename ImageType::difference_type n, 
		const typename SliceImage<ImageType>::template Iterator<Type, Pointer, Reference>& i
	) {
		typename SliceImage<ImageType>::template Iterator<Type, Pointer, Reference> tmp(i);
		return (tmp += n);
	}

	/** @brief the function to help to make a SliceImage
	 *  @param image the base image
	 *  @param channel the slice channel number
	 */
	template<typename ImageType>
	const SliceImage<ImageType> slice_image(ImageType& image, size_t channel)
	{
		return SliceImage<ImageType>(image, channel);
	}
	
} // namespace image
} // namespace cmlib

#endif // IMAGE_SLICEIMAGE_H
