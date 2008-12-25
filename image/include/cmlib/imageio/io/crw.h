#ifndef CMLIB_IMAGE_IO_CRW_H
#define CMLIB_IMAGE_IO_CRW_H

#include <cstdio>
#include <stdexcept>
#include <vector>

#include "../../image/Converter.h"

namespace cmlib {
namespace image {

	class DLLAPI CrwReader {
	public:
		template <template<typename, typename> class Converter, typename I>
		void operator ()(I& image, FILE* f)
		{
			check(f);
			std::vector<Short4*> row_pointers(my_height);
			std::vector<Short4> row_data(my_height*my_width+my_meta_length);
			row_pointers[0] = &row_data[0];
			for (size_t i = 1; i < my_height; ++i)
				row_pointers[i] = row_pointers[i-1] + my_width;

			read((unsigned short(*)[4])row_pointers[0]);
			image.resize(my_width, my_height);

			Converter<Short4, typename I::ColorType> converter;
			for (size_t h = 0; h < my_height; ++h)
				for (size_t w = 0; w < my_width; ++w)
					image(w, h) = converter(row_pointers[h][w]);
					//I::Converter::ext2int(image(w, h), row_pointers[h][w]);
		}
		template <typename I>
		void operator ()(I& image, FILE* f)
		{
			this->operator()<DefaultConverter, I>(image, f);
		}
	protected:
		void check(FILE* f);
		void init();
		void read(unsigned short (*row_pointers)[4]);
		void finish();
	private:
		const static size_t MAGIC_NUMBER = 8;
		void *my_png_ptr;
		void *my_info_ptr;
		size_t my_width;
		size_t my_height;
		size_t my_channels;
		size_t my_meta_length;
	};

	/* there is no support for writing */
} // namespace image
} // namespace cmlib

#endif // CMLIB_IMAGE_IO_CRW_H
