#ifndef GIL_FORMATTER_H

#include <cstdio>
#include "Color.h"

namespace gil {

    class PngReader {
	public:
	    template <typename I>
	    void operator ()(I& image, FILE* f);
    };

    class PngWriter {
	public:
	    template <typename I>
	    void operator ()(const I& image, FILE* f);
    };

    class TiffReader {
	public:
	    template <typename I>
	    void operator ()(I& image, FILE* f);
    };

    class TiffWriter {
	public:
	    template <typename I>
	    void operator ()(const I& image, FILE* f);
    };


} // namespace gil

#endif // GIL_FORMATTER_H
