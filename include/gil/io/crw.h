#ifndef GIL_CRW_H
#define GIL_CRW_H

#include <cstdio>
#include <stdexcept>
#include "../Color.h"

namespace gil {
    class DLLAPI CrwReader {
	public:
	    template <typename I>
	    void operator ()(I& image, FILE* f)
	    {
		check(f);
		Short4 **row_pointers = new Short4*[my_height];
		try {
		    row_pointers[0] = 
			new Short4[my_height*my_width+my_meta_length];
		} catch (std::exception &e) {
		    delete[] row_pointers;
		    throw e;
		}
		for (size_t i = 1; i < my_height; ++i)
		    row_pointers[i] = row_pointers[i-1] + my_width;

		read((unsigned short(*)[4])row_pointers[0]);
		image.allocate(my_width, my_height);
		for (size_t h = 0; h < my_height; ++h)
		    for (size_t w = 0; w < my_width; ++w)
			I::Converter::ext2int(image(w, h), row_pointers[h][w]);
		delete[] row_pointers[0];
		delete[] row_pointers;
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
} // namespace gil

#endif // GIL_CRW_H
