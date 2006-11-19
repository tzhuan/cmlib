#ifndef GIL_PFM_H
#define GIL_PFM_H

#include <cstdio>
#include <stdexcept>
#include <utility>
#include <vector>

#include "../Exception.h"
#include "../Color.h"
#include "../Converter.h"

/* XXX: This code only works when sizeof(char) == 1 */

namespace gil {
    template<typename T, bool = false>
    struct ByteReverser {
	static bool is_little_endian()
	{
	    int i = 1;
	    return ( *((char*)&i) == 1 );
	}

	void operator()(T&) {}
    };

    template<typename T>
    struct ByteReverser <T, true> {
	void operator()(T &data)
	{
	    char *cp = (char*)&data;
	    switch(sizeof(T)) {
		case 1:
		    return;
		case 2:
		    std::swap(cp[0], cp[1]);
		    return;
		case 4:
		    std::swap(cp[0], cp[3]);
		    std::swap(cp[1], cp[2]);
		    return;
		case 8:
		    std::swap(cp[0], cp[7]);
		    std::swap(cp[1], cp[6]);
		    std::swap(cp[2], cp[5]);
		    std::swap(cp[3], cp[4]);
		    return;
		default:
		    for (size_t i = 0; i < sizeof(T)/2; ++i)
			std::swap(cp[i], cp[sizeof(T)-i-1]);
		    return;
	    }
	}
    };

    // because PfmReader is written in header completely,
    // here we should not use DLLAPI
	template<template<typename, typename> class Converter = DefaultConverter>
    class PfmReader {
	public:
	    template <typename I>
	    void operator ()(I& image, FILE* f)
	    {
		check_and_init(image, f);

		if (my_channels == 1) {
		    if (my_if_reverse)
			read<I, Float1, true>(image, f);
		    else
			read<I, Float1, false>(image, f);
		} else if (my_channels == 3) {
		    if (my_if_reverse)
			read<I, Float3, true>(image, f);
		    else
			read<I, Float3, false>(image, f);
		} else
		    throw InvalidFormat("unexpected number of channels");
	    }
	protected:
	    void test_and_throw(FILE *f)
	    {
		if (feof(f))
		    throw EndOfFile("unexpected end-of-file");
		throw IOError("unknown read error");
	    }

	    template<typename I>
	    void check_and_init(I &image, FILE *f)
	    {
		char magic[2];
		fread(magic, sizeof(char), 2, f);
		if (magic[0] != 'P' || (magic[1] != 'F' && magic[1] != 'f'))
		    throw InvalidFormat("pfm");

		my_channels = (magic[1] == 'F') ? 3 : 1;

		int width, height;
		float scale;
		if (fscanf(f, " %d %d %f", &width, &height, &scale) != 3)
		    test_and_throw(f);
		int c = fgetc(f); // eat the last blankspace
		if (c == EOF)
		    test_and_throw(f);

		if (width <= 0 || height <= 0 || scale == 0)
		    throw InvalidFormat("pfm");

		// scale > 0: big endian, scale < 0: little endian
		bool file_is_little_endian = (scale < 0);
		bool machine_is_little_endian = 
		    ByteReverser<char>::is_little_endian();
		my_if_reverse = 
		   (file_is_little_endian != machine_is_little_endian);

		image.allocate(width, height);
	    }

	    template<typename I, typename ColorType, bool is_reverse>
	    void read(I &image, FILE *f)
	    {
		std::vector<ColorType> row(image.width());
		ByteReverser<ColorType, is_reverse> reverser;

		for (size_t h = 0; h < image.height()-1; ++h) {
		    if (fread((void*)&row[0], 
			    sizeof(ColorType)*image.width(), 1, f) != 1) {
			throw IOError("unknown read error");
		    }
		    for (size_t w = 0; w < image.width(); ++w) {
			reverser(row[w]);
			I::Converter::
			    ext2int(image(w, image.height()-h-1), row[w]);
		    }
		}
	    }

	private:
	    size_t my_channels;
	    bool my_if_reverse;
    };

    // as same as PfmReader, no DLLAPI
	template<template<typename, typename> class Converter = DefaultConverter>
    class PfmWriter {
	public:
	    template<typename I>
	    void operator ()(const I& image, FILE* f)
	    {
		size_t c = image.channels();
		if (c >= 2)
		    write<I, Float3>(image, f);
		else
		    write<I, Float1>(image, f);
	    }
	protected:
	    void test_and_throw(FILE *f)
	    {
		if (feof(f))
		    throw EndOfFile("unexpected end-of-file");
		throw IOError("unknown read error");
	    }


	    template<typename I, typename ColorType>
	    void write(const I& image, FILE *f) 
	    {
		std::vector<ColorType> row(image.width());
		size_t channels = ColorTrait<ColorType>::channels();
		char magic = (channels == 3) ? 'F' : 'f';
		double scale = ByteReverser<char>::is_little_endian() ? -1 : 1;
		unsigned int width = static_cast<unsigned int>( image.width() );
		unsigned int height = static_cast<unsigned int>( image.height() );

		if (fprintf(f, "P%c\n%u %u\n%f\n", 
			    magic, width, height, scale) < 0)
		    test_and_throw(f);

		for (size_t h = 0; h < image.height(); ++h) {
		    for (size_t w = 0; w < image.width(); ++w)
			I::Converter::
			    int2ext(row[w], image(w, image.height()-h-1));
		    if (fwrite((void*)&row[0], 
			    sizeof(ColorType)*image.width(), 1, f) != 1) {
			throw IOError("unknown write error");
		    }
		}
	    }
    };
} // namespace gil

#endif // GIL_PFM_H
