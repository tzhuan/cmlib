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
			typedef typename I::difference_type difference_type ;
			typedef typename I::size_type size_type;

			typedef typename I::Converter Converter;

			SubImage(I& i, size_type x, size_type y, size_type w, size_type h)
				: my_image(i), 
				  my_x_offset(x), my_y_offset(y),
				  my_width(w), my_height(h)
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

			reference operator ()(size_type x, size_type y) const
			{
				return my_image(x+my_x_offset, y+my_y_offset);
			}

			const reference operator ()(size_type x, size_type y) const
			{
				return my_image(x+my_x_offset, y+my_y_offset);
			}

			template <typename I2>
			SubImage& operator =(const I2& i);

		private:
			I& my_image;
			size_type my_x_offset;
			size_type my_y_offset;
			size_type my_width;
			size_type my_height;
	};

	template <typename I>
	template <typename I2>
	SubImage<I>& SubImage<I>::operator =(const I2& i)
	{
		const size_type W = std::min(width(),  i.width());
		const size_type H = std::min(height(), i.height());
		SubImage<I>& self = *this;
		for(size_type y = 0; y < H; y++)
			for(size_type x = 0; x < W; x++)
				self(x,y) = i(x,y);
		return self;
	}

	template <typename I>
	SubImage<I> sub_image(I& img, size_t x, size_t y, size_t w, size_t h)
	{
		return SubImage<I>(img, x, y, w, h);
	}
	
} // namespace gil

#endif // GIL_SUBIMAGE_H
