#ifndef CMLIB_IMAGE_COLOR_H
#define CMLIB_IMAGE_COLOR_H

/** @file Color.h
 *  @brief This file contains the basic pixel type in cmlib::image. 
 *
 *  @author littleshan, tzhuan
 */

#include <iostream>
#include <iterator>
#include <algorithm>
#include <functional>

#include <cmlib/image/Type.h>
#include <cmlib/image/ColorTrait.h>
#include <cmlib/image/ColorSelector.h>
#include <cmlib/image/Functional.h>
#include <cmlib/image/Algorithm.h>

namespace cmlib {
namespace image {

	template<typename Type>
	typename TypeTrait<Type>::OutputType output(const Type& value)
	{
		return static_cast<typename TypeTrait<Type>::OutputType>(value);
	}

	/** @class Color
	 *  @brief Color is the type of a pixel
	 *
	 *  @tparam Type the type of color
	 *  @tparam Channel the channel number of color
	 */
	template <typename Type, size_t Channel>
	class Color { // {{{
	public:
		typedef Color ColorType;

		// STL-compliance
		typedef Type value_type;
		typedef Type* iterator;
		typedef const Type*	const_iterator;
		typedef Type& reference;
		typedef const Type&	const_reference;
		typedef Type* pointer;
		typedef std::ptrdiff_t difference_type;
		typedef std::size_t	size_type;

		typedef typename TypeTrait<Type>::MathType MathType;

		reference operator[](size_type i)
		{
			return my_data[i]; 
		}

		const_reference operator[](size_type i) const 
		{ 
			return my_data[i];
		}

		Color()
		{
			// empty
		}

		template<typename T>
		explicit Color(T c0)
		{
			set(c0);
		}

		template<typename T>
		Color(T c0, T c1, T c2)
		{
			set(c0, c1, c2);
		}

		template<typename T>
		Color(T c0, T c1, T c2, T c3)
		{
			set(c0, c1, c2, c3);
		}

		template<typename T>
		void fill(T c0)
		{
			std::fill(begin(), end(), static_cast<Type>(c0));
		}

		template<typename T>
		void set(T c0)
		{
			fill(c0);
		}

		template<typename T>
		void set(T c0, T c1, T c2)
		{
			(*this)[0] = static_cast<Type>(c0);
			if(Channel >= 2) (*this)[1] = static_cast<Type>(c1);
			if(Channel >= 3) (*this)[2] = static_cast<Type>(c2);
		}

		template<typename T>
		void set(T c0, T c1, T c2, T c3)
		{
			(*this)[0] = static_cast<Type>(c0);
			if(Channel >= 2) (*this)[1] = static_cast<Type>(c1);
			if(Channel >= 3) (*this)[2] = static_cast<Type>(c2);
			if(Channel >= 4) (*this)[3] = static_cast<Type>(c3);
		}

		template<typename T>
		Color& operator=(const T& value)
		{
			fill(value);
			return *this;
		}

		template<typename T>
		Color& operator=(const Color<T, 3>& color)
		{
			set(color[0], color[1], color[2]);
			return (*this);
		}

		template<typename T>
		Color& operator=(const Color<T, 4>& color)
		{
			set(color[0], color[1], color[2], color[3]);
			return (*this);
		}

		// some calculations...
		template<typename T>
		Color& operator+=(const Color<T, Channel>& color)
		{
			cmlib::image::inplace_transform(begin(), end(), color.begin(), PlusAssigns<value_type, T>());
			return *this;
		}

		template<typename T>
		Color& operator-=(const Color<T, Channel>& color)
		{
			cmlib::image::inplace_transform(begin(), end(), color.begin(), MinusAssigns<value_type, T>());
			return *this;
		}

		template<typename T>
		Color& operator*=(const Color<T, Channel>& color)
		{
			cmlib::image::inplace_transform(begin(), end(), color.begin(), MultiplyAssigns<value_type, T>());
			return *this;
		}

		template<typename T>
		Color& operator/=(const Color<T, Channel>& color)
		{
			cmlib::image::inplace_transform(begin(), end(), color.begin(), DivideAssigns<value_type, T>());
			return *this;
		}

		template<typename T>
		Color& operator%=(const Color<T, Channel>& color)
		{
			cmlib::image::inplace_transform(begin(), end(), color.begin(), ModuluAssigns<value_type, T>());
			return *this;
		}

		template<typename T>
		Color& operator+=(T value)
		{
			std::for_each(begin(), end(), std::bind2nd(PlusAssigns<value_type, T>(), value));
			return *this;
		}

		template<typename T>
		Color& operator-=(T value)
		{
			std::for_each(begin(), end(), std::bind2nd(MinusAssigns<value_type, T>(), value));
			return *this;
		}

		template<typename T>
		Color& operator*=(T value)
		{
			std::for_each(begin(), end(), std::bind2nd(MultiplyAssigns<value_type, T>(), value));
			return *this;
		}

		template<typename T>
		Color& operator/=(T value)
		{
			std::for_each(begin(), end(), std::bind2nd(DivideAssigns<value_type, T>(), value));
			return *this;
		}

		template<typename T>
		Color& operator%=(T value)
		{
			std::for_each(begin(), end(), std::bind2nd(ModuluAssigns<value_type, T>(), value));
			return *this;
		}

		Color operator-() const
		{
			Color r;
			std::transform(begin(), end(), r.begin(), std::negate<Type>());
			return r;
		}

		bool operator==(const Color& v) const
		{
			for(size_t i = 0; i < Channel; ++i)
				if(my_data[i] != v[i])
					return false;

			return true;
		}

		iterator begin() { return my_data; }
		const_iterator begin() const { return my_data; }
		iterator end() { return my_data+Channel; }
		const_iterator end() const { return my_data+Channel; }

		static size_type channels() { return Channel; }

		std::ostream& output(std::ostream& os) const
		{
			os << cmlib::image::output(*begin());
			for (const_iterator v = begin()+1; v != end(); ++v)
				os << ' ' << cmlib::image::output(*v);
			return os;
		}

	protected:

	private:
		Type my_data[Channel];
	}; // }}}

	// VC will produce warning if I use struct here.
	template <typename T>
	class Color<T, 1> {
	public:
		typedef T ColorType;
	};

	//@{
	/**
	 *  @defgroup Color<Type, Channel> arithmetic functions.
	 */
	// Color + constant, constant + Color, Color + Color {{{
	// Color + constant
	template<typename ColorType, size_t Channel, typename Type>
	Color<ColorType, Channel> operator+(const Color<ColorType, Channel>& color, const Type& value)
	{
		Color<ColorType, Channel> tmp(color);
		tmp += value;
		return tmp;
	}
	// constant + Color
	template<typename Type, typename ColorType, size_t Channel>
	Color<ColorType, Channel> operator+(const Type& value, const Color<ColorType, Channel>& color)
	{
		Color<ColorType, Channel> tmp(color);
		tmp += value;
		return tmp;
	}
	// Color + Color
	template<typename Type1, typename Type2, size_t Channel>
	typename ColorSelector< Color<Type1, Channel>, Color<Type2, Channel> >::priority_type 
	operator+(const Color<Type1, Channel>& color1, const Color<Type2, Channel>& color2)
	{
		Color<typename ColorSelector<Type1, Type2>::priority_type, Channel> tmp(color1);
		tmp += color2;
		return tmp;
	}
	// }}}

	// Color - constant, constant - Color, Color - Color {{{
	// Color - constant
	template<typename ColorType, size_t Channel, typename Type>
	Color<ColorType, Channel> operator-(const Color<ColorType, Channel>& color, const Type& value)
	{
		Color<ColorType, Channel> tmp(color);
		tmp -= value;
		return tmp;
	}
	// constant - Color
	template<typename Type, typename ColorType, size_t Channel>
	Color<ColorType, Channel> operator-(const Type& value, const Color<ColorType, Channel>& color)
	{
		Color<ColorType, Channel> tmp(color);
		std::transform(tmp.begin(), tmp.end(), tmp.begin(), bind1st(std::minus<ColorType>(), value));
		return tmp;
	}
	// Color - Color
	template<typename Type1, typename Type2, size_t Channel>
	typename ColorSelector< Color<Type1, Channel>, Color<Type2, Channel> >::priority_type 
	operator-(const Color<Type1, Channel>& color1, const Color<Type2, Channel>& color2)
	{
		Color<typename ColorSelector<Type1, Type2>::priority_type, Channel> tmp(color1);
		tmp -= color2;
		return tmp;
	}
	// }}}

	// Color * constant, constant * Color, Color * Color {{{
	// Color * constant
	template<typename ColorType, size_t Channel, typename Type>
	Color<ColorType, Channel> operator*(const Color<ColorType, Channel>& color, const Type& value)
	{
		Color<ColorType, Channel> tmp(color);
		tmp *= value;
		return tmp;
	}
	// constant * Color
	template<typename Type, typename ColorType, size_t Channel>
	Color<ColorType, Channel> operator*(const Type& value, const Color<ColorType, Channel>& color)
	{
		Color<ColorType, Channel> tmp(color);
		tmp *= value;
		return tmp;
	}
	// Color * Color
	template<typename Type1, typename Type2, size_t Channel>
	typename ColorSelector< Color<Type1, Channel>, Color<Type2, Channel> >::priority_type 
	operator*(const Color<Type1, Channel>& color1, const Color<Type2, Channel>& color2)
	{
		Color<typename ColorSelector<Type1, Type2>::priority_type, Channel> tmp(color1);
		tmp *= color2;
		return tmp;
	}
	// }}}

	// Color / constant, constant / Color, Color / Color {{{
	// Color / constant
	template<typename ColorType, size_t Channel, typename Type>
	Color<ColorType, Channel> operator/(const Color<ColorType, Channel>& color, const Type& value)
	{
		Color<ColorType, Channel> tmp(color);
		tmp /= value;
		return tmp;
	}
	// constant / Color
	template<typename Type, typename ColorType, size_t Channel>
	Color<ColorType, Channel> operator/(const Type& value, const Color<ColorType, Channel>& color)
	{
		Color<ColorType, Channel> tmp(color);
		std::transform(tmp.begin(), tmp.end(), tmp.begin(), bind1st(std::divides<ColorType>(), value));
		return tmp;
	}
	// Color / Color
	template<typename Type1, typename Type2, size_t Channel>
	typename ColorSelector< Color<Type1, Channel>, Color<Type2, Channel> >::priority_type 
	operator/(const Color<Type1, Channel>& color1, const Color<Type2, Channel>& color2)
	{
		Color<typename ColorSelector<Type1, Type2>::priority_type, Channel> tmp(color1);
		tmp /= color2;
		return tmp;
	}
	// }}}

	// Color % constant, constant % Color, Color % Color {{{
	// Color % constant
	template<typename ColorType, size_t Channel, typename Type>
	Color<ColorType, Channel> operator%(const Color<ColorType, Channel>& color, const Type& value)
	{
		Color<ColorType, Channel> tmp(color);
		tmp %= value;
		return tmp;
	}
	// constant % Color
	template<typename Type, typename ColorType, size_t Channel>
	Color<ColorType, Channel> operator%(const Type& value, const Color<ColorType, Channel>& color)
	{
		Color<ColorType, Channel> tmp(color);
		std::transform(tmp.begin(), tmp.end(), tmp.begin(), bind1st(std::modulus<ColorType>(), value));
		return tmp;
	}
	// Color % Color
	template<typename Type1, typename Type2, size_t Channel>
	typename ColorSelector< Color<Type1, Channel>, Color<Type2, Channel> >::priority_type 
	operator%(const Color<Type1, Channel>& color1, const Color<Type2, Channel>& color2)
	{
		Color<typename ColorSelector<Type1, Type2>::priority_type, Channel> tmp(color1);
		tmp %= color2;
		return tmp;
	}
	// }}}
	//@}

	//@{
	/**
	 *  @defgroup Color<Type, Channel> output functions.
	 */
	template<typename Type, size_t Channel>
	const Color<Type, Channel>& output(const Color<Type, Channel>& color)
	{
		return color;
	}

	template <typename T, size_t C>
	std::ostream& operator<<(std::ostream& os, const Color<T, C>& color)
	{
		return color.output(os);
	}
	//@}

	// other utilities
	// minimal and maximal
	template <typename T, size_t C>
	Color<T,C> min(const Color<T,C>& a, const Color<T,C>& b)
	{
		Color<T,C> result;
		for(size_t i = 0; i < C; i++)
			result[i] = std::min(a[i], b[i]);
		return result;
	}

	template <typename T, size_t C>
	Color<T,C> max(const Color<T,C>& a, const Color<T,C>& b)
	{
		Color<T,C> result;
		for(size_t i = 0; i < C; i++)
			result[i] = std::max(a[i], b[i]);
		return result;
	}

	template <typename T, size_t C>
	Color<T,C> clamp(
		const Color<T,C>& value, 
		const Color<T,C>& lower, 
		const Color<T,C>& upper)
	{
		Color<T,C> result;
		for(size_t i = 0; i < C; i++)
			result[i] = clamp(value[i], lower[i], upper[i]);

		return result;
	}

	// linear interpolation
	template <typename T, size_t C>
	Color<T,C> mix(const Color<T,C>& a, const Color<T,C>& b, T w)
	{
		Color<T,C> result;
		for(size_t i = 0; i < C; i++)
			result[i] = TypeTrait<T>::mix(a[i], b[i], w);
		return result;
	}

	template <typename T, size_t C, typename W>
	Color<T,C> mix(const Color<T,C>& a, const Color<T,C>& b, W w)
	{
		Color<T,C> result;
		typedef TypeTrait<W> TTW;
		const W _w = TTW::max() - w;
		const W w_ = w - TTW::min();
		for(size_t i = 0; i < C; i++)
			result[i] = static_cast<T>( 
				( _w * a[i] + w_ * b[i] ) / TTW::domain() 
			);
		return result;
	}

	template <typename T>
	T mix(T a, T b, T w)
	{
		return TypeTrait<T>::mix(a, b, w);
	}

	template <typename T, typename W>
	T mix(T a, T b, W w)
	{
		typedef TypeTrait<W> TTW;
		const W _w = TTW::max() - w;
		const W w_ = w - TTW::min();
		return static_cast<T>( (_w * a + w_ * b) / TTW::domain() );
	}

	/** @brief cmlib::image::Color's ColorTrait properties.
	 *
	 */
	template <typename T, size_t C>
	struct ColorTrait< Color<T,C> > {
		typedef T BaseType;
		typedef Color<typename TypeTrait<T>::ExtendedType, C> ExtendedColor;
		static size_t channels() { return C; }
		static T& select_channel(Color<T, C>& color, size_t c)
		{
			return color[c];
		}
		static const T& select_channel(const Color<T, C>& color, size_t c)
		{
			return color[c];
		}
		enum { Channels = C };
	};

	typedef Color<Byte1, 3> Byte3;
	typedef Color<Byte1, 4> Byte4;
	typedef Color<Short1, 3> Short3;
	typedef Color<Short1, 4> Short4;
	typedef Color<Float1, 3> Float3;
	typedef Color<Float1, 4> Float4;
	typedef Color<Double1, 3> Double3;
	typedef Color<Double1, 4> Double4;

} // namespace image
} // namespace cmlib

#endif // CMLIB_IMAGE_COLOR_H
