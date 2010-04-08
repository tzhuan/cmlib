#ifndef CMLIB_IMAGE_CONVERTER_H
#define CMLIB_IMAGE_CONVERTER_H

#include <algorithm>

#include "Color.h"
#include "ColorSelector.h"

namespace cmlib {
namespace image {

	template <typename Tf, typename Tt>
	struct DefaultConverter {
		typedef Tt To;
		typedef Tf From;
		To operator()(const From &from) const
		{
			typedef typename TypeTrait<
				typename ColorSelector<
					typename TypeTrait<From>::channel_type, 
					typename TypeTrait<To>::channel_type
				>::priority_type
			>::extended_type ratio_type;
			const ratio_type ratio = 
				static_cast<ratio_type>(TypeTrait<To>::domain()) / 
				static_cast<ratio_type>(TypeTrait<From>::domain());
			return static_cast<To>(from * ratio);
		}
	};

    // exactly same 
    template <typename T, size_t C>
    struct DefaultConverter< Color<T,C>, Color<T,C> > {
        typedef Color<T,C> From;
        typedef Color<T,C> To;
        To operator()(const From& from) const
        {
			return from;
        }
    };  

	template <typename T>
	struct DefaultConverter<T, T> {
		typedef T From;
		typedef T To;
		T operator()(const T& from) const
		{
			return from;
		}
	};

	// different type, same channel number
	template <typename F, typename T, size_t C>
	struct DefaultConverter< Color<F,C>, Color<T,C> > {
		typedef Color<F,C> From;
		typedef Color<T,C> To;
		To operator()(const From& from) const
		{
			DefaultConverter<F, T> converter;
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
	template <typename T, size_t Cf, size_t Ct>
	struct DefaultConverter< Color<T,Cf>, Color<T,Ct> > {
		typedef Color<T,Cf> From;
		typedef Color<T,Ct> To;
		To operator()(const From& from) const
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
				std::fill( &tmp[Cf], &tmp[Ct], TypeTrait<T>::domain() );

			return tmp;
		}
	};

	// 3 -> 1
	template <typename T>
	struct DefaultConverter< Color<T,3>, T > {
		typedef Color<T,3> From;
		typedef T To;
		To operator()(const From& from) const
		{
			return ( from[0]*3 + from[1]*4 + from[2] ) / 8;
		}
	};

	// 4 -> 1
	template <typename T>
	struct DefaultConverter< Color<T,4>, T > {
		typedef Color<T,4> From;
		typedef T To;
		To operator()(const From& from) const
		{
			return ( from[0]*3 + from[1]*4 + from[2] ) / 8;
		}
	};

	// 1 -> 3
	template <typename T>
	struct DefaultConverter< T, Color<T,3> > {
		typedef T From;
		typedef Color<T,3> To;
		To operator()(From from) const
		{
			return To(from, from, from);
		}
	};

	// 1 -> 4
	template <typename T>
	struct DefaultConverter< T, Color<T,4> > {
		typedef T From;
		typedef Color<T,4> To;
		To operator()(From from) const
		{
			return To( from, from, from, TypeTrait<T>::domain() );
		}
	};

	template <typename Tf, typename Tt, size_t Cf>
	struct DefaultConverter< Color<Tf, Cf>, Tt > {
		typedef Color<Tf, Cf> From;
		typedef Tt To;
		To operator()(const From& from) const
		{
			DefaultConverter<From, Tf> channel_converter;
			DefaultConverter<Tf, To> type_converter;
			return type_converter( channel_converter(from) );
		}
	};

	template <typename Tf, typename Tt, size_t Ct>
	struct DefaultConverter< Tf, Color<Tt,Ct> > {
		typedef Tf From;
		typedef Color<Tt,Ct> To;
		To operator()(const From& from) const
		{
			DefaultConverter<From, Tt> type_converter;
			DefaultConverter<Tt, To> channel_converter;
			return channel_converter( type_converter(from) );
		}
	};

	// arbitary conversion
	template <typename Tf, typename Tt, size_t Cf, size_t Ct>
	struct DefaultConverter< Color<Tf,Cf>, Color<Tt,Ct> > {
		typedef Color<Tf,Cf> From;
		typedef Color<Tt,Ct> To;
		To operator()(const From& from) const
		{
			// choose a smaller type to operator() first
			typedef Color<Tt,Cf> TMP1;
			typedef Color<Tf,Ct> TMP2;
			if( sizeof(TMP1) < sizeof(TMP2) ){
				DefaultConverter< From, TMP1 > converter1;
				DefaultConverter< TMP1, To > converter2;
				return converter2( converter1(from) );
			} else {
				DefaultConverter< From, TMP2 > converter1;
				DefaultConverter< TMP2, To > converter2;
				return converter2( converter1(from) );
			}
		}
	};

	template<class DstColor>
	class DefaultConvert {
	public:

		template<class SrcColor>
		DstColor operator()(const SrcColor& src) const
		{
			return DefaultConverter<SrcColor, DstColor>()(src);
		}

		template<class SrcColor>
		DstColor& operator()(const SrcColor& src, DstColor& dst) const
		{
			return (dst = DefaultConverter<SrcColor, DstColor>()(src));
		}
		
	};

	template<class DstImage, template<class> class Conv = DefaultConvert>
	class Convert {
	public:

		template<class SrcImage>
		DstImage& operator()(const SrcImage& src, DstImage& dst) const
		{
			typedef typename DstImage::value_type DstColor;
			std::transform(src.begin(), src.end(), dst.begin(), Conv<DstColor>());
			return dst;
		}

		template<class SrcImage>
		DstImage operator()(const SrcImage& src) const
		{
			DstImage dst(src.width(), src.height());
			return (*this)(src, dst);
		}

	};

	template<class SrcColor, class DstColor>
	DstColor& default_convert(const SrcColor& src, DstColor& dst)
	{
		return (dst = DefaultConvert<DstColor>()(src));
	}

	template<class SrcColor, class DstColor>
	DstColor default_convert(const SrcColor& src)
	{
		return DefaultConvert<DstColor>()(src);
	}

	template<class SrcImage, class DstImage>
	DstImage& convert(const SrcImage& src, DstImage& dst)
	{
		return Convert<DstImage>()(src, dst);
	}

	template<class SrcImage, class DstImage>
	DstImage convert(const SrcImage& src)
	{
		return Convert<DstImage>()(src);
	}

} // namespace image
} // namespace cmlib

#endif // CMLIB_IMAGE_CONVERTER_H
