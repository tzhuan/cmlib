#ifndef CMLIB_IMAGE_IO_PNG_H
#define CMLIB_IMAGE_IO_PNG_H

#include <cstdio>
#include <cassert>
#include <vector>
#include <stdexcept>
#include <cassert>

#include "../../image/Converter.h"
#include "../Exception.h"

namespace cmlib {
namespace image {

	class DLLAPI PngReader {
	public:
		template <template<typename, typename> class Converter, typename I>
		void operator ()(I& image, FILE* f)
		{
			init(f);

			if (my_depth == 8) {
				switch (my_channels) {
					case 1:
						read<Converter, I, Byte1, 1>(image);
						break;
					case 3:
						read<Converter, I, Byte1, 3>(image);
						break;
					case 4:
						read<Converter, I, Byte1, 4>(image);
						break;
					default:
						throw InvalidFormat("Unsupported channels");
				}
			} else if(my_depth == 16) {
				switch (my_channels) {
					case 1:
						read<Converter, I, Short1, 1>(image);
						break;
					case 3:
						read<Converter, I, Short1, 3>(image);
						break;
					case 4:
						read<Converter, I, Short1, 4>(image);
						break;
					default:
						throw InvalidFormat("Unsupported channels");
				}
			} else {
				throw InvalidFormat("Unsupported depth");
			}

			finish();
		}
		template <typename I>
		void operator ()(I& image, FILE* f)
		{
			this->operator()<DefaultConverter, I>(image, f);
		}
	protected:
		void init(FILE* f);
		void finish();
		void read_row_byte(unsigned char* row);
		void read_row_short(unsigned short* row);

		void read_row(unsigned char *row);
		void read_row(unsigned short *row);

		template<
			template<typename, typename> class Converter,
			typename I, 
			typename Type, 
			size_t Channel
		>
		void read(I& image)
		{
			typedef typename Color<Type, Channel>::ColorType ColorType;
			Converter<ColorType, typename I::ColorType> converter;
			image.resize(my_width, my_height);

			std::vector<ColorType> row(my_width);
			for (size_t h = 0; h < my_height; ++h) {
				assert(my_rowbytes == sizeof(Type)*Channel*my_width);
				read_row((Type*)&(row[0]));

				for (size_t w = 0; w < my_width; ++w)
					image(w, h) = converter(row[w]);
					// I::Converter::ext2int(image(w, h), row[w]);
			}
		}
	private:
		const static size_t MAGIC_NUMBER = 8;
		void *my_png_ptr;
		void *my_info_ptr;
		void *my_end_info;

		size_t my_width;
		size_t my_height;
		size_t my_channels;
		size_t my_depth;

		size_t my_rowbytes;
		size_t my_color_type;
		size_t my_interlace_type;
	};

	class DLLAPI PngWriter {
	public:
		template <template<typename, typename> class Converter, typename I>
		void operator ()(const I& image, FILE* f)
		{
			init(f);
			if (image.channels() >= 4)
				write<Converter, I, Byte4>(image);
			else if (image.channels() == 3)
				write<Converter, I, Byte3>(image);
			else
				write<Converter, I, Byte1>(image);
			finish();
		}
		template <typename I>
		void operator ()(I& image, FILE* f)
		{
			this->operator()<DefaultConverter, I>(image, f);
		}
	protected:
		void init(FILE* f);
		void write(unsigned char** row_pointers);
		void finish();
		template<
			template<typename, typename> class Converter,
			typename I, 
			typename ColorType
		>
		void write(const I& image)
		{
			my_width = image.width();
			my_height = image.height();
			my_channels = ColorTrait<ColorType>::channels();

			Converter<typename I::ColorType, ColorType> converter;

			std::vector<ColorType*> row_pointers(my_height);
			std::vector<ColorType> row_data(my_height*my_width);
			row_pointers[0] = &row_data[0];
			for (size_t i = 1; i < my_height; ++i)
				row_pointers[i] = row_pointers[i-1] + my_width;

			for (size_t h = 0; h < my_height; ++h)
				for (size_t w = 0; w < my_width; ++w)
					row_pointers[h][w] = converter( image(w, h) );
					//I::Converter::int2ext(row_pointers[h][w], image(w, h));

			write((unsigned char**)&row_pointers[0]);
		}
	private:
		const static size_t BIT_DEPTH = 8;
		void *my_png_ptr; // png_structp
		void *my_info_ptr; // png_infop
		size_t my_width;
		size_t my_height;
		size_t my_channels;
		size_t my_bit_depth;
	};

} // namespace image
} // namespace cmlib

#endif // CMLIB_IMAGE_IO_PNG_H
