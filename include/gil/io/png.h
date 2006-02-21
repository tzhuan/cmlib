#ifndef GIL_PNG_H
#define GIL_PNG_H

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

} // namespace gil

#endif // GIL_PNG_H
