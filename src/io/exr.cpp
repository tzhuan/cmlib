#include <stdexcept>
#include <cstdio>
#include <ImfInputFile.h>
#include <ImfChannelList.h>
#include <ImfFrameBuffer.h>
#include <half.h>
#include "gil/io/exr.h"

using namespace std;
using namespace Imf;
using namespace Imath;
using namespace gil;

class C_IStream : public IStream {
    public:
	C_IStream(FILE* f, const char* filename = NULL)
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
	    throw runtime_error("error reading file")
	else
	    throw runtime_error("unexpected end of file");
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
    fseek(my_file, pos, SEEK_SET);
}

void C_IStream::clear()
{
    clearerr(my_file);
}

class C_OStream : public OStream {
    public:
	C_OStream(FILE* f, const char* filename = NULL)
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
	    throw runtime_error("error writing file")
	else
	    throw runtime_error("unexpected end of file");
    }
}

Int64 C_OStream::tellp()
{
    return ftell(my_file);
}

void C_OStream::seekp(Int64 pos)
{
    clearerr(my_file);
    fseek(my_file, pos, SEEK_SET);
}

void C_OStream::clear()
{
    clearerr(my_file);
}

void ExrReader::init(FILE* f, size_t& w, size_t& h)
{
    my_istream = new C_IStream(f);
    my_input_file = new RgbaInputFile(*my_istream);
    box2i dw = my_input_file.dataWindow();
    w = static_cast<size_t>( dw.max.x - dw.min.x + 1 );
    h = static_cast<size_t>( dw.max.y - dw.min.y + 1 );
}

void ExrReader::read_scanline(vector<Byte4>& buf)
{
    
}

