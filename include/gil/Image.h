#ifndef GIL_IMAGE_H
#define GIL_IMAGE_H

#include <algorithm>
#include <string>
#include <cstdio>
#include <cstddef>
#include <cassert>

#include "Color.h"
#include "Converter.h"

namespace gil {

	template<typename, template<typename,typename> class Conv=DefaultConverter> 
	class Image;

	template<typename Type, template<typename,typename> class Conv>
	void swap(Image<Type,Conv>& a, Image<Type,Conv>& b);

	template<typename Type, template<typename,typename> class Conv>
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
				delete [] my_data; 
				delete [] my_row; 
				my_data = 0;
				my_row = 0;
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
				if(w != my_width || h != my_height){
					// the delete operator will automatically check for NULL
					delete [] my_data;
					delete [] my_row;

					if(w != 0 && h != 0){
						my_data = new Type[w*h];
						my_row = new Type*[h];
						my_width = w;
						my_height = h;
						init_row();
					}else{
						my_width = my_height = 0;
						my_data = 0;
						my_row = 0;
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

			// bilinear interpolation, quite useful
			Type lerp(double x, double y) const;

			Image& operator =(const Image& img)
			{
				return this->operator=<Image>(img);
			}

			template <typename I>
			Image& operator =(const I& img)
			{
				if(this != &img){
					resize(img.width(), img.height());
					std::copy(img.begin(), img.end(), this->begin());
				}
				return *this;
			}

			template <typename I>
			void replace(const I& img, size_type pos_x = 0, size_type pos_y = 0)
			{
				// FIXME use exception instead of assert.
				assert( pos_x >= 0 && pos_x < this->width() );
				assert( pos_y >= 0 && pos_y < this->height() );
				assert( pos_x + img.width() < this->width() );
				assert( pos_y + img.height() < this->height() );

				for (size_type y(pos_y), iy(0); iy < img.height(); ++iy, ++y) {
					for (size_type x(pos_x), ix(0); ix < img.width(); ++ix, ++x) {
						(*this)(x, y) = img(ix, iy);
					}
				}
			}

			iterator begin()
			{
				return my_data;
			}

			const_iterator begin() const
			{
				return my_data;
			}

			iterator end()
			{
				return my_data + my_width*my_height;
			}

			const_iterator end() const
			{
				return my_data + my_width*my_height;
			}

			void swap(Image& i);

			// converter
			struct Converter {
				typedef Type Internal;

				template <typename External>
					static void ext2int(Internal& to, const External& from){
						Conv<Internal,External>::convert(to, from);
					}

				template <typename External>
					static void int2ext(External& to, const Internal& from){
						Conv<External,Internal>::convert(to, from);
					}
			};

		protected:
			void init_row(){
				my_row[0] = my_data;
				for (size_t i = 1; i < my_height; ++i)
					my_row[i] = my_row[i-1] + my_width;
			}

		private:
			size_type my_width;
			size_type my_height;
			value_type *my_data;
			value_type **my_row;
	};

	template<typename Type, template<typename,typename> class Conv>
	Type Image<Type,Conv>::lerp(double x, double y) const
	{
		// we assert that  0 <= x <= width-1 and 0 <= y <= height-1
		int x0 = static_cast<int>(x);
		int y0 = static_cast<int>(y);
		double xf = x - x0;
		double yf = y - y0;

		if(xf == 0 && yf == 0)
			return my_row[y0][x0];

		if(xf == 0){
			return mix( my_row[y0][x0], my_row[y0+1][x0], yf );
		}else if(yf == 0){
			return mix( my_row[y0][x0], my_row[y0][x0+1], xf );
		}else{
			return mix(
					mix( my_row[y0][x0], my_row[y0][x0+1], xf ),
					mix( my_row[y0+1][x0], my_row[y0+1][x0+1], xf ),
					yf );
		}
	}

	template <typename Type, template<typename,typename> class Conv>
	void Image<Type,Conv>::swap(Image<Type,Conv>& i)
	{
		if(this == &i) return;

		using std::swap;
		swap(my_width, i.my_width);
		swap(my_height, i.my_height);
		swap(my_data, i.my_data);
		swap(my_row, i.my_row);
	}

	template<typename Type, template<typename,typename> class Conv>
	inline void swap(Image<Type,Conv>& a, Image<Type,Conv>& b)
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
