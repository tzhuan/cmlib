#ifndef CMLIB_IMAGE_IO_TIFF_H
#define CMLIB_IMAGE_IO_TIFF_H

#include <stdexcept>
#include <string>
#include <vector>
#include <cstdio>

#include "../../image/Converter.h"
#include "../Exception.h"

namespace cmlib {
namespace image {

	class DLLAPI TiffReader {
	public:
		TiffReader() : my_tiff(NULL)
		{
			// empty
		}

		template <template<typename, typename> class Converter, typename I>
		bool operator ()(I& image, const std::string& name)
		{
			size_t width, height, channel;
			init(name, width, height, channel);
			if(my_tiff == NULL)
				return false; // unable to read file

			image.resize(width, height);

			// template banzai!
			if(channel == 1) {
				read_pixels<Converter, Byte1>(image);
			} else if(channel == 3) {
				read_pixels<Converter, Byte3>(image);
			} else if(channel == 4){
				read_pixels<Converter, Byte4>(image);
			} else {
				finish();
				throw InvalidFormat("unsupported tiff channel number");
			}
			finish();
			return true;
		}
		template <typename I>
		void operator ()(I& image, FILE* f)
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
		void read_pixels(I& image)
		{
			//typedef typename I::Converter Conv;
			Converter<T, typename I::ColorType> converter;
			size_t w = image.width(), h = image.height();
			std::vector<T> buffer(w);
			for(size_t y = 0; y < h; y++){
				read_scanline(buffer, static_cast<unsigned int>(y));
				for(size_t x = 0; x < w; x++)
					image(x, y) = converter(buffer[x]);
					// Conv::ext2int( image(x, y), buffer[x] );
			}
		}

		void init(const std::string& name, size_t& w, size_t& h, size_t& c);
		void finish();
		void read_scanline(std::vector<Byte1>& buf, unsigned int y);
		void read_scanline(std::vector<Byte3>& buf, unsigned int y);
		void read_scanline(std::vector<Byte4>& buf, unsigned int y);

		void* my_tiff;
	};


	class DLLAPI TiffWriter {
	public:
		TiffWriter() : my_tiff(NULL)
		{
			// empty
		}

		template <template<typename, typename> class Converter, typename I>
		bool operator ()(const I& image, const std::string& name)
		{
			size_t c = image.channels();

			if(c >= 4)
				c = 4;
			else if(c >= 3)
				c = 3;
			else 
				c = 1;

			init( name, image.width(), image.height(), c);
			if(my_tiff == NULL)
				return false;
			if(c == 1)
				write_pixels<Converter, Byte1>(image);
			else if(c == 3)
				write_pixels<Converter, Byte3>(image);
			else
				write_pixels<Converter, Byte4>(image);

			finish();
			return true;
		}
		template <typename I>
		void operator ()(I& image, FILE* f)
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
					buffer[x] = converter( image(x, y) );
					//Conv::int2ext( buffer[x], image(x, y) );

				write_scanline(buffer, static_cast<unsigned int>(y));
			}
		}


		void init(const std::string& name, size_t w, size_t h, size_t c);
		void write_scanline(std::vector<Byte1>& buf, unsigned int y);
		void write_scanline(std::vector<Byte3>& buf, unsigned int y);
		void write_scanline(std::vector<Byte4>& buf, unsigned int y);
		void finish();

		void* my_tiff;
	};

} // namespace image
} // namespace cmlib

#endif // CMLIB_IMAGE_IO_TIFF_H
