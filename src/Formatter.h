#ifndef GIL_FORMATTER_H

#include <cstdio>
#include "Color.h"

namespace gil {

    class PngReader {
	public:
	    PngReader(FILE* fh);
	    ~PngReader();
	    
	    template <typename I>
	    void operator ()(I& image);
    };

    class PngWriter {
	public:
	    PngWriter(FILE* fh);
	    ~PngWriter();
	    
	    template <typename I>
	    void operator ()(const I& image);
    };


} // namespace gil

#endif // GIL_FORMATTER_H
