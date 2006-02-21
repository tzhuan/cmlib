#ifndef GIL_EXR_H
#define GIL_EXR_H

namespace gil {

    class ExrReader {
	public:
	    template <typename I>
	    void operator ()(I& image, FILE* f);
    };

    class ExrWriter {
	public:
	    template <typename I>
	    void operator ()(const I& image, FILE* f);
    };

} // namespace gil

#endif // GIL_EXR_H
