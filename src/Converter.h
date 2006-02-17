#ifndef GIL_CONVERTER_H
#define GIL_CONVERTER_H

#include "Color.h"

namespace gil {

    template <typename T, typename F>
    struct SimpleConverter {
	static T convert(F v)
	{
	    return static_cast<T>(v);
	}
    };


    template <>
    struct SimpleConverter<Byte1, Float1> {
	static Byte1 convert(Float1 v)
	{
	    return static_cast<Byte1>(v * 255.0f);
	}
    };

    template <>
    struct SimpleConverter<Float1, Byte1> {
	static Float1 convert(Float1 v)
	{
	    return static_cast<Float1>(v) / 255.0f;
	}
    };
    

    template <typename T, typename F>
    struct DefaultConverter {
	static T convert(const F& v)
	{
	    return static_cast<T>(v);
	}
    };

    template <typename T>
    struct DefaultConverter<T, T> {
	static T convert(const T& v)
	{
	    return v;
	}
    };

    template <typename T, typename F, size_t C>
    struct DefaultConverter< Color<T,C>, Color<F,C> > {
	static Color<T,C> convert(const Color<F,C>& v)
	{
	    Color<T,C> result;
	    result[0] = SimpleConverter<T,F>::convert(v[0]);
	    if(C >= 2) result[1] = SimpleConverter<T,F>::convert(v[1]);
	    if(C >= 3) result[2] = SimpleConverter<T,F>::convert(v[2]);
	    if(C >= 4) result[3] = SimpleConverter<T,F>::convert(v[3]);
	    
	    if(C >= 5)
		for(size_t i = 4; i < C; i++)
		    result[i] = SimpleConverter<T,F>::convert(v[i]);

	    return result;
	}
    };

    template <typename T>
    struct DefaultConverter< T, Color<T,3> > {
	static T convert(const Color<T,3>& v)
	{
	    return ( v[0]*3 + v[1]*4 + v[2] ) / 8;
	}
    };
    
    template <typename T>
    struct DefaultConverter< T, Color<T,4> > {
	static T convert(const Color<T,4>& v)
	{
	    return ( v[0]*3 + v[1]*4 + v[2] ) / 8;
	}
    };

    template <typename T>
    struct DefaultConverter< Color<T,4>, T> {
	static Color<T,4> convert(T v)
	{
	    return Color<T,4>( v, v, v, TypeTrait<T>::opaque() );
	}
    };
    
    template <typename T>
    struct DefaultConverter< Color<T,3>, T> {
	static Color<T,3> convert(T v)
	{
	    return Color<T,3>(v, v, v);
	}
    };
    
    template <typename T>
    struct DefaultConverter< Color<T,3>, Color<T,4> > {
	static Color<T,3> convert(const Color<T,4>& v)
	{
	    return Color<T,3>(v[0], v[1], v[2]);
	}
    };
    
    template <typename T>
    struct DefaultConverter< Color<T,4>, Color<T,3> > {
	static Color<T,4> convert(const Color<T,3>& v)
	{
	    return Color<T,4>( v[0], v[1], v[2], TypeTrait<T>::opaque() );
	}
    };

    /*
    template <typename T>
    struct Converter {
	template <typename F>
	T operator () const (const F& v)
	{
	    return DefaultConverter<T, F>::convert(v);
	}
    };*/

} // namespace gil

#endif // GIL_CONVERTER_H
