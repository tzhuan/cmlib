#ifndef CMLIB_DIP_PYRAMID_H
#define CMLIB_DIP_PYRAMID_H

#include <vector>
#include <string>
#include <stdexcept>
#include <cmath>

#include "gil/Color.h"
#include "NearestFilter.h"
#include "GaussianFilter.h"

namespace cmlib {
namespace dip {
	
	template<class ImageType, typename Type = float>
	class Pyramid {
	public:
		typedef ImageType image_type;
		typedef typename ImageType::value_type value_type;

		Pyramid()
		{
		}

		/*
		template<class Filter>
		Pyramid(
			const ImageType& image, 
			const Filter& filter = GaussianFilter<ImageType, Type>(1.0, 1.0)
		) {
			this->reset(image, filter);
		}

		template<template<class, typename>Scalar, class Filter>
		Pyramid(
			const ImageType& image, 
			const Filter& filter = NearestFilter<ImageType, Type>, 
		) {
			this->reset<Scalar>(image, filter);
		}
		*/

		template<template<class, typename>class Scaler, class Filter>
		void reset(
			const ImageType &image, 
			const Filter& filter = GaussianFilter<ImageType, Type>(1.0, 1.0)
		) {
			size_t width = image.width();
			size_t height = image.height();

			my_pyramids.clear();
			my_pyramids.push_back(image);

			while (width > 1 && height > 1) {
				width = (width+1) / 2;
				height = (height+1) / 2;
				const ImageType &img = my_pyramids[ my_pyramids.size()-1 ];

				my_pyramids.push_back(
					Scaler<ImageType, Type>(width, height)(filter(img))
				);
			}
		}

		template<class Filter>
		void reset(
			const ImageType &image, 
			const Filter& filter = GaussianFilter<ImageType, Type>(1.0, 1.0),
			bool
		) {
			this->reset<NearestFilter>(image, filter);
		}

		const ImageType operator ()(Type layer) const
		{
			ImageType tmp;
			(*this)(tmp, layer);
			return tmp;
		}

		void operator ()(ImageType &dst, Type layer) const
		{
			my_check_layer(layer);

			size_t ilayer = static_cast<size_t>(layer);
			if (layer == ilayer) {
				dst = my_pyramids[ilayer];
				return;
			}

			size_t width, height;
			my_get_size(layer, width, height);
			dst.resize(width, height);
			for (size_t h = 0; h < height; ++h)
				for (size_t w = 0; w < width; ++w)
					dst(w, h) = my_texel(
						layer, 
						static_cast<Type>(w)/width, 
						static_cast<Type>(h)/height
					);
		}

		const value_type pixel(Type layer, Type x, Type y) const
		{
			my_check_layer(layer);

			size_t ilayer = static_cast<size_t>(layer);
			if (layer == ilayer) {
				const size_t width = my_pyramids[ilayer].width();
				const size_t height = my_pyramids[ilayer].height();
				if (x < 0 || x >= width || y < 0 || y >= height)
					throw std::runtime_error("pixel index out of range");
				return my_pyramids[ilayer].lerp(x, y);
			}

			size_t width, height;
			my_get_size(layer, width, height);
			return texel(layer, x/(width-1), y/(height-1));
		}

		const value_type texel(Type layer, Type x, Type y) const
		{
			my_check_layer(layer);
			my_check_texel(x);
			my_check_texel(y);
			if ( layer == static_cast<size_t>(layer) )
				return my_texel(static_cast<size_t>(layer), x, y);
			return my_texel(layer, x, y);
		}

		size_t size(void) const
		{
			return my_pyramids.size();
		}

	protected:
		void my_check_layer(Type layer) const
		{
			if (layer < 0 || layer >= my_pyramids.size())
				throw std::runtime_error("pyramid layer out of range");
		}

		void my_check_texel(Type x) const
		{
			if (x < 0 || x >= 1)
				throw std::runtime_error("texel index out of range");
		}

		void my_get_size(size_t layer, size_t &width, size_t &height) const
		{
			width = my_pyramids[layer].width();
			height = my_pyramids[layer].height();
		}

		void my_get_size(Type layer, size_t &width, size_t &height) const
		{
			Type integral;
			Type fractional = std::modf(layer, &integral);
			const size_t integer = static_cast<size_t>(integral);

			Type ratio = std::pow(static_cast<Type>(2), fractional);
			width = 
				static_cast<size_t>(my_pyramids[integer].width()/ratio);
			height = 
				static_cast<size_t>(my_pyramids[integer].height()/ratio);
		}

		const value_type my_texel(size_t layer, Type x, Type y) const
		{
			const size_t width = my_pyramids[layer].width();
			const size_t height = my_pyramids[layer].height();
			return my_pyramids[layer].lerp(x * (width-1), y * (height-1));
		}

		const value_type my_texel(Type layer, Type x, Type y) const
		{
			using namespace cmlib::gil;
			size_t prev = static_cast<size_t>(floor(layer));
			size_t next = static_cast<size_t>(ceil(layer));
			value_type prev_texel = my_texel(prev, x, y);
			value_type next_texel = my_texel(next, x, y);
			return 
				prev_texel * 
				static_cast<typename ColorTrait<value_type>::BaseType>(
					next-layer
				) 
				+ 
				next_texel * 
				static_cast<typename ColorTrait<value_type>::BaseType>(
					layer - prev
				);
		}

	private:
		std::vector<ImageType> my_pyramids;
	};

} // namespace dip
} // namespace cmlib

#endif // CMLIB_DIP_PYRAMID_H
