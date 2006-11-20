#include <exception>
#include <stdexcept>
#include <cstdio>

// OpenEXR headers
// turn off the turnoff warnings if we're using VC
#ifdef _MSC_VER
#pragma warning(disable:4290 4996) 
#endif

#include <ImfRgbaFile.h>
#include <ImfIO.h>
#include <half.h>

#include "gil/Exception.h"
#include "gil/io/exr.h"

using namespace std;
using namespace gil;

using namespace Imf;
using namespace Imath;

namespace {

	class C_IStream : public IStream {
		public:
			C_IStream(FILE* f, const char* filename = "")
				: IStream(filename), my_file(f)
			{
				// empty;
			}

			bool read(char c[], int n);
			Int64 tellg();
			void seekg(Int64 pos);
			void clear();

		private:
			FILE* my_file;
	};

	bool C_IStream::read(char c[], int n)
	{
		if( fread(c, n, 1, my_file) != 1 )
		{
			// fread() failed, but the return value does not distinguish
			// between I/O errors and end of file, so we call ferror() to
			// determine what happened.
			if( ferror(my_file) )
				throw IOError("error reading file");
			else
				throw EndOfFile("unexpected EOF in C_IStream::read()");
		}
		return feof(my_file) == 0;
	}

	Int64 C_IStream::tellg()
	{
		return ftell(my_file);
	}

	void C_IStream::seekg(Int64 pos)
	{
		clearerr(my_file);
		fseek(my_file, static_cast<long>(pos), SEEK_SET);
	}

	void C_IStream::clear()
	{
		clearerr(my_file);
	}

	class C_OStream : public OStream {
		public:
			C_OStream(FILE* f, const char* filename = "")
				: OStream(filename), my_file(f)
			{
				// empty
			}

			void write(const char c[], int n);
			Int64 tellp();
			void seekp(Int64 pos);
			void clear();

		private:
			FILE* my_file;
	};

	void C_OStream::write(const char c[], int n)
	{
		if( fwrite(c, n, 1, my_file) != 1 ){
			if( ferror(my_file) )
				throw IOError("error writing file");
			else
				throw EndOfFile("unexpected EOF in C_OStream::write()");
		}
	}

	Int64 C_OStream::tellp()
	{
		return ftell(my_file);
	}

	void C_OStream::seekp(Int64 pos)
	{
		clearerr(my_file);
		fseek(my_file, static_cast<long>(pos), SEEK_SET);
	}

	void C_OStream::clear()
	{
		clearerr(my_file);
	}

} // anonymous namespace

template<template<typename, typename> class Converter>
void ExrReader<Converter>::init(FILE* f, size_t& w, size_t& h)
{
	try{
		my_istream = new C_IStream(f);
		my_input_file = new RgbaInputFile( *(C_IStream*)my_istream );
		RgbaInputFile* input_file = (RgbaInputFile*)my_input_file;
		Box2i dw = input_file->dataWindow();
		w = static_cast<size_t>( dw.max.x - dw.min.x + 1 );
		h = static_cast<size_t>( dw.max.y - dw.min.y + 1 );
		my_min_x = dw.min.x;
		my_min_y = dw.min.y;
	}
	catch(exception&){
		cleanup();
		throw;
	}
}

template<template<typename, typename> class Converter>
void ExrReader<Converter>::read_scanline(vector<Float4>& buf_float, int y)
{
	try{
		RgbaInputFile* input_file = (RgbaInputFile*)my_input_file;
		const size_t width = buf_float.size();
		int cur_y = my_min_y + y;
		vector<Rgba> buf_half(width);
		input_file->setFrameBuffer( &buf_half[0] - my_min_x - cur_y * width, 1, width );
		input_file->readPixels(cur_y);
		for(size_t i = 0; i < width; i++){
			buf_float[i][0] = buf_half[i].r;
			buf_float[i][1] = buf_half[i].g;
			buf_float[i][2] = buf_half[i].b;
			buf_float[i][3] = buf_half[i].a;
		}
	}
	catch(exception&){
		cleanup();
		throw;
	}
}

template<template<typename, typename> class Converter>
void ExrReader<Converter>::cleanup() throw()
{
	delete (RgbaInputFile*)my_input_file;
	delete (C_IStream*)my_istream;
	my_input_file = NULL;
	my_istream = NULL;
}


template<template<typename, typename> class Converter>
void ExrWriter<Converter>::init(FILE* f, size_t w, size_t h, size_t c)
{
	RgbaChannels ch;
	if(c == 1) ch = WRITE_Y;
	else if(c == 3) ch = WRITE_RGB;
	else ch = WRITE_RGBA;

	try{
		my_ostream = new C_OStream(f);
		my_output_file = new RgbaOutputFile( *(C_OStream*)my_ostream, Header(w, h), ch );
	}
	catch(exception&){
		cleanup();
		throw;
	}
}

template<template<typename, typename> class Converter>
void ExrWriter<Converter>::write_scanline(vector<Float4>& buf_float, int y)
{
	try{
		RgbaOutputFile* output_file = (RgbaOutputFile*)my_output_file;
		const size_t width = buf_float.size();
		vector<Rgba> buf_half(width);

		for(size_t i = 0; i < width; i++){
			buf_half[i].r = buf_float[i][0];
			buf_half[i].g = buf_float[i][1];
			buf_half[i].b = buf_float[i][2];
			buf_half[i].a = buf_float[i][3];
		}

		output_file->setFrameBuffer( &buf_half[0] - y * width, 1, width );
		output_file->writePixels(1);
	}
	catch(exception&){
		cleanup();
		throw;
	}
}

template<template<typename, typename> class Converter>
void ExrWriter<Converter>::cleanup() throw()
{
	delete (RgbaOutputFile*)my_output_file;
	delete (C_OStream*)my_ostream;
	my_output_file = NULL;
	my_ostream = NULL;
}
