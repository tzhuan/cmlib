#ifndef GIL_CONVERTER_H
#define GIL_CONVERTER_H

#include "Color.h"

namespace gil {


    template <typename T, typename F>
    struct DefaultConverter {
	static T convert(const F& v)
	{
	    return static_cast<T>(v);
	}
    };
    
    template <>
    struct DefaultConverter<Byte1, Float1> {
	static Byte1 convert(Float1 v)
	{
	    return static_cast<Byte1>(v * 255.0f);
	}
    };

    template <>
    struct DefaultConverter<Float1, Byte1> {
	static Float1 convert(Byte1 v)
	{
	    return static_cast<Float1>(v) / 255.0f;
	}
    };
    

    template <typename T>
    struct DefaultConverter<T, T> {
	static T convert(const T& v)
	{
	    return v;
	}
    };

    // exactly same
    template <typename T, size_t C>
    struct DefaultConverter< Color<T,C>, Color<T,C> > {
	static Color<T,C> convert(const Color<T,C>& v)
	{
	    return v;
	}
    };
    
    // different type, same channel number
    template <typename T, typename F, size_t C>
    struct DefaultConverter< Color<T,C>, Color<F,C> > {
	static Color<T,C> convert(const Color<F,C>& v)
	{
	    Color<T,C> result;
	    result[0] = DefaultConverter<T,F>::convert(v[0]);
	    if(C >= 2) result[1] = DefaultConverter<T,F>::convert(v[1]);
	    if(C >= 3) result[2] = DefaultConverter<T,F>::convert(v[2]);
	    if(C >= 4) result[3] = DefaultConverter<T,F>::convert(v[3]);
	    
	    if(C >= 5)
		for(size_t i = 4; i < C; i++)
		    result[i] = DefaultConverter<T,F>::convert(v[i]);

	    return result;
	}
    };

    // same type, different channel number
    template <typename T, size_t C1, size_t C2>
    struct DefaultConverter< Color<T,C1>, Color<T,C2> > {
	static Color<T,C1> convert(const Color<T,C2>& v)
	{
	    Color<T,C1> result;
	    const size_t cmin = C1 < C2 ? C1 : C2;
	    result[0] = v[0];
	    if(cmin >= 2) result[1] = v[1];
	    if(cmin >= 3) result[2] = v[2];
	    if(cmin >= 4) result[3] = v[3];
	    
	    if(cmin >= 5)
		for(size_t i = 4; i < cmin; i++)
		    result[i] = v[i];

	    if(C1 > C2)
		for(size_t i = C2; i < C1; i++)
		    result[i] = TypeTrait<T>::opaque();
	    
	    return result;
	}
    };

    // 3 -> 1
    template <typename T>
    struct DefaultConverter< T, Color<T,3> > {
	static T convert(const Color<T,3>& v)
	{
	    return ( v[0]*3 + v[1]*4 + v[2] ) / 8;
	}
    };
    
    // 4 -> 1
    template <typename T>
    struct DefaultConverter< T, Color<T,4> > {
	static T convert(const Color<T,4>& v)
	{
	    return ( v[0]*3 + v[1]*4 + v[2] ) / 8;
	}
    };

    // 1 -> 4
    template <typename T>
    struct DefaultConverter< Color<T,4>, T> {
	static Color<T,4> convert(T v)
	{
	    return Color<T,4>( v, v, v, TypeTrait<T>::opaque() );
	}
    };
    
    // 1 -> 3
    template <typename T>
    struct DefaultConverter< Color<T,3>, T> {
	static Color<T,3> convert(T v)
	{
	    return Color<T,3>(v, v, v);
	}
    };
    
    // 4 -> 3
    template <typename T>
    struct DefaultConverter< Color<T,3>, Color<T,4> > {
	static Color<T,3> convert(const Color<T,4>& v)
	{
	    return Color<T,3>(v[0], v[1], v[2]);
	}
    };
    
    // 3 -> 4
    template <typename T>
    struct DefaultConverter< Color<T,4>, Color<T,3> > {
	static Color<T,4> convert(const Color<T,3>& v)
	{
	    return Color<T,4>( v[0], v[1], v[2], TypeTrait<T>::opaque() );
	}
    };

    // arbitary conversion
    template <typename T1, size_t C1, typename T2, size_t C2>
    struct DefaultConverter< Color<T1,C1>, Color<T2,C2> > {
	static Color<T1,C1> convert(const Color<T2,C2>& v)
	{
	    // choose a smaller type to convert first
	    if( sizeof(Color<T1,C2>) < sizeof(Color<T2,C1>) ){
		return DefaultConverter< Color<T1,C1>, Color<T1,C2> >::convert(
			DefaultConverter< Color<T1,C2>, Color<T2,C2> >::convert(v) );
	    } else {
		return DefaultConverter< Color<T1,C1>, Color<T2,C1> >::convert(
			DefaultConverter< Color<T2,C1>, Color<T2,C2> >::convert(v) );
	    }
	}
    };

    template <typename T>
    struct Converter {
	template <typename F>
	T operator ()(const F& v) const
	{
	    return DefaultConverter<T, F>::convert(v);
	}
    };

} // namespace gil

#endif // GIL_CONVERTER_H
