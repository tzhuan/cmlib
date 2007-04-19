#ifndef GIL_IMAGE_H
#define GIL_IMAGE_H

#include <algorithm>
#include <string>
#include <cstdio>
#include <cstddef>
#include <cassert>
#include <vector>

#include "Color.h"
#include "ImageProxy.h"

namespace gil {

	template<typename, template<typename> class> class Image;

	template<typename Type, template<typename> class Allocator>
	void swap(Image<Type, Allocator>& a, Image<Type, Allocator>& b);

	template<typename Type, template<typename> class Allocator=std::allocator >
	class Image {
		public:
			// STL-compliance
			typedef Type 		value_type;
			typedef Type* 		iterator;
			typedef const Type*	const_iterator;
			typedef Type&		reference;
			typedef const Type&	const_reference;
			typedef Type*		pointer;
			typedef std::ptrdiff_t	difference_type;
			typedef std::size_t		size_type;

			typedef Type ColorType;
			typedef Type* PtrType;
			typedef const Type* ConstPtrType;
			typedef Type& RefType;
			typedef const Type& ConstRefType;

			Image()
				: my_width(0), my_height(0), my_data(0), my_row(0)
			{
				// empty
			}

			Image(size_t w, size_t h)
				: my_width(0), my_height(0), my_data(0), my_row(0)
			{
				resize(w, h);
			}

			Image(const Image& img)
				: my_width(0), my_height(0), my_data(0), my_row(0)
			{
				*this = img;
			}

			template <typename I>
			Image(const I& img)
				: my_width(0), my_height(0), my_data(0), my_row(0)
			{
				*this = img;
			}

			~Image() { 
				/*
				delete [] my_data; 
				delete [] my_row; 
				my_data = 0;
				my_row = 0;
				*/
				my_data.resize(0);
				my_row.resize(0);
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
				return my_width*my_height;
			}

			size_type channels() const
			{
				return ColorTrait<Type>::channels();
			}

			void fill(const_reference pixel)
			{
				std::fill(begin(), end(), pixel);
			}

			// deprecated
			void allocate(size_type w, size_type h)
			{
				resize(w, h);	
			}

			void resize(size_t w, size_t h){
				if (w != my_width || h != my_height) {
					// the delete operator will automatically check for NULL
					// delete [] my_data;
					// delete [] my_row;

					if (w != 0 && h != 0) {
						// my_data = new Type[w*h];
						my_data.resize(w*h);
						// my_row = new Type*[h];
						my_row.resize(h);
						my_width = w;
						my_height = h;
						init_row();
					} else {
						my_width = my_height = 0;
						my_data.resize(0);
						my_row.resize(0);
					}
				}
			}

			reference operator ()(size_type x, size_type y)
			{
				return my_row[y][x];
			}

			const_reference operator ()(size_type x, size_type y) const
			{
				return my_row[y][x];
			}

			template<class Filter, class To, class From>
			Image& operator =(const ImageProxy<Filter, To, From>& image_proxy)
			{
				return image_proxy(*this);
			}

			Image& operator =(const Image& img)
			{
				return this->operator=<Image>(img);
			}

			template <typename I>
			Image& operator =(const I& img)
			{
				if (this != reinterpret_cast<const Image*>(&img)) {
					resize(img.width(), img.height());
					std::copy(img.begin(), img.end(), this->begin());
				}
				return *this;
			}

			template <typename I>
			void replace(const I& img, size_type pos_x = 0, size_type pos_y = 0)
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

			// bilinear interpolation, quite useful
			template<typename T>
			const value_type lerp(T x, T y) const
			{
				// we assert that  0 <= x <= width-1 and 0 <= y <= height-1
				int x0 = static_cast<int>(x);
				int y0 = static_cast<int>(y);
				double xf = x - x0;
				double yf = y - y0;

				if (xf == 0 && yf == 0)
					return my_row[y0][x0];

				if (xf == 0) {
					return mix( my_row[y0][x0], my_row[y0+1][x0], yf );
				} else if (yf == 0) {
					return mix( my_row[y0][x0], my_row[y0][x0+1], xf );
				} else {
					return mix(
							mix( my_row[y0][x0], my_row[y0][x0+1], xf ),
							mix( my_row[y0+1][x0], my_row[y0+1][x0+1], xf ),
							yf );
				}
			}

			void swap(Image& i)
			{
				if (this == &i) return;

				using std::swap;
				swap(my_width, i.my_width);
				swap(my_height, i.my_height);
				my_data.swap(i.my_data);
				my_row.swap(i.my_row);
			}

			iterator begin()
			{
				return my_data.size() ? &my_data[0] : 0;
			}

			const_iterator begin() const
			{
				return my_data.size() ? &my_data[0] : 0;
			}

			iterator end()
			{
				return my_data.size() ? &my_data[0] + my_width*my_height : 0;
			}

			const_iterator end() const
			{
				return my_data.size() ? &my_data[0] + my_width*my_height : 0;
			}

		protected:
			void init_row(){
				my_row[0] = &my_data[0];
				for (size_t i = 1; i < my_height; ++i)
					my_row[i] = my_row[i-1] + my_width;
			}

		private:
			size_type my_width;
			size_type my_height;
			// value_type *my_data;
			std::vector<value_type, Allocator<value_type> > my_data;
			// value_type **my_row;
			std::vector<iterator, Allocator<iterator> > my_row;
	};

	template<typename Type>
	inline void swap(Image<Type>& a, Image<Type>& b)
	{
		a.swap(b);
	}

	typedef Image<Byte1> ByteImage1;
	typedef Image<Byte3> ByteImage3;
	typedef Image<Byte4> ByteImage4;
	typedef Image<Short1> ShortImage1;
	typedef Image<Short3> ShortImage3;
	typedef Image<Short4> ShortImage4;
	typedef Image<Float1> FloatImage1;
	typedef Image<Float3> FloatImage3;
	typedef Image<Float4> FloatImage4;

} // namespace gil

#endif // GIL_IMAGE_H
