#ifndef GIL_CONVERTER_H
#define GIL_CONVERTER_H

#include "Color.h"

namespace gil {

    template <typename T, typename F>
    struct DefaultConverter {
	typedef T To;
	typedef F From;
	static void convert(To& to, const From& from)
	{
	    to = static_cast<T>(from);
	}
    };
    
    template <>
    struct DefaultConverter<Byte1, Float1> {
	typedef Byte1 To;
	typedef Float1 From;
	static void convert(To& to, From from)
	{
	    to = static_cast<Byte1>(from * 255.0f);
	}
    };

    template <>
    struct DefaultConverter<Float1, Byte1> {
	typedef Float1 To;
	typedef Byte1 From;
	static void convert(To& to, From from)
	{
	    to = static_cast<Float1>(from) / 255.0f;
	}
    };

    template <>
    struct DefaultConverter<Short1, Byte1> {
	typedef Short1 To;
	typedef Byte1 From;
	static void convert(To& to, From from)
	{
	    to = static_cast<Short1>(from * 255);
	}
    };

    template <>
    struct DefaultConverter<Byte1, Short1> {
	typedef Byte1 To;
	typedef Short1 From;
	static void convert(To& to, From from)
	{
	    to = static_cast<Byte1>(from / 255);
	}
    };

    template <>
    struct DefaultConverter<Short1, Float1> {
	typedef Short1 To;
	typedef Float1 From;
	static void convert(To& to, From from)
	{
	    to = static_cast<Short1>(from * 65535.0f);
	}
    };
    
    template <>
    struct DefaultConverter<Float1, Short1> {
	typedef Float1 To;
	typedef Short1 From;
	static void convert(To& to, From from)
	{
	    to = static_cast<Float1>(from) / 65535.0f;
	}
    };

    template <typename T>
    struct DefaultConverter<T, T> {
	typedef T To;
	typedef T From;
	static void convert(T& to, const T& from)
	{
	    to = from;
	}
    };

    // exactly same
    template <typename T, size_t C>
    struct DefaultConverter< Color<T,C>, Color<T,C> > {
	typedef Color<T,C> To;
	typedef Color<T,C> From;
	static void convert(To& to, const From& from)
	{
	    to = from;
	}
    };
    
    // different type, same channel number
    template <typename T, typename F, size_t C>
    struct DefaultConverter< Color<T,C>, Color<F,C> > {
	typedef Color<T,C> To;
	typedef Color<F,C> From;
	static void convert(To& to, const From& from)
	{
	    DefaultConverter<T,F>::convert(to[0], from[0]);
	    if(C >= 2) DefaultConverter<T,F>::convert(to[1], from[1]);
	    if(C >= 3) DefaultConverter<T,F>::convert(to[2], from[2]);
	    if(C >= 4) DefaultConverter<T,F>::convert(to[3], from[3]);
	    
	    if(C >= 5)
		for(int i = 4; i < static_cast<int>(C); i++)
		    DefaultConverter<T,F>::convert(to[i], from[i]);
	}
    };

    // same type, different channel number
    // this covers 3 -> 4 and 4 -> 3
    template <typename T, size_t C1, size_t C2>
    struct DefaultConverter< Color<T,C1>, Color<T,C2> > {
	typedef Color<T,C1> To;
	typedef Color<T,C2> From;
	static void convert(To& to, const From& from)
	{
	    const int cmin = C1 < C2 ? (int)C1 : (int)C2;
	    to[0] = from[0];
	    if(cmin >= 2) to[1] = from[1];
	    if(cmin >= 3) to[2] = from[2];
	    if(cmin >= 4) to[3] = from[3];
	    
	    if(cmin >= 5)
		for(int i = 4; i < cmin; i++)
		    to[i] = from[i];

	    for(int i = (int)C2; i < (int)C1; i++)
		to[i] = TypeTrait<T>::opaque();
	}
    };

    // 3 -> 1
    template <typename T>
    struct DefaultConverter< T, Color<T,3> > {
	typedef T To;
	typedef Color<T,3> From;
	static void convert(To& to, const From& from)
	{
	    to = ( from[0]*3 + from[1]*4 + from[2] ) / 8;
	}
    };
    
    // 4 -> 1
    template <typename T>
    struct DefaultConverter< T, Color<T,4> > {
	typedef T To;
	typedef Color<T,4> From;
	static void convert(To& to, const From& from)
	{
	    to = ( from[0]*3 + from[1]*4 + from[2] ) / 8;
	}
    };

    // 1 -> 3
    template <typename T>
    struct DefaultConverter< Color<T,3>, T> {
	typedef Color<T,3> To;
	typedef T From;
	static void convert(To& to, From from)
	{
	    to.set(from, from, from);
	}
    };
    
    // 1 -> 4
    template <typename T>
    struct DefaultConverter< Color<T,4>, T> {
	typedef Color<T,4> To;
	typedef T From;
	static void convert(To& to, From from)
	{
	    to.set( from, from, from, TypeTrait<T>::opaque() );
	}
    };

    template <typename T1, typename T2, size_t C>
    struct DefaultConverter< T1, Color<T2,C> > {
	typedef T1 To;
	typedef Color<T2,C> From;
	static void convert(To& to, const From& from){
	    T2 tmp;
	    DefaultConverter<T2, From>::convert(tmp, from);
	    DefaultConverter<To, T2>::convert(to, tmp);
	}
    };
    
    template <typename T1, typename T2, size_t C>
    struct DefaultConverter< Color<T1,C>, T2 > {
	typedef Color<T1,C> To;
	typedef T2 From;
	static void convert(To& to, const From& from){
	    T1 tmp;
	    DefaultConverter<T1, From>::convert(tmp, from);
	    DefaultConverter<To, T1>::convert(to, tmp);
	}
    };
    
    // arbitary conversion
    template <typename T1, size_t C1, typename T2, size_t C2>
    struct DefaultConverter< Color<T1,C1>, Color<T2,C2> > {
	typedef Color<T1,C1> To;
	typedef Color<T2,C2> From;
	static void convert(To& to, const From& from)
	{
	    // choose a smaller type to convert first
	    typedef Color<T1,C2> TMP1;
	    typedef Color<T2,C1> TMP2;
	    if( sizeof(TMP1) < sizeof(TMP2) ){
		TMP1 tmp;
		DefaultConverter< TMP1, From >::convert(tmp, from);
		DefaultConverter< To, TMP1 >::convert(to, tmp);
	    } else {
		TMP2 tmp;
		DefaultConverter< TMP2, From >::convert(tmp, from);
		DefaultConverter< To, TMP2 >::convert(to, tmp);
	    }
	}
    };

} // namespace gil

#endif // GIL_CONVERTER_H
