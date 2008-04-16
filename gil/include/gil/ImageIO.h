#ifndef GIL_IMAGE_IO_H
#define GIL_IMAGE_IO_H

#include <iostream>

#include <string>
#include <cstdio>
#include "Image.h"
#include "Formatter.h"
#include "FileFormat.h"
#include "Converter.h"

namespace gil {

	// Read functions
	template <
		template<typename, typename> class Converter, 
		typename R, 
		typename I
	>
	inline void read(I& image, FILE* f, R& reader)
	{
#ifdef _MSC_VER
		reader.operator()<Converter>(image, f);
#else
		reader.template operator()<Converter>(image, f);
#endif
	}
	template <typename R, typename I>
	inline void read(I& image, FILE* f, R& reader)
	{
		read<DefaultConverter>(image, f, reader);
	}

	template <
		template<typename, typename> class Converter, 
		typename R, 
		typename I
	>
	inline void read(I& image, FILE* f)
	{
		R reader;
		read<Converter>(image, f, reader);
	}
	template <typename R, typename I>
	inline void read(I& image, FILE* f)
	{
		read<DefaultConverter>(image, f);
	}

	template <
		template<typename, typename> class Converter,
		typename R, 
		typename I
	>
	bool read(I& image, const std::string& filename, R& reader)
	{
		FILE* f = fopen(filename.c_str(), "rb");
		if(f == NULL)
			return false;
		read<Converter>(image, f, reader);
		fclose(f);
		return true;
	}
	template <typename R, typename I>
	bool read(I& image, const std::string& filename, R& reader)
	{
		return read<DefaultConverter>(image, filename, reader);
	}

	// take need special care of TIFF
	/*
	template <
		template<typename, typename> class Converter, 
		typename I
	>
	inline bool read(
		I& image, const std::string& filename, TiffReader& reader)
	{
		return reader.operator()<Converter>(image, filename);
	}
	*/
	/*
	template <typename I>
	inline bool read(I& image, const std::string& filename, TiffReader& reader)
	{
		return read<DefaultConverter>(image, filename, reader);
	}
	*/

	// no implementation, just to make linking error.
	template <template<typename, typename> class Converter, typename I>
	void read(I& image, FILE* f, TiffReader& reader);
	template <typename I>
	void read(I& image, FILE* f, TiffReader& reader);

	template <
		template<typename, typename> class Converter,
		typename R, 
		typename I
	>
	inline bool read(I& image, const std::string& filename)
	{
		R reader;
		return read<Converter>(image, filename, reader);
	}
	template <typename R, typename I>
	inline bool read(I& image, const std::string& filename)
	{
		return read<DefaultConverter, R>(image, filename);
	}

	template <template<typename, typename> class Converter, typename I>
	bool read(I& image, const std::string& filename)
	{
		FILE* f = fopen(filename.c_str(), "rb");
		if(f == NULL)
			return false;

		switch( Formater::get_format(f) ){
			case FF_PNG:
				read<Converter, PngReader>(image, f);
				break;

			case FF_JPEG:
				read<Converter, JpegReader>(image, f);
				break;

			case FF_TIFF: {
				fclose(f);
				//return read<Converter, TiffReader>(image, filename);
				TiffReader reader;
				return reader.operator()<Converter>(image, filename);
			}

			case FF_EXR:
				read<Converter, ExrReader>(image, f);
				break;

			case FF_HDR:
				read<Converter, HdrReader>(image, f);
				break;

			case FF_PPM:
				read<Converter, PpmReader<Byte3, '6'> >(image, f);
				break;

			case FF_PGM:
				read<Converter, PpmReader<Byte1, '5'> >(image, f);
				break;

			case FF_PFM:
				read<Converter, PfmReader>(image, f);
				break;

			case FF_BMP:
				read<Converter, BmpReader>(image, f);
				break;

			case FF_FLT:
				read<Converter, FltReader>(image, f);
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
	template <
		template<typename, typename> class Converter,
		typename W, 
		typename I
	>
	inline void write(const I& image, FILE* f, W& writer)
	{
#ifdef _MSC_VER
		writer.operator()<Converter>(image, f);
#else
		writer.template operator()<Converter>(image, f);
#endif

	}
	template <typename W, typename I>
	inline void write(const I& image, FILE* f, W& writer)
	{
		write<DefaultConverter>(image, f, writer);
	}

	template <
		template<typename, typename> class Converter,
		typename W, 
		typename I
	>
	inline void write(const I& image, FILE* f)
	{
		W writer;
		write<Converter>(image, f, writer);
	}
	template <typename W, typename I>
	inline void write(const I& image, FILE* f)
	{
		write<DefaultConverter>(image, f);
	}

	template <
		template<typename, typename> class Converter,
		typename W, 
		typename I
	>
	bool write(const I& image, const std::string& filename, W& writer)
	{
		FILE* f = fopen(filename.c_str(), "wb");
		if(f == NULL)
			return false;
		write<Converter>(image, f, writer);
		fclose(f);
		return true;
	}
	template<typename W, typename I>
	bool write(const I& image, const std::string& filename, W& writer)
	{
		return write<DefaultConverter>(image, filename, writer);
	}

	// take care of TiffWriter
	/*
	template <template<typename, typename> class Converter, typename I>
	inline bool write(const I& image, const std::string& filename, TiffWriter& writer)
	{
		return writer.operator()<Converter>(image, filename);
	}

	template <typename W, typename I>
	bool write(const I& image, const std::string& filename, W& writer)
	{
		return write<DefaultConverter>(image, filename, writer);
	}
	*/
	/*
	template <typename I>
	inline bool write(const I& image, const std::string& filename, TiffWriter& writer)
	{
		return write<DefaultConverter>(image, filename, writer);
	}
	*/

	// no implementation, just to make linking error.
	/*
	template <template<typename, typename> class Converter, typename I>
	bool write(const I& image, FILE* f, TiffWriter& writer);
	template <typename I>
	bool write(const I& image, FILE* f, TiffWriter& writer);
	*/

	template <
		template<typename, typename> class Converter,
		typename W,
		typename I
	>
	inline bool write(const I& image, const std::string& filename)
	{
		W writer; 
		return write<Converter>(image, filename, writer);
	}
	template <typename W, typename I>
	inline bool write(const I& image, const std::string& filename)
	{
		return write<DefaultConverter, W>(image, filename);
	}

	template <template<typename, typename> class Converter, typename I>
	bool write(const I& image, const std::string& filename)
	{
		switch( Formater::get_format(filename) ){
			case FF_PNG:
				return write<Converter, PngWriter>(image, filename);

			case FF_JPEG:
				return write<Converter, JpegWriter>(image, filename);

			case FF_TIFF: {
				//return write<Converter, TiffWriter>(image, filename);
				TiffWriter writer;
				return writer.operator()<Converter>(image, filename);
			}

			case FF_EXR:
				return write<Converter, ExrWriter>(image, filename);

			case FF_HDR:
				return write<Converter, HdrWriter>(image, filename);

			case FF_PPM:
				return write<Converter, PpmWriter<Byte3, '6'> >(image, filename);

			case FF_PGM:
				return write<Converter, PpmWriter<Byte1, '5'> >(image, filename);

			case FF_PFM:
				return write<Converter, PfmWriter>(image, filename);

			case FF_BMP:
				return write<Converter, BmpWriter>(image, filename);

			case FF_FLT:
				return write<Converter, FltWriter>(image, filename);

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
