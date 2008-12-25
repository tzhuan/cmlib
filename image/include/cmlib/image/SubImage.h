#ifndef CMLIB_IMAGE_SUBIMAGE_H
#define CMLIB_IMAGE_SUBIMAGE_H

#include <limits>

#include <algorithm>
#include <iterator>

namespace cmlib {
namespace image {
	
	/** 
	 * @brief SubImage is used to extract a rectangle region from 
	 * a Image. 
	 *
	 * SubImage is used to representated a rectangle region for a Image.
	 * SubImage and Image will share the same storage thus
	 * operations on SubImage will affact Image as well.
	 */
	template <typename ImageType>
	class SubImage {
	public:

		typedef SubImage<ImageType> self_type;

		typedef typename ImageType::value_type value_type;
		typedef typename ImageType::difference_type difference_type;
		typedef typename ImageType::size_type size_type;
		typedef typename ImageType::pointer pointer;
		typedef typename ImageType::const_pointer const_pointer;
		typedef typename ImageType::reference reference;
		typedef typename ImageType::const_reference const_reference;

		typedef value_type ColorType;

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

			friend class SubImage;
			typedef Iterator<Type, Pointer, Reference> self_type;

			Iterator()
			{
				// empty
			}

			Iterator(const self_type& i)
				: my_iterator(i.my_iterator),
				my_width(i.my_width), my_height(i.my_height),
				my_subwidth(i.my_subwidth), my_subheight(i.my_subheight),
				my_xoffset(i.my_xoffset), my_yoffset(i.my_yoffset),
				my_offset(i.my_offset),
				my_suboffset(i.my_suboffset),
				my_x(i.my_x), my_y(i.my_y),
				my_subx(i.my_subx), my_suby(i.my_suby)
			{
				// empty
			}

			self_type& operator =(const self_type& i)
			{
				my_iterator = i.my_iterator;
				my_width = i.my_width;
				my_height = i.my_height;
				my_subwidth = i.my_subwidth;
				my_subheight = i.my_subheight;
				my_xoffset = i.my_xoffset;
				my_yoffset = i.my_yoffset;
				my_offset = i.my_offset;
				my_suboffset = i.my_suboffset;
				my_x = i.my_x;
				my_y = i.my_y;
				my_subx = i.my_subx;
				my_suby = i.my_suby;
			}

			reference operator *() const
			{
				return *my_iterator;
			}

			pointer operator ->() const
			{
				return &(operator *());
			}

			Iterator& operator ++()
			{
				return (*this += 1);
			}

			Iterator operator ++(int)
			{
				Iterator tmp(*this);
				++(*this);
				return tmp;
			}

			Iterator operator --()
			{
				return (*this -= 1);
			}

			Iterator operator --(int)
			{
				Iterator tmp(*this);
				--(*this);
				return tmp;
			}

			self_type& operator +=(difference_type n)
			{
				difference_type new_suboffset = my_suboffset + n;
				difference_type new_subx = new_suboffset % static_cast<difference_type>(my_subwidth);
				difference_type new_suby = new_suboffset / static_cast<difference_type>(my_subwidth);

				if (new_subx < 0) {
					--new_suby;
					new_subx += my_subwidth;
				}

				difference_type new_x = new_subx + my_xoffset;
				difference_type new_y = new_suby + my_yoffset;
				difference_type new_offset = my_height*new_y + new_x;

				my_iterator += (new_offset - my_offset);
				my_offset = new_offset;
				my_suboffset = new_suboffset;
				my_x = new_x;
				my_y = new_y;
				my_subx = new_subx;
				my_suby = new_suby;

				return *this;
			}

			self_type& operator -=(difference_type n)
			{
				return (*this += -n);
			}

			self_type operator +(difference_type n) const
			{
				self_type tmp(*this);
				return (tmp += n);
			}

			self_type operator -(difference_type n) const
			{
				self_type tmp(*this);
				return (tmp -= n);
			}

			reference operator [](difference_type n) const
			{
				return *(*this + n);
			}

			difference_type operator -(const self_type& rhs) const
			{
				return my_suboffset - rhs.my_suboffset;
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

			const size_type my_width, my_height;
			const size_type my_subwidth, my_subheight;
			const difference_type my_xoffset, my_yoffset;
			difference_type my_offset;
			difference_type my_suboffset;
			difference_type my_x, my_y;
			difference_type my_subx, my_suby;

			Iterator(
				const ImageType& image,
				const SubImage& subimage,
				difference_type xoffset, 
				difference_type yoffset,
				Type i,
				Type begin
			) : my_iterator(i), 
				my_width(image.width()), my_height(image.height()),
				my_subwidth(subimage.width()), my_subheight(subimage.height()),
				my_xoffset(xoffset), my_yoffset(yoffset),
				my_offset(i - begin)
			{
				my_x = my_offset % my_width;
				my_y = my_offset / my_height;
				my_subx = my_x - my_xoffset;
				my_suby = my_y - my_yoffset;
				my_suboffset = my_width*my_suby + my_subx;
			}

		}; // }}}

		typedef Iterator<typename ImageType::iterator, pointer, reference> iterator;
		typedef Iterator<typename ImageType::const_iterator, const_pointer, const_reference> const_iterator;
		typedef std::reverse_iterator<iterator> reverse_iterator;
		typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

		SubImage(ImageType& i, size_type x, size_type y, size_type w, size_type h)
			: my_image(i), 
			  my_xoffset(x), my_yoffset(y),
			  my_width(w), my_height(h)
		{
			// empty
		}

		~SubImage()
		{
			// empty
		}

		size_type width() const
		{
			return my_width;
		}

		size_type height() const
		{
			return my_height;	
		}

		size_type size() const
		{
			return width()*height();
		}

		reference operator ()(size_type x, size_type y)
		{
			return my_image(x+my_xoffset, y+my_yoffset);
		}

		const_reference operator ()(size_type x, size_type y) const
		{
			return my_image(x+my_xoffset, y+my_yoffset);
		}

		void fill(const_reference pixel)
		{
			std::fill(begin(), end(), pixel);
		}

		template <typename I>
		void replace(const I& img, size_type pos_x = 0, size_type pos_y = 0)
		{
			for (size_type y(pos_y), iy(0); iy < img.height(); ++iy, ++y)
				for (size_type x(pos_x), ix(0); ix < img.width(); ++ix, ++x)
					(*this)(x, y) = img(ix, iy);
		}

		iterator begin()
		{
			return iterator(
				my_image, *this, 
				my_xoffset, my_yoffset, 
				my_image.begin() + my_image.width()*my_yoffset + my_xoffset, 
				my_image.begin()
			);
		}

		const_iterator begin() const
		{
			return const_iterator(
				my_image, *this, 
				my_xoffset, my_yoffset, 
				my_image.begin() + my_image.width()*my_yoffset + my_xoffset, 
				my_image.begin()
			);
		}

		iterator end()
		{
			return begin() + size();
		}

		const_iterator end() const
		{
			return begin() + size();
		}

		reverse_iterator rbegin()
		{
			return reverse_iterator(end());
		}

		const_reverse_iterator rbegin() const
		{
			return const_reverse_iterator(end());
		}

		reverse_iterator rend()
		{
			return reverse_iterator(begin());
		}

		const_reverse_iterator rend() const
		{
			return const_reverse_iterator(begin());
		}

		size_type channels() const
		{
			return my_image.channels();
		}

		void resize(size_type, size_type)
		{
			// empty
		}

	private:

		ImageType& my_image;
		size_type my_xoffset, my_yoffset;
		size_type my_width, my_height;

		self_type& operator =(const self_type&);
	};

	/** @brief n + iterator
	 */
	template<typename ImageType, typename Type, typename Pointer, typename Reference>
	const typename SubImage<ImageType>::template Iterator<Type, Pointer, Reference>
	operator +(
		typename ImageType::difference_type n, 
		const typename SubImage<ImageType>::template Iterator<Type, Pointer, Reference>& i
	) {
		typename SubImage<ImageType>::template Iterator<Type, Pointer, Reference> tmp(i);
		return (tmp += n);
	}

	template <typename ImageType>
	const SubImage<ImageType> sub_image(ImageType& img, size_t x, size_t y, size_t w, size_t h)
	{
		return SubImage<ImageType>(img, x, y, w, h);
	}
	
} // namespace image
} // namespace cmlib

#endif // CMLIB_IMAGE_SUBIMAGE_H
