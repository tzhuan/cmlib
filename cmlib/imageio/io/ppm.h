/*
Copyright NTU CSIE CMLAB 2005 - 2012
Distributed under the Boost Software License, Version 1.0.
(See accompanying file ../../../LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef CMLIB_IMAGE_IO_PPM_H
#define CMLIB_IMAGE_IO_PPM_H

/* PpmReader/PpmWriter supports PPM and PGM now
 * -PPM: use PpmReader<3, '6'> / PpmWriter<Byte3, '6'>
 * -PGM: use PpmReader<1, '5'> / PpmWriter<Byte1, '5'>
 *
 * other similar format (PBM etc) might be supported soon.
 */

#include <cstdio>
#include <stdexcept>
#include <cstring>
#include <vector>

#include "io_common.h"
#include "../../image/Converter.h"
#include "../Exception.h"

namespace cmlib {
namespace image {

	template<typename ByteType, typename ShortType, char Magic>
	class PpmReader {
	public:
		template <template<typename, typename> class Converter, typename ImageType>
		void operator ()(ImageType& image, FILE* file)
		{
			check(file);
			init(file);

			bool machine_is_little_endian = ByteReverser<char>::is_little_endian();
			if (my_max_value < 256) {
				read<Converter, ByteType, ImageType, false>(image, file);
			} else {
				if (machine_is_little_endian)
					read<Converter, ShortType, ImageType, true>(image, file);
				else
					read<Converter, ShortType, ImageType, false>(image, file);
			}
		}

		template <typename ImageType>
		void operator ()(ImageType& image, FILE* file)
		{
			this->operator()<DefaultConverter, ImageType>(image, file);
		}

	protected:
		void test_and_throw(FILE *f)
		{
			if (feof(f))
				throw EndOfFile("unexpected end-of-file");
			throw IOError("unknown read error");
		}

		void check(FILE *f)
		{
			char header[2];
			if (fread(header, 2, 1, f) != 1)
				test_and_throw(f);
			if (header[0] != 'P' || header[1] != Magic)
				throw InvalidFormat("invalid format");
		}

		void init(FILE* f)
		{
			size_t numbers[3];		

			for(size_t i = 0; i < 3;) {
				int num;
				if (fscanf(f, " %d", &num) == 1) {
					numbers[i++] = (size_t)num;
					continue;
				}
				char c;
				if (fscanf(f, "%c", &c) != 1 || c != '#')
					throw InvalidFormat("invalid format");
				while(true) {
					char comment[256];
					if (!fgets(comment, 256, f))
						test_and_throw(f);
					if (comment[std::strlen(comment)-1] == '\n')
						break;
				}
			}

			int c = fgetc(f); // eat the last blankspace
			if (c == EOF)
				test_and_throw(f);

			my_width = numbers[0];
			my_height = numbers[1];
			my_max_value = numbers[2];
			if (my_max_value <= 0 || my_max_value > 65536)
				throw InvalidFormat("Maxval must be less than 65536 and more than zero");
		}

		template<template<typename, typename> class Converter, typename ColorType, class ImageType, bool Reverse>
		void read(ImageType& image, FILE* file)
		{
			image.resize(my_width, my_height);
			ByteReverser<ColorType, Reverse> reverser;
			Converter<ColorType, typename ImageType::ColorType> converter;

			std::vector<ColorType> row(my_width);
			for (size_t h = 0; h < my_height; ++h) {
				if (fread((void*)&row[0], sizeof(ColorType)*my_width, 1, file) != 1) {
					if (feof(file))
						throw EndOfFile("unexpected end-of-file");
					throw IOError("unknown read error");
				}
				for (size_t w = 0; w < my_width; ++w) {
					reverser(row[w]);
					image(w, h) = converter(row[w]);
					// ImageType::Converter::ext2int(image(w, h), row[w]);
				}
			}
		}
	private:
		size_t my_width;
		size_t my_height;
		size_t my_max_value;
	};

	template<typename ColorType, char Magic>
	class PpmWriter {
	public:
		template <template<typename, typename> class Converter, typename I>
		void operator ()(const I& image, FILE* f)
		{
			if (fprintf(f, "P%c\n%d %d %d\n",
				Magic, (int)image.width(), (int)image.height(), 255) < 0)
				throw IOError("unknown write error");
			std::vector<ColorType> row(image.width());
			Converter<typename I::ColorType, ColorType> converter;
			for (size_t h = 0; h < image.height(); ++h) {
				for (size_t w = 0; w < image.width(); ++w)
					row[w] = converter( image(w, h) );
					//I::Converter::int2ext(row[w], image(w, h));
				if (fwrite((void*)&row[0], 
							sizeof(ColorType)*image.width(), 1, f) != 1) {
					throw IOError("unknown write error");
				}
			}
		}
		template <typename I>
		void operator ()(I& image, FILE* f)
		{
			this->operator()<DefaultConverter, I>(image, f);
		}
	};

} // namespace image
} // namespace cmlib

#endif // CMLIB_IMAGE_IO_PPM_H
