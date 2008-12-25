#ifndef GIL_PYRAMID_H
#define GIL_PYRAMID_H

#include <vector>
#include <string>
#include <stdexcept>
#include <cmath>

#include "NearestFilter.h"
#include "GaussianFilter.h"

namespace gil {
	
	template<
		class Image, 
		class Scaler = NearestFilter<Image>, 
		typename T = TypeTrait<Byte1>::MathType
	>
	class Pyramid {
		public:
			typedef Image image_type;
			typedef typename Image::value_type value_type;

			Pyramid()
			{
			}

			Pyramid(const Image& image)
			{
				reset(image);
			}

			void reset(const Image &image)
			{
				size_t width = image.width();
				size_t height = image.height();

				my_pyramids.clear();
				my_pyramids.push_back(image);

				while (width > 1 && height > 1) {
					width = (width+1) / 2;
					height = (height+1) / 2;
					const Image &img = my_pyramids[ my_pyramids.size()-1 ];

					my_pyramids.push_back(
						Scaler(width, height)(
							GaussianFilter<Image>(1., 1.)(img)
						)
					);
				}
			}

			const Image operator ()(T layer) const
			{
				Image tmp;
				(*this)(tmp, layer);
				return tmp;
			}

			void operator ()(Image &dst, T layer) const
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
							static_cast<T>(w)/width, 
							static_cast<T>(h)/height
						);
			}

			const value_type pixel(T layer, T x, T y) const
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

			const value_type texel(T layer, T x, T y) const
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
			void my_check_layer(T layer) const
			{
				if (layer < 0 || layer >= my_pyramids.size())
					throw std::runtime_error("pyramid layer out of range");
			}

			void my_check_texel(T x) const
			{
				if (x < 0 || x >= 1)
					throw std::runtime_error("texel index out of range");
			}

			void my_get_size(size_t layer, size_t &width, size_t &height) const
			{
				width = my_pyramids[layer].width();
				height = my_pyramids[layer].height();
			}

			void my_get_size(T layer, size_t &width, size_t &height) const
			{
				T integral;
				T fractional = std::modf(layer, &integral);
				const size_t integer = static_cast<size_t>(integral);

				T ratio = std::pow(static_cast<T>(2), fractional);
				width = 
					static_cast<size_t>(my_pyramids[integer].width()/ratio);
				height = 
					static_cast<size_t>(my_pyramids[integer].height()/ratio);
			}

			const value_type my_texel(size_t layer, T x, T y) const
			{
				const size_t width = my_pyramids[layer].width();
				const size_t height = my_pyramids[layer].height();
				return my_pyramids[layer].lerp(x * (width-1), y * (height-1));
			}

			const value_type my_texel(T layer, T x, T y) const
			{
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
			std::vector<Image> my_pyramids;
	};

}

#endif
