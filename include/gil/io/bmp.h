
#ifndef GIL_BMP_H
#define GIL_BMP_H

#include <stdexcept>
#include <cstdio>

#include "../Exception.h"
#include "../Color.h"

namespace gil {

	class DLLAPI BmpReader {
		public:
			BmpReader()
			{
				// empty
			}

			template <typename I>
			void operator ()(I& image, FILE* f);

		private:
			// use type T as scanline buffer
			template <typename I>
			void read_pixels(I& image);

			void init(FILE* f, size_t& w, size_t& h);
			void read_scanline(std::vector<Byte3>& buf);

			std::vector<Byte3> my_color_table; // may or may not be used.
			size_t my_pixel_offset; // offset to pixel data
			FILE* my_file;
	};

	template <typename I>
	void BmpReader::read_pixels(I& image)
	{
		typedef typename I::Converter Conv;
		size_t w = image.width(), h = image.height();
		std::vector<Byte3> buffer(w);
		for(size_t y = 0; y < h; y++){
			read_scanline(buffer);
			// pixels in BMP file is stored upside down.
			for(size_t x = 0; x < w; x++)
				Conv::ext2int( image(x, h-y-1), buffer[x] );
		}
	}

	template <typename I>
	void BmpReader::operator ()(I& image, FILE* f)
	{
		size_t width, height;
		init(f, width, height);
		image.allocate(width, height);
		read_pixels(image);
	}

	class DLLAPI BmpWriter {
		public:
			BmpWriter()
			{
				// empty
			}

			~BmpWriter() throw ()
			{
				// empty
			}

			template <typename I>
			void operator ()(const I& image, FILE* f);

		private:
			// use type T as scanline buffer
			template <typename T, typename I>
			void write_pixels(I& image);

			void init(FILE* f, size_t w, size_t h, size_t c);
			void write_scanline(std::vector<Byte1>& buf);
			void write_scanline(std::vector<Byte3>& buf);

			std::vector<Byte3> my_color_table; // may or may not be used.
			FILE* my_file;
	};

	template <typename T, typename I>
	void BmpWriter::write_pixels(I& image)
	{
		typedef typename I::Converter Conv;
		size_t w = image.width(), h = image.height();
		std::vector<T> buffer(w);
		for(size_t y = 0; y < h; y++){
			for(size_t x = 0; x < w; x++)
				Conv::int2ext( buffer[x], image(x, y) );

			write_scanline(buffer);
		}
	}

	template <typename I>
	void BmpWriter::operator ()(const I& image, FILE* f)
	{
		size_t c = (image.channels()==1) ? 1 : 3;
		init( f, image.width(), image.height(), c);
		if(c == 1)
			write_pixels<Byte1>(image);
		else
			write_pixels<Byte3>(image);
	}

} // namespace gil

#endif // GIL_BMP_H
