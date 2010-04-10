#include <exception>
#include <vector>
#include <cstdio>

// libjpeg is stupid and didn't use this to remove C++ name mangling
// in their official header file
extern "C" {
#include <jpeglib.h>
}

#include <cmlib/imageio/io/jpeg.h>

using namespace std;
using namespace cmlib::image;

namespace cmlib {
namespace image {

	void JpegReader::init(FILE* f, size_t& w, size_t& h, size_t& c)
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

	void JpegWriter::init(FILE* f, size_t w, size_t h, size_t c)
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

	void JpegWriter::write_scanline(vector<Byte1>& buf)
	{
		Byte1* bufp = &buf[0];
		JSAMPARRAY p = &bufp;
		jpeg_write_scanlines( (jpeg_compress_struct*)my_cinfo, p, 1);
	}

	void JpegWriter::write_scanline(vector<Byte3>& buf)
	{
		Byte1* bufp = &buf[0][0];
		JSAMPARRAY p = &bufp;
		jpeg_write_scanlines( (jpeg_compress_struct*)my_cinfo, p, 1);
	}

	void JpegWriter::finish() {
		jpeg_compress_struct* cinfo = (jpeg_compress_struct*)my_cinfo;
		jpeg_finish_compress(cinfo);
		jpeg_destroy_compress(cinfo);
		cleanup();
	}

	void JpegWriter::cleanup() throw()
	{
		delete (jpeg_compress_struct*)my_cinfo;
		delete (jpeg_error_mgr*)my_jerr;
		my_cinfo = NULL;
		my_jerr = NULL;
	}

} // namespace image
} // namespace cmlib
