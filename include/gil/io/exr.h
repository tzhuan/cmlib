#ifndef GIL_EXR_H
#define GIL_EXR_H

#include <memory>
#include <vector>
#include "../Image.h"

class Imf::RgbaInputFile;
class Imf::RgbaOutputFile;

namespace gil {
    
    class C_IStream;

    class ExrReader {
	public:
	    template <typename I>
	    void operator ()(I& image, FILE* f);

	private:
	    void init(FILE* f, size_t& w, size_t& h);
	    void read_scanline(std::vector<Byte4>& buf);
	    
	    auto_ptr<C_IStream> my_istream;
	    auto_ptr<Imf::RgbaInputFile> my_input_file;
    };

    class ExrWriter {
	public:
	    template <typename I>
	    void operator ()(const I& image, FILE* f);
    };

    template <typename I>
    void ExrReader::operator ()(I& image, FILE* f)
    {
	size_t width, height;
	init(f, width, height);
	image.allocate(width, height);
	std::vector<Byte4> buffer(width); // scanline buffer
    }

} // namespace gil

#endif // GIL_EXR_H
