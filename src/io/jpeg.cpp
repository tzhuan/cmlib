#include <exception>
#include <vector>
#include <cstdio>
#include <jpeglib.h>

#include "gil/io/jpeg.h"

using namespace std;
using namespace gil;

void JpegReader::init(FILE* f, size_t& w, size_t& h, size_t& c)
{
    try{
	my_jerr = new jpeg_error_mgr;
	my_cinfo = new jpeg_decompress_struct;
    }
    catch(exception& e){
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

void JpegReader::read_scanline(vector<Byte1>& buf)
{
    Byte1* bufp = &buf[0];
    JSAMPARRAY p = &bufp;
    jpeg_read_scanlines( (jpeg_decompress_struct*)my_cinfo, p, 1);
}

void JpegReader::read_scanline(vector<Byte3>& buf)
{
    Byte1* bufp = &buf[0][0];
    JSAMPARRAY p = &bufp;
    jpeg_read_scanlines( (jpeg_decompress_struct*)my_cinfo, p, 1);
}

void JpegReader::finish()
{
    jpeg_decompress_struct* cinfo = (jpeg_decompress_struct*)my_cinfo;
    jpeg_finish_decompress(cinfo);
    jpeg_destroy_decompress(cinfo);
    cleanup();
}

void JpegReader::cleanup() throw ()
{
    delete (jpeg_decompress_struct*)my_cinfo;
    delete (jpeg_error_mgr*)my_jerr;
    my_cinfo = NULL;
    my_jerr = NULL;
}
