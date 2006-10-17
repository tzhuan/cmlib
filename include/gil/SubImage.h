#ifndef GIL_SUBIMAGE_H
#define GIL_SUBIMAGE_H

#include <algorithm>

namespace gil {
	
	template <typename I>
	class SubImage {
		public:
			typedef typename I::value_type value_type;
			typedef typename I::reference reference;
			typedef typename I::const_reference const_reference;
			typedef typename I::difference_type difference_type;
			typedef typename I::size_type size_type;

			template<typename P> class Iterator;
			typedef Iterator<value_type> iterator;
			typedef Iterator<const value_type> const_iterator;

			typedef typename I::Converter Converter;

			SubImage(I& i, size_type x, size_type y, size_type w, size_type h)
				: my_image(i), 
				  my_x_offset(x), my_y_offset(y),
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

			reference operator ()(size_type x, size_type y)
			{
				return my_image(x+my_x_offset, y+my_y_offset);
			}

			const_reference operator ()(size_type x, size_type y) const
			{
				return my_image(x+my_x_offset, y+my_y_offset);
			}

			void fill(const_reference pixel)
			{
				std::fill(begin(), end(), pixel);
			}

			template <typename I2>
			void replace(const I2& img, int pos_x = 0, int pos_y = 0)
			{
				// FIXME use exception instead of assert.
				assert( pos_x >= 0 && pos_x < this->width() );
				assert( pos_y >= 0 && pos_y < this->height() );
				assert( pos_x + img.width() < this->width() );
				assert( pos_y + img.height() < this->height() );

				for (size_t y(pos_y), iy(0); iy < img.height(); ++iy, ++y) {
					for (size_t x(pos_x), ix(0); ix < img.width(); ++ix, ++x) {
						(*this)(x, y) = img(ix, iy);
					}
				}
			}

			// iterator of subimage
			template<typename P>
			class Iterator {
				friend class SubImage<I>;
				public:
					typedef Iterator<P> self_type;

					P& operator *() const
					{
						return my_subimage(my_x, my_y);
					}

					P* operator ->() const
					{
						return &my_subimage(my_x, my_y);
					}

					self_type& operator ++()
					{
						++my_x;
						if (my_x == my_subimage->width()) {
							my_x = 0;
							++my_y;
						}
					}

					self_type operator ++(int)
					{
						self_type tmp = *this;
						++*this;
						return tmp;
					}

					bool operator ==(const self_type &rhs) const
					{
						return (my_subimage == rhs.my_subimage && 
								my_x == rhs.my_x && my_y == rhs.my_y);
					}

					bool operator !=(const self_type &rhs) const
					{
						return !(*this == rhs);
					}

				protected:
					Iterator(SubImage<I>& subimage, size_type x, size_type y)
						: my_subimage(subimage), my_x(x), my_y(y)
					{}
				private:
					SubImage<I> *my_subimage;
					size_type my_x, my_y;
			};

			iterator begin()
			{
				return iterator(my_image, 0, 0);
			}

			const_iterator begin() const
			{
				return const_iterator(my_image, 0, 0);
			}

			iterator end()
			{
				return iterator(my_image, my_height, 0);
			}

			const_iterator end() const
			{
				return const_iterator(my_image, my_height, 0);
			}

		private:
			I& my_image;
			size_type my_x_offset;
			size_type my_y_offset;
			size_type my_width;
			size_type my_height;

			template <typename I2>
			SubImage<I>& operator =(const I2& i)
			{
				const size_type W = std::min(width(),  i.width());
				const size_type H = std::min(height(), i.height());
				SubImage<I>& self = *this;
				for(size_type y = 0; y < H; y++)
					for(size_type x = 0; x < W; x++)
						self(x,y) = i(x,y);
				return self;
			}
	};


	template <typename I>
	SubImage<I> sub_image(I& img, size_t x, size_t y, size_t w, size_t h)
	{
		return SubImage<I>(img, x, y, w, h);
	}
	
} // namespace gil

#endif // GIL_SUBIMAGE_H
