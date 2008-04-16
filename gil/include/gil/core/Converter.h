#ifndef GIL_CONVERTER_H
#define GIL_CONVERTER_H

#include <algorithm>
#include "Color.h"

namespace gil {

	template <typename Tt, typename Tf>
	struct DefaultConverter {
		typedef Tt To;
		typedef Tf From;
		const To operator()(const From &from) const
		{
			return static_cast<To>(from);
		}
	};

	// Float1 -> Byte1
	template <>
	struct DefaultConverter<Byte1, Float1> {
		typedef Byte1 To;
		typedef Float1 From;
		const Byte1 operator()(Float1 from) const
		{
			const Float1 ratio = 
				static_cast<Float1>(TypeTrait<Byte1>::opaque());
			return static_cast<Byte1>(from * ratio);
		}
	};

	// Byte1 -> Float1
	template <>
	struct DefaultConverter<Float1, Byte1> {
		typedef Float1 To;
		typedef Byte1 From;
		const Float1 operator()(Byte1 from) const
		{
			const Float1 ratio = 
				static_cast<Float1>(TypeTrait<Byte1>::opaque());
			return static_cast<Float1>(from) / ratio;
		}
	};

	// Byte1 -> Short1
	template <>
	struct DefaultConverter<Short1, Byte1> {
		typedef Short1 To;
		typedef Byte1 From;
		const Short1 operator()(Byte1 from) const
		{
			const Short1 ratio = 
				TypeTrait<Short1>::opaque() / TypeTrait<Byte1>::opaque();
			return static_cast<Short1>(from * ratio);
		}
	};

	// Short1 -> Byte1
	template <>
	struct DefaultConverter<Byte1, Short1> {
		typedef Byte1 To;
		typedef Short1 From;
		const Byte1 operator()(Short1 from) const
		{
			const Short1 ratio = 
				TypeTrait<Short1>::opaque() / TypeTrait<Byte1>::opaque();
			return static_cast<Byte1>(from / ratio);
		}
	};

	// Float1 -> Short1
	template <>
	struct DefaultConverter<Short1, Float1> {
		typedef Short1 To;
		typedef Float1 From;
		const Short1 operator()(Float1 from) const
		{
			const Short1 ratio = TypeTrait<Short1>::opaque();
			return static_cast<Short1>(from * ratio);
		}
	};

	// Short1 -> Float1
	template <>
	struct DefaultConverter<Float1, Short1> {
		typedef Float1 To;
		typedef Short1 From;
		const Float1 operator()(Short1 from) const
		{
			const Short1 ratio = TypeTrait<Short1>::opaque();
			return static_cast<Float1>(from) / ratio;
		}
	};

    // exactly same 
    template <typename T, size_t C>
    struct DefaultConverter< Color<T,C>, Color<T,C> > {
        typedef Color<T,C> To;
        typedef Color<T,C> From;
        const To operator()(const From& from) const
        {
			return from;
        }
    };  

	template <typename T>
	struct DefaultConverter<T, T> {
		typedef T To;
		typedef T From;
		const T operator()(const T& from) const
		{
			return from;
		}
	};

	// different type, same channel number
	template <typename T, typename F, size_t C>
	struct DefaultConverter< Color<T,C>, Color<F,C> > {
		typedef Color<T,C> To;
		typedef Color<F,C> From;
		const To operator()(const From& from) const
		{
			DefaultConverter<T, F> converter;
			To tmp;
			tmp[0] = converter(from[0]);
			if(C >= 2) tmp[1] = converter(from[1]);
			if(C >= 3) tmp[2] = converter(from[2]);
			if(C >= 4) tmp[3] = converter(from[3]);

			if(C >= 5)
				std::transform(&from[4], &from[C], &tmp[4], converter);

			return tmp;
		}
	};

	// same type, different channel number
	// this covers 3 -> 4 and 4 -> 3
	template <typename T, size_t Ct, size_t Cf>
	struct DefaultConverter< Color<T,Ct>, Color<T,Cf> > {
		typedef Color<T,Ct> To;
		typedef Color<T,Cf> From;
		const To operator()(const From& from) const
		{
			const size_t cmin = std::min(Ct, Cf);
			To tmp;

			tmp[0] = from[0];
			if(cmin >= 2) tmp[1] = from[1];
			if(cmin >= 3) tmp[2] = from[2];
			if(cmin >= 4) tmp[3] = from[3];

			if(cmin >= 5)
				std::copy(&from[4], &from[int(cmin)], &tmp[4]);

			if (Cf < Ct)
				std::fill( &tmp[Cf], &tmp[Ct], TypeTrait<T>::opaque() );

			return tmp;
		}
	};

	// 3 -> 1
	template <typename T>
	struct DefaultConverter< T, Color<T,3> > {
		typedef T To;
		typedef Color<T,3> From;
		const To operator()(const From& from) const
		{
			return ( from[0]*3 + from[1]*4 + from[2] ) / 8;
		}
	};

	// 4 -> 1
	template <typename T>
	struct DefaultConverter< T, Color<T,4> > {
		typedef T To;
		typedef Color<T,4> From;
		const To operator()(const From& from) const
		{
			return ( from[0]*3 + from[1]*4 + from[2] ) / 8;
		}
	};

	// 1 -> 3
	template <typename T>
	struct DefaultConverter< Color<T,3>, T> {
		typedef Color<T,3> To;
		typedef T From;
		const To operator()(From from) const
		{
			return To(from, from, from);
		}
	};

	// 1 -> 4
	template <typename T>
	struct DefaultConverter< Color<T,4>, T> {
		typedef Color<T,4> To;
		typedef T From;
		const To operator()(From from) const
		{
			return To( from, from, from, TypeTrait<T>::opaque() );
		}
	};

	template <typename Tt, typename Tf, size_t Cf>
	struct DefaultConverter< Tt, Color<Tf, Cf> > {
		typedef Tt To;
		typedef Color<Tf, Cf> From;
		const To operator()(const From& from) const
		{
			DefaultConverter<Tf, From> channel_converter;
			DefaultConverter<To, Tf> type_converter;
			return type_converter( channel_converter(from) );
		}
	};

	template <typename Tt, typename Tf, size_t Ct>
	struct DefaultConverter< Color<Tt,Ct>, Tf > {
		typedef Color<Tt,Ct> To;
		typedef Tf From;
		const To operator()(const From& from) const
		{
			DefaultConverter<Tt, From> type_converter;
			DefaultConverter<To, Tt> channel_converter;
			return channel_converter( type_converter(from) );
		}
	};

	// arbitary conversion
	template <typename Tt, size_t Ct, typename Tf, size_t Cf>
	struct DefaultConverter< Color<Tt,Ct>, Color<Tf,Cf> > {
		typedef Color<Tt,Ct> To;
		typedef Color<Tf,Cf> From;
		const To operator()(const From& from) const
		{
			// choose a smaller type to operator() first
			typedef Color<Tt,Cf> TMP1;
			typedef Color<Tf,Ct> TMP2;
			if( sizeof(TMP1) < sizeof(TMP2) ){
				DefaultConverter< TMP1, From > converter1;
				DefaultConverter< To, TMP1 > converter2;
				return converter2( converter1(from) );
			} else {
				DefaultConverter< TMP2, From > converter1;
				DefaultConverter< To, TMP2 > converter2;
				return converter2( converter1(from) );
			}
		}
	};

} // namespace gil

#endif // GIL_CONVERTER_H
