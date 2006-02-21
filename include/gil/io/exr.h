#ifndef GIL_EXR_H
#define GIL_EXR_H

#include <memory>
#include <vector>
#include "../Image.h"


#ifndef INCLUDED_IMF_HEADER_H
// forward declaration
namespace Imf{
    class RgbaInputFile;
    class RgbaOutputFile;
}
#endif

namespace gil {
    
    class C_IStream;

    class ExrReader {
	public:
	    ExrReader() : my_istream(NULL), my_input_file(NULL)
	    {
		// empty
	    }

	    ~ExrReader() {
		finish();
	    }
	    
	    template <typename I>
	    void operator ()(I& image, FILE* f);

	private:
	    void init(FILE* f, size_t& w, size_t& h);
	    void read_scanline(std::vector<Float4>& buf, int y);
	    void finish();
	    
	    C_IStream* my_istream;
	    Imf::RgbaInputFile* my_input_file;
    };

    class ExrWriter {
	public:
	    template <typename I>
	    void operator ()(const I& image, FILE* f);
    };

    template <typename I>
    void ExrReader::operator ()(I& image, FILE* f)
    {
	typedef typename I::Converter Conv;
	size_t width, height;
	init(f, width, height);
	image.allocate(width, height);
	std::vector<Float4> buffer(width); // scanline buffer
	for(size_t y = 0; y < height; y++){
	    read_scanline(buffer, y);
	    for(size_t x = 0; x < width; x++)
		Conv::ext2int( image(x,y), buffer[x] );
	}
	finish();
    }

} // namespace gil

#endif // GIL_EXR_H
