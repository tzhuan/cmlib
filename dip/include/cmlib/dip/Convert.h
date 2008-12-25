#ifndef GIL_CONVERT_H
#define GIL_CONVERT_H

#include "Filter.h"
#include "../core/Converter.h"

namespace gil {

	template<
		class DstImage, 
		template<class, class> class Converter = DefaultConverter
	>
	class DefaultConvert {
		public:
			DefaultConvert()
			{
				// empty
			}

			template<class SrcImage>
			inline void operator ()(DstImage& dst, const SrcImage& src) const
			{
				dst.resize(src.width(), src.height());
				Converter<
					typename DstImage::value_type, 
					typename SrcImage::value_type
				> converter;
				for (size_t y = 0; y < src.height(); ++y)
					for (size_t x = 0; x < src.width(); ++x)
						dst(x, y) = converter(src(x, y));
			}

			template<
				class ProxyFilter, class ProxyDstImage, class ProxySrcImage
			>
			inline void operator ()(
				DstImage& dst,
				const ImageProxy<ProxyFilter, ProxyDstImage, ProxySrcImage>&
				src_proxy
			) const
			{
				ProxyDstImage tmp;
				(*this)(dst, src_proxy(tmp));
			}

			template<class SrcImage>
			inline 
			ImageProxy<
				DefaultConvert<DstImage, Converter>, 
				DstImage,
				SrcImage
			>
			operator ()(const SrcImage& src) const
			{
				return 
					ImageProxy<
						DefaultConvert<DstImage, Converter>, 
						DstImage,
						SrcImage
					>
					(*this, src);
			}
	};

	template<template<class> class Converter>
	class Convert {
		public:
			Convert()
			{
				// empty
			}

			template<class I>
			inline void operator ()(
				Image<typename Converter<typename I::value_type>::To>& dst, 
				const I& src
			) const
			{
				dst.resize(src.width(), src.height());
				Converter<typename I::value_type> converter;
				for (size_t y = 0; y < src.height(); ++y)
					for (size_t x = 0; x < src.width(); ++x)
						dst(x, y) = converter(src(x, y));
			}

			template<
				class ProxyFilter, class ProxyDstImage, class ProxySrcImage
			>
			inline void operator ()(
				Image<
					typename Converter<typename ProxyDstImage::value_type>::To
				>& dst,
				const ImageProxy<ProxyFilter, ProxyDstImage, ProxySrcImage>&
				src_proxy
			) const
			{
				ProxyDstImage tmp;
				(*this)(dst, src_proxy(tmp));
			}

			template<class I>
			inline 
			ImageProxy<
				Convert<Converter>, 
				Image<typename Converter<typename I::value_type>::To>, 
				I
			>
			operator ()(const I& src) const
			{
				return 
					ImageProxy<
						Convert<Converter>, 
						Image<typename Converter<typename I::value_type>::To>, 
						I
					>
					(*this, src);
			}

		protected:

		private:

	};
}

#endif
