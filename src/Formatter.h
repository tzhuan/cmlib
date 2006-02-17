#ifndef GIL_FORMATTER_H

#include <cstdio>
#include "Color.h"

namespace gil {

    struct Formatter{
	virtual void init_read(FILE* fh) {}
	virtual void init_write(FILE* fh) {}
	virtual bool good() { return false; }

	virtual void read_header(size_t& w, size_t& h, size_t& c) {}
	virtual void write_header(size_t w, size_t h, size_t c) {}

	virtual void finish() {}
    };

    class PngFormatter : public Formatter {
	public:
	    typedef Byte1 BaseType;

	    void init_read(FILE* fh) {}
	    void init_write(FILE* fh) {}

	    void read_header(size_t& w, size_t& h, size_t& c);
	    void write_header(size_t w, size_t h, size_t c);

	    void read_pixel(Byte1* pixels);

	    void write_pixel(Byte1* pixels);
	    
	    void finish();
    };

} // namespace gil

#endif // GIL_FORMATTER_H
