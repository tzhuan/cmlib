#ifndef CMLIB_IMAGE_IO_FLT_H
#define CMLIB_IMAGE_IO_FLT_H

/* FIXME: this ONLY works on little-endian machines like Intel!
 * If the standard of FLT format exists, I'll make it portable soon.
 */

#include <cstdio>
#include <stdexcept>

#include "../../image/Converter.h"
#include "../Exception.h"

namespace cmlib {
namespace image {

	class DLLAPI FltReader {
	public:
		template <template<typename, typename> class Converter, typename I>
		void operator ()(I& image, FILE* f)
		{
			check_and_init(image, f);
			if (my_channels == 1)
				read<Converter, Byte1>(image, f);
			else if (my_channels == 3)
				read<Converter, Byte3>(image, f);
			else if (my_channels == 4)
				read<Converter, Byte4>(image, f);
			else
				throw InvalidFormat("unexpected number of channels");
		}
		template <typename I>
		void operator ()(I& image, FILE* f)
		{
			this->operator()<DefaultConverter, I>(image, f);
		}
	protected:
		const size_t file_size(FILE *f) const
		{
			if (fseek(f, 0, SEEK_END) == -1)
				throw IOError("unknown fseek error");
			int size = ftell(f);
			if (size == -1)
				throw IOError("unknown ftell error");

			// rewind, use fseek to check the return value.
			if (fseek(f, 0, SEEK_SET) == -1)
				throw IOError("unknown fseek error");
			return size;
		}

		void test_and_throw(FILE *f)
		{
			if (feof(f))
				throw EndOfFile("unexpected end-of-file");
			throw IOError("unknown read error");
		}

		template<typename I>
		void check_and_init(I &image, FILE *f)
		{
			int width, height;
			fread(&width, sizeof(int), 1, f);
			fread(&height, sizeof(int), 1, f);

			size_t size = file_size(f) - 2*sizeof(int);
			my_channels = size / sizeof(float) / width / height;
			if(width*height*my_channels*sizeof(float) != size)
				throw InvalidFormat("invalid format");

			image.resize(width, height);
		}

		template<
			template<typename, typename> class Converter, 
			typename ColorType,
			typename I
		>
		void read(I &image, FILE *f)
		{
			std::vector<ColorType> row(image.width());
			Converter<ColorType, typename I::ColorType> converter;
			for (size_t h = 0; h < image.height(); ++h) {
				if (fread(
						(void*)&row[0], 
						sizeof(ColorType)*image.width(),
						1, 
						f
					) != 1) {
					throw IOError("unknown read error");
				}
				for (size_t w = 0; w < image.width(); ++w)
					image(w, h) = converter(row[w]);
					//I::Converter::int2ext(row[w], image(w, h));
			}
		}

	private:
		size_t my_channels;
	};

	class DLLAPI FltWriter {
	public:
		template<template<typename, typename> class Converter, typename I>
		void operator ()(const I& image, FILE* f)
		{
			int width = static_cast<int>( image.width() );
			int height = static_cast<int>( image.height() );
			if (fwrite(&width, sizeof(int), 1, f) != 1)
				throw IOError("unknown write error");
			if (fwrite(&height, sizeof(int), 1, f) != 1)
				throw IOError("unknown write error");

			if (image.channels() >= 4)
				write<Converter, Byte4>(image, f);
			else if (image.channels() == 3)
				write<Converter, Byte3>(image, f);
			else 
				write<Converter, Byte1>(image, f);
		}
		template <typename I>
		void operator ()(I& image, FILE* f)
		{
			this->operator()<DefaultConverter, I>(image, f);
		}

	protected:
		template<
			template<typename, typename> class Converter,
			typename ColorType,
			typename I
		>
		void write(const I& image, FILE *f) 
		{
			Converter<typename I::ColorType, ColorType> converter;
			std::vector<ColorType> row(image.width());
			for (size_t h = 0; h < image.height(); ++h) {
				for (size_t w = 0; w < image.width(); ++w)
					row[w] = converter( image(w, h) );
					// I::Converter::int2ext(row[w], image(w, h));
				if (fwrite(
						(void*)&row[0], 
						sizeof(ColorType)*image.width(), 
						1, 
						f
					) != 1) {
					throw IOError("unknown write error");
				}
			}
		}
	};
} // namespace image
} // namespace cmlib

#endif // CMLIB_IMAGE_IO_FLT_H
