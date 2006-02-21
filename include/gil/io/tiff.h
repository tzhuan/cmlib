#ifndef GIL_TIFF_H
#define GIL_TIFF_H

namespace gil {
    
    class TiffReader {
	public:
	    template <typename I>
	    void operator ()(I& image, const string& str);
    };

    class TiffWriter {
	public:
	    template <typename I>
	    void operator ()(const I& image, const string& str);
    };

} // namespace gil

#endif // GIL_TIFF_H
