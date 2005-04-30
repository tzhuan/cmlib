#ifndef __IMAGE_H__
#define __IMAGE_H__

#include <string>

#include "Channels.h"
#include "Column.h"
#include "FileFormat.h"
#include "FileIO.h"
#include "Converter.h"

namespace Gil {
    using std::string;

    template<typename Type, Channels m_channels>
    class Image {
	public:
	    typedef Pixel<Type, m_channels> pixel_type;

	    Image(): m_width(0), m_height(0), m_data(0), m_row(0)
	    {}

	    Image(const size_type w, const size_type h)
		: m_width(w), m_height(h), m_data(0), m_row(0) 
	    { allocate(w, h); }

	    virtual ~Image()
	    { delete[] m_data; delete[] m_row; }

	    const Column<Type, m_channels> operator[] (const size_type c) const
	    { return Column<Type, m_channels>(c, m_row); }

	    Column<Type, m_channels> operator[] (const size_type c)
	    { return Column<Type, m_channels>(c, m_row); }

	    const size_type channels() const { return m_channels; }
	    const size_type width() const { return m_width; }
	    const size_type height() const { return m_height; }

	    void fill(pixel_type& pixel);

	    void allocate(const size_type w, const size_type h);

	    template<typename DeallocateType>
	    void deallocate(DeallocateType raw_data)
	    { if (raw_data) delete[] raw_data; }

	    bool read(const string& filename);

	    bool write(const string& filename);
	protected:
	    void m_init_row();

	    template<typename FType>
	    void m_read(FType* data, Channels channels);
	    template<typename FType>
	    void m_write(FType* data, Channels channels);
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
    void Image<Type, m_channels>::m_init_row()
    {
	m_row[0] = m_data;
	for (size_type i = 1; i < m_height; ++i)
	    m_row[i] = m_row[i-1] + m_width;
    }

    template<typename Type, Channels m_channels> template<typename FType>
    void Image<Type, m_channels>::m_read(FType* data, Channels channels)
    {
	if (channels == OneChannel) {
	    FType* d = data;
	    Converter<FType, Type, OneChannel, m_channels> conv;
	    for (size_type h = 0; h < m_height; ++h) {
		for (size_type w = 0; w < m_width; ++w) {
		    m_row[h][w] = conv(d);
		    ++d;
		}
	    }
	}
	else if (channels == ThreeChannels) {
	    FType* d = data;
	    Converter<FType, Type, ThreeChannels, m_channels> conv;
	    for (size_type h = 0; h < m_height; ++h) {
		for (size_type w = 0; w < m_width; ++w) {
		    m_row[h][w] = conv(d);
		    d += 3;
		}
	    }
	} else if (channels == FourChannels) {
	    FType* d = data;
	    Converter<FType, Type, FourChannels, m_channels> conv;
	    for (size_type h = 0; h < m_height; ++h) {
		for (size_type w = 0; w < m_width; ++w) {
		    m_row[h][w] = conv(d);
		    d += 4;
		}
	    }
	}
    }

    template<typename Type, Channels m_channels>
    bool Image<Type, m_channels>::read(const string& filename)
    {
	FileFormat format = get_format(filename);
	if (format == FF_UNKNOWN) return false;
	FileType type = get_type(format);
	if (type == FT_UNKNOWN) return false;
	else if (type == FT_BYTE) {
	    unsigned char *data;
	    int channels;
	    switch(format) {
		case FF_PGM:
		    data = readPGM(filename.c_str(), 
			    m_width, m_height, channels);
		    break;
		case FF_PPM:
		    data = readPPM(filename.c_str(),
			    m_width, m_height, channels);
		    break;
		case FF_BMP:
		    data = readBMP(filename.c_str(),
			    m_width, m_height, channels);
		    break;
		case FF_TGA:
		    data = readTGA(filename.c_str(),
			    m_width, m_height, channels);
		    break;
		case FF_PNG:
		    data = readPNG(filename.c_str(),
			    m_width, m_height, channels);
		    break;
		case FF_JPG:
		    data = readJPG(filename.c_str(),
			    m_width, m_height, channels);
		    break;
		case FF_TIF:
		    data = readTIF(filename.c_str(),
			    m_width, m_height, channels);
		    break;
	    }
	    if (!data) return false;
	    m_read(data, channels);
	    delete[] data;
	} else if (type == FT_FLOAT) {
	    float *data;
	    int channels;
	    switch(format) {
		case FF_HDR:
		    data = readHDR(filename.c_str(),
			    m_width, m_height, channels);
		    break;
		case FF_FLT:
		    data = readFLT(filename.c_str(),
			    m_width, m_height, channels);
		    break;
		case FF_PFM:
		    data = readPFM(filename.c_str(),
			    m_width, m_height, channels);
		    break;
		case FF_CRW:
		    data = readCRW(filename.c_str(),
			    m_width, m_height, channels);
		    break;
		case FF_UVE:
		    data = readUVE(filename.c_str(),
			    m_width, m_height, channels);
		    break;
		case FF_DPX:
		    data = readDPX(filename.c_str(),
			    m_width, m_height, channels);
		    break;
	    }
	    if (!data) return false;
	    m_read(data, channels);
	    delete[] data;
	} 
	return false;
    }

    template<typename Type, Channels m_channels>
    bool write(string& filename)
    {

    }
}

#endif
