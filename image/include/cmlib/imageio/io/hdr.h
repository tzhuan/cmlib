#ifndef CMLIB_IMAGE_IO_HDR_H
#define CMLIB_IMAGE_IO_HDR_H

#include <stdexcept>
#include <cstdio>
#include <vector>

#include "../../image/Converter.h"

// turn off the turnoff warnings
#ifdef _MSC_VER
#pragma warning(disable:4251) 
#endif

namespace cmlib {
namespace image {

	class DLLAPI HdrReader {
	public:
		HdrReader() : my_file(NULL)
		{
			// empty
		}

		template <template<typename, typename> class Converter, typename I>
		void operator ()(I& image, FILE* f)
		{
			size_t width, height;
			init(f, width, height);
			image.resize(width, height);

			// typedef typename I::Converter Conv;
			Converter<Float3, typename I::ColorType> converter;
			std::vector<Float3> buffer(width);
			for(size_t y = 0; y < height; y++){
				read_scanline(buffer);
				for(size_t x = 0; x < width; x++)
					image(x, y) = converter(buffer[x]);
					//Conv::ext2int( image(x, y), buffer[x] );
			}

			finish();
		}
		template <typename I>
		void operator ()(I& image, FILE* f)
		{
			this->operator()<DefaultConverter, I>(image, f);
		}

	private:
		void init(FILE* f, size_t& w, size_t& h);
		void finish();
		void read_scanline(std::vector<Float3>& buf);

		void read_encoded();
		void read_encoded_old(size_t pos = 0);

		std::vector<Byte4> my_enc_buffer;
		FILE* my_file;
	};


	class DLLAPI HdrWriter {
	public:
		HdrWriter() : my_file(NULL)
		{
			// empty
		}

		template <template<typename, typename> class Converter, typename I>
		void operator ()(const I& image, FILE* f)
		{
			size_t width = image.width(), height = image.height();
			init(f, width, height);

			// typedef typename I::Converter Conv;
			Converter<typename I::ColorType, Float3> converter;
			std::vector<Float3> buffer(width);
			for(size_t y = 0; y < height; y++){
				for(size_t x = 0; x < width; x++)
					buffer[x] = converter( image(x, y) );
					//Conv::int2ext( buffer[x], image(x, y) );
				write_scanline(buffer);
			}

			finish();
		}
		template <typename I>
		void operator ()(I& image, FILE* f)
		{
			this->operator()<DefaultConverter, I>(image, f);
		}

	private:
		void init(FILE* f, size_t w, size_t h);
		void finish();
		void write_scanline(const std::vector<Float3>& buf);

		void write_encoded();

		std::vector<Byte4> my_enc_buffer;
		FILE* my_file;
	};

} // namespace image
} // namespace cmlib

#endif // CMLIB_IMAGE_IO_HDR_H
