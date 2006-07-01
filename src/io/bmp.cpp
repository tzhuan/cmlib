#include <algorithm>
#include <vector>
#include <cstdio>
#include "gil/io/bmp.h"
#include "gil/Exception.h"

using namespace std;
using namespace gil;

namespace {
	const size_t FILE_HEADER_SIZE = 14;
	const size_t IMAGE_HEADER_SIZE = 40;

	bool read_color_table(FILE* f, vector<Byte3>& table)
	{
		table.resize(256);

		for(size_t i = 0; i < 256; i++){
			int r, g, b, reserved;
			r = fgetc(f);
			g = fgetc(f);
			b = fgetc(f);
			reserved = fgetc(f);

			if(r == EOF || g == EOF || b == EOF || reserved == EOF)
				return false;

			table[i][0] = b;
			table[i][1] = g;
			table[i][2] = r;
		}
		return true;
	}

	size_t parse_word(const unsigned char* c)
	{
		return (c[1] << 8) | c[0];
	}

	size_t parse_dword(const unsigned char* c)
	{
		return ( ( ( ( (c[3]<<8) | c[2] ) << 8 ) | c[1] ) << 8 ) | c[0];
	}

	void read_and_check(void* buf, size_t len, FILE* f, const string& context)
	{
		if(fread(buf, len, 1, f) != 1){
			if(feof(f))
				throw EndOfFile("unexpect end of file while reading " + context);
			else
				throw IOError("unexpect IO error while reading " + context);
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

		return parse_dword(h+10);
	}

	void read_image_header(FILE* f, size_t& w, size_t& h, size_t& bpp)
	{
		
		unsigned char ih[IMAGE_HEADER_SIZE];
		read_and_check(ih, IMAGE_HEADER_SIZE, f, "BMP image header");

		if(parse_word(ih+12) != 1)
			throw InvalidFormat("biPlane must be 1 in BMP image header");

		if(parse_dword(ih+16) != 0)
			throw InvalidFormat("unknown compression type in BMP image header");

		size_t real_size = parse_dword(ih);
		w = parse_dword(ih+4);
		h = parse_dword(ih+8);
		bpp = parse_word(ih+14);

		// seek to the end of header
		fseek(f, FILE_HEADER_SIZE + real_size, SEEK_SET);
	}
}

void BmpReader::init(FILE* f, size_t& w, size_t& h)
{
	my_file = f;
	my_pixel_offset = read_file_header(f);
	size_t bpp;
	read_image_header(f, w, h, bpp);
	if(bpp == 8)
		read_color_table(f, my_color_table);
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
		// channel order is BGR, not RGB
		for(size_t i = 0; i < buf.size(); i++)
			swap(buf[i][0], buf[i][2]);

		for(; padding > 0; padding--)
			fgetc(my_file);
	}
}
