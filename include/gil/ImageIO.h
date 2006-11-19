#ifndef GIL_IMAGE_IO_H
#define GIL_IMAGE_IO_H

#include <iostream>

#include <string>
#include <cstdio>
#include "Image.h"
#include "Formatter.h"
#include "FileFormat.h"

namespace gil {

	// Read functions
	template <typename R, typename I>
	inline void read(I& image, FILE* f, R& reader)
	{
		reader(image, f);
	}

	template <typename R, typename I>
	inline void read(I& image, FILE* f)
	{
		R reader;
		read(image, f, reader);
	}

	template <typename R, typename I>
	bool read(I& image, const std::string& filename, R& reader)
	{
		FILE* f = fopen(filename.c_str(), "rb");
		if(f == NULL)
			return false;
		read(image, f, reader);
		fclose(f);
		return true;
	}

	// take need special care of TIFF
	template <template<typename, typename> class Converter, typename I>
	inline bool read(
		I& image, 
		const std::string& filename, 
		TiffReader<Converter>& reader)
	{
		return reader(image, filename);
	}

	template <typename I>
	inline bool read(
		I& image, 
		const std::string& filename, 
		TiffReader<DefaultConverter>& reader)
	{
		return reader(image, filename);
	}

	// no implementation, just to make linking error.
	template <template<typename, typename> class Converter, typename I>
	void read(I& image, FILE* f, TiffReader<Converter>& reader);
	template <typename I>
	void read(I& image, FILE* f, TiffReader<DefaultConverter>& reader);


	template <typename R, typename I>
	inline bool read(I& image, const std::string& filename)
	{
		R reader;
		return read(image, filename, reader);
	}

	template <template<typename, typename> class Converter, typename I>
	bool read(I& image, const std::string& filename)
	{
		FILE* f = fopen(filename.c_str(), "rb");
		if(f == NULL)
			return false;

		switch( Formater::get_format(f) ){
			case FF_PNG:
				read<PngReader<Converter> >(image, f);
				break;

			case FF_JPEG:
				read<JpegReader<Converter> >(image, f);
				break;

			case FF_TIFF:
				fclose(f);
				return read<TiffReader<Converter> >(image, filename);

			case FF_EXR:
				read<ExrReader<Converter> >(image, f);
				break;

			case FF_HDR:
				read<HdrReader<Converter> >(image, f);
				break;

			case FF_PPM:
				read<PpmReader<Byte3, '6', Converter> >(image, f);
				break;

			case FF_PGM:
				read<PpmReader<Byte1, '5', Converter> >(image, f);
				break;

			case FF_PFM:
				read<PfmReader<Converter> > (image, f);
				break;

			case FF_BMP:
				read<BmpReader<Converter> >(image, f);
				break;

			case FF_FLT:
				read<FltReader<Converter> >(image, f);
				break;

			default:
				fclose(f);
				return false;
		}

		fclose(f);
		return true;
	}
	template <typename I>
	bool read(I& image, const std::string& filename)
	{
		return read<DefaultConverter>(image, filename);
	}

	// Write functions
	template <typename W, typename I>
	inline void write(const I& image, FILE* f, W& writer)
	{
		writer(image, f);
	}

	template <typename W, typename I>
	inline void write(const I& image, FILE* f)
	{
		W writer;
		writer(image, f);
	}

	template <typename W, typename I>
	bool write(const I& image, const std::string& filename, W& writer)
	{
		FILE* f = fopen(filename.c_str(), "wb");
		if(f == NULL)
			return false;
		write(image, f, writer);
		fclose(f);
		return true;
	}

	// take care of TiffWriter
	template <template<typename, typename> class Converter, typename I>
	inline bool write(
		const I& image, 
		const std::string& filename, 
		TiffWriter<Converter>& writer)
	{
		return writer(image, filename);
	}

	template <typename I>
	inline bool write(
		const I& image, 
		const std::string& filename, 
		TiffWriter<DefaultConverter>& writer)
	{
		return writer(image, filename);
	}

	// no implementation, just to make linking error.
	template <template<typename, typename> class Converter, typename I>
	bool write(const I& image, FILE* f, TiffWriter<Converter>& writer);

	template <typename I>
	bool write(const I& image, FILE* f, TiffWriter<DefaultConverter>& writer);

	template <typename W, typename I>
	inline bool write(const I& image, const std::string& filename)
	{
		W writer; 
		return write(image, filename, writer);
	}

	template <template<typename, typename> class Converter, typename I>
	bool write(const I& image, const std::string& filename)
	{
		switch( Formater::get_format(filename) ){
			case FF_PNG:
				return write<PngWriter<Converter> >(image, filename);

			case FF_JPEG:
				return write<JpegWriter<Converter> >(image, filename);

			case FF_TIFF:
				return write<TiffWriter<Converter> >(image, filename);

			case FF_EXR:
				return write<ExrWriter<Converter> >(image, filename);

			case FF_HDR:
				return write<HdrWriter<Converter> >(image, filename);

			case FF_PPM:
				return write<PpmWriter<Byte3, '6', Converter> >(image, filename);

			case FF_PGM:
				return write<PpmWriter<Byte1, '5', Converter> >(image, filename);

			case FF_PFM:
				return write<PfmWriter<Converter> >(image, filename);

			case FF_BMP:
				return write<BmpWriter<Converter> >(image, filename);

			case FF_FLT:
				return write<FltWriter<Converter> >(image, filename);

			default:
				return false;
		}
	}

	template <typename I>
	bool write(const I& image, const std::string& filename)
	{
		return write<DefaultConverter>(image, filename);
	}

} // namespace gil

#endif // GIL_FILE_IO_H
