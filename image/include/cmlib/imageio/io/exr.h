#ifndef CMLIB_IMAGE_IO_EXR_H
#define CMLIB_IMAGE_IO_EXR_H

#include <memory>
#include <vector>

#include "../../image/Converter.h"
#include "../Exception.h"

namespace cmlib {
namespace image {

	//class C_IStream;
	//class C_OStream;

	class DLLAPI ExrReader {
	public:
		ExrReader() 
			: my_istream(NULL), my_input_file(NULL)
		{
			// empty
		}

		~ExrReader() {
			cleanup();
		}

		template <template<typename, typename> class Converter, typename I>
		void operator ()(I& image, FILE* f)
		{
			//typedef typename I::Converter Conv;
			Converter<Float4, typename I::ColorType> converter;
			size_t width, height;
			init(f, width, height);
			image.resize(width, height);
			std::vector<Float4> buffer(width); // scanline buffer
			for(size_t y = 0; y < height; y++){
				read_scanline(buffer, static_cast<int>(y) );
				for(size_t x = 0; x < width; x++)
					image(x, y) = converter(buffer[x]);
					//Conv::ext2int( image(x, y), buffer[x] );
			}
			cleanup();
		}
		template <typename I>
		void operator ()(I& image, FILE* f)
		{
			this->operator()<DefaultConverter, I>(image, f);
		}

	private:
		void init(FILE* f, size_t& w, size_t& h);
		void read_scanline(std::vector<Float4>& buf, int y);
		void cleanup() throw();

		void* my_istream;	 // XXX actual type is C_IStream*
		void* my_input_file; // XXX actual type is Imf::RgbaInputFile*
		int my_min_x;
		int my_min_y;
	};

	class DLLAPI ExrWriter {
	public:
		ExrWriter() 
			: my_ostream(NULL), my_output_file(NULL)
		{
			// empty
		}

		~ExrWriter() 
		{
			cleanup();
		}

		template <template<typename, typename> class Converter, typename I>
		void operator ()(const I& image, FILE* f)
		{
			//typedef typename I::Converter Conv;
			Converter<typename I::ColorType, Float4> converter;
			size_t width = image.width();
			size_t height = image.height();

			init( f, width, height, image.channels() );
			std::vector<Float4> buffer(width);
			for(size_t y = 0; y < height; y++){
				for(size_t x = 0; x < width; x++)
					buffer[x] = converter( image(x, y) );
					// Conv::int2ext( buffer[x], image(x, y) );
				write_scanline( buffer, static_cast<int>(y) );
			}
			cleanup();
		}
		template <typename I>
		void operator ()(I& image, FILE* f)
		{
			this->operator()<DefaultConverter, I>(image, f);
		}

	private:
		void init(FILE* f, size_t w, size_t h, size_t c);
		void write_scanline(std::vector<Float4>& buf, int y);
		void cleanup() throw();

		void* my_ostream;	  // XXX actual type is C_IStream*
		void* my_output_file; // XXX actual type is Imf::RgbaInputFile*
	};

} // namespace image
} // namespace cmlib

#endif // CMLIB_IMAGE_IO_EXR_H
