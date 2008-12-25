#ifndef CMLIB_IMAGE_IO_BMP_H
#define CMLIB_IMAGE_IO_BMP_H

#include <stdexcept>
#include <algorithm>
#include <cstdio>

#include "../../image/Converter.h"
#include "../Exception.h"

namespace cmlib {
namespace image {

	class DLLAPI BmpReader {
	public:
		BmpReader()
		{
			// empty
		}

		template <template<typename, typename> class Converter, typename I>
		void operator ()(I& image, FILE* f)
		{
			size_t width, height;
			init(f, width, height);
			image.resize(width, height);
			read_pixels<Converter>(image);
		}
		template <typename I>
		void operator ()(I& image, FILE* f)
		{
			this->operator()<DefaultConverter, I>(image, f);
		}

	private:
		// use type T as scanline buffer
		template <template<typename, typename> class Converter, typename I>
		void read_pixels(I& image)
		{
			// typedef typename I::Converter Conv;
			Converter<Byte3, typename I::ColorType> converter;
			size_t w = image.width(), h = image.height();
			std::vector<Byte3> buffer(w);
			for(size_t y = 0; y < h; y++){
				read_scanline(buffer);
				// pixels in BMP file is stored upside down.
				for(size_t x = 0; x < w; x++){
					// change BGR to RGB
					std::swap(buffer[x][0], buffer[x][2]);
					image(x, h-y-1) = converter(buffer[x]);
					//Conv::ext2int( image(x, h-y-1), buffer[x] );
				}
			}
		}

		void init(FILE* f, size_t& w, size_t& h);
		void read_scanline(std::vector<Byte3>& buf);

		std::vector<Byte3> my_color_table; // may or may not be used.
		size_t my_pixel_offset; // offset to pixel data
		FILE* my_file;
	};


	class DLLAPI BmpWriter {
	public:
		BmpWriter()
		{
			// empty
		}

		template <template<typename, typename> class Converter, typename I>
		void operator ()(const I& image, FILE* f)
		{
			size_t c = (image.channels()==1) ? 1 : 3;
			init( f, image.width(), image.height(), c);
			if(c == 1)
				write_pixels<Converter, Byte1>(image);
			else
				write_pixels<Converter, Byte3>(image);
		}
		template <typename I>
		void operator ()(const I& image, FILE* f)
		{
			this->operator()<DefaultConverter, I>(image, f);
		}

	private:
		// use type T as scanline buffer
		template <
			template<typename, typename> class Converter,
			typename T, 
			typename I
		>
		void write_pixels(I& image)
		{
			//typedef typename I::Converter Conv;
			Converter<typename I::ColorType, T> converter;
			size_t w = image.width(), h = image.height();
			std::vector<T> buffer(w);
			for(size_t y = 0; y < h; y++){
				for(size_t x = 0; x < w; x++)
					buffer[x] = converter( image(x, h-y-1) );
					//Conv::int2ext( buffer[x], image(x, h-y-1) );

				write_scanline(buffer);
			}
		}


		void init(FILE* f, size_t w, size_t h, size_t c);
		void write_scanline(std::vector<Byte1>& buf);
		void write_scanline(std::vector<Byte3>& buf);

		std::vector<Byte3> my_color_table; // may or may not be used.
		FILE* my_file;
	};

} // namespace image
} // namespace cmlib

#endif // CMLIB_IMAGE_IO_BMP_H
