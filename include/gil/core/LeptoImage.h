#ifndef GIL_LEPTOTIMAGE_H
#define GIL_LEPTOTIMAGE_H

#include <algorithm>
#include <cassert>

#include "Color.h"

// FIXME: Now LeptoImage can only be used when image with more than 1 channel.

namespace gil {
	
	template<typename T> 
	class LeptoImage;

	template <typename T, size_t C>
	class LeptoImage< Color<T, C> > {
		public:
			typedef Color<T, C> I;
			typedef typename I::value_type value_type;
			typedef typename I::reference reference;
			typedef typename I::const_reference const_reference;
			typedef typename I::difference_type difference_type;
			typedef typename I::size_type size_type;

			template<typename P> class Iterator;
			typedef Iterator<value_type> iterator;
			typedef Iterator<const value_type> const_iterator;

			typedef typename I::Converter Converter;

			LeptoImage(I& i, size_type c)
				: my_image(i), my_channel(c)
			{
				// FIXME: throw exception instead of assert
				assert(c < my_image.channels());
			}

			~LeptoImage()
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

			reference operator ()(size_type x, size_type y)
			{
				if (my_image.channels() == 1) return my_image(x, y);
				return my_image(x, y)[my_channel];
			}

			const_reference operator ()(size_type x, size_type y) const
			{
				if (my_image.channels() == 1) return my_image(x, y);
				return my_image(x, y)[my_channel];
			}

			void fill(const_reference pixel)
			{
				std::fill(begin(), end(), pixel);
			}

			template <typename I2>
			void replace(const I2& img)
			{
				// FIXME use exception instead of assert.
				assert( img.width() ==  this->width() );
				assert( img.height() ==  this->height() );
				assert( img.channels() == 1 );

				std::copy(img.begin(), img.end(), begin());
			}

			// iterator of subimage
			template<typename P>
			class Iterator {
				friend class LeptoImage<I>;
				public:
					typedef Iterator<P> self_type;

					P& operator *() const
					{
						return (*my_iterator)[my_channel];
					}

					P* operator ->() const
					{
						return (*my_iterator)[my_channel];
					}

					self_type& operator ++()
					{
						++my_iterator;
						return *this;
					}

					self_type operator ++(int)
					{
						self_type tmp = *this;
						++my_iterator;
						return tmp;
					}

					bool operator ==(const self_type &rhs) const
					{
						return (my_iterator == rhs.my_iterator);
					}

					bool operator !=(const self_type &rhs) const
					{
						return !(*this == rhs);
					}

				protected:
					Iterator(typename P::iterator i, size_type c)
						: my_iterator(i), my_channel(c)
					{}
				private:
					typename P::iterator *my_iterator;
					size_type my_channel;
			};

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

			size_type channels() const
			{
				return 1;
			}

		private:
			I& my_image;
			size_type my_channel;
			void operator =(const LeptoImage<I> &i);
	};


	template <typename I>
	LeptoImage<I> lepto_image(I& img, size_t c)
	{
		return LeptoImage<I>(img, c);
	}
	
} // namespace gil

#endif // GIL_LEPTOIMAGE_H
