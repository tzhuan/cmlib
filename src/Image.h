#ifndef __IMAGE_H__
#define __IMAGE_H__

#include <string>
#include <iostream>

#include "Channels.h"
#include "Column.h"
#include "FileFormat.h"
#include "FileIO.h"
#include "Converter.h"

using namespace std;

namespace Gil {
    using std::string;

    template<typename Type, Channels m_channels>
    class Image {
	public:
	    typedef Pixel<Type, m_channels> pixel_type;

	    Image(): m_width(0), m_height(0), m_data(0), m_row(0) {}

	    Image(const size_type w, const size_type h)
		: m_width(w), m_height(h), m_data(0), m_row(0) 
	    { 
		m_allocate_data();
	    }

	    virtual ~Image() { delete[] m_data; delete[] m_row; }

	    const Column<Type, m_channels> operator[] (const size_type c) const
		{ return Column<Type, m_channels>(c, m_row); }

	    Column<Type, m_channels> operator[] (const size_type c)
		{ return Column<Type, m_channels>(c, m_row); }

	    const size_type channels() const { return m_channels; }
	    const size_type width() const { return m_width; }
	    const size_type height() const { return m_height; }

	    void fill(pixel_type& pixel);

	    template<typename AllocateType>
	    AllocateType* allocate(const size_type size)
		{ return new AllocateType[size]; }

	    template<typename DeallocateType>
	    void deallocate(DeallocateType raw_data)
		{ if (raw_data) { delete[] raw_data; raw_data = 0;} }

	    bool read(const string& filename);
	    bool write(const string& filename) const;

	    template<typename FromType, Channels FromChannels>
	    struct TmpImage {
		TmpImage(const size_type x, const size_type y, 
		    const size_type w, const size_type h,
		    const Image<FromType, FromChannels>& m)
		    : m_x(x), m_y(y), m_width(w), m_height(h), m_image(m) {}
		size_type m_x, m_y, m_width, m_height;
		Image<FromType, FromChannels>& m_image;
	    };

	    template<typename FromType, Channels FromChannels>
	    Image<Type, m_channels>& operator= (
		    const Image<FromType, FromChannels>&);

	    template<typename FromType, Channels FromChannels>
	    Image<Type, m_channels>& operator= (
		    const TmpImage<FromType, FromChannels>&);

	    const TmpImage<Type, m_channels>
	    subimage(const size_type x, const size_type y, 
		const size_type w, const size_type h) const;
	protected:
	    void m_init_row();
	    void m_allocate_data()
	    {
		deallocate(m_data);
		deallocate(m_row);
		m_data = allocate<pixel_type>(m_width*m_height); 
		m_row = allocate<pixel_type*>(m_height);
		m_init_row();
	    }

	    template<typename FromType, Channels FromChannels>
	    void m_convert(const FromType *data);

	    template<typename FromType>
	    bool m_read(const int size, const string& filename, 
		const FileFormat format, const FileFormat* formats, 
		FromType* (*(*funcs))(const char*, int&, int &, int &));
	private:
	    size_type m_width;
	    size_type m_height;
	    Pixel<Type, m_channels> *m_data;
	    Pixel<Type, m_channels> **m_row;
    };

    template<typename Type, Channels m_channels>
    void Image<Type, m_channels>::fill(pixel_type& pixel)
    {
	if (!m_data) return;
	for (size_type h = 0; h < m_height; ++h)
	    for (size_type w = 0; w < m_width; ++w)
		m_row[h][w] = pixel;
    }

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
    */

    template<typename Type, Channels m_channels>
    void Image<Type, m_channels>::m_init_row()
    {
	m_row[0] = m_data;
	for (size_type i = 1; i < m_height; ++i)
	    m_row[i] = m_row[i-1] + m_width;
    }

    template<typename Type, Channels m_channels>
    template<typename FromType, Channels FromChannels>
    void Image<Type, m_channels>::m_convert(const FromType *data)
    {
	const FromType* d = data;
	Converter<FromType, Type, FromChannels, m_channels> conv;
	for (size_type h = 0; h < m_height; ++h) {
	    for (size_type w = 0; w < m_width; ++w) {
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
	    for (size_type h = 0; h < m_height; ++h)
		for (size_type w = 0; w < m_width; ++w)
		    for (size_type c = 0; c < m_channels; ++c) {
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
	    for (size_type h = 0; h < m_height; ++h)
		for (size_type w = 0; w < m_width; ++w)
		    for (size_type c = 0; c < m_channels; ++c) {
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
		    /* Write with CRW format is meaningless.
		case FF_CRW:
		    status = writeCRW(filename.c_str(), data,
			    m_width, m_height, m_channels);
		    break;
		    */
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

    /*
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
}

#endif
