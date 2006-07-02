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
    inline void read(I& image, FILE* f, R& reader){
	reader(image, f);
    }

    template <typename R, typename I>
    inline void read(I& image, FILE* f){
	R reader;
	read(image, f, reader);
    }

    template <typename R, typename I>
    bool read(I& image, const std::string& filename, R& reader){
	FILE* f = fopen(filename.c_str(), "rb");
	if(f == NULL)
	    return false;
	read(image, f, reader);
	fclose(f);
	return true;
    }

    // take need special care of TIFF
    template <typename I>
    inline bool read(I& image, const std::string& filename, TiffReader& reader){
	return reader(image, filename);
    }
    // no implementation, just to make linking error.
    template <typename I>
    void read(I& image, FILE* f, TiffReader& reader);
    
    
    template <typename R, typename I>
    inline bool read(I& image, const std::string& filename){
	R reader;
	return read(image, filename, reader);
    }
    
    template <typename I>
    bool read(I& image, const std::string& filename){
	FILE* f = fopen(filename.c_str(), "rb");
	if(f == NULL)
	    return false;

	switch( Formater::get_format(f) ){
	    case FF_PNG:
		read<PngReader>(image, f);
		break;
		
	    case FF_JPEG:
		read<JpegReader>(image, f);
		break;
 
	    case FF_TIFF:
 	        fclose(f);
 	        return read<TiffReader>(image, filename);

	    case FF_EXR:
		read<ExrReader>(image, f);
		break;

	    case FF_HDR:
		read<HdrReader>(image, f);
		break;

	    case FF_PPM:
		read<PpmReader<Byte3, '6'> >(image, f);
		break;

	    case FF_PGM:
		read<PpmReader<Byte1, '5'> >(image, f);
		break;

	    case FF_PFM:
		read<PfmReader> (image, f);
		break;

	    case FF_BMP:
	    	read<BmpReader>(image, f);

	    default:
		fclose(f);
		return false;
	}
	
	fclose(f);
	return true;
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
    bool write(const I& image, const std::string& filename, W& writer){
	FILE* f = fopen(filename.c_str(), "wb");
	if(f == NULL)
	    return false;
	write(image, f, writer);
	fclose(f);
	return true;
    }
    
    // take care of TiffWriter
    template <typename I>
    inline bool write(const I& image, const std::string& filename, TiffWriter& writer)
    {
	return writer(image, filename);
    }

    // no implementation, just to make linking error.
    template <typename I>
    bool write(const I& image, FILE* f, TiffWriter& writer);
    
    template <typename W, typename I>
    inline bool write(const I& image, const std::string& filename){
	W writer; 
	return write(image, filename, writer);
    }

    template <typename I>
    bool write(const I& image, const std::string& filename)
    {
	switch( Formater::get_format(filename) ){
	    case FF_PNG:
		return write<PngWriter>(image, filename);
		
	    case FF_JPEG:
		return write<JpegWriter>(image, filename);

	    case FF_TIFF:
		return write<TiffWriter>(image, filename);

	    case FF_EXR:
		return write<ExrWriter>(image, filename);

	    case FF_HDR:
		return write<HdrWriter>(image, filename);

	    case FF_PPM:
		return write<PpmWriter<Byte3, '6'> >(image, filename);

	    case FF_PGM:
		return write<PpmWriter<Byte1, '5'> >(image, filename);

	    case FF_PFM:
		return write<PfmWriter>(image, filename);

	    case FF_BMP:
		return write<BmpWriter>(image, filename);

	    default:
		return false;
	}
    }

} // namespace gil

#endif // GIL_FILE_IO_H
