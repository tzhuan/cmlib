#ifndef GIL_IMAGE_IO_H
#define GIL_IMAGE_IO_H

#include <string>
#include <cstdio>
#include "Image.h"

namespace gil {

    template <typename R, typename I>
    void read(I& image, FILE* f, R& reader){
	reader(image, f);
    }

    template <typename R, typename I>
    void read(I& image, FILE* f){
	R reader;
	read(image, f, reader);
    }
    
    template <typename I>
    bool read(I& image, const std::string& filename){
	FILE* f = fopen(filename.c_str(), "rb");
	if(f == NULL)
	    return false;
	// TODO detect file type here
	// 
	fclose(f);
	return true;
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
    
    template <typename R, typename I>
    bool read(I& image, const std::string& filename){
	R reader;
	return read(image, filename, reader);
    }

    // take need special care of TIFF
    class TiffReader;
    
    template <typename I>
    bool read(I& image, const std::string& filename, TiffReader& reader){
	reader(image, filename);
	return true;
    }

    // no implementation, just to make linking error.
    template <typename I>
    void read(I& image, FILE* f, TiffReader& reader);

    template <typename W, typename I>
    void write(const I& image, FILE* f, W& writer)
    {
	writer(image, f);
    }
    
    template <typename W, typename I>
    void write(const I& image, FILE* f)
    {
	W writer;
	writer(image, f);
    }
    
    template <typename I>
    bool write(const I& image, const std::string& filename)
    {
	FILE* f = fopen(filename.c_str(), "wb");
	if(f == NULL)
	    return false;
	// TODO detect file type here
	// 
	fclose(f);
	return true;
    }

    template <typename W, typename I>
    bool write(const I& image, const std::string& filename, W& writer){
	FILE* f = fopen(filename.c_str(), "wb");
	if(f == NULL)
	    return false;
	read(image, f, writer);
	fclose(f);
	return true;
    }
    
    template <typename W, typename I>
    inline bool write(const I& image, const std::string& filename){
	W writer; 
	return write(image, filename, writer);
    }

    // take need special care of TIFF
    class TiffWriter;
    
    template <typename I>
    inline bool write(I& image, const std::string& filename, TiffWriter& writer)
    {
	return writer(image, filename);
    }

    // no implementation, just to make linking error.
    template <typename I>
    bool write(I& image, FILE* f, TiffWriter& writer);

} // namespace gil

#endif // GIL_FILE_IO_H
