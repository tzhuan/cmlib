#ifndef GIL_PPM_H
#define GIL_PPM_H

/* PpmReader/PpmWriter supports PPM and PGM now
 * -PPM: use PpmReader<Byte3, '6'> / PpmWriter<Byte3, '6'>
 * -PGM: use PpmReader<Byte1, '5'> / PpmWriter<Byte1, '5'>
 *
 * other similar format (PBM etc) might be supported soon.
 */

#include <cstdio>
#include <stdexcept>

#include "../Exception.h"
#include "../Color.h"

namespace gil {
    template<typename ColorType, char Magic>
    class DLLAPI PpmReader {
	public:
	    template <typename I>
	    void operator ()(I& image, FILE* f)
	    {
		check(f);
		init(f);

		image.allocate(my_width, my_height);

		ColorType *row = new ColorType[my_width];
		for (size_t h = 0; h < my_height; ++h) {
		    if (fread((void*)row, 
			    sizeof(ColorType)*my_width, 1, f) != 1) {
			delete[] row;
			if (feof(f))
			    throw EndOfFile("unexpected end-of-file");
			throw FileError("unknown read error");
		    }
		    for (size_t w = 0; w < my_width; ++w)
			I::Converter::ext2int(image(w, h), row[static_cast<int>(w)]);
		}
		delete[] row;
	    }
	protected:
	    void test_and_throw(FILE *f)
	    {
		if (feof(f))
		    throw EndOfFile("unexpected end-of-file");
		throw FileError("unknown read error");
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
		    size_t num;
		    if (fscanf(f, " %d", &num) == 1) {
			numbers[i++] = num;
			continue;
		    }
		    char c;
		    if (fscanf(f, "%c", &c) != 1 || c != '#')
			throw InvalidFormat("invalid format");
		    while(true) {
			char comment[256];
			if (!fgets(comment, 256, f))
			    test_and_throw(f);
			if (comment[strlen(comment)-1] == '\n')
			    break;
		    }
		}

		int c = fgetc(f); // eat the last blankspace
		if (c == EOF)
		    test_and_throw(f);

		my_width = numbers[0];
		my_height = numbers[1];
		my_max_value = numbers[2];
	    }

	private:
	    size_t my_width;
	    size_t my_height;
	    size_t my_max_value;
    };

    template<typename ColorType, char Magic>
    class DLLAPI PpmWriter {
	public:
	    template <typename I>
	    void operator ()(const I& image, FILE* f)
	    {
		if (fprintf(f, "P%c\n%d %d %d\n",
		    Magic, image.width(), image.height(), 255) < 0)
		    throw FileError("unknown write error");
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

#endif // GIL_PPM_H
