#ifndef __GIL_FORMATTER_H__

#include <cstdio>
#include "Color.h"

namespace Gil {

    struct Formatter{
	virtual void open(FILE* fh, bool write) {}
	virtual bool good() { return false; }

	virtual void read_header(size_t& w, size_t& h, size_t& c) {}
	virtual void write_header(size_t w, size_t h, size_t c) {}

	virtual void finish() {}
	virtual void close() {}
	
    };

    class PngFormatter : public Formatter {
	public:
	    void open(FILE* fh, bool write);

	    void read_header(size_t& w, size_t& h, size_t& c);
	    void write_header(size_t w, size_t h, size_t c);

	    void read_pixel(byte* pixels);
	    void read_pixel(Color<byte,3>* pixels);
	    void read_pixel(Color<byte,4>* pixels);

	    void write_pixel(byte* pixels);
	    void write_pixel(Color<byte,3>* pixels);
	    void write_pixel(Color<byte,4>* pixels);
	    
	    void finish();
	    void close();
    };

} // namespace Gil

#endif // __GIL_FORMATTER_H__
