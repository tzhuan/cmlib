#ifndef GIL_COLOR_H
#define GIL_COLOR_H

#include <iostream>
#include <iterator>
#include <algorithm>
#include <functional>

// set DLLAPI if we're using VC

#ifdef _MSC_VER
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

    // utilities
    template <typename T>
    T clamp(T value, T lower, T upper){
	return std::min( std::max(value, lower), upper );
    }

    // type traits
    template <typename T>
    struct TypeTrait {
	typedef T DebugType;
	
	static T zero();
	
	// for alpha channel
	static T transparent();
	static T opaque();
	static T mix(T a, T b, T w);
    };

    template <>
    struct TypeTrait<Byte1> {
	typedef int DebugType;
	static Byte1 zero() { return 0; }
	static Byte1 transparent() { return 0; }
	static Byte1 opaque() { return 255; }
	static Byte1 mix(Byte1 a, Byte1 b, Byte1 w)
	{
	    return ( a * (255-w) + b * w ) / 255;
	}
    };

    template <>
    struct TypeTrait<Short1> {
	typedef int DebugType;
	static Short1 zero() { return 0; }
	static Short1 transparent() { return 0; }
	static Short1 opaque() { return 65535; }
	static Short1 mix(Short1 a, Short1 b, Short1 w)
	{
	    return ( a * (65535-w) + b * w ) / 65535;
	}
    };

    template <>
    struct TypeTrait<Float1> {
	typedef float DebugType;
	static Float1 zero() { return 0.0f; }
	static Float1 transparent() { return 0.0f; }
	static Float1 opaque() { return 1.0f; }
	static Float1 mix(Float1 a, Float1 b, Float1 w)
	{
	    return a * (1.0f-w) + b * w;
	}
    };
    
    template <>
    struct TypeTrait<double> {
	typedef double DebugType;
	static double zero() { return 0.0; }
	static double transparent() { return 0.0; }
	static double opaque() { return 1.0; }
	static double mix(double a, double b, double w)
	{
	    return a * (1.0-w) + b * w;
	}
    };
    
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
	    
	    RefType operator [](int i){ return my_data[i]; }
	    
	    ConstRefType operator [](int i) const { return my_data[i]; }

	    Color(){
		std::fill(begin(), end(), TypeTrait<Type>::zero());
	    }

	    Color(Type c0){
		set(c0);
	    }

	    Color(Type c0, Type c1, Type c2){
		set(c0, c1, c2);
	    }

	    Color(Type c0, Type c1, Type c2, Type c3){
		set(c0, c1, c2, c3);
	    }

	    void fill(Type c0){
		std::fill(begin(), end(), c0);
	    }
	    
	    void set(Type c0){
		fill(c0);
	    }

	    void set(Type c0, Type c1, Type c2){
		my_data[0] = c0;
		if(Channel >= 2) my_data[1] = c1;
		if(Channel >= 3) my_data[2] = c2;
	    }
	    
	    void set(Type c0, Type c1, Type c2, Type c3){
		my_data[0] = c0;
		if(Channel >= 2) my_data[1] = c1;
		if(Channel >= 3) my_data[2] = c2;
		if(Channel >= 4) my_data[3] = c3;
	    }

	    ColorType& operator =(const Color<Type,3>& color){
		set(color[0], color[1], color[2]);
		return *this;
	    }

	    ColorType& operator =(const Color<Type,4>& color){
		set(color[0], color[1], color[2], color[3]);
		return *this;
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

    // default behavior to print all channels in a pixel
    // to avoid unprintable characters, values are converted
    // to DebugType defined in TypeTrait.
    template <typename T, size_t C>
    std::ostream& operator <<(std::ostream& out, Color<T, C> color){
	typedef typename TypeTrait<T>::DebugType DebugType;
	std::copy(color.begin(),
		  color.end(),
		  std::ostream_iterator<DebugType>(out, " "));
	return out;
    }

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
	const W opaque = TypeTrait<W>::opaque();
	W u = opaque - w;
	for(size_t i = 0; i < C; i++)
	    result[i] = static_cast<T>( ( u * a[i] + w * b[i] ) / opaque );
	return result;
    }
    
    template <typename T>
    struct ColorTrait {
	typedef T BaseType;
	static size_t channels() { return 1; }
    };
    
    template <typename T, size_t C>
    struct ColorTrait< Color<T,C> > {
	typedef T BaseType;
	static size_t channels() { return C; }
    };

    typedef Color<Byte1,3> Byte3;
    typedef Color<Byte1,4> Byte4;
    typedef Color<Short1,3> Short3;
    typedef Color<Short1,4> Short4;
    typedef Color<Float1,3> Float3;
    typedef Color<Float1,4> Float4;

} // namespace gil

#endif // GIL_COLOR_H
