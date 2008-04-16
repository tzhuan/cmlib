#ifndef GIL_COLOR_H
#define GIL_COLOR_H

#include <iostream>
#include <iterator>
#include <algorithm>
#include <functional>
#include <limits>

// set DLLAPI if we're using VC

#ifdef _MSC_VER
	#pragma warning(disable: 4996)

	#ifdef BUILD_GIL_DLL
		#define DLLAPI __declspec(dllexport)
	#elif defined(LINK_GIL_DLL)
		#define DLLAPI __declspec(dllimport)
	#else
		#define DLLAPI
	#endif // BUILD_GIL_DLL
#else
	#define DLLAPI
#endif // _MSC_VER

namespace gil {
	// scalar type, may be used later...
	typedef unsigned char Byte1;
	typedef unsigned short Short1;
	typedef float Float1;
	typedef double Double1;

	// utilities
	template <typename T>
	T clamp(T value, T lower, T upper){
		return std::min( std::max(value, lower), upper );
	}

	// type traits
	template <typename T>
	class TypeTrait {
		public:
			typedef T DebugType;
			typedef float ExtendedType;
			typedef float MathType;

			static T zero() { return 0; }

			// for alpha channel
			static T transparent() { return 0; } 
			static T opaque() { return std::numeric_limits<T>::max(); }
			static T mix(T a, T b, T w)
			{
				return ( a * (opaque()-w) + b * w ) / opaque();
			}
	};
	
	template <> inline Float1 TypeTrait<float>::opaque() { return 1.0f; }
	template <> inline Double1 TypeTrait<double>::opaque() { return 1.0; }

	// Basic features for a pixel
	template <typename Type, size_t Channel>
	class Color {
		public:
			typedef Color<Type,Channel> ColorType;
			typedef const Color<Type,Channel> ConstColorType;
			typedef Type* PtrType;
			typedef const Type* ConstPtrType;
			typedef Type& RefType;
			typedef const Type& ConstRefType;

			typedef typename TypeTrait<Type>::MathType MathType;

			RefType operator [](int i){ return my_data[i]; }

			ConstRefType operator [](int i) const { return my_data[i]; }

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
			ColorType& operator =(const Color<T, 3>& color)
			{
				set(color[0], color[1], color[2]);
				return *this;
			}

			template<typename T>
			ColorType& operator =(const Color<T, 4>& color)
			{
				set(color[0], color[1], color[2], color[3]);
				return *this;
			}

			// some calculations...
			template<typename T>
			ColorType& operator +=(const Color<T, Channel>& color)
			{
				for (size_t i = 0; i < Channel; ++i)
					(*this)[i] += static_cast<Type>(color[i]);
				return *this;
			}

			template<typename T>
			ColorType& operator -=(const Color<T, Channel>& color)
			{
				for (size_t i = 0; i < Channel; ++i)
					(*this)[i] -= static_cast<Type>(color[i]);
				return *this;
			}

			template<typename T>
			ColorType& operator *=(T v)
			{
				for (size_t i = 0; i < Channel; ++i)
					(*this)[i] *= static_cast<Type>(v);
				return *this;
			}

			template<typename T>
			ColorType& operator /=(T v)
			{
				for (size_t i = 0; i < Channel; ++i)
					(*this)[i] /= static_cast<Type>(v);
				return *this;
			}

			ConstColorType operator -() const
			{
				ColorType r;
				std::transform(begin(), end(), r.begin(), std::negate<Type>());
				return r;
			}

			template<typename T>
			ConstColorType operator +(const Color<T, Channel>& c) const
			{
				ColorType r(*this);
				r += c;
				return r;
			}

			template<typename T>
			ConstColorType operator -(const Color<T, Channel>& c) const
			{
				ColorType r(*this);
				r -= c;
				return r;
			}

			template<typename T>
			ConstColorType operator *(T v) const
			{
				ColorType r(*this);
				r *= v;
				return r;
			}

			template<typename T>
			ConstColorType operator /(T v) const
			{
				ColorType r(*this);
				r /= v;
				return r;
			}

			PtrType begin() { return my_data; }
			ConstPtrType begin() const { return my_data; }
			PtrType end() { return my_data+Channel; }
			ConstPtrType end() const { return my_data+Channel; }

			static size_t channels() { return Channel; }

		protected:
			Type my_data[Channel];
	};

	// VC will produce warning if I use struct here.
	template <typename T>
	class Color<T, 1> {
		public:
			typedef T ColorType;
	};

	// Scalar * Color
	template <typename S, typename T, size_t C>
	const Color<T,C> operator *(S v, const Color<T,C>& c)
	{
		return (c * v);
	}

	// Scalar / Color
	template <typename S, typename T, size_t C>
	const Color<T,C> operator /(S v, const Color<T,C>& c)
	{
		Color<T,C> r;
		for (size_t i = 0; i < C; ++i)
			r[i] = v / c[i];
		return r;
	}

	// default behavior to print all channels in a pixel
	// to avoid unprintable characters, values are converted
	// to DebugType defined in TypeTrait.
	/*
	template <typename T>
	std::ostream& operator <<(std::ostream& out, const T& color)
	{
		typedef typename TypeTrait<T>::DebugType DebugType;
		out << static_cast<DebugType>(color);
		return out;
	}
	*/

	template <typename T, size_t C>
	std::ostream& operator <<(std::ostream& out, const Color<T, C>& color)
	{
		typedef typename TypeTrait<T>::DebugType DebugType;
		std::copy(color.begin(),
				color.end(),
				std::ostream_iterator<DebugType>(out, " "));
		return out;
	}

	// other utilities
	// minimal and maximal
	template <typename T, size_t C>
	const Color<T,C> min(const Color<T,C>& a, const Color<T,C>& b)
	{
		Color<T,C> result;
		for(size_t i = 0; i < C; i++)
			result[i] = std::min(a[i], b[i]);
		return result;
	}

	template <typename T, size_t C>
	const Color<T,C> max(const Color<T,C>& a, const Color<T,C>& b)
	{
		Color<T,C> result;
		for(size_t i = 0; i < C; i++)
			result[i] = std::max(a[i], b[i]);
		return result;
	}

	template <typename T, size_t C>
	const Color<T,C> clamp(
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
	const Color<T,C> mix(const Color<T,C>& a, const Color<T,C>& b, T w)
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
		const W opaque = TypeTrait<W>::opaque();
		W u = opaque - w;
		for(size_t i = 0; i < C; i++)
			result[i] = static_cast<T>( ( u * a[i] + w * b[i] ) / opaque );
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
		W o = TypeTrait<W>::opaque();
		W u = o - w;
		return static_cast<T>( (u * a + w * b) / o );
	}

	template <typename T>
	struct ColorTrait {
		typedef T BaseType;
		typedef typename TypeTrait<T>::ExtendedType ExtendedColor;
		static size_t channels() { return 1; }
		static inline T& select_channel(T& color, size_t) 
		{ 
			return color; 
		}
		static inline const T& select_channel(const T& color, size_t) 
		{ 
			return color;
		}
		enum { Channels = 1 };
	};

	template <typename T, size_t C>
	struct ColorTrait< Color<T,C> > {
		typedef T BaseType;
		typedef Color<typename TypeTrait<T>::ExtendedType, C> ExtendedColor;
		static size_t channels() { return C; }
		static inline T& select_channel(Color<T, C>& color, size_t c)
		{
			return color[c];
		}
		static inline const T& select_channel(const Color<T, C>& color, size_t c)
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

} // namespace gil

#endif // GIL_COLOR_H
