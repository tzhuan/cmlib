#ifndef GIL_LEPTOTIMAGE_H
#define GIL_LEPTOTIMAGE_H

#include <algorithm>
#include <cassert>
#include <stdexcept>

#include "Color.h"

namespace gil {

	template <typename ImageType>
	class LeptoImage {
		public:
			typedef typename ImageType::value_type color_type;

			typedef typename ColorTrait<color_type>::BaseType value_type;
			typedef value_type& reference;
			typedef const value_type& const_reference;
			typedef typename ImageType::difference_type difference_type;
			typedef typename ImageType::size_type size_type;

			template<typename P> class Iterator;
			typedef Iterator<value_type> iterator;
			typedef Iterator<const value_type> const_iterator;

			template <typename ColorType, size_t C> struct ChannelSelector;

			typedef 
				ChannelSelector< color_type, ColorTrait<color_type>::Channels > 
				selector_type;

			LeptoImage(ImageType& i, size_type c): my_image(i), my_channel(c)
			{
				// empty
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
				return selector_type()(my_image(x, y), my_channel);
			}

			const_reference operator ()(size_type x, size_type y) const
			{
				return selector_type()(my_image(x, y), my_channel);
			}

			void fill(const_reference pixel)
			{
				std::fill(begin(), end(), pixel);
			}

			template <typename I>
			void replace(const I& img, size_type pos_x = 0, size_type pos_y = 0)
			{
				// FIXME use exception instead of assert.
				assert( pos_x < this->width() );
				assert( pos_y < this->height() );
				assert( pos_x + img.width() < this->width() );
				assert( pos_y + img.height() < this->height() );

				for (size_type y(pos_y), iy(0); iy < img.height(); ++iy, ++y)
					for (size_type x(pos_x), ix(0); ix < img.width(); ++ix, ++x)
						(*this)(x, y) = img(ix, iy);
			}

			// iterator of LeptoImage
			template<typename P>
			class Iterator: public std::iterator<std::forward_iterator_tag, P> {
				friend class LeptoImage<ImageType>;

				public:
					typedef Iterator<P> self_type;

					P& operator *() const
					{
						return selector_type()(*my_iterator, my_channel);
					}

					P* operator ->() const
					{
						return selector_type()(*my_iterator, my_channel);
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
						return (my_iterator != rhs.my_iterator);
					}

				private:

					template <typename T> class ResolveConst {
						// T is assumed to be a color type, 
						// either const or not.
						public:
							typedef typename ImageType::iterator result;
					};
					// partial specialize the const one
					template <typename T> class ResolveConst<const T> {
						public:
							typedef typename ImageType::const_iterator result;
					};

					typedef typename ResolveConst<P>::result IteratorType;

					IteratorType my_iterator;
					size_type my_channel;

					Iterator(IteratorType i, size_type c)
						: my_iterator(i), my_channel(c)
					{
						// empty
					}
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

			template <typename ColorType, size_t C>
			struct ChannelSelector {

				inline
				value_type&
				operator ()(ColorType& color, size_t c)
				{
					return color[c];
				}

				inline
				const value_type& 
				operator ()(const ColorType& color, size_t c) const
				{
					return color[c];
				}
			};

			template<typename ColorType>
			struct ChannelSelector<ColorType, 1> {

				inline
				value_type&
				operator ()(ColorType& color, size_t c)
				{
					return color;
				}

				inline
				const value_type&
				operator ()(const ColorType& color, size_t c) const
				{
					return color;
				}
			};

		private:

			ImageType& my_image;
			size_type my_channel;
			void operator =(const LeptoImage<ImageType> &i);
	};

	template <typename ImageType>
	LeptoImage<ImageType> lepto_image(ImageType& img, size_t c)
	{
		return LeptoImage<ImageType>(img, c);
	}
	
} // namespace gil

#endif // GIL_LEPTOIMAGE_H
