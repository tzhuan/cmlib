#ifndef GIL_PPM_H
#define GIL_PPM_H

#include <cstdio>
#include <stdexcept>

#include "../Exception.h"
#include "../Color.h"

namespace gil {
    class DLLAPI PpmReader {
	public:
	    template <typename I>
	    void operator ()(I& image, FILE* f)
	    {
		check(f);
		init(f);

		image.allocate(my_width, my_height);

		Byte3 *row = new Byte3[my_width];
		for (size_t h = 0; h < my_height; ++h) {
		    if (fread((void*)row, sizeof(Byte3)*my_width, 1, f) != 1) {
			delete[] row;
			throw FileError("PPM");
		    }
		    for (size_t w = 0; w < my_width; ++w)
			I::Converter::ext2int(image(w, h), row[w]);
		}
		delete[] row;
	    }
	protected:
	    void check(FILE* f)
	    {
		assert(f);
		char header[2];
		if (fread(header, 2, 1, f) != 1)
		    throw FileError("I can't read it!");
		if (header[0] != 'P' || header[1] != '6')
		    throw InvalidFormat("not PPM");
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
			throw InvalidFormat("PPM");
		    while(true) {
			char comment[256];
			if (!fgets(comment, 256, f)) {
			    if (feof(f))
				throw EndOfFile("PPM");
			    throw FileError("PPM");
			}
			if (comment[strlen(comment)-1] == '\n')
			    break;
		    }
		}

		int c = fgetc(f); // eat the last blankspace
		if (c == EOF) {
		    if (feof(f))
			throw EndOfFile("PPM");
		    throw FileError("PPM");
		}

		my_width = numbers[0];
		my_height = numbers[1];
		my_max_value = numbers[2];
	    }

	private:
	    size_t my_width;
	    size_t my_height;
	    size_t my_max_value;
    };

    class DLLAPI PpmWriter {
	public:
	    template <typename I>
	    void operator ()(const I& image, FILE* f)
	    {
		fprintf(f, "P6\n%d %d %d\n",image.width(), image.height(), 255);
		Byte3 *row = new Byte3[image.width()];
		for (size_t h = 0; h < image.height(); ++h) {
		    for (size_t w = 0; w < image.width(); ++w)
			I::Converter::int2ext(row[w], image(w, h));
		    if (fwrite((void*)row, sizeof(Byte3)*image.width(), 1, f)
			    != 1) {
			delete[] row;
			throw FileError("PPM");
		    }
		}
		delete[] row;
	    }
    };
} // namespace gil

#endif // GIL_PPM_H
