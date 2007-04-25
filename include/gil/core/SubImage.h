#ifndef GIL_SUBIMAGE_H
#define GIL_SUBIMAGE_H

#include <algorithm>
#include <iterator>

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

			typedef value_type ColorType;

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

			size_type size() const
			{
				return this->width() * this->height();
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
			void replace(const I2& img, size_type pos_x=0, size_type pos_y=0)
			{
				// FIXME use exception instead of assert.
				assert( pos_x < this->width() );
				assert( pos_y < this->height() );
				assert( pos_x + img.width() <= this->width() );
				assert( pos_y + img.height() <= this->height() );

				for (size_type y(pos_y), iy(0); iy < img.height(); ++iy, ++y)
					for (size_type x(pos_x), ix(0); ix < img.width(); ++ix, ++x)
						(*this)(x, y) = img(ix, iy);
			}

			// iterator of subimage
			template<typename P>
			class Iterator: public std::iterator<std::forward_iterator_tag, P> {
				friend class SubImage<I>;
				public:
					typedef Iterator<P> self_type;

					P& operator *() const
					{
						return (*my_subimage)(my_x, my_y);
					}

					P* operator ->() const
					{
						return &( (*my_subimage)(my_x, my_y) );
					}

					self_type& operator ++()
					{
						++my_x;
						if (my_x == my_subimage->width()) {
							my_x = 0;
							++my_y;
						}
						return *this;
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

				private:

					// when P has const modifier, we should also add
					// const modifier to my_subimage.
					// This helper class is doing this dirty work.
					template <typename T> class ResolveConst {
						// T is assumed to be a color type, 
						// either const or not.
						public:
							typedef SubImage<I> result;
					};
					// partial specialize the const one
					template <typename T> class ResolveConst<const T> {
						public:
							typedef const SubImage<I> result;
					};

					typedef typename ResolveConst<P>::result SubImageType;

					SubImageType *my_subimage;
					size_type my_x, my_y;

					Iterator(SubImageType *subimage, size_type x, size_type y)
						: my_subimage(subimage), my_x(x), my_y(y)
					{}
			};

			iterator begin()
			{
				return iterator(this, 0, 0);
			}

			const_iterator begin() const
			{
				return const_iterator(this, 0, 0);
			}

			iterator end()
			{
				return iterator(this, 0, my_height);
			}

			const_iterator end() const
			{
				return const_iterator(this, 0, my_height);
			}

			size_type channels() const
			{
				return my_image.channels();
			}

		private:
			I& my_image;
			size_type my_x_offset;
			size_type my_y_offset;
			size_type my_width;
			size_type my_height;

			void operator =(const SubImage<I> &i);
	};


	template <typename I>
	SubImage<I> sub_image(I& img, size_t x, size_t y, size_t w, size_t h)
	{
		return SubImage<I>(img, x, y, w, h);
	}
	
} // namespace gil

#endif // GIL_SUBIMAGE_H
