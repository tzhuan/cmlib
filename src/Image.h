#ifndef __GIL_IMAGE_H__
#define __GIL_IMAGE_H__

//#include <string>
//#include <iostream>

#include <algorithm>

#include "FileFormat.h"
#include "FileIO.h"
#include "Color.h"
//#include "Converter.h"

namespace Gil {

    template<typename Type>
    class Image {
	public:
	    typedef Type* PtrType;
	    typedef const Type* ConstPtrType;
	    typedef Type& RefType;
	    typedef const Type& ConstRefType;
	    
	    Image()
		: my_width(0), my_height(0), my_data(NULL), my_row(NULL)
	    {
		// empty
	    }

	    Image(size_t w, size_t h)
		: Image() // initialize members to zero first
	    {
		allocate(w, h);
	    }

	    ~Image() { 
		delete [] my_data; 
		delete [] my_row; 
	    }

	    size_t width() const { return my_width; }
	    size_t height() const { return my_height; }

	    void fill(ConstRefType pixel){
		std::fill(begin(), end(), pixel);
	    }

	    void allocate(size_t w, size_t h){
		if(w != my_width or h != my_height){
		    // the delete operator will automatically check for NULL
		    delete [] my_data;
		    delete [] my_row;
		    
		    if(w != 0 and h != 0){
			my_data = new PixelType[w*h];
			my_row = new PixelType*[h];
			my_width = w;
			my_height = h;
			init_row();
		    }else{
			my_width = my_height = 0;
			my_data = NULL;
			my_row = NULL;
		    }
		}
	    }

	    /*
	    template<typename FromType, Channels FromChannels>
	    struct TmpImage {
		TmpImage(const size_t x, const size_t y, 
		    const size_t w, const size_t h,
		    const Image<FromType, FromChannels>& m)
		    : my_x(x), my_y(y), my_width(w), my_height(h), my_image(m) {}
		size_t my_x, my_y, my_width, my_height;
		Image<FromType, FromChannels>& my_image;
	    };

	    template<typename FromType, Channels FromChannels>
	    Image<Type, my_channels>& operator= (
		    const Image<FromType, FromChannels>&);

	    template<typename FromType, Channels FromChannels>
	    Image<Type, my_channels>& operator= (
		    const TmpImage<FromType, FromChannels>&);

	    const TmpImage<Type, my_channels>
	    subimage(const size_t x, const size_t y, 
		const size_t w, const size_t h) const;
		*/

	    RefType operator ()(int x, int y) { return my_row[y][x]; }
	    ConstRefType operator ()(int x, int y) const { return my_row[y][x]; }

	    PtrType begin() { return my_data; }
	    ConstPtrType begin() const { return my_data; }
	    PtrType end() { return my_data + my_width*my_height; }
	    ConstPtrType end() const { return my_data + my_width*my_height; }

	protected:
	    void init_row(){
		my_row[0] = my_data;
		for (size_t i = 1; i < my_height; ++i)
		    my_row[i] = my_row[i-1] + my_width;
	    }

	    //template<typename FromType, Channels FromChannels>
	    //void my_convert(const FromType *data);

	private:
	    size_t my_width;
	    size_t my_height;
	    Type *my_data;
	    Type **my_row;
    };

    /*
    template<typename Type, Channels m_channels>
    void Image<Type, m_channels>::allocate(
	const size_type w, const size_type h) 
    {
	if (m_width == w && m_height == h && m_data && m_row) return;
	m_width = w;
	m_height = h;
	deallocate(m_data);
	deallocate(m_row);
	m_data = new pixel_type [w*h];
	m_row = new pixel_type* [h];
	m_init_row();
    }

    template<typename Type, Channels m_channels>
    template<typename FromType, Channels FromChannels>
    void Image<Type, m_channels>::m_convert(const FromType *data)
    {
	const FromType* d = data;
	Converter<FromType, Type, FromChannels, m_channels> conv;
	for (size_t h = 0; h < m_height; ++h) {
	    for (size_t w = 0; w < m_width; ++w) {
		m_row[h][w] = conv(d);
		d += FromChannels;
	    }
	}
    }

    template<typename Type, Channels m_channels>
    template<typename FromType>
    bool Image<Type, m_channels>::m_read(
	const int size, const string& filename, const FileFormat format,
	const FileFormat* formats, 
	FromType* (*(*funcs))(const char*, int&, int &, int &))
    {
	Channels channels;
	for (int i = 0; i < size; ++i)
	    if (format == formats[i]) {
		FromType* data = (*funcs[i])
			(filename.c_str(), m_width, m_height, channels);
		if (!data) return false;
		m_allocate_data();
		if (channels == OneChannel)
		    m_convert<FromType, OneChannel>(data);
		else if (channels == ThreeChannels)
		    m_convert<FromType, ThreeChannels>(data);
		else if (channels == FourChannels)
		    m_convert<FromType, FourChannels>(data);
		else {
		    delete[] data;
		    return false;
		}
		delete[] data;
		return true;
	    }
	return false;	
    }

    template<typename Type, Channels m_channels>
    bool Image<Type, m_channels>::read(const string& filename)
    {
	FileFormat format = get_format(filename);
	if (format == FF_UNKNOWN) return false;
	FileType type = get_type(format);
	if (type == FT_UNKNOWN) return false;
	else if (type == FT_BYTE) {
	    FileFormat formats[] = {
		FF_PGM, FF_PPM, FF_BMP, FF_TGA, FF_PNG, FF_JPG, FF_TIF};
	    unsigned char* (*(funcs[]))(const char*, int&, int&, int&) = {
		readPGM, readPPM, readBMP, readTGA, readPNG, readJPG, readTIF};
	    const int size = sizeof(formats)/sizeof(FileFormat);
	    return m_read<unsigned char>
			(size, filename, format, formats, funcs);
	} else if (type == FT_FLOAT) {
	    FileFormat formats[] = {
		FF_HDR, FF_FLT, FF_PFM, FF_CRW, FF_UVE, FF_DPX};
	    float* (*(funcs[]))(const char*, int&, int&, int&) = {
		readHDR, readFLT, readPFM, readCRW, readUVE, readDPX};
	    const int size = sizeof(formats)/sizeof(FileFormat);
	    return m_read<float>(size, filename, format, formats, funcs);
	}
	return false;
    }

    template<typename Type, Channels m_channels>
    bool Image<Type, m_channels>::write(const string& filename) const
    {
	FileFormat format = get_format(filename);
	if (format == FF_UNKNOWN) return false;
	FileType type = get_type(format);
	if (type == FT_UNKNOWN) return false;
	else if (type == FT_BYTE) {
	    unsigned char *data = 
		new unsigned char[m_height*m_width*m_channels];
	    TypeConverter<Type, unsigned char> tc;
	    unsigned char *d = data;
	    for (size_t h = 0; h < m_height; ++h)
		for (size_t w = 0; w < m_width; ++w)
		    for (size_t c = 0; c < m_channels; ++c) {
			*d = tc(m_row[h][w][c]);
			++d;
		    }

	    bool status = false;
	    switch(format) {
		case FF_PGM:
		    status = writePGM(filename.c_str(), data, 
			    m_width, m_height, m_channels);
		    break;
		case FF_PPM:
		    status = writePPM(filename.c_str(), data,
			    m_width, m_height, m_channels);
		    break;
		case FF_BMP:
		    status = writeBMP(filename.c_str(), data,
			    m_width, m_height, m_channels);
		    break;
		case FF_TGA:
		    status = writeTGA(filename.c_str(), data,
			    m_width, m_height, m_channels);
		    break;
		case FF_PNG:
		    status = writePNG(filename.c_str(), data,
			    m_width, m_height, m_channels);
		    break;
		case FF_JPG:
		    status = writeJPG(filename.c_str(), data,
			    m_width, m_height, m_channels);
		    break;
		case FF_TIF:
		    status = writeTIF(filename.c_str(), data,
			    m_width, m_height, m_channels);
		    break;
		default:
		    return false;
		    break;
	    }
	    delete[] data;
	    return status;
	} else if (type == FT_FLOAT) {
	    float* data = new float[m_height*m_width*m_channels];
	    TypeConverter<Type, float> tc;
	    float* d = data;
	    for (size_t h = 0; h < m_height; ++h)
		for (size_t w = 0; w < m_width; ++w)
		    for (size_t c = 0; c < m_channels; ++c) {
			*d = tc(m_row[h][w][c]);
			++d;
		    }
	    bool status = false;
	    switch(format) {
		case FF_HDR:
		    status = writeHDR(filename.c_str(), data,
			    m_width, m_height, m_channels);
		    break;
		case FF_FLT:
		    status = writeFLT(filename.c_str(), data,
			    m_width, m_height, m_channels);
		    break;
		case FF_PFM:
		    status = writePFM(filename.c_str(), data,
			    m_width, m_height, m_channels);
		    break;
		    //Write with CRW format is meaningless.
		case FF_CRW:
		    status = writeCRW(filename.c_str(), data,
			    m_width, m_height, m_channels);
		    break;
		case FF_UVE:
		    status = writeUVE(filename.c_str(), data,
			    m_width, m_height, m_channels);
		    break;
		case FF_DPX:
		    status = writeDPX(filename.c_str(), data,
			    m_width, m_height, m_channels);
		    break;
		default: 
		    return false;
		    break;
	    }
	    delete[] data;
	    return status;
	} else 
	    return false;
	return false;
    }

    template<typename Type, Channels m_channels>
    template<typename FromType, Channels FromChannels>
    Image<Type, m_channels>& Image<Type, m_channels>::
    operator= (const Image<FromType, FromChannels>& src)
    {
	if (this == &src)
	    return *this;
	if (m_height == src.m_height && m_width == src.m_width &&
		m_channels == FromChannels) {
	    memmove(m_data, src.m_data, 
		m_height*m_width*m_channels*sizeof(Type));
	} else {
	    m_height = src.m_height;
	    m_width = src.m_width;
	    m_allocate_data();
	    m_convert<FromType, FromChannels>(src.m_data);
	}
	return *this;
    }

    template<typename Type, Channels m_channels>
    template<typename FromType, Channels FromChannels>
    Image<Type, m_channels>& Image<Type, m_channels>::
    operator= (const TmpImage<FromType, FromChannels>& srctmp)
    {
	const Image<FromType, FromChannels> &src = srctmp.m_image;

    }

    template<typename Type, Channels m_channels>
    const TmpImage<Type, m_channels>
    Image<Type, m_channels>::subimage(const size_type x, const size_type y, 
	    const size_type w, const size_type h) const
    {
	return TmpImage<Type, m_channels>(x, y, w, h, *this);
    }
    */

} // namespace Gil

#endif // __GIL_IMAGE_H__
