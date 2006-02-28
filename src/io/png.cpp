#include <png.h>

#include "gil/Exception.h"
#include "gil/io/png.h"

using namespace gil;

void PngReader::check(FILE *f)
{
    png_byte header[MAGIC_NUMBER];
    assert(f);
    fread(header, 1, MAGIC_NUMBER, f);
    if (png_sig_cmp(header, 0, MAGIC_NUMBER))
	throw InvalidFormat("not png");
}

void PngReader::read(unsigned char** row_pointers)
{
    png_structp png_ptr = (png_structp)my_png_ptr;
    //png_infop info_ptr = (png_infop)my_info_ptr;

    png_read_image(png_ptr, row_pointers);
}

void PngReader::init(FILE *f)
{
    my_png_ptr = (void*)png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
    if (!my_png_ptr)
	throw std::bad_alloc();

    png_structp png_ptr = (png_structp)my_png_ptr;

    my_info_ptr = (void*)png_create_info_struct(png_ptr);
    if (!my_info_ptr) {
	png_destroy_read_struct((png_struct**)&my_png_ptr, 0, 0);
	throw std::bad_alloc();
    }

    png_infop info_ptr = (png_infop)my_info_ptr;

    if (setjmp(png_jmpbuf(png_ptr))) {
	png_destroy_read_struct(
	    (png_structpp)&my_png_ptr, (png_infopp)&my_info_ptr, 0);
	throw std::runtime_error("setjmp");	
    }

    png_init_io(png_ptr, f);
    png_set_sig_bytes(png_ptr, MAGIC_NUMBER);

    png_read_info(png_ptr, info_ptr);
    png_uint_32 width, height;
    int bit_depth, color_type, interlace_type, compression_type, filter_type;
    png_get_IHDR(png_ptr, info_ptr, 
	&width, &height, &bit_depth, 
	&color_type, &interlace_type, 
	&compression_type, &filter_type);

    // The following code transforms grayscale images of less than 8 to 8 bits,
    // changes paletted images to RGB, and adds a full alpha channel if there
    // is transparency information in a tRNS chunk.
    if (color_type == PNG_COLOR_TYPE_PALETTE && bit_depth <= 8)
	png_set_expand(png_ptr);
    if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
	png_set_expand(png_ptr);
    if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
	png_set_expand(png_ptr);

    if (bit_depth == 16) //ensure 8-bit
	png_set_strip_16(png_ptr);
    if (bit_depth < 8) //ensure 8-bit
	png_set_packing(png_ptr);

    my_width = png_get_image_width(png_ptr, info_ptr);
    my_height = png_get_image_height(png_ptr, info_ptr);
    my_channels = png_get_channels(png_ptr, info_ptr);
}

void PngReader::finish()
{
    png_structp png_ptr = (png_structp)my_png_ptr;

    png_read_end(png_ptr, NULL);
    png_destroy_read_struct(
	(png_struct**)&my_png_ptr, (png_info**)&my_info_ptr, 0);
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
