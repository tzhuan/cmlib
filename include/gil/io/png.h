#ifndef GIL_PNG_H
#define GIL_PNG_H

#include <cstdio>
#include <stdexcept>
#include "../Color.h"

namespace gil {
    class PngReader {
	public:
	    template <typename I>
	    void operator ()(I& image, FILE* f)
	    {
		check(f);
		init(f);
		if (my_channels == 1)
		    read<I, Color<Byte1, 1>::ColorType>(image);
		else if (my_channels == 3)
		    read<I, Color<Byte1, 3>::ColorType>(image);
		else if (my_channels == 4)
		    read<I, Color<Byte1, 4>::ColorType>(image);
		else
		    throw std::runtime_error("channels");
		finish();
	    }
	protected:
	    void check(FILE* f);
	    void init(FILE* f);
	    void finish();
	    void read(unsigned char** row_pointers);
	    template<typename I, typename color_type>
	    void read(I& image)
	    {
		color_type **row_pointers = new color_type*[my_height];
		try {
		    row_pointers[0] = new color_type[my_height*my_width];
		} catch (std::exception &e) {
		    delete[] row_pointers;
		    throw e;
		}
		for (size_t i = 1; i < my_height; ++i)
		    row_pointers[i] = row_pointers[i-1] + my_width;

		read((unsigned char**)row_pointers);
		image.allocate(my_width, my_height);
		for (size_t h = 0; h < my_height; ++h)
		    for (size_t w = 0; w < my_width; ++w)
			I::Converter::ext2int(image(w, h), row_pointers[h][w]);
		delete[] row_pointers[0];
		delete[] row_pointers;
	    }
	private:
	    const static size_t MAGIC_NUMBER = 8;
	    void *my_png_ptr;
	    void *my_info_ptr;
	    size_t my_width;
	    size_t my_height;
	    size_t my_channels;
    };

    class PngWriter {
	public:
	    template <typename I>
	    void operator ()(const I& image, FILE* f)
	    {
		init(f);
		if (image.channels() >= 4)
		    write<I, Color<Byte1, 4>::ColorType>(image);
		else if (image.channels() == 3)
		    write<I, Color<Byte1, 3>::ColorType>(image);
		else
		    write<I, Color<Byte1, 1>::ColorType>(image);
		finish();
	    }
	protected:
	    void init(FILE* f);
	    void write(unsigned char** row_pointers);
	    void finish();
	    template<typename I, typename color_type>
	    void write(const I& image)
	    {
		my_width = image.width();
		my_height = image.height();
		my_channels = ColorTrait<color_type>::channels();

		color_type **row_pointers = new color_type*[my_height];
		try {
		    row_pointers[0] = new color_type[my_height*my_width];
		} catch (std::exception &e) {
		    delete[] row_pointers;
		    throw e;
		}
		for (size_t i = 1; i < my_height; ++i)
		    row_pointers[i] = row_pointers[i-1] + my_width;

		for (size_t h = 0; h < my_height; ++h)
		    for (size_t w = 0; w < my_width; ++w)
			I::Converter::int2ext(row_pointers[h][w], image(w, h));

		write((unsigned char**)row_pointers);

		delete[] row_pointers[0];
		delete[] row_pointers;
	    }
	private:
	    const static size_t BIT_DEPTH = 8;
	    void *my_png_ptr;
	    void *my_info_ptr;
	    size_t my_width;
	    size_t my_height;
	    size_t my_channels;
	    size_t my_bit_depth;
    };
} // namespace gil

#endif // GIL_PNG_H
