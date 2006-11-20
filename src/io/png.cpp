#include <cassert>
#include <png.h>

#include "gil/Exception.h"
#include "gil/io/png.h"

using namespace gil;

void PngReader::read_row(unsigned char *row)
{
	/*
	png_structp png_ptr = (png_structp)my_png_ptr;
	//png_infop info_ptr = (png_infop)my_info_ptr;

	png_read_image(png_ptr, row_pointers);
	*/

	png_structp png_ptr = (png_structp)my_png_ptr;

	if (my_interlace_type == PNG_INTERLACE_NONE) {
		png_read_row(png_ptr, row, 0);
	} else if (my_interlace_type == PNG_INTERLACE_ADAM7) {
		throw InvalidFormat(
			"interlace type PNG_INTERLACE_ADAM7 is not supported yet"
		);
	} else {
		throw InvalidFormat("unknown interlace type");
	}
}

void PngReader::read_row(unsigned short *row)
{
	throw InvalidFormat("16-bit depth is not supported yet");
}

void PngReader::init(FILE *f)
{
	png_byte header[MAGIC_NUMBER];
	assert(f);
	fread(header, 1, MAGIC_NUMBER, f);
	if (png_sig_cmp(header, 0, MAGIC_NUMBER))
		throw InvalidFormat("not png format");

	png_structp png_ptr = png_create_read_struct(
		PNG_LIBPNG_VER_STRING, (png_voidp)0, 0, 0
	);
	if (!png_ptr)
		throw std::bad_alloc();

	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr) {
		png_destroy_read_struct(&png_ptr, (png_infopp)0, (png_infopp)0);
		throw std::bad_alloc();
	}

	// FIXME: end_info seems unnecessary!?
    png_infop end_info = png_create_info_struct(png_ptr);
    if (!end_info) {
        png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)0);
		throw std::bad_alloc();
    }

	my_png_ptr = png_ptr;
	my_info_ptr = info_ptr;
	my_end_info = end_info;

	if (setjmp(png_jmpbuf(png_ptr))) {
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		throw std::runtime_error(
			"libpng encounters error, back to setjmp point"
		);
	}

	png_init_io(png_ptr, f);
	png_set_sig_bytes(png_ptr, MAGIC_NUMBER);

	png_read_info(png_ptr, info_ptr);

	/*
	png_uint_32 width, height;
	int bit_depth, color_type, interlace_type, compression_type, filter_type;
	png_get_IHDR(png_ptr, info_ptr, 
			&width, &height, &bit_depth, 
			&color_type, &interlace_type, 
			&compression_type, &filter_type);
	*/

    my_width = png_get_image_width(png_ptr, info_ptr);
    my_height = png_get_image_height(png_ptr, info_ptr);
    my_interlace_type = png_get_interlace_type(png_ptr, info_ptr);
    my_channels = png_get_channels(png_ptr, info_ptr);
    my_rowbytes = png_get_rowbytes(png_ptr, info_ptr);

    png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    png_byte color_type = png_get_color_type(png_ptr, info_ptr);

	// The following code transforms grayscale images of less than 8 to 8 bits,
	// changes paletted images to RGB, and adds a full alpha channel if there
	// is transparency information in a tRNS chunk.
	
	if (color_type == PNG_COLOR_TYPE_PALETTE) {
		png_set_palette_to_rgb(png_ptr);
		my_rowbytes *= (3/my_channels);
		my_channels = 3;
	}

	if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8) {
		png_set_gray_1_2_4_to_8(png_ptr);
		bit_depth = 8;
	} else if (bit_depth < 8) {
		png_set_packing(png_ptr);
		bit_depth = 8;
	}

	if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
		png_set_tRNS_to_alpha(png_ptr);

	// FIXME: 16bit channel might be handled well in the future
	if (bit_depth == 16) {
		png_set_strip_16(png_ptr);
		bit_depth = 8;
	}

	my_depth = bit_depth;
}

void PngReader::finish()
{
	png_structp png_ptr = (png_structp)my_png_ptr;
	png_infop info_ptr = (png_infop)my_info_ptr;
	png_infop end_info = (png_infop)my_end_info;

	png_read_end(png_ptr, end_info);
	png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
}

void PngWriter::init(FILE *f)
{
	my_png_ptr = (void*)png_create_write_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
	if (!my_png_ptr)
		throw std::bad_alloc();

	png_structp png_ptr = (png_structp)my_png_ptr;

	my_info_ptr = (void*)png_create_info_struct(png_ptr);
	if (!my_info_ptr) {
		png_destroy_write_struct((png_structpp)&my_png_ptr, 0);
		throw std::bad_alloc();
	}

	//png_infop info_ptr = (png_infop)my_info_ptr;

	if (setjmp(png_jmpbuf(png_ptr))) {
		png_destroy_write_struct(
				(png_structpp)&my_png_ptr, (png_infopp)&my_info_ptr);
		throw std::runtime_error("setjmp");	
	}

	png_init_io(png_ptr, f);

}

void PngWriter::write(unsigned char** row_pointers)
{
	png_structp png_ptr = (png_structp)my_png_ptr;
	png_infop info_ptr = (png_infop)my_info_ptr;

	int color_type;
	if (my_channels == 1)
		color_type = PNG_COLOR_TYPE_GRAY;
	else if (my_channels == 3)
		color_type = PNG_COLOR_TYPE_RGB;
	else if (my_channels == 4)
		color_type = PNG_COLOR_TYPE_RGBA;
	else
		throw InvalidFormat("color type");

	png_uint_32 width = my_width, height = my_height;

	png_set_IHDR(png_ptr, info_ptr, width, height,
			BIT_DEPTH, color_type, PNG_INTERLACE_NONE,
			PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
	png_write_info(png_ptr, info_ptr);
	png_write_image(png_ptr, row_pointers);
}

void PngWriter::finish()
{
	png_structp png_ptr = (png_structp)my_png_ptr;

	png_write_end(png_ptr, NULL);
	png_destroy_write_struct(
			(png_structpp)&my_png_ptr, (png_infopp)&my_info_ptr);
}
