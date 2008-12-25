#include <algorithm>
#include <vector>
#include <cstdio>

#include "cmlib/imageio/io/bmp.h"
#include "cmlib/imageio/Exception.h"

using namespace std;
using namespace cmlib::image;

namespace {
	const size_t FILE_HEADER_SIZE = 14;
	const size_t IMAGE_HEADER_SIZE = 40;

	void read_color_table(FILE* f, vector<Byte3>& table, size_t num)
	{
		table.resize(num);

		for(size_t i = 0; i < num; i++){
			int r, g, b, reserved;
			r = fgetc(f);
			g = fgetc(f);
			b = fgetc(f);
			reserved = fgetc(f);

			if(r == EOF || g == EOF || b == EOF || reserved == EOF)
				throw EndOfFile("unexpected end of file in color table");

			table[i][0] = r;
			table[i][1] = g;
			table[i][2] = b;
		}
	}

	// write color table for gray level images
	void write_color_table(FILE* f)
	{
		unsigned char table[1024];
		for(int i = 0; i < 256; i++){
			int j = i * 4;
			table[j] = table[j+1] = table[j+2] = i;
			table[j+3] = 0;
		}
		if(fwrite(table, 1024, 1, f) != 1)
			throw IOError("IO error while writing color table");
	}
	

	// to avoid endian problems...
	size_t unpack_word(const unsigned char* c)
	{
		return (c[1] << 8) | c[0];
	}

	size_t unpack_dword(const unsigned char* c)
	{
		return ( ( ( ( (c[3]<<8) | c[2] ) << 8 ) | c[1] ) << 8 ) | c[0];
	}

	void pack_word(size_t n, unsigned char* c)
	{
		c[0] = (n & 0x00FF);
		c[1] = (n & 0xFF00) >> 8;
	}

	void pack_dword(size_t n, unsigned char* c)
	{
		c[0] = (n & 0x000000FF);
		c[1] = (n & 0x0000FF00) >> 8;
		c[2] = (n & 0x00FF0000) >> 16;
		c[3] = (n & 0xFF000000) >> 24;
	}

	void read_and_check(void* buf, size_t len, FILE* f, const string& context)
	{
		if(fread(buf, len, 1, f) != 1){
			if(feof(f))
				throw EndOfFile("unexpected end of file while reading " + context);
			else
				throw IOError("IO error while reading " + context);
		}
	}

	// read BMP file header and check if the format is valid
	// returns the offset to pixel data
	size_t read_file_header(FILE* f)
	{
		unsigned char h[FILE_HEADER_SIZE];
		read_and_check(h, FILE_HEADER_SIZE, f, "BMP file header");

		if(h[0] != 'B' || h[1] != 'M')
			throw InvalidFormat("BMP file header does not start with 'BM'");

		return unpack_dword(h+10);
	}

	void read_image_header(FILE* f, size_t& w, size_t& h, size_t& bpp, size_t& table_num)
	{
		
		unsigned char ih[IMAGE_HEADER_SIZE];
		read_and_check(ih, IMAGE_HEADER_SIZE, f, "BMP image header");

		if(unpack_word(ih+12) != 1)
			throw InvalidFormat("biPlane must be 1 in BMP image header");

		if(unpack_dword(ih+16) != 0)
			throw InvalidFormat("unknown compression type in BMP image header");

		size_t real_size = unpack_dword(ih);
		w = unpack_dword(ih+4);
		h = unpack_dword(ih+8);
		bpp = unpack_word(ih+14);
		table_num = unpack_dword(ih+32);

		// seek to the end of header
		fseek(f, FILE_HEADER_SIZE + real_size, SEEK_SET);
	}

	void write_header(FILE* f, size_t w, size_t h, size_t c)
	{
		const size_t HEADER_SIZE = FILE_HEADER_SIZE + IMAGE_HEADER_SIZE;
		unsigned char header[HEADER_SIZE] = {'B', 'M', 0};

		size_t file_size = w * h * c + HEADER_SIZE;
		size_t offset = HEADER_SIZE;
		if(c == 1){
			file_size += 1024;
			offset += 1024;
		}

		pack_dword(file_size, header+2);
		pack_dword(offset, header+10);
		pack_dword(IMAGE_HEADER_SIZE, header+14);
		pack_dword(w, header+18);
		pack_dword(h, header+22);
		pack_word(1, header+26);   // biPlane, must be 1
		pack_word(c*8, header+28); // bits per pixel

		// number of color table
		if(c == 1){
			pack_dword(256, header+46);
			pack_dword(256, header+50);
		}

		if(fwrite(header, HEADER_SIZE, 1, f) != 1)
			throw IOError("IO error while writing BMP header");
	}
} // anonymous namespace

namespace cmlib {
namespace image {
	void BmpReader::init(FILE* f, size_t& w, size_t& h)
	{
		my_file = f;
		my_pixel_offset = read_file_header(f);
		size_t bpp, table_num;
		read_image_header(f, w, h, bpp, table_num);
		if(bpp == 8)
			read_color_table(f, my_color_table, table_num);
		else if(bpp != 24)
			throw InvalidFormat("unexpected bits per pixel in BMP image header");
	}

	void BmpReader::read_scanline(vector<Byte3>& buf)
	{
		if(my_color_table.size() == 256){ // color indexed
			size_t scan_size = buf.size();
			// BMP has padding after each scanline
			int padding = 4 - scan_size % 4;
			if(padding < 4) 
				scan_size += padding;

			vector<unsigned char> index(scan_size);
			read_and_check(&index[0], scan_size, my_file, "BMP pixel data");
			for(size_t i = 0; i < buf.size(); i++)
				buf[i] = my_color_table[ index[i] ];
		}else{
			int padding = 4 - ( buf.size() * 3 ) % 4;
			if(padding == 4) padding = 0;
			
			read_and_check(&buf[0][0], buf.size()*3, my_file, "BMP pixel data");

			for(; padding > 0; padding--)
				fgetc(my_file);
		}
	}

	void BmpWriter::init(FILE* f, size_t w, size_t h, size_t c)
	{
		my_file = f;
		write_header(f, w, h, c);
		if(c == 1)
			write_color_table(f);
	}

	void BmpWriter::write_scanline(vector<Byte1>& buf)
	{
		const char PADDING[4] = {0,0,0,0};
		size_t padding_len = 4 - buf.size() % 4;
		if(padding_len == 4) padding_len = 0;
		

		if( fwrite(&buf[0], buf.size(), 1, my_file) != 1 )
			throw IOError("IO error while writing BMP image");
		fwrite(PADDING, padding_len, 1, my_file);
	}

	void BmpWriter::write_scanline(vector<Byte3>& buf)
	{
		const char PADDING[4] = {0,0,0,0};
		size_t padding_len = 4 - ( buf.size()*3 ) % 4;
		if(padding_len == 4) padding_len = 0;

		for(size_t i = 0; i < buf.size(); i++)
			swap(buf[i][0], buf[i][2]);

		if( fwrite(&buf[0][0], buf.size()*3, 1, my_file) != 1 )
			throw IOError("IO error while writing BMP image");

		fwrite(PADDING, padding_len, 1, my_file);
	}
} // namespace image
} // namespace cmlib
