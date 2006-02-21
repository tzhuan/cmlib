#include "png.h" 
#include "gil/io/png.h"

using namespace gil;

void PngReader::check(FILE *f)
{
    png_byte header[MAGIC_NUMBER];
    assert(f);
    fread(header, 1, 8, f);
    if (!png_sig_cmp(header, 0, MAGIC_NUMBER))
	throw std::runtime_error("not png");
}

void PngReader::read(unsigned char** row_pointers)
{
    png_structp png_ptr = (png_structp)my_png_ptr;
    png_infop info_ptr = (png_infop)my_info_ptr;

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
	png_destroy_read_struct(&png_ptr, &info_ptr, 0);
	throw std::runtime_error("setjmp");	
    }

    png_init_io(png_ptr, f);
    png_set_sig_bytes(png_ptr, MAGIC_NUMBER);
    png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, 0);

    my_width = png_get_image_width(png_ptr, info_ptr);
    my_height = png_get_image_height(png_ptr, info_ptr);
    my_channels = png_get_channels(png_ptr, info_ptr);
    my_bit_depth = png_get_bit_depth(png_ptr, info_ptr);
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
	png_destroy_write_struct((png_struct**)&my_png_ptr, 0);
	throw std::bad_alloc();
    }

    png_infop info_ptr = (png_infop)my_info_ptr;

    if (setjmp(png_jmpbuf(png_ptr))) {
	png_destroy_write_struct(&png_ptr, &info_ptr);
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
	throw std::runtime_error("color type");

    png_set_IHDR(png_ptr, info_ptr, my_width, my_height,
	BIT_DEPTH, color_type, PNG_INTERLACE_NONE,
	PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
    png_write_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, 0);
    png_write_image(png_ptr, row_pointers);
}

void PngWriter::finish()
{
    png_structp png_ptr = (png_structp)my_png_ptr;

    png_write_end(png_ptr, NULL);
    png_destroy_write_struct(
	(png_struct**)&my_png_ptr, (png_info**)&my_info_ptr);
}
