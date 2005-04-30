//*****************************************************************
//
// jpg.cpp : routines for jpg I/O
//
//*****************************************************************

#include <stdio.h>
#include <string.h>

// <setjmp.h> is used for the optional error recovery mechanism.
#include <setjmp.h>
extern "C" {
#include <jpeglib.h>
}

//#pragma comment (linker, "/defaultlib:jpeg.lib")

unsigned char* readJPG(const char* filename, int& width, int& height, int& depth)
{	
	FILE * fd;
	if ((fd = fopen(filename, "rb")) == NULL) {
		return false;
	}
	
	struct jpeg_error_mgr jerr;
	struct jpeg_decompress_struct cinfo;

	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);
	jpeg_stdio_src(&cinfo, fd);
	jpeg_read_header(&cinfo, TRUE);
	jpeg_start_decompress(&cinfo);

	width = cinfo.output_width;
	height = cinfo.output_height;
	depth = cinfo.output_components;
	int nRowSize = width * depth;
	unsigned char *buffer = new unsigned char [width*height*depth];
	JSAMPARRAY pBuffer = (*cinfo.mem->alloc_sarray)
		((j_common_ptr) &cinfo, JPOOL_IMAGE, nRowSize, 1);

	unsigned char *p=buffer;
	while (cinfo.output_scanline < cinfo.output_height) {
		jpeg_read_scanlines(&cinfo, pBuffer, 1);
		memcpy(p, pBuffer[0], nRowSize);
		p+=nRowSize;
	}
 
	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);
	fclose(fd);

	return buffer;
}

struct extended_error_mgr {
	struct jpeg_error_mgr pub;
	jmp_buf setjmp_buffer;
};

typedef struct extended_error_mgr * extended_error_ptr;

METHODDEF(void)
extended_error_exit(j_common_ptr cinfo)
{
	extended_error_ptr err = (extended_error_ptr)cinfo->err;
	(*cinfo->err->output_message)(cinfo);
	
	longjmp(err->setjmp_buffer, 1);
}

METHODDEF(void)
extended_reset_error_mgr(j_common_ptr cinfo)
{
	cinfo->err->num_warnings = 0;
	cinfo->err->msg_code = 0;
}

METHODDEF(void)
extended_output_message(j_common_ptr cinfo)
{
	char buffer[JMSG_LENGTH_MAX];

	(*cinfo->err->format_message)(cinfo, buffer);
}

bool writeJPG(const char *filename, unsigned char* data, int width, int height, int depth, int quality)
{
	FILE* fd;
	if(depth != 3 && depth != 1)
	    return false;
	
	if ((fd = fopen(filename, "wb")) == NULL)
	    return false;
	

	struct jpeg_compress_struct cinfo;
	struct extended_error_mgr jerr;

	cinfo.err = jpeg_std_error(&jerr.pub);
	jerr.pub.error_exit = extended_error_exit;
	jerr.pub.output_message = extended_output_message;
	jerr.pub.reset_error_mgr = extended_reset_error_mgr;

	if (setjmp(jerr.setjmp_buffer)) {
		jpeg_destroy_compress(&cinfo);
		fclose(fd);
		return false;
	}

	jpeg_create_compress(&cinfo);
	jpeg_stdio_dest(&cinfo, fd);

	cinfo.image_width = width; 	
	cinfo.image_height = height;
	cinfo.input_components = depth;
	if(depth == 1)
	    cinfo.in_color_space = JCS_GRAYSCALE;
	else
	    cinfo.in_color_space = JCS_RGB;

	jpeg_set_defaults(&cinfo);
	jpeg_set_quality(&cinfo, quality, TRUE);

	jpeg_start_compress(&cinfo, TRUE);

	int row_stride = width * depth;

	JSAMPROW row_pointer[1];
	while (cinfo.next_scanline < cinfo.image_height) {
		row_pointer[0] = & data[cinfo.next_scanline * row_stride];
		jpeg_write_scanlines(&cinfo, row_pointer, 1);
	}

	jpeg_finish_compress(&cinfo);
	fclose(fd);
	jpeg_destroy_compress(&cinfo);

	return TRUE;
}
