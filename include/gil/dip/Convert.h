#ifndef CONVERT_H
#define CONVERT_H

#include "Filter.h"

namespace gil {

	template<class Converter>
	class DefaultConvert: 
		public 
			Filter< DefaultConvert<Converter>, Image<typename Converter::To> > 
	{
		friend 
			class Filter< 
				DefaultConvert<Converter>, 
				Image<typename Converter::To> 
			>;
		public:
			DefaultConvert(): 
				Filter<
					DefaultConvert<Converter>, Image<typename Converter::To>
				>(*this)
			{
				// empty
			}

		protected:
			template<class I>
			inline void filter(
				Image<typename Converter::To>& dst, const I& src
			) const
			{
				dst.resize(src.width(), src.height());
				Converter converter;
				for (size_t y = 0; y < src.height(); ++y)
					for (size_t x = 0; x < src.width(); ++x)
						dst(x, y) = converter(src(x, y));
			}

		private:

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
