#ifndef CMLIB_IMAGE_IMAGE_H
#define CMLIB_IMAGE_IMAGE_H

#include <algorithm>
#include <string>
#include <cstdio>
#include <cstddef>
#include <cassert>
#include <vector>
#include <functional>

#include <cmlib/image/Color.h>
#include <cmlib/image/ColorSelector.h>

namespace cmlib {
namespace image {

	template<typename Type, template<typename> class Allocator>
	void swap(Image<Type, Allocator>& a, Image<Type, Allocator>& b);

	/**
	 * @brief Image is the basic image type in cmlib.
	 */
	template<typename Type, template<typename> class Allocator=std::allocator>
	class Image {
	public:
		typedef Image<Type, Allocator> self_type;
		typedef std::vector<Type, Allocator<Type> > data_type;

		// STL-compliance
		typedef typename data_type::value_type value_type;
		typedef typename data_type::pointer pointer;
		typedef typename data_type::const_pointer const_pointer;
		typedef typename data_type::reference reference;
		typedef typename data_type::const_reference const_reference;
		typedef typename data_type::difference_type difference_type;
		typedef typename data_type::size_type size_type;

		typedef typename data_type::iterator iterator;
		typedef typename data_type::const_iterator const_iterator;
		typedef typename data_type::reverse_iterator reverse_iterator;
		typedef typename data_type::const_reverse_iterator const_reverse_iterator;

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

		Image(size_type w, size_type h)
			: my_width(0), my_height(0), my_data(0), my_row(0)
		{
			resize(w, h);
		}

		Image(const self_type& img)
			: my_width(0), my_height(0), my_data(0), my_row(0)
		{
			*this = img;
		}

		template <typename I>
		explicit Image(const I& img)
			: my_width(0), my_height(0), my_data(0), my_row(0)
		{
			*this = img;
		}

		~Image() 
		{
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
			return width()*height();
		}

		size_type channels() const
		{
			return ColorTrait<Type>::channels();
		}

		void fill(const_reference pixel)
		{
			std::fill(begin(), end(), pixel);
		}

		template<typename T>
		void fill(const T& pixel)
		{
			std::fill(begin(), end(), pixel);
		}

		// deprecated
		void allocate(size_type w, size_type h)
		{
			resize(w, h);	
		}

		void resize(size_type w, size_type h){
			if (w != width() || h != height()) {
				if (w != 0 && h != 0) {
					my_data.resize(w*h);
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

		reference operator()(size_type x, size_type y)
		{
			return my_row[y][x];
		}

		const_reference operator()(size_type x, size_type y) const
		{
			return my_row[y][x];
		}

		reference operator()(size_type i)
		{
			return (*this)[i];
		}

		const_reference operator()(size_type i) const
		{
			return (*this)[i];
		}

		reference operator[](size_type i)
		{
			return my_data[i];
		}

		const_reference operator[](size_type i) const
		{
			return my_data[i];
		}

		self_type& operator=(const self_type& img)
		{
			if (this != &img) {
				resize(img.width(), img.height());
				std::copy(img.begin(), img.end(), begin());
			}
			return *this;
		}

		template <typename ImageType>
		self_type& operator=(const ImageType& img)
		{
			resize(img.width(), img.height());
			std::copy(img.begin(), img.end(), begin());
			return *this;
		}

		template <typename ImageType>
		void replace(ImageType& img, size_type px = 0, size_type py = 0)
		{
			/* FIXME use exception instead of assert.
			assert( px < width() );
			assert( py < height() );
			assert( px + img.width() <= width() );
			assert( py + img.height() <= height() );
			*/

			for (size_type y(py), iy(0); iy < img.height(); ++iy, ++y)
				for (size_type x(px), ix(0); ix < img.width(); ++ix, ++x)
					(*this)(x, y) = img(ix, iy);
		}

		// bilinear interpolation, quite useful
		template<typename T>
		const value_type lerp(T x, T y) const
		{
			// we assume that 0 <= x <= width-1 and 0 <= y <= height-1
			int x0 = static_cast<int>(x);
			int y0 = static_cast<int>(y);
			double xf = x - x0;
			double yf = y - y0;

			if (xf == 0 && yf == 0)
				return (*this)(x0, y0);

			if (xf == 0) {
				return mix( (*this)(x0, y0), (*this)(x0, y0+1), yf );
			} else if (yf == 0) {
				return mix( (*this)(x0, y0), (*this)(x0+1, y0), xf );
			} else {
				return mix(
					mix( (*this)(x0, y0), (*this)(x0+1, y0), xf ),
					mix( (*this)(x0, y0+1), (*this)(x0+1, y0+1), xf ),
					yf 
				);
			}
		}

		void swap(self_type& i)
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
			return my_data.begin();
		}

		const_iterator begin() const
		{
			return my_data.begin();
		}

		iterator end()
		{
			return my_data.end();
		}

		const_iterator end() const
		{
			return my_data.end();
		}

		reverse_iterator rbegin()
		{
			return my_data.rbegin();
		}

		const_reverse_iterator rbegin() const
		{
			return my_data.rbegin();
		}

		reverse_iterator rend()
		{
			return my_data.rend();
		}

		const_reverse_iterator rend() const
		{
			return my_data.rend();
		}

		//@{
		/**
		 *  @defgroup OperatorAssigns<Op, T, ImageTag>, the help function
		 *  object to distinguish between image op= color and image op= image.
		 */
		// image op= color
		template<typename Tag, typename RhsType, template<typename, typename> class Op> 
		struct OperatorAssigns { // {{{
			Image& operator()(Image& lhs, const RhsType& rhs) const
			{
				std::for_each(
					lhs.begin(), lhs.end(), 
					std::bind2nd(Op<value_type, RhsType>(), rhs)
				);
				return lhs;
			}
		}; // }}}
		// image op= image
		template<typename RhsType, template<typename, typename> class Op>
		struct OperatorAssigns<ImageTag, RhsType, Op> { // {{{
			Image& operator()(Image& lhs, const RhsType& rhs) const
			{
				cmlib::image::inplace_transform(
					lhs.begin(), lhs.end(), rhs.begin(), 
					Op<value_type, typename RhsType::value_type>()
				);
				return lhs;
			}
		}; // }}}
		//@}


		template<typename T>
		self_type& operator+=(const T& rhs) 
		{
			return 
				OperatorAssigns<
					typename TypeTrait<T>::type_category, 
					T, PlusAssigns
				>()(*this, rhs);
		}

		template<typename T>
		self_type& operator-=(const T& rhs) 
		{
			return 
				OperatorAssigns<
					typename TypeTrait<T>::type_category, 
					T, MinusAssigns
				>()(*this, rhs);
		}

		template<typename T>
		self_type& operator*=(const T& rhs) 
		{
			return
				OperatorAssigns<
					typename TypeTrait<T>::type_category, T, 
					MultiplyAssigns
				>()(*this, rhs);
		}

		template<typename T>
		self_type& operator/=(const T& rhs) 
		{
			return 
				OperatorAssigns<
					typename TypeTrait<T>::type_category, T,
					DivideAssigns
				>()(*this, rhs);
		}

		template<typename T>
		self_type& operator%=(const T& rhs) 
		{
			return
				OperatorAssigns<
					typename TypeTrait<T>::type_category, T, 
					ModuluAssigns
				>()(*this, rhs);
		}

		std::ostream& output(std::ostream& os) const
		{
			os << '(' << cmlib::image::output((*this)(0, 0)) << ')';
			for (size_type w = 1; w < width(); ++w)
				os << " (" << cmlib::image::output((*this)(w, 0)) << ')';
			for (size_type h = 1; h < height(); ++h) {
				os << std::endl << '(' << cmlib::image::output((*this)(0, h)) << ')';
				for (size_type w = 1; w < width(); ++w)
					os << " (" << cmlib::image::output((*this)(w, h)) << ')';
			}
			return os;
		}

	protected:
		void init_row()
		{
			my_row[0] = my_data.begin();
			for (size_type i = 1; i < height(); ++i)
				my_row[i] = my_row[i-1] + width();
		}

	private:
		size_type my_width;
		size_type my_height;
		std::vector<value_type, Allocator<value_type> > my_data;
		std::vector<iterator, Allocator<iterator> > my_row;
	};

	template<typename Type, template<typename> class Allocator>
	void swap(Image<Type, Allocator>& a, Image<Type, Allocator>& b)
	{
		a.swap(b);
	}

	//@{
	/**
	 *  @defgroup Image<Type, Allocator> arithmetic functions.
	 */
	// -Image {{{
	template<typename ColorType, template<typename> class Allocator>
	Image<ColorType, Allocator> operator-(const Image<ColorType, Allocator>& image)
	{
		Image<ColorType, Allocator> tmp(image.width(), image.height());
		std::transform(image.begin(), image.end(), tmp.begin(), std::negate<ColorType>());
		return tmp;
	} // }}}

	// Image + {Color, constant}, {Color, constant} + Image, Image + Image {{{
	// Image + Image
	template<typename Type1, template<typename> class Allocator1, typename Type2, template<typename> class Allocator2>
	Image<typename ColorSelector<Type1, Type2>::priority_type>
	operator+(const Image<Type1, Allocator1>& image1, const Image<Type2, Allocator2>& image2)
	{
		Image<typename ColorSelector<Type1, Type2>::priority_type> tmp(image1);
		tmp += image2;
		return tmp;
	}
	// Image + constant
	template<typename ColorType, template<typename> class Allocator, typename Type>
	Image<ColorType, Allocator> operator+(const Image<ColorType, Allocator>& image, const Type& value)
	{
		Image<ColorType, Allocator> tmp(image);
		tmp += value;
		return tmp;
	}
	// constant + Image
	template<typename Type, typename ColorType, template<typename> class Allocator>
	Image<ColorType, Allocator> operator+(const Type& value, const Image<ColorType, Allocator>& image)
	{
		Image<ColorType, Allocator> tmp(image);
		tmp += value;
		return tmp;
	}
	// Image + Color
	template<typename ColorType, template<typename> class Allocator, typename Type>
	Image<ColorType, Allocator> operator+(const Image<ColorType, Allocator>& image, const Color<Type, ColorTrait<ColorType>::Channels>& value)
	{
		Image<ColorType, Allocator> tmp(image);
		tmp += value;
		return tmp;
	}
	// Color + Image
	template<typename Type, typename ColorType, template<typename> class Allocator>
	Image<ColorType, Allocator> operator+(const Color<Type, ColorTrait<ColorType>::Channels>& value, const Image<ColorType, Allocator>& image)
	{
		Image<ColorType, Allocator> tmp(image);
		tmp += value;
		return tmp;
	}
	// }}}

	// Image - {Color, constant}, {Color, constant} - Image, Image - Image {{{
	// Image - Image
	template<typename Type1, template<typename> class Allocator1, typename Type2, template<typename> class Allocator2>
	Image<typename ColorSelector<Type1, Type2>::priority_type>
	operator-(const Image<Type1, Allocator1>& image1, const Image<Type2, Allocator2>& image2)
	{
		Image<typename ColorSelector<Type1, Type2>::priority_type> tmp(image1);
		tmp -= image2;
		return tmp;
	}
	// Image - constant
	template<typename ColorType, template<typename> class Allocator, typename Type>
	Image<ColorType, Allocator> operator-(const Image<ColorType, Allocator>& image, const Type& value)
	{
		Image<ColorType, Allocator> tmp(image);
		tmp -= value;
		return tmp;
	}
	// constant - Image
	template<typename Type, typename ColorType, template<typename> class Allocator>
	Image<ColorType, Allocator> operator-(const Type& value, const Image<ColorType, Allocator>& image)
	{
		Image<ColorType, Allocator> tmp(image.width(), image.height());
		std::transform(
			image.begin(), image.end(), tmp.begin(), 
			bind1st(Minus<Type, ColorType, ColorType>(), value)
		);
		return tmp;
	}
	// Image - Color
	template<typename ColorType, template<typename> class Allocator, typename Type>
	Image<ColorType, Allocator> operator-(const Image<ColorType, Allocator>& image, const Color<Type, ColorTrait<ColorType>::Channels>& value)
	{
		Image<ColorType, Allocator> tmp(image);
		tmp -= value;
		return tmp;
	}
	// Color - Image
	template<typename Type, typename ColorType, template<typename> class Allocator>
	Image<ColorType, Allocator> operator-(const Color<Type, ColorTrait<ColorType>::Channels>& value, const Image<ColorType, Allocator>& image)
	{
		Image<ColorType, Allocator> tmp(image.width(), image.height());
		std::transform(
			image.begin(), image.end(), tmp.begin(), 
			bind1st(Minus<Color<Type, ColorTrait<ColorType>::Channels>, ColorType, ColorType>(), value)
		);
		return tmp;
	}
	// }}}
	
	// Image * {Color, constant}, {Color, constant} * Image, Image * Image {{{
	// Image * Image
	template<typename Type1, template<typename> class Allocator1, typename Type2, template<typename> class Allocator2>
	Image<typename ColorSelector<Type1, Type2>::priority_type>
	operator*(const Image<Type1, Allocator1>& image1, const Image<Type2, Allocator2>& image2)
	{
		Image<typename ColorSelector<Type1, Type2>::priority_type> tmp(image1);
		tmp *= image2;
		return tmp;
	}
	// Image * constant
	template<typename ColorType, template<typename> class Allocator, typename Type>
	Image<ColorType, Allocator> operator*(const Image<ColorType, Allocator>& image, const Type& value)
	{
		Image<ColorType, Allocator> tmp(image);
		tmp *= value;
		return tmp;
	}
	// constant * Image
	template<typename Type, typename ColorType, template<typename> class Allocator>
	Image<ColorType, Allocator> operator*(const Type& value, const Image<ColorType, Allocator>& image)
	{
		Image<ColorType, Allocator> tmp(image);
		tmp *= value;
		return tmp;
	}
	// Image * Color
	template<typename ColorType, template<typename> class Allocator, typename Type>
	Image<ColorType, Allocator> operator*(const Image<ColorType, Allocator>& image, const Color<Type, ColorTrait<ColorType>::Channels>& value)
	{
		Image<ColorType, Allocator> tmp(image);
		tmp *= value;
		return tmp;
	}
	// Color * Image
	template<typename Type, typename ColorType, template<typename> class Allocator>
	Image<ColorType, Allocator> operator*(const Color<Type, ColorTrait<ColorType>::Channels>& value, const Image<ColorType, Allocator>& image)
	{
		Image<ColorType, Allocator> tmp(image);
		tmp *= value;
		return tmp;
	}
	// }}}

	// Image / {Color, constant}, {Color, constant} / Image, Image / Image {{{
	// Image / Image
	template<typename Type1, template<typename> class Allocator1, typename Type2, template<typename> class Allocator2>
	Image<typename ColorSelector<Type1, Type2>::priority_type>
	operator/(const Image<Type1, Allocator1>& image1, const Image<Type2, Allocator2>& image2)
	{
		Image<typename ColorSelector<Type1, Type2>::priority_type> tmp(image1);
		tmp /= image2;
		return tmp;
	}
	// Image / constant
	template<typename ColorType, template<typename> class Allocator, typename Type>
	Image<ColorType, Allocator> operator/(const Image<ColorType, Allocator>& image, const Type& value)
	{
		Image<ColorType, Allocator> tmp(image);
		tmp /= value;
		return tmp;
	}
	// constant / Image
	template<typename Type, typename ColorType, template<typename> class Allocator>
	Image<ColorType, Allocator> operator/(const Type& value, const Image<ColorType, Allocator>& image)
	{
		Image<ColorType, Allocator> tmp(image.width(), image.height());
		std::transform(
			image.begin(), image.end(), tmp.begin(), 
			bind1st(Divides<Type, ColorType, ColorType>(), value)
		);
		return tmp;
	}
	// Image / Color
	template<typename ColorType, template<typename> class Allocator, typename Type>
	Image<ColorType, Allocator> operator/(const Image<ColorType, Allocator>& image, const Color<Type, ColorTrait<ColorType>::Channels>& value)
	{
		Image<ColorType, Allocator> tmp(image);
		tmp /= value;
		return tmp;
	}
	// Color / Image
	template<typename Type, typename ColorType, template<typename> class Allocator>
	Image<ColorType, Allocator> operator/(const Color<Type, ColorTrait<ColorType>::Channels>& value, const Image<ColorType, Allocator>& image)
	{
		Image<ColorType, Allocator> tmp(image.width(), image.height());
		std::transform(
			image.begin(), image.end(), tmp.begin(), 
			bind1st(Divides<Color<Type, ColorTrait<ColorType>::Channels>, ColorType, ColorType>(), value)
		);
		return tmp;
	}
	// }}}

	// Image % {Color, constant}, {Color, constant} % Image, Image % Image {{{
	// Image % Image
	template<typename Type1, template<typename> class Allocator1, typename Type2, template<typename> class Allocator2>
	Image<typename ColorSelector<Type1, Type2>::priority_type>
	operator%(const Image<Type1, Allocator1>& image1, const Image<Type2, Allocator2>& image2)
	{
		Image<typename ColorSelector<Type1, Type2>::priority_type> tmp(image1);
		tmp %= image2;
		return tmp;
	}
	// Image % constant
	template<typename ColorType, template<typename> class Allocator, typename Type>
	Image<ColorType, Allocator> operator%(const Image<ColorType, Allocator>& image, const Type& value)
	{
		Image<ColorType, Allocator> tmp(image);
		tmp %= value;
		return tmp;
	}
	// constant % Image
	template<typename Type, typename ColorType, template<typename> class Allocator>
	Image<ColorType, Allocator> operator%(const Type& value, const Image<ColorType, Allocator>& image)
	{
		Image<ColorType, Allocator> tmp(image.width(), image.height());
		std::transform(
			image.begin(), image.end(), tmp.begin(), 
			bind1st(Modulus<Type, ColorType, ColorType>(), value)
		);
		return tmp;
	}
	// Image % Color
	template<typename ColorType, template<typename> class Allocator, typename Type>
	Image<ColorType, Allocator> operator%(const Image<ColorType, Allocator>& image, const Color<Type, ColorTrait<ColorType>::Channels>& value)
	{
		Image<ColorType, Allocator> tmp(image);
		tmp %= value;
		return tmp;
	}
	// Color % Image
	template<typename Type, typename ColorType, template<typename> class Allocator>
	Image<ColorType, Allocator> operator%(const Color<Type, ColorTrait<ColorType>::Channels>& value, const Image<ColorType, Allocator>& image)
	{
		Image<ColorType, Allocator> tmp(image.width(), image.height());
		std::transform(
			image.begin(), image.end(), tmp.begin(), 
			bind1st(Modulus<Color<Type, ColorTrait<ColorType>::Channels>, ColorType, ColorType>(), value)
		);
		return tmp;
	}
	// }}}
	//@}

	template<typename Type, template<typename> class Allocator>
	const Image<Type, Allocator>& output(const Image<Type, Allocator>& image)
	{
		return image;
	}

	template<typename Type, template<typename> class Allocator>
	std::ostream& operator<<(std::ostream& os, const Image<Type, Allocator>& image)
	{
		return image.output(os);
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
	typedef Image<Double1> DoubleImage1;
	typedef Image<Double3> DoubleImage3;
	typedef Image<Double4> DoubleImage4;

} // namespace image
} // namespace cmlib

#endif // CMLIB_IMAGE_IMAGE_H
