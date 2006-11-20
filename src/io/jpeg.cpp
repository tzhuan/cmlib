#include <exception>
#include <vector>
#include <cstdio>

// libjpeg is stupid and didn't use this to remove C++ name mangling
// in their official header file
extern "C" {
#include <jpeglib.h>
}

#include "gil/io/jpeg.h"

using namespace std;
using namespace gil;

template<template<typename, typename> class Converter>
void JpegReader<Converter>::init(FILE* f, size_t& w, size_t& h, size_t& c)
{
	try{
		my_jerr = new jpeg_error_mgr;
		my_cinfo = new jpeg_decompress_struct;
	}
	catch(exception&){
		cleanup();
		throw;
	}

	jpeg_decompress_struct* cinfo = (jpeg_decompress_struct*)my_cinfo;
	cinfo->err = jpeg_std_error( (jpeg_error_mgr*)my_jerr );
	jpeg_create_decompress(cinfo);
	jpeg_stdio_src(cinfo, f);
	jpeg_read_header(cinfo, TRUE);
	jpeg_start_decompress(cinfo);

	w = cinfo->output_width;
	h = cinfo->output_height;
	c = cinfo->output_components;
}

template<template<typename, typename> class Converter>
void JpegReader<Converter>::read_scanline(vector<Byte1>& buf)
{
	Byte1* bufp = &buf[0];
	JSAMPARRAY p = &bufp;
	jpeg_read_scanlines( (jpeg_decompress_struct*)my_cinfo, p, 1);
}

template<template<typename, typename> class Converter>
void JpegReader<Converter>::read_scanline(vector<Byte3>& buf)
{
	Byte1* bufp = &buf[0][0];
	JSAMPARRAY p = &bufp;
	jpeg_read_scanlines( (jpeg_decompress_struct*)my_cinfo, p, 1);
}

template<template<typename, typename> class Converter>
void JpegReader<Converter>::finish()
{
	jpeg_decompress_struct* cinfo = (jpeg_decompress_struct*)my_cinfo;
	jpeg_finish_decompress(cinfo);
	jpeg_destroy_decompress(cinfo);
	cleanup();
}

template<template<typename, typename> class Converter>
void JpegReader<Converter>::cleanup() throw ()
{
	delete (jpeg_decompress_struct*)my_cinfo;
	delete (jpeg_error_mgr*)my_jerr;
	my_cinfo = NULL;
	my_jerr = NULL;
}

template<template<typename, typename> class Converter>
void JpegWriter<Converter>::init(FILE* f, size_t w, size_t h, size_t c)
{
	try{
		my_jerr = new jpeg_error_mgr;
		my_cinfo = new jpeg_compress_struct;
	}
	catch(exception&){
		cleanup();
		throw;
	}

	jpeg_compress_struct* cinfo = (jpeg_compress_struct*)my_cinfo;
	cinfo->err = jpeg_std_error( (jpeg_error_mgr*)my_jerr );

	jpeg_create_compress(cinfo);
	jpeg_stdio_dest(cinfo, f);
	cinfo->image_width = w;
	cinfo->image_height = h;
	cinfo->input_components = c;
	cinfo->in_color_space = (c==1) ? JCS_GRAYSCALE : JCS_RGB;

	jpeg_set_defaults(cinfo);
	jpeg_set_quality(cinfo, 90, TRUE);

	jpeg_start_compress(cinfo, TRUE);
}

template<template<typename, typename> class Converter>
void JpegWriter<Converter>::write_scanline(vector<Byte1>& buf)
{
	Byte1* bufp = &buf[0];
	JSAMPARRAY p = &bufp;
	jpeg_write_scanlines( (jpeg_compress_struct*)my_cinfo, p, 1);
}

template<template<typename, typename> class Converter>
void JpegWriter<Converter>::write_scanline(vector<Byte3>& buf)
{
	Byte1* bufp = &buf[0][0];
	JSAMPARRAY p = &bufp;
	jpeg_write_scanlines( (jpeg_compress_struct*)my_cinfo, p, 1);
}

template<template<typename, typename> class Converter>
void JpegWriter<Converter>::finish() {
	jpeg_compress_struct* cinfo = (jpeg_compress_struct*)my_cinfo;
	jpeg_finish_compress(cinfo);
	jpeg_destroy_compress(cinfo);
	cleanup();
}

template<template<typename, typename> class Converter>
void JpegWriter<Converter>::cleanup() throw()
{
	delete (jpeg_compress_struct*)my_cinfo;
	delete (jpeg_error_mgr*)my_jerr;
	my_cinfo = NULL;
	my_jerr = NULL;
}
