#ifndef CMLIB_IMAGE_IO_JPEG_H
#define CMLIB_IMAGE_IO_JPEG_H

#include <stdexcept>
#include <cstdio>

#include "../../image/Converter.h"
#include "../Exception.h"

namespace cmlib {
namespace image {

	class DLLAPI JpegReader {
	public:
		JpegReader() : my_cinfo(NULL), my_jerr(NULL)
		{
			// empty
		}

		~JpegReader() throw ()
		{
			cleanup();
		}

		template <template<typename, typename> class Converter, typename I>
		void operator ()(I& image, FILE* f)
		{
			size_t width, height, channel;
			init(f, width, height, channel);
			image.resize(width, height);

			// template banzai!
			if(channel == 1) {
				read_pixels<Converter, Byte1>(image);
			} else if(channel == 3) {
				read_pixels<Converter, Byte3>(image);
			} else {
				finish();
				throw InvalidFormat("unsupported jpeg channel number");
			}
			finish();
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
			// typedef typename I::Converter Conv;
			Converter<T, typename I::ColorType> converter;
			size_t w = image.width(), h = image.height();
			std::vector<T> buffer(w);
			for(size_t y = 0; y < h; y++){
				read_scanline(buffer);
				for(size_t x = 0; x < w; x++)
					image(x, y) = converter(buffer[x]);
					//Conv::ext2int( image(x, y), buffer[x] );
			}
		}

		void init(FILE* f, size_t& w, size_t& h, size_t& c);
		void finish();
		void read_scanline(std::vector<Byte1>& buf);
		void read_scanline(std::vector<Byte3>& buf);
		void cleanup() throw();

		void* my_cinfo;
		void* my_jerr;
	};


	class DLLAPI JpegWriter {
	public:
		JpegWriter() : my_cinfo(NULL), my_jerr(NULL)
		{
			// empty
		}

		~JpegWriter() throw ()
		{
			cleanup();
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
			finish();
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
				// Conv::int2ext( buffer[x], image(x, y) );

				write_scanline(buffer);
			}
		}

		void init(FILE* f, size_t w, size_t h, size_t c);
		void write_scanline(std::vector<Byte1>& buf);
		void write_scanline(std::vector<Byte3>& buf);
		void finish();
		void cleanup() throw ();

		void* my_cinfo;
		void* my_jerr;
	};

} // namespace image
} // namespace cmlib

#endif // CMLIB_IMAGE_IO_JPEG_H
