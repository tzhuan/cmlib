#ifndef GIL_FORMATTER_H

#include <cstdio>
#include "Color.h"

namespace gil {

    class PngReader {
	public:
	    PngReader(FILE* fh);
	    ~PngReader();
	    
	    template <typename Conv>
	    void read_pixel(typename Conv::Internal* &pixel, size_t& w, size_t& h);
    };

    class PngWriter {
	public:
	    PngWriter(FILE* fh);
	    ~PngWriter();
	    
	    template <typename Conv>
	    void write_pixel(typename Conv::Internal* pixel, size_t w, size_t h);
    };


} // namespace gil

#endif // GIL_FORMATTER_H
