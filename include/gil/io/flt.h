#ifndef GIL_FLT_H
#define GIL_FLT_H

/* FIXME: this ONLY works on little-endian machines like Intel!
 * If the standard of FLT format exists, I'll make it portable soon.
 */

#include <cstdio>
#include <stdexcept>

#include "../Exception.h"
#include "../Color.h"

namespace gil {
    class DLLAPI FltReader {
	public:
	    template <typename I>
	    void operator ()(I& image, FILE* f)
	    {
		check_and_init(image, f);
		if (my_channels == 1)
		    read<I, Byte1>(image, f);
		else if (my_channels == 3)
		    read<I, Byte3>(image, f);
		else if (my_channels == 4)
		    read<I, Byte4>(image, f);
		else
		    throw InvalidFormat("unexpected number of channels");
	    }
	protected:
	    const size_t file_size(FILE *f) const
	    {
		if (fseek(f, 0, SEEK_END) == -1)
		    throw FileError("unknown fseek error");
		int size = ftell(f);
		if (size == -1)
		    throw FileError("unknown ftell error");

		// rewind, use fseek to check the return value.
		if (fseek(f, 0, SEEK_SET) == -1)
		    throw FileError("unknown fseek error");
		return size;
	    }

	    void test_and_throw(FILE *f)
	    {
		if (feof(f))
		    throw EndOfFile("unexpected end-of-file");
		throw FileError("unknown read error");
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

		image.allocate(width, height);
	    }

	    template<typename I, typename ColorType>
	    void read(I &image, FILE *f)
	    {
		ColorType *row = new ColorType[image.width()];
		for (size_t h = 0; h < image.height(); ++h) {
		    for (size_t w = 0; w < image.width(); ++w)
			I::Converter::int2ext(row[w], image(w, h));
		    if (fwrite((void*)row, 
			    sizeof(ColorType)*image.width(), 1, f) != 1) {
			delete[] row;
			throw FileError("unknown write error");
		    }
		}
		delete[] row;
	    }

	private:
	    size_t my_channels;
    };

    class DLLAPI FltWriter {
	public:
	    template<typename I>
	    void operator ()(const I& image, FILE* f)
	    {
		int width = image.width();
		int height = image.height();
		if (fwrite(&width, sizeof(int), 1, f) != 1)
		    throw FileError("unknown write error");
		if (fwrite(&height, sizeof(int), 1, f) != 1)
		    throw FileError("unknown write error");

		if (image.channels() >= 4)
		    write<I, Byte4>(image, f);
		else if (image.channels() == 3)
		    write<I, Byte3>(image, f);
		else 
		    write<I, Byte1>(image, f);
	    }

	    template<typename I, typename ColorType>
	    void write(const I& image, FILE *f) 
	    {
		ColorType *row = new ColorType[image.width()];
		for (size_t h = 0; h < image.height(); ++h) {
		    for (size_t w = 0; w < image.width(); ++w)
			I::Converter::int2ext(row[w], image(w, h));
		    if (fwrite((void*)row, 
			    sizeof(ColorType)*image.width(), 1, f) != 1) {
			delete[] row;
			throw FileError("unknown write error");
		    }
		}
		delete[] row;
	    }
    };
} // namespace gil

#endif // GIL_FLT_H
