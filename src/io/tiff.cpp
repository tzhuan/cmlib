#include <iostream>
#include <exception>
#include <string>
#include <vector>

#include "gil/io/tiff.h"

// must include tiffio.h after gil/io/tiff.h
// or you will failed to compile under VC++
#include <tiffio.h>


using namespace std;
using namespace gil;

template<template<typename, typename> class Converter>
void TiffReader<Converter>::init(const string& name, size_t& w, size_t& h, size_t& c)
{
	my_tiff = TIFFOpen(name.c_str(), "r");
	if(my_tiff == NULL)
		return;

	TIFF* tiff = (TIFF*)my_tiff;
	// don't pass the address of w, h and c directly!
	// it will fail on 64bit machines because sizeof(size_t) = 8
	uint32 wi, hi;
	// SAMPLESPERPIXEL should be uint16!
	uint16 ci;
	TIFFGetField(tiff, TIFFTAG_IMAGEWIDTH, &wi);
	TIFFGetField(tiff, TIFFTAG_IMAGELENGTH, &hi);
	TIFFGetField(tiff, TIFFTAG_SAMPLESPERPIXEL, &ci);
	w = wi; h = hi; c = ci;
}

template<template<typename, typename> class Converter>
void TiffReader<Converter>::read_scanline(vector<Byte1>& buf, unsigned int y)
{

	TIFFReadScanline( (TIFF*)my_tiff, (char*)&buf[0], y);
}

template<template<typename, typename> class Converter>
void TiffReader<Converter>::read_scanline(vector<Byte3>& buf, unsigned int y)
{
	TIFFReadScanline( (TIFF*)my_tiff, (char*)&buf[0][0], y);
}

template<template<typename, typename> class Converter>
void TiffReader<Converter>::read_scanline(vector<Byte4>& buf, unsigned int y)
{
	TIFFReadScanline( (TIFF*)my_tiff, (char*)&buf[0][0], y);
}

template<template<typename, typename> class Converter>
void TiffReader<Converter>::finish()
{
	TIFFClose( (TIFF*)my_tiff );
	my_tiff = NULL;
}


template<template<typename, typename> class Converter>
void TiffWriter<Converter>::init(const string& name, size_t w, size_t h, size_t c)
{
	my_tiff = TIFFOpen(name.c_str(), "w");
	if(my_tiff == NULL)
		return;

	TIFF* tiff = (TIFF*)my_tiff;
	TIFFSetField(tiff, TIFFTAG_IMAGEWIDTH, (int)w);
	TIFFSetField(tiff, TIFFTAG_IMAGELENGTH, (int)h);
	TIFFSetField(tiff, TIFFTAG_SAMPLESPERPIXEL, (int)c);

	// default options
	TIFFSetField(tiff, TIFFTAG_BITSPERSAMPLE, 8);
	TIFFSetField(tiff, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
	if(c > 1)
		TIFFSetField(tiff, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);
	else
		TIFFSetField(tiff, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);

	TIFFSetField(tiff, TIFFTAG_COMPRESSION, COMPRESSION_LZW);
}

template<template<typename, typename> class Converter>
void TiffWriter<Converter>::write_scanline(vector<Byte1>& buf, unsigned int y)
{
	TIFFWriteScanline( (TIFF*)my_tiff, (char*)&buf[0], y);
}

template<template<typename, typename> class Converter>
void TiffWriter<Converter>::write_scanline(vector<Byte3>& buf, unsigned int y)
{
	TIFFWriteScanline( (TIFF*)my_tiff, (char*)&buf[0][0], y);
}

template<template<typename, typename> class Converter>
void TiffWriter<Converter>::write_scanline(vector<Byte4>& buf, unsigned int y)
{
	TIFFWriteScanline( (TIFF*)my_tiff, (char*)&buf[0][0], y);
}

template<template<typename, typename> class Converter>
void TiffWriter<Converter>::finish()
{
	TIFFClose( (TIFF*)my_tiff );
	my_tiff = NULL;
}

