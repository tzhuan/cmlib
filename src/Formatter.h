#ifndef GIL_FORMATTER_H

#include <cstdio>
#include "Color.h"

namespace gil {

    class PngReader {
	public:
	    PngReader(FILE* fh);
	    ~PngReader();
	    
	    void read_header(size_t& w, size_t& h, size_t& c);
	    
	    template <typename Conv>
	    void read_pixel(typename Conv::Internal* pixels, Conv convert);
    };

    class PngWriter {
	public:
	    PngWriter(FILE* fh);
	    ~PngWriter();
	    
	    void write_header(size_t w, size_t h, size_t c);
	    
	    template <typename Conv>
	    void write_pixel(typename Conv::Internal* pixels, Conv converter);
    };


} // namespace gil

#endif // GIL_FORMATTER_H
